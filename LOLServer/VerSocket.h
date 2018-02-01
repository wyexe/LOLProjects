#ifndef __LOL_VERSOCKET_H__
#define __LOL_VERSOCKET_H__

#define  _WIN32_WINNT  0x0600 
//configuration
#define SERVER_PORT		12365
#define REUSE_OBJECT //use objects pool
#define ENHANCED_STABILITY

#define PACKER_UNPACKER_TYPE	1

#include "VerificationSystem.h"
#include "../include/st_asio_wrapper_server.h"
#include <stack>



using namespace st_asio_wrapper;


////////////定义要使用的结构//////////////////////////////////////////////////////////////
typedef struct _AffairThreadSrc
{
	BOOL bExit;													// 是否退出线程
}AffairThreadSrc;

typedef struct _StackMsg
{
	boost::shared_ptr<st_server_socket> pst_server;				// 共享对象指针
	st_server_socket::msg_type msg;								// 内容结构
}StackMsg;

typedef enum _SocketStatus
{
	SocketStatus_Normal,
	SocketStatus_Pause
}SocketStatus;


//////////////全局变量////////////////////////////////////////////////////////////
extern CRITICAL_SECTION g_CrSection;
extern CRITICAL_SECTION g_LstCrSection;
extern SocketStatus g_SocketStatus;
extern BOOL			g_bCloseClient;

class CMy_st_server : public st_server_socket
{
public:
	CMy_st_server(i_server& server_) : st_server_socket(server_){  };

	virtual bool on_msg(msg_type& msg);
	virtual bool on_msg_handle(msg_type& msg, bool link_down);
	static stack<StackMsg>* GetTaskQueue();			// 获取任务队列栈
private:
};




DWORD WINAPI HandleAffairThread(LPVOID lpParm);					// 事务处理线程
extern BOOL Lol_OutputFile();

//restore configuration
#undef SERVER_PORT
#undef REUSE_OBJECT
#undef ENHANCED_STABILITY

#endif// __LOL_VERSOCKET_H__