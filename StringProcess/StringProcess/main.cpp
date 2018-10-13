#include"content_process.h"
#include<iostream>
#include"folder_traversal.h"

int main()
{
	IContentProcess* oContentProcess=new CContentProcess();
	string strSourceCpp = "C:\\Users\\feng.wang\\Desktop\\TXT\\test.txt";
	string strDestCpp = "C:\\Users\\feng.wang\\Desktop\\TXT\\test_new.txt";
	oContentProcess->Process(strSourceCpp, strDestCpp);



	IFolderTraversal* poFolderTraversal = new CFolderTraversal("E:\\project\\y-server\\servers\\authserver", "authserver", "authserver_new");
	poFolderTraversal->Process();
	poFolderTraversal = new CFolderTraversal("E:\\project\\y-server\\servers\\common", "common", "common_new");
	poFolderTraversal->Process();
	poFolderTraversal = new CFolderTraversal("E:\\project\\y-server\\servers\\dbserver", "dbserver", "dbserver_new");
	poFolderTraversal->Process();
	poFolderTraversal = new CFolderTraversal("E:\\project\\y-server\\servers\\gameserver", "gameserver", "gameserver_new");
	poFolderTraversal->Process();
	poFolderTraversal = new CFolderTraversal("E:\\project\\y-server\\servers\\gateserver", "gateserver", "gateserver_new");
	poFolderTraversal->Process();
	poFolderTraversal = new CFolderTraversal("E:\\project\\y-server\\servers\\mailserver", "mailserver", "mailserver_new");
	poFolderTraversal->Process();
	poFolderTraversal = new CFolderTraversal("E:\\project\\y-server\\servers\\managerserver", "managerserver", "managerserver_new");
	poFolderTraversal->Process();
	poFolderTraversal = new CFolderTraversal("E:\\project\\y-server\\servers\\publicserver", "publicserver", "publicserver_new");
	poFolderTraversal->Process();
	system("pause");
	return 0;
}