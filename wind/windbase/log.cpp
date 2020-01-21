#include "log.h"
#include "wtime.h"
#include <io.h>
#include <direct.h>
#include <mutex>

string RollOutCallBack(const char * szOldFile)
{
	return "";
}

typedef std::lock_guard<std::recursive_mutex>  ScopedMutex;

void CDefaultLogDispatchCallback::Handle(const CLogDispatchData * pData)
{
	if (pData == nullptr)
	{
		return;
	}

	ScopedMutex scopedMutex(CLogDispatchCallback::GetMutex());
	m_pData = pData;
	Dispatch(pData->GetMsg()->GetLogger()->GetLogBuilder()->Build(pData->GetMsg()));
}

void CDefaultLogDispatchCallback::Dispatch(string && strLogLine)
{
	CLogger * pLogger = m_pData->GetMsg()->GetLogger();
	if (pLogger->GetConf().GetToStand() == true)
	{
		pLogger->RollbackFileName();
		std::fstream * fs = pLogger->GetFstream();
		if (fs == nullptr)
		{
			cout << "CDefaultLogDispatchCallback::Dispatch fstream is nullptr" << endl;
			return;
		}
		fs->write(strLogLine.c_str(), strLogLine.size());
		if (fs->fail())
		{
			cout << "CDefaultLogDispatchCallback::Dispatch fs-> write fail. filename : " 
				<< pLogger->GetConf().GetFileName() << endl;
		}
		fs->flush();
	}
	if (pLogger->GetConf().GetToStand() == true)
	{
		std::cout << strLogLine;
	}
}

void CLogDispatcher::Dispatch(CLogMessage * pMsg)
{
	//����һ�ι���
	CLogDispatchCallback * pCallback = CStorage::Instance()->GetCallBack();
	if (pCallback == nullptr)
	{
		std::cout << "call back is nullptr." << endl;
	}

	CLogDispatchData oData(pMsg);
	pCallback->Handle(&oData);
}

void CWriter::ProcessMsg()
{
	CLogDispatcher::Instance()->Dispatch(m_pMsg);
	if (m_pMsg != nullptr)
	{
		delete m_pMsg;
	}
}

string CDefaultLogBuilder::Build(const CLogMessage * pLogMsg)
{
	stringstream oSS;
	oSS << GetTime(nullptr) + " " + Level2Str(pLogMsg->GetLevel()) + " " <<pLogMsg->GetFileName() << ":"
		<< pLogMsg->GetLine() << " | " + pLogMsg->GetMsg() + "\n";
	return oSS.str();
}

string CDefaultLogBuilder::GetTime(const char * szFmt)
{
	stringstream oSS;
	oSS << CTime::Instance()->ToString(szFmt) + ",";
	oSS << CTime::Instance()->GetNowMilTime() % 1000;
	return oSS.str();
}

string CDefaultLogBuilder::Level2Str(ELevel eLv)
{
	switch (eLv)
	{
	case ELevel::Global:
		break;
	case ELevel::Trace:
		return "[TRACE]";
		break;
	case ELevel::Debug:
		return "[DEBUG]";
		break;
	case ELevel::Fatal:
		return "[FATAL]";
		break;
	case ELevel::Error:
		return "[ERROR]";
		break;
	case ELevel::Warning:
		return "[WARNS]";
		break;
	case ELevel::Verbose:
		break;
	case ELevel::Info:
		return "[INFO]";
		break;
	default:
		break;
	}
	return "[UNKNOW]";
}

void CStorage::Init(uint32 nMaxSize, string strPath, string strPrefix)
{
	m_pLogDispatchCallBack = new(CDefaultLogDispatchCallback);
	m_pLogger = new CLogger(new(CDefaultLogBuilder), RollOutCallBack);
	m_pLogger->GetConf().SetMaxSize(nMaxSize);
	m_pLogger->GetConf().SetPath(strPath);
	m_pLogger->GetConf().SetPrefix(strPrefix);
}

CLogger::CLogger(CLogBuilder * pLogBuilder, PreRollOutCallback * pRollOut)
	: m_pLogBuilder(pLogBuilder), m_pRollOut(pRollOut)
{
	m_oTypedConf = CTypedConfigurations();
}

void CLogger::RollbackFileName()
{
	//if (m_pStream->tellg() >= m_oTypedConf.GetMaxSize())
	//{

	//}
	string strNewFile = CTime::Instance()->ToString("%Y%m%d_%H");
	strNewFile = m_oTypedConf.GetPrefix() + strNewFile + ".log";
	strNewFile = m_oTypedConf.GetPath() + strNewFile;
	if (m_oTypedConf.GetFileName().find(strNewFile) == string::npos)
	{
		RollFile(strNewFile);
	}

	if (m_pStream->tellg() > m_oTypedConf.GetMaxSize())
	{
		strNewFile = GetNewFile(m_oTypedConf.GetFileName());
		RollFile(strNewFile);
	}
}

