#pragma once
#include<iostream>
#include"define.h"
#include<string.h>

class CString
{
	friend CString operator+(const CString& oStr1,const CString& oStr2);

public:
	//���캯��
	CString();
	CString(const char *);

	//�������캯��
	CString(const CString &ostr);

	//������ֵ�����
	CString& operator=(const CString& ostr);

	CString& operator=(char pcStr);//���˸����غ����� ����ʽ�����Ϸ� CString oStr=NULL; �ж�����غ���
	CString& operator=(const char * pcStr);

	// element access ���ط��������
	char& operator[](uint32 nPos);
	const char& operator[](uint32 nPos)const;


	//��������
	~CString();

	// capacity
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;
	//���ڸ�ֵ�ͺϲ����� �ַ����Ķ��ڴ�������ָ����С ���Ұ�ԭ���ַ������ݸ��Ƶ��µ��ڴ���� 
	void ReSize(uint32 nSize = 0);
	bool Empty() const;
	void Clear();

	// operations
	char* Data()const;

private:
	// ����ֵ���� ���Ǵ��뱾������
	void _Assign(const char * pcStr, const uint32 nSize);

	// memory alloc free
	char* _Alloc(const uint32 nSize);
	void _Free(char* pData);

	inline uint32 _CalCap(const uint32 nSize);

	inline uint32 _Size() const;
	inline uint32 _Capacity() const;

	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};