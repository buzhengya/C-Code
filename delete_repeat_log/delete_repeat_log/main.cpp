#include<stdio.h>
#include<unordered_set>
#define MAX_LINE 1024

using namespace std;

int delete_repeat();

int main(int argc,char* argv[])
{
	FILE *fp;
	char strLine[MAX_LINE];
	for(int i=1;i<argc;i++)
	{
		if((fp=fopen(argv[i],"r"))==NULL)
		{
			printf("Open %s Failed.\n", argv[i]);
			continue;
		}

		printf("%s:\n\n", argv[i]);

		unordered_set<int> setRowId;
		while (!feof(fp))
		{
			fgets(strLine, MAX_LINE, fp);

			int j = 0;
			while (strLine[j] != '\0')
			{
				if (strLine[j] == ':')
				{
					if (j - 2 >= 0 && strLine[j - 2] == '.'&&strLine[j - 1] == 'h')
					{
						int nRow = j;
						int k = j + 1;
						while (strLine[k] >= '0'&&strLine[k] <= '9') nRow = nRow * 10 + strLine[k++] - '0';
						if (setRowId.find(nRow) == setRowId.end())
						{
							printf("%s\n", strLine);
							setRowId.insert(nRow);
						}
					}
					else if (j - 4 >= 0 && strLine[j - 4] == '.'&&strLine[j - 3] == 'c'&&strLine[j - 2] == 'p'&&strLine[j - 1] == 'p')
					{
						int nRow = j;
						int k = j + 1;
						while (strLine[k] >= '0'&&strLine[k] <= '9') nRow = nRow * 10 + strLine[k++] - '0';
						if (setRowId.find(nRow) == setRowId.end())
						{
							printf("%s\n", strLine);
							setRowId.insert(nRow);
						}
					}
				}
				j++;
			}
		}
	}
	return 0;
}

int delete_repeat()
{
	FILE *fp;
	char strLine[MAX_LINE];
	unordered_set<int> setRowId;

	if ((fp = fopen("C:\\Users\\feng.wang\\Desktop\\error_db-20180830_14.log", "r")) == NULL)
	{
		printf("Open %s Failed.\n", "");
		return -1;
	}

	while (!feof(fp))
	{
		fgets(strLine, MAX_LINE, fp);

		int j = 0;
		while (strLine[j] != '\0')
		{
			if (strLine[j] == ':')
			{
				if (j - 2 >= 0 && strLine[j - 2] == '.'&&strLine[j - 1] == 'h')
				{
					int nRow = j;
					int k = j + 1;
					while (strLine[k] >= '0'&&strLine[k] <= '9') nRow = nRow * 10 + strLine[k++] - '0';
					if (setRowId.find(nRow) == setRowId.end())
					{
						printf("%s\n", strLine);
						setRowId.insert(nRow);
					}
				}
				else if (j - 4 >= 0 && strLine[j - 4] == '.'&&strLine[j - 3] == 'c'&&strLine[j - 2] == 'p'&&strLine[j - 1] == 'p')
				{
					int nRow = j;
					int k = j + 1;
					while (strLine[k] >= '0'&&strLine[k] <= '9') nRow = nRow * 10 + strLine[k++] - '0';
					if (setRowId.find(nRow) == setRowId.end())
					{
						printf("%s\n", strLine);
						setRowId.insert(nRow);
					}
				}
			}
			j++;
		}
	}
	unordered_set<int>::iterator it;
	for (it = setRowId.begin(); it != setRowId.end(); it++)
	{
		printf("%d", *it);
	}
	return 0;
}