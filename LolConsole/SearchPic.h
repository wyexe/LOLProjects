#ifndef __LOL_SEARCHPIC_H__
#define __LOL_SEARCHPIC_H__

#include "BmpFind.h"
#include "../../MyTools/MyTools/CLHead.h"

typedef struct _BmpPoint
{
	int x, y;
}BmpPoint;

class CSearchPic
{
public:
	CSearchPic();
	~CSearchPic();

	static BOOL ExistBmp(LPCWSTR pwszBmpName, BmpPoint& RetPT);
	
	// 获取游戏屏幕坐标
	static BOOL GetGameRect(RECT& r);

	static HWND GetGameHwnd();

private:
	static CBmpFind CBF;
};


#endif// __LOL_SEARCHPIC_H__