fstream * CLogger::GenFstream(string strPath)
{
	CreateFile(strPath);
	fstream * fs = new fstream;
	fs->open(strPath.c_str(), ios::app);
	if (fs->is_open() == false)
	{
		cout << "file not exist. path : " << strPath << endl;
		return nullptr;
	}
	return fs;
}

string& replace_all(string& str, const string& old_value, const string& new_value)
{
	string::size_type  pos(0);
	while (true)
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else
		{
			break;
		}
	}

	return  str;
}

bool CLogger::CreateFile(string strSrcFilePath)
{
	char buff[1000];
	_getcwd(buff, 1000);
	strSrcFilePath = "/" + strSrcFilePath;
	strSrcFilePath = buff + strSrcFilePath;
	string strFilePath = replace_all(strSrcFilePath, "/", "\\");
	string::size_type rFirstPos = strFilePath.rfind("\\");
	if (strFilePath.size() != (rFirstPos + 1))
	{
		//������һ���Ƿ�Ϊ�ļ���
		string strTemp = strFilePath.substr(rFirstPos, strFilePath.size());
		if (string::npos != strTemp.find("."))
		{
			//���һ�������ļ�����
			strFilePath = strFilePath.substr(0, rFirstPos + 1);
		}
		else
		{
			//���һ�����ļ�������
			strFilePath += "\\";
		}
	}
	else
	{
		strFilePath += "\\";
	}

	string::size_type startPos(0);
	string::size_type endPos(0);

	while (true)
	{
		if ((endPos = strFilePath.find("\\", startPos)) != string::npos)
		{
			string strFolderPath = strFilePath.substr(0, endPos);
			startPos = endPos + string::size_type(1);

			if (strFolderPath.rfind(":") == (strFolderPath.size() - 1))
			{
				//����ֻ��ϵͳ�̵�·����������磺D:
				continue;
			}

			struct _stat fileStat = { 0 };
			if (_stat(strFolderPath.c_str(), &fileStat) == 0)
			{
				//�ļ����ڣ��ж���ΪĿ¼�����ļ�
				if (!(fileStat.st_mode & _S_IFDIR))
				{
					//�����ļ��У��򴴽�ʧ���˳�����
					return false;
				}
			}
			else
			{
				//�ļ��в����ڣ�����д���
				if (-1 == _mkdir(strFolderPath.c_str()))
				{
					return false;
				}
			}

			continue;
		}

		break;
	}
	return true;
}

string CLogger::GetNewFile(string strOld)
{
	/// ����С������־�ļ�  
	const char* pLeftDelim = "_";
	const char* pRightDelim = ".log";
	//struct::tm curTimeTM = base::utils::DateTime::getCurDateTime();
	std::string strFilename = strOld;
	size_t leftPos = strFilename.find(pLeftDelim);
	size_t rightPos = strFilename.find(pRightDelim);
	if (leftPos == std::string::npos || rightPos == std::string::npos)
	{
		//error
		return std::string();
	}

	std::string strNewFile("");
	strNewFile.assign(strFilename, 0, leftPos + 1);
	std::string strDate("");
	strDate.assign(strFilename, leftPos + 1, rightPos - leftPos - 1);

	uint32 nSt = (uint32)strFilename.size() - 1;
	for (; nSt >= 0 && strFilename[nSt] >= '0' && strFilename[nSt] <= '9'; nSt--)
	{
	}

	uint32 nIndex = 0;
	for (nSt++; nSt < strFilename.size(); nSt++)
	{
		nIndex = nIndex * 10 + strFilename[nSt] - '0';
	}
	nIndex++;

	std::string strSuffix = "";
	if (nIndex > 0)
	{
		strSuffix = "." + std::to_string(nIndex);
	}

	strNewFile = strNewFile + strDate + ".log" + strSuffix;

	return strNewFile.c_str();
}

void CLogger::RollFile(string strNewFile)
{
	if (m_pStream != nullptr)
	{
		m_pStream->flush();
		m_pStream->close();
		delete m_pStream;
	}
	m_oTypedConf.SetFileName(strNewFile);
	m_pStream = GenFstream(m_oTypedConf.GetFileName());
}

CTypedConfigurations::CTypedConfigurations()
{
	m_bToFile = true;
	m_bToStand = true;
	m_nMaxLogSize = 1024;
}