#pragma once
#include<string>
#include<fstream>
#include<vector>
#include<assert.h>
#include<iostream>

using namespace std;

#define SUCESS 0
#define FILE_OPEN_FAIL 1
//1.�����ļ��� �������ļ� һ�������� һ�������
//2.��ȡһ��
//3.�жϸ��е�״̬
//4.���Ҫ������������ݵõ��µ�����

//���ฺ�����̿��
class IContentProcess
{
private:
	ifstream m_oFileIn;
	ofstream m_oFileOut;
	
protected:
	string m_strSource;
	string m_strDest;

//protected:
	int Init(string strSource, string strDest);
	void Close();
	string GetLine();
	virtual bool IsProcess(string strSource) = 0;
	virtual string ProcessLine(string strSource) = 0;
	virtual bool IsLegalEnd(string strSource) = 0;
	void WriteLine(string strDest);

public:
	int Process(string strSource, string strDest);
};

//�̳е�����ֻ�����ж��ַ����Ƿ�Ҫ�������δ���
class CContentProcess:public IContentProcess
{
private:
	string m_strMark = "ELOG_";
	string m_strReplace = "EXLOG_";
private:
	bool IsProcess(string strSource);
	string ProcessLine(string strSource);
	bool IsLegalEnd(string strSource);
};