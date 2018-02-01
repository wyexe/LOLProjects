#ifndef __LOL_BMPFILE_H__
#define __LOL_BMPFILE_H__

#include "VarBase.h"
#include <string>
using namespace std;
#ifndef tstring
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
#endif

struct Points
{
	POINT pts;
	struct Points *next;
};

class CBmpFind
{
public:
	CBmpFind(void);
	virtual ~CBmpFind(void);
	int BindWindow(HWND hWnd);
	void SetPath(LPCTSTR lpszPath);

	void GetScreenData(int dx, int dy);
	bool GetBmpData(LPCTSTR lpszName);
	//释放lpBuf
	int ReleaseLP();
	int GetScrPixel(int x, int y);//获取屏幕颜色
	int GetBmpPixel(int x, int y);//获取位图颜色
	//查找矩形区域内颜色值为 color的点
	//从左往右逐行往下找
	//找到返回1,并将坐标赋值给参数（fx，fy）,找不到则返回0
	int FindColor(int x1, int y1, int x2, int y2, int color, int &fx, int &fy);
	POINT FindFirstPoint(int x1, int y1, int x2, int y2);
	//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图相同
	int Compare(POINT pt);
	//从数据char *lpBufscr中查找*lpBufbmp;
	int FindBmpBuf(int x1, int y1, int x2, int y2, int &fx, int &fy);
	//从矩形x1,y1,x2,y2内查找与位图lpszName完全匹配的起始点
	//若找到,(fx,fy)为图片第一次出现的左上角顶点,并返回1
	//若找不到,(fx,fy)=(-1,-1)并返回0
	int FindPic(int x1, int y1, int x2, int y2, LPCTSTR lpszName, int &fx, int &fy);
	//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图条件匹配
	//忽略位图backcolor颜色点，颜色偏差deltacolor，匹配精度sim
	//比较成功返回1，否则返回0
	int CompareEx(POINT pt, int backcolor, int deltacolor, double sim);
	int ColorCompare(int colorB, int colorS, int deltacolor);
	//从矩形x1,y1,x2,y2内查找与位图lpszName条件匹配的起始点
	//若有匹配的,(fx,fy)为图片第一次出现的左上角顶点,并返回匹配个数
	//忽略位图backcolor颜色点，颜色偏差deltacolor，匹配精度sim
	int FindPic(int x1, int y1, int x2, int y2, LPCTSTR lpszName, int backcolor, int deltacolor, double sim, int &fx, int &fy);
	void FromWndPoint(int &x1, int &y1, int &x2, int &y2);
private:
	char   *lpBufscr, *lpBufbmp;
	BITMAP   bb, bm;//bb=屏幕  bm=图片
	DWORD   dwX, dwY;
	HWND	hWnd;
	tstring  sPath;
};


#endif// __LOL_BMPFILE_H__