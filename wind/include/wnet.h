#ifndef _WNET_H_
#define _WNET_H_
#include "base_define.h"
#include "wbase.h"
#include <string>
namespace wind
{
	static const SVersion GNNET_MODULE_VERSION = { 3, 2, 0, 0 };

	const char NET_MODULENAME[] = "wNet";

	const SVersion	NET_VERSION = GNNET_MODULE_VERSION;

	enum NetErrCode {
		NET_BIND_FAIL = -7,
		NET_CONNECT_FAIL = -6,
		NET_SYSTEM_ERROR = -5,
		NET_RECV_ERROR = -4,
		NET_SEND_ERROR = -3,
		NET_SEND_OVERFLOW = -2,    
		NET_PACKET_ERROR = -1,
		NET_SUCCESS = 0
	};

	const uint32 NETIO_COMPLETIONPORT = 0x0001;


	const uint32 NETIO_COMPLETIONPORT_GATE = 0x0002;

	const uint32 NET_THREADED = 0x0004;

	const uint32 NET_NONETHREADED = 0x0008;

	const uint32 NETIO_ASYNCSELECT = 2;

	const uint32 NETIO_EPOLL = 0x0010;

	const uint32 NETIO_EPOLL_GATE = 0x0020;

	const uint32 INVALID_IO_TYPE = 0xFFFFFFFF;

	const uint32 UNKNOWN_SIZE = 0xFFFFFFFF;

	class IPacketParser;
	class ISession;
	class ISessionFactory;

	class IConnection
	{
	public:
		virtual bool WAPI IsConnected(void) = 0;

		virtual void WAPI Send(const char* pBuf, uint32 dwLen, bool bAtOnce = false) = 0;

		virtual void WAPI SetOpt(uint32 dwType, void* pOpt) = 0;

		virtual void WAPI Disconnect(void) = 0;

		virtual const uint32 WAPI GetRemoteIP(void) = 0;

		virtual const char* WAPI GetRemoteIPStr(void) = 0;

		virtual uint16 WAPI GetRemotePort(void) = 0;

		virtual const uint32 WAPI GetLocalIP(void) = 0;

		virtual const char* WAPI GetLocalIPStr(void) = 0;

		virtual uint16 WAPI GetLocalPort(void) = 0;

		virtual uint32 WAPI GetSendBufFree(void) = 0;
	};

	class IListener
	{
	public:
		virtual void WAPI SetPacketParser(IGNPacketParser* poPacketParser) = 0;

		virtual void WAPI SetSessionFactory(ISessionFactory* poSessionFactory) = 0;

		virtual void WAPI SetBufferSize(uint32 dwRecvBufSize, uint32 dwSendBufSize) = 0;

		virtual void WAPI SetOpt(uint32 dwType, void* pOpt) = 0;

		virtual bool WAPI Start(const char* pszIP, uint16 wPort, bool bReUseAddr = true) = 0;

		virtual bool WAPI Stop(void) = 0;

		virtual void WAPI Release(void) = 0;

		virtual const uint32 WAPI GetLocalIP(void) = 0;

		virtual uint16 WAPI GetLocalPort(void) = 0;
	};

	class IGNConnector
	{
	public:
		virtual void WAPI SetPacketParser(IGNPacketParser* poPakcetParser) = 0;

		virtual void WAPI SetSession(IGNSession* poSession) = 0;

		virtual void WAPI SetBufferSize(uint32 dwRecvBufSize, uint32 dwSendBufSize) = 0;

		virtual bool WAPI Connect(const char* pszIP, uint16 wPort) = 0;

		virtual bool WAPI ReConnect(void) = 0;

		virtual void WAPI Release(void) = 0;

		virtual void WAPI SetOpt(uint32 dwType, void* pOpt) = 0;
	};

	class INet : public IBase
	{
	public:

		virtual IGNConnector* WAPI CreateConnector(uint32 dwNetIOType) = 0;

		virtual IListener* WAPI CreateListener(uint32 dwNetIOType) = 0;

		virtual bool WAPI Run(int32 nCount = -1) = 0;

		virtual std::string WAPI DebugInfo() = 0;
	};

	class IGNPacketParser
	{
	public:
		virtual int32 WAPI ParsePacket(const char* pBuf, uint32 dwLen) = 0;
	};

	class IGNSession
	{
	public:
		virtual void WAPI SetConnection(IConnection* poConnection) = 0;

		virtual void WAPI OnEstablish(void) = 0;

		virtual void WAPI OnTerminate(void) = 0;

		virtual bool WAPI OnError(int32 nModuleErr, int32 nSysErr) = 0;

		virtual void WAPI OnRecv(const char* pBuf, uint32 dwLen) = 0;

#ifdef GNNET_HAS_SEND_REPORT

		virtual void WAPI OnSend(const char* pBuf, uint32 dwLen)
		{}
#endif

		virtual void WAPI Release(void) = 0;
	};

	class ISessionFactory
	{
	public:
		virtual IGNSession* WAPI CreateSession(IConnection* poConnection) = 0;
	};

	INet* WAPI NetGetModule(const SVersion* pstVersion);


	typedef INet* (WAPI *PFN_GNNetGetModule)(const SVersion* pstVersion);


	void WAPI NetSetOpt(uint32 dwType, void* pOpt);
}
#endif // _WNET_H_
