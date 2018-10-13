#pragma once
#include<string>
#include<fstream>
#include<vector>
#include<assert.h>
#include<iostream>

using namespace std;

#define SUCESS 0
#define FILE_OPEN_FAIL 1
//1.输入文件名 打开两个文件 一个是输入 一个是输出
//2.读取一行
//3.判断该行的状态
//4.如果要处理处理该行数据得到新的数据

//父类负责流程框架
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

//继承的子类只负责判断字符串是否要处理和如何处理
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