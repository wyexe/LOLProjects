#ifndef __LOL_VERCLIENTSOCKET_H__
#define __LOL_VERCLIENTSOCKET_H__

#define  _WIN32_WINDOWS  0x0600
//configuration
#define SERVER_PORT		12365
#define REUSE_OBJECT //use objects pool
#define ENHANCED_STABILITY

#define PACKER_UNPACKER_TYPE	1
#define BOOST_ASIO_HAS_IOCP

#include "../include/st_asio_wrapper_tcp_client.h"
#include "VarBase.h"
#include "ByteBuffer.h"
#include <stack>

using namespace st_asio_wrapper;

extern stack<ByteBuffer> RecvLst;
extern DWORD dwLoseConTick;

class CVerClient : public st_tcp_sclient
{
public:
	CVerClient(st_service_pump& service_pump_) : st_tcp_sclient(service_pump_){ InitLock(); }
public:
	virtual bool on_msg(msg_type& msg)
	{
		Lock();
		static ByteBuffer bf;
		bf.clear();
		bf.append(msg.data(), msg.size());
		RecvLst.push(bf);
		UnLock();
		return true;
	}
	virtual void on_recv_error(const boost::system::error_code& ec)
	{
		connected = false;
		//CPrintLog::PrintLog_W(L"VerClientSocket.h", __LINE__, L"与服务端失去连接……等待重连!");
		//dwLoseConTick = ::GetTickCount();
		//reset();
		return;
	}
	static void InitLock() 
	{ 
		static bool bInit = false;
		if (!bInit)
		{
			bInit = true;
			InitializeCriticalSection(&CrSction);
		}
	}
	static void Lock()
	{
		EnterCriticalSection(&CrSction);
	}

	static void UnLock()
	{
		LeaveCriticalSection(&CrSction);
	}
public:
	static CRITICAL_SECTION CrSction;
};

class CVerClientSocket
{
public:
	CVerClientSocket();
	~CVerClientSocket();

	static BOOL StartSocket(st_service_pump* pst_service_pump, CVerClient* pCVerClient);
	BOOL Start();
	BOOL Stop();

	static DWORD WINAPI RecvThread(LPVOID lpParm);
	BOOL HandleAccount(ByteBuffer& Bf);
	BOOL HandleGroup(ByteBuffer& Bf);
	BOOL HandleLogin(ByteBuffer& Bf);
	BOOL HandleKeepAlive(ByteBuffer& Bf);
	static BOOL CreatePacket(__in DWORD dwCmd, __out ByteBuffer& RetBf);
	BOOL _Send_JoinGroup();
	BOOL _Wait_Login();
	BOOL _Send_LeaveGroup();
	BOOL _Send_IdSealed();
	BOOL _Send_GetAccount();
	BOOL _Send_ClientMsg(LPCWSTR pwszClientMsg);
	BOOL _Send_Record();

	CVerClient* pVerClient;
	st_service_pump* pservice_pump;
private:
	HANDLE hRecvThread;
	BOOL bStop;
};



#endif