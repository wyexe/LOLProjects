#ifndef __LOL_VERIFICATIONSYSTEM_H__
#define __LOL_VERIFICATIONSYSTEM_H__

#include <winsock2.h>
#include "LoginManger.h"
#include "ByteBuffer.h"

#include <MSWSock.h>
#pragma comment(lib,"ws2_32.lib")

typedef struct _GroupKeepAliveData
{
	SockClientGroup Captain;
	SockClientGroup Member;
}GroupKeepAliveData;

typedef enum _em_GroupPlayer
{
	em_GroupPlayer_Captain,
	em_GroupPlayer_Member,
	em_GroupPlayer_None,
	em_GroupPlayer_Error
}em_GroupPlayer;

extern vector<ServerAccountInfo> AccountList;
extern CLLock AccountListLock;

class CVerificationSystem
{
public:
	CVerificationSystem();
	~CVerificationSystem();

	//
	static BOOL RechargeLoginUser(SockLoginData& SLD, LPCWSTR pwszIp, __out ByteBuffer& RetBf);

	//
	static BOOL GetCardInfo_By_LoginUser(SockLoginData& SLD, LPCWSTR pwszIp, __out ByteBuffer& RetBf);

	//
	static BOOL InitCardInfo();

	// 
	static BOOL SetSockKeepCon(__in const SockClientGroup& ClientGroup);

	//
	static BOOL KeepAlive(DWORD dwCmd, SockClientGroup& ClientGroup, LPCWSTR pwszIp, __out ByteBuffer& RetBf);

	////////ื้อล//////////////////////////////////////////////////////////////////

	static BOOL _InitSockClientGroup(SockClientGroup& ClientGroup);

	//
	static BOOL _InitGroup();

	static BOOL _CreateGroupServer(Srt_ServerGroup* pServerGroup);

	static BOOL _CreateGroupMap(vector<Map_GroupMember>& vMaplst, Srt_MapGroup* pMapGroup);

	static Server_GroupMember* _ExistGroupServer(LPCWSTR pwszServerName);

	static Map_GroupMember* _ExistGroupMap(Server_GroupMember* pGroupServer, LPCWSTR pwszMapName);

	static em_GroupPlayer _ExistGroupUser(Map_GroupMember* pGroupMap, SockClientGroup& ClientGroup);

	static BOOL _HandleGroup(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _JoinGroup(Map_GroupMember* pMapGroup, __in const ServerAccountInfo& __ServerAccountInfo, SockClientGroup& ClientGroup);

	static BOOL _HandleServerStatus(__in const SockClientGroup& ClientGroup, BOOL bLeave);

	static BOOL _RefushGroupMember_To_Captain(Map_GroupMember* pMapGroup, const SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _RefushGroupCaptain(Map_GroupMember* pMapGroup, const SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _InitHero(__in vector<int>& vlst, __out SockClientGroup& ClientGroup);

	static BOOL _LeaveGroup(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _AddWaitGroupMember(__in Map_GroupMember* pMapGroup, __in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _JoinWaitList(__in SockClientGroup& SCG);

	static BOOL _DeleteGroupList(__in SockClientGroup& ClientGroup);

	static BOOL _IdSealed(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _GetAccount(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _ClientMsg(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _GameRecord(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf);

	static BOOL _RefushLoseCon();
public:
	static vector<Server_GroupMember> GroupLst;
	static vector<SockClientGroup> WaitGroupLst;
	static vector<SockKeepConSrc> SocketConectLst;
};

#endif// __LOL_VERIFICATIONSYSTEM_H__