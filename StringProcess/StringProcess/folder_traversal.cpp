#include "folder_traversal.h"

#include<io.h>

int IFolderTraversal::FolderTraversal(string strPath, int nRecur)
{
	_finddata_t FileInfo;
	int nHandle = _findfirst(strPath.c_str(), &FileInfo);
	if (nHandle == -1) return FAIL;
	do
	{
		string strCurPath = strPath.substr(0, strPath.size() - 1) + FileInfo.name;
		if(FileInfo.attrib ==_A_SUBDIR)
		{
			if (strcmp(FileInfo.name, ".") == 0 || strcmp(FileInfo.name, "..") == 0) continue;
			if (nRecur == TRUE && FolderTraversal(strCurPath + "\\*", nRecur) == FAIL)
			{
				return FAIL;
			}
		}
		else if (FileInfo.attrib = _A_ARCH)
		{
			FileProcess(strCurPath);
		}
	} while (!_findnext(nHandle, &FileInfo));
	_findclose(nHandle);
	return SUCCESS;
}

int IFolderTraversal::PathIsExist(string strPath)
{
	DWORD ftyp = GetFileAttributesA(strPath.c_str());
	if(ftyp==INVALID_FILE_ATTRIBUTES)
	{
		return NOT_EXIST;
	}
	return EXIST;
}

int IFolderTraversal::FileIsExist(string strPath)
{
	DWORD ftyp = GetFileAttributesA(strPath.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		return NOT_EXIST;
	}
	if (ftyp&FILE_ATTRIBUTE_ARCHIVE)
	{
		return EXIST;
	}
	return NOT_EXIST;
}

int IFolderTraversal::DirIsExist(string strPath)
{
	DWORD ftyp = GetFileAttributesA(strPath.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		return NOT_EXIST;
	}
	if (ftyp&FILE_ATTRIBUTE_DIRECTORY)
	{
		return EXIST;
	}
	return NOT_EXIST;
}

int IFolderTraversal::CreateDir(string strPath)
{
	if (DirIsExist(strPath) == EXIST) return SUCCESS;
	bool bFlag = CreateDirectory(strPath.c_str(), NULL);
	if (bFlag) return SUCCESS;
	return FAIL;
}

int IFolderTraversal::CreateFiles(string strPath)
{
	if (FileIsExist(strPath) == EXIST)
	{
		return SUCCESS;
	}
	HANDLE handle = CreateFile(strPath.c_str(),0,0,0,CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE,0);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return FAIL;
	}
	return SUCCESS;
}

vector<string> IFolderTraversal::PathSeparate(string strPath)
{
	vector<string> vecStr;
	int nSt = 0, nEnd = 0;
	while (nSt<strPath.size())
	{
		while (nEnd<strPath.size() && strPath[nEnd] != '\\')
		{
			nEnd++;
		}
		vecStr.push_back(strPath.substr(nSt, nEnd - nSt));
		nSt = ++nEnd;
	}
	return vecStr;
}

int IFolderTraversal::CreateDirRec(string strPath)
{
	vector<string> vecStrPath = PathSeparate(strPath);
	if (vecStrPath.size() <= 1) return FAIL;
	string strPathTemp = vecStrPath[0];
	for(int i=1;i<vecStrPath.size();i++)
	{
		strPathTemp += "\\" + vecStrPath[i];
		if(CreateDir(strPathTemp)==FAIL)
		{
			return FAIL;
		}
	}
	return SUCCESS;
}

int IFolderTraversal::CreateFileRec(string strPath)
{
	vector<string> vecStrPath = PathSeparate(strPath);
	if (vecStrPath.size() <= 1) return FAIL;
	string strPathTemp = vecStrPath[0];
	for (int i = 1; i<vecStrPath.size() - 1; i++)
	{
		strPathTemp += "\\" + vecStrPath[i];
		if (CreateDir(strPathTemp) == FAIL)
		{
			return FAIL;
		}
	}
	strPathTemp += "\\" + vecStrPath[vecStrPath.size() - 1];
	if(CreateFiles(strPathTemp) == FAIL)
	{
		return FAIL;
	}
	return SUCCESS;
}

string CFolderTraversal::GetDestPath(string strSourcePath)
{
	string strDestPath = m_strRoot.substr(0, m_strRoot.size() - m_strSource.size()) + m_strDest;
	strDestPath += strSourcePath.substr(m_strRoot.size(), strSourcePath.size() - m_strRoot.size());
	return strDestPath;
}

bool CFolderTraversal::IsProcess(string strSourcePath)
{
	//忽略resource.h文件 再改
	string strIgnor = "resource.h";
	int k = strSourcePath.size() - 1;
	int j = strIgnor.size() - 1;
	while (k >= 0 && j >= 0 && strSourcePath[k] == strIgnor[j])
	{
		k--;
		j--;
	}
	if (j < 0) return false;

	int nStrSize = strSourcePath.size();
	int i = nStrSize - 1;
	while(i>=0&&strSourcePath[i]!='.')
	{
		i--;
	}
	i++;
	string strSuf = strSourcePath.substr(i, nStrSize - i);
	if(strSuf=="cpp"||strSuf=="h")
	{
		return true;
	}
	return false;
}

CFolderTraversal::CFolderTraversal(string strRoot, string strSource, string strDest)
{
	m_strRoot = strRoot;
	m_strSource = strSource;
	m_strDest = strDest;
}

void CFolderTraversal::FileProcess(string strSourcePath)
{
	if(!IsProcess(strSourcePath))
	{
		return;
	}

	string strDestPath = GetDestPath(strSourcePath);
	if (CreateFileRec(strDestPath) == FAIL)
	{
		cout << "创建文件失败" << strDestPath << endl;
		return;
	}

	IContentProcess* oContentProcess = new CContentProcess();
	oContentProcess->Process(strSourcePath, strDestPath);
}

void CFolderTraversal::Process()
{
	FolderTraversal(m_strRoot+"\\*", TRUE);
}
