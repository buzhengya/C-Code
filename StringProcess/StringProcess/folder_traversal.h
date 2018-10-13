#pragma once
#include<string>
#include<Windows.h>
#include<vector>
#include "content_process.h"

using namespace std;

#define SUCCESS 0
#define EXIST 0
#define NOT_EXIST -1
#define FAIL -1
#define TRUE 1
#define FALSE 0
class IFolderTraversal
{

public:
	virtual int FolderTraversal(string strPath, int nRecur);
	virtual int PathIsExist(string strPath);
	virtual int FileIsExist(string strPath);
	virtual int DirIsExist(string strPath);
	virtual int CreateDir(string strPath);
	virtual int CreateFiles(string strPath);
	virtual void FileProcess(string strSourcePath) = 0;

	virtual vector<string> PathSeparate(string strPath);
	virtual int CreateDirRec(string strPath);
	virtual int CreateFileRec(string strPath);

	virtual void Process() = 0;
};

class CFolderTraversal :public IFolderTraversal
{
private:
	string m_strRoot;
	string m_strSource;
	string m_strDest;

	string GetDestPath(string strSourcePath);
	bool IsProcess(string strSourcePath);
public:
	CFolderTraversal(string strRoot, string strSource, string strDest);
	void FileProcess(string strSourcePath);

	void Process();
};