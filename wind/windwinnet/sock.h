#ifndef _SOCK_H_
#define _SOCK_H_
#include "wnet.h"
#include <WinSock2.h>
#include "wnet_opt.h"
using namespace wind;

#define BUF_SIZE 1024

class CSock;
enum EIocpOperation
{
	IOCP_RECV = 0,
	IOCP_SEND,
	IOCP_ACCEPT,
	IOCP_CONNECT,
	IOCP_CLOSE
};

struct SPerHandleData
{
	bool		bListen;
	void*		ptr;
};

struct SPerIoData
{
	OVERLAPPED		stOverlapped;
	SOCKET			hSock;
	EIocpOperation	nOp;
	WSABUF			stWsaBuf;
	char			Buf[BUF_SIZE];
};

struct CConnData;


class CSock
{
public:
	CSock();
	~CSock();

	//void Reset();

	inline void SetSock(SOCKET hSock) { m_hSock = hSock; }

	inline SOCKET GetSock() { return m_hSock; }

	//void AttachRecvBuf(char* pRecvBuf, uint32 dwRecvBufSize);

	//uint32 DetachRecvBuf(char** ppRecvBuf);

	//void AttachSendBuf(char* pSendBuf, uint32 dwSendBufSize);

	//uint32 DetachSendBuf(char** ppSendBuf);

	//void SetConnData(CConnData* pConnData);

	//inline uint32 GetConnectionID();

	//CConnData *GetConnData() { return this->m_pConnData; }

	//inline void SetPacketParser(IPacketParser* poPacketParser) { m_poPacketParser = poPacketParser; }

	inline void SetConnect(bool bConnect) { m_bConnect = bConnect; }

	inline bool IsConnect()
	{
		return m_bConnect;
	}

	void Send(const char* pData, uint32 dwLen);

	int32 SyncSend(const char * pData, uint32 dwLen);

	//int32 AsyncSend(const char* pData, uint32 dwLen);

	void Close(); 

	bool AssociateWithIocp();

	void OnRecv(DWORD dwBytes);

	bool PostRecv();

	void OnSend(DWORD dwBytes);

	//void OnClose(bool bPassive = true); 

	//void DoClose();

	inline void IncPostSend()
	{
		InterlockedIncrement((long*)&m_dwPostSend);
	}

	inline void DecPostSend()
	{
		InterlockedDecrement((long*)&m_dwPostSend);
	}

	uint32 GetPostSend(void)
	{
		return InterlockedExchange((long*)&m_dwPostSend, m_dwPostSend);
	}

	inline void IncPostRecv()
	{
		InterlockedIncrement((long*)&m_dwPostRecv);
	}

	inline void DecPostRecv()
	{
		InterlockedDecrement((long*)&m_dwPostRecv);
	}

	uint32 GetPostRecv(void)
	{
		return InterlockedExchange((long*)&m_dwPostRecv, m_dwPostRecv);
	}

	//inline uint32 GetSendBufFree(void) { return m_dwSendBufSize - m_nDataSend; }

	//inline uint32  GetSendBufSize() { return m_dwSendBufSize; }

	//inline uint32  GetRecvBufSize() { return m_dwRecvBufSize; }

	//void SetSendStrategy(const SSendStrategy &stSendStrategy) { m_stSendStrategy = stSendStrategy; }
protected:

	bool PostSend();

	//bool PostClose(SOCKET hSock);

	//void OnError(int32 nSDError, int32 nSysError);

protected:
	SOCKET				m_hSock;
	bool				m_bConnect;
	//char*				m_pSendBuf;
	//char*				m_pRecvBuf;
	//uint32				m_dwRecvBufSize;
	//uint32				m_dwSendBufSize;
	//int32				m_nDataReceived;
	//int32				m_nDataSend;
	SPerHandleData		m_stPerHandleData;
	SPerIoData			m_stRecvIoData;
	SPerIoData			m_stSendIoData;
	//SPerIoData			m_stCloseIoData;

	CConnData*			m_pConnData;

	//IPacketParser*		m_poPacketParser;

	uint32 m_dwPostSend;
	uint32 m_dwPostRecv;
};
#endif // 
