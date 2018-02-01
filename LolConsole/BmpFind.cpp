#include "BmpFind.h"
#include "VarBase.h"

CBmpFind::CBmpFind(void)
{
	lpBufscr = lpBufbmp = NULL;
	hWnd = NULL;
}


CBmpFind::~CBmpFind(void)
{
}

int CBmpFind::BindWindow(HWND hWnd)
{
	this->hWnd = hWnd;
	return 1;
}

void CBmpFind::SetPath(LPCTSTR lpszPath)
{
	sPath = lpszPath;
	if (sPath.length() && sPath[sPath.length() - 1] != (L'\\'))
		sPath += (L'\\');
}

void CBmpFind::GetScreenData(int dx, int dy)
{

	HBITMAP   hBitmap, hOld;
	HDC   hDC, hcDC;
	BITMAPINFO   b;
	HANDLE   hp;

	//	DWORD   dwX,dwY; 
	dwX = GetSystemMetrics(SM_CXSCREEN);
	dwY = GetSystemMetrics(SM_CYSCREEN);
	dwX = min(dwX, (unsigned)dx);
	dwY = min(dwY, (unsigned)dy);
	hDC = ::GetWindowDC(NULL);
	hcDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, dwX, dwY);
	hOld = (HBITMAP)SelectObject(hcDC, hBitmap);
	BitBlt(hcDC, 0, 0, dwX, dwY, hDC, 0, 0, SRCCOPY | CAPTUREBLT);
#define WIDTH_BYTES(bits) (((bits) + 31) / 32*4) 

	bb.bmWidth = dwX;
	bb.bmHeight = dwY;
	bb.bmPlanes = 1;
	bb.bmWidthBytes = WIDTH_BYTES(bb.bmWidth * 24);
	bb.bmBitsPixel = 24;
	bb.bmType = 0;
	b.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	b.bmiHeader.biWidth = dwX;
	b.bmiHeader.biHeight = dwY;
	b.bmiHeader.biPlanes = 1;
	b.bmiHeader.biBitCount = 24;
	b.bmiHeader.biCompression = BI_RGB;
	b.bmiHeader.biSizeImage = 0;
	b.bmiHeader.biXPelsPerMeter = 0;
	b.bmiHeader.biYPelsPerMeter = 0;
	b.bmiHeader.biClrUsed = 0;
	b.bmiHeader.biClrImportant = 0;
	b.bmiColors[0].rgbBlue = 8;
	b.bmiColors[0].rgbGreen = 8;
	b.bmiColors[0].rgbRed = 8;
	b.bmiColors[0].rgbReserved = 0;

	hp = GetProcessHeap();
	lpBufscr = (char *)HeapAlloc(hp, HEAP_ZERO_MEMORY, bb.bmHeight*bb.bmWidth * 4);
	GetDIBits(hcDC, hBitmap, 0, dwY, lpBufscr, &b, DIB_RGB_COLORS);

	ReleaseDC(NULL, hDC);
	DeleteDC(hcDC);
	DeleteObject(hBitmap);
	DeleteObject(hOld);
	//	HeapFree(hp,0,lpBufscr);

}

bool CBmpFind::GetBmpData(LPCTSTR lpszName)
{
	//OutputDebugString(lpszName);
	HANDLE hp;
	HDC hdc, hdcmem;
	HBITMAP hBm;
	BITMAPINFO   b;
	hBm = (HBITMAP)LoadImageW(NULL, lpszName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (hBm == NULL)
	{
		CPrintLog::PrintLog_W(L"BmpFind.cpp", __LINE__, L"你大爷的, 这个图片木有啊! 路径:%s", lpszName);
		return false;
	}
	GetObject(hBm, sizeof(BITMAP), (LPVOID)&bm);
	hdc = ::GetWindowDC(NULL);
	hdcmem = CreateCompatibleDC(hdc);
	SelectObject(hdcmem, hBm);
	BitBlt(hdcmem, 0, 0, bm.bmWidth, bm.bmHeight, hdcmem, 0, 0, SRCCOPY | CAPTUREBLT);

	b.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	b.bmiHeader.biWidth = bm.bmWidth;
	b.bmiHeader.biHeight = bm.bmHeight;
	b.bmiHeader.biPlanes = 1;
	b.bmiHeader.biBitCount = 24;
	b.bmiHeader.biCompression = BI_RGB;
	b.bmiHeader.biSizeImage = 0;
	b.bmiHeader.biXPelsPerMeter = 0;
	b.bmiHeader.biYPelsPerMeter = 0;
	b.bmiHeader.biClrUsed = 0;
	b.bmiHeader.biClrImportant = 0;
	b.bmiColors[0].rgbBlue = 8;
	b.bmiColors[0].rgbGreen = 8;
	b.bmiColors[0].rgbRed = 8;
	b.bmiColors[0].rgbReserved = 0;

	hp = GetProcessHeap();
	lpBufbmp = (char *)HeapAlloc(hp, HEAP_ZERO_MEMORY, bm.bmHeight*bm.bmWidth * 4);
	GetDIBits(hdc, hBm, 0, bm.bmHeight, lpBufbmp, &b, DIB_RGB_COLORS);

	DeleteObject(hBm);
	DeleteDC(hdc);
	DeleteDC(hdcmem);
	return true;
}

//释放lpBuf
int CBmpFind::ReleaseLP()
{
	HANDLE hp = GetProcessHeap();
	HeapFree(hp, 0, lpBufscr);
	HeapFree(hp, 0, lpBufbmp);
	return 0;
}


int CBmpFind::GetScrPixel(int x, int y)//获取屏幕颜色
{

	if (x < 0 || x >= (int)dwX || y < 0 || y >= (int)dwY)return-1;
	BYTE   bBlue, bGreen, bRed;
	bBlue = lpBufscr[bb.bmWidthBytes*(dwY - y - 1) + bb.bmBitsPixel*x / 8 + 0];
	bGreen = lpBufscr[bb.bmWidthBytes*(dwY - y - 1) + bb.bmBitsPixel*x / 8 + 1];
	bRed = lpBufscr[bb.bmWidthBytes*(dwY - y - 1) + bb.bmBitsPixel*x / 8 + 2];

	COLORREF   color = RGB(bRed, bGreen, bBlue);
	return int(color);
}

int CBmpFind::GetBmpPixel(int x, int y)//获取位图颜色
{
	if (x<0 || x>(int)dwX || y<0 || y>(int)dwY)return-1;
	BYTE   bBlue, bGreen, bRed;
	bBlue = lpBufbmp[bm.bmWidthBytes*(bm.bmHeight - y - 1) + bm.bmBitsPixel*x / 8 + 0];
	bGreen = lpBufbmp[bm.bmWidthBytes*(bm.bmHeight - y - 1) + bm.bmBitsPixel*x / 8 + 1];
	bRed = lpBufbmp[bm.bmWidthBytes*(bm.bmHeight - y - 1) + bm.bmBitsPixel*x / 8 + 2];

	COLORREF   color = RGB(bRed, bGreen, bBlue);
	return int(color);
	//return lpBufbmp[1];
}


//查找矩形区域内颜色值为 color的点
//从左往右逐行往下找
//找到返回1,并将坐标赋值给参数（fx，fy）,找不到则返回0
int CBmpFind::FindColor(int x1, int y1, int x2, int y2, int color, int &fx, int &fy)
{
	GetScreenData(x2, y2);
	fx = fy = -1;
	int xtmp = x1;
	for (; y1 < y2; y1++)
	{
		for (x1 = xtmp; x1 <= x2; x1++)
		{
			if (GetScrPixel(x1, y1) == color)
			{
				fx = x1;
				fy = y1;
				ReleaseLP();
				return 1;
			}
		}
	}
	ReleaseLP();
	return 0;
}

POINT CBmpFind::FindFirstPoint(int x1, int y1, int x2, int y2)
{
	int color = GetBmpPixel(0, 0);
	POINT pt;
	pt.x = -1;
	pt.y = -1;
	int fx, fy, xtmp = x1;;
	fx = fy = -1;
	for (; y1 < y2; y1++)
	for (x1 = xtmp; x1 < x2; x1++)
	{
		if (GetScrPixel(x1, y1) == color)
		{
			pt.x = x1;
			pt.y = y1;
			return pt;
		}
	}
	return pt;
}


//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图相同
int CBmpFind::Compare(POINT pt)
{
	int x, y;
	if ((unsigned)bm.bmHeight > dwY - pt.y)return 0;
	for (y = 0; y < bm.bmHeight - 1; y++)
	{
		for (x = 0; x < bm.bmWidth - 1; x++)
		{
			if (GetBmpPixel(x, y) == GetScrPixel(pt.x + x, pt.y + y))
			{

			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

//从数据char *lpBufscr中查找*lpBufbmp;
int CBmpFind::FindBmpBuf(int x1, int y1, int x2, int y2, int &fx, int &fy)
{
	POINT pt, pttmp;
	fx = -1;
	fy = -1;

	pt = FindFirstPoint(x1, y1, x2, y2);
	if (Compare(pt)){ fx = pt.x; fy = pt.y; return 1; }

	else//出现第一个点找到后却并不与位图吻合
	{
		pttmp.y = pt.y + 1;
		while (pt = FindFirstPoint(pt.x + 1, pt.y, x2, pt.y + 1), pt.x >= 0)
		{
			if (Compare(pt)){ fx = pt.x; fy = pt.y; return 1; }
		}
		while (pt = FindFirstPoint(x1, pttmp.y, x2, y2), pt.x >= 0)
		{
			if (Compare(pt)){ fx = pt.x; fy = pt.y; return 1; }
			else
			{
				pttmp.y = pt.y + 1;
				while (pt = FindFirstPoint(pt.x + 1, pt.y, x2, pt.y + 1), pt.x >= 0)
				if (Compare(pt)){ fx = pt.x; fy = pt.y; return 1; }
			}
		}
	}
	return 0;
}

//从矩形x1,y1,x2,y2内查找与位图lpszName完全匹配的起始点
//若找到,(fx,fy)为图片第一次出现的左上角顶点,并返回1
//若找不到,(fx,fy)=(-1,-1)并返回0
int CBmpFind::FindPic(int x1, int y1, int x2, int y2, LPCTSTR lpszName, int &fx, int &fy)
{
	Sleep(50);
	FromWndPoint(x1, y1, x2, y2);
	if (!GetBmpData((sPath + lpszName).c_str()))
		return 0;

	OutputDebugString((sPath + lpszName).c_str());
	GetScreenData(x2, y2);

	FindBmpBuf(x1, y1, x2, y2, fx, fy);

	ReleaseLP();
	if (fx > 0)
	{
		if (hWnd)
		{
			POINT pt = { fx, fy };
			ScreenToClient(hWnd, &pt);
			fx = pt.x;
			fy = pt.y;
		}
		return 1;
	}
	return 0;

}

//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图条件匹配
//忽略位图backcolor颜色点，颜色偏差deltacolor，匹配精度sim
//比较成功返回1，否则返回0
int CBmpFind::CompareEx(POINT pt, int backcolor, int deltacolor, double sim)
{
	int x, y, colorB, colorS, count = 0, total = 0;
	double p;
	if ((unsigned)bm.bmHeight > dwY - pt.y)return 0;
	total = bm.bmHeight *bm.bmWidth;
	for (y = 0; y < bm.bmHeight - 1; y++)
	for (x = 0; x < bm.bmWidth - 1; x++)
	{
		colorB = GetBmpPixel(x, y);
		colorS = GetScrPixel(pt.x + x, pt.y + y);
		if (colorB == colorS || colorB == backcolor);
		else
		{
			if (ColorCompare(colorB, colorS, deltacolor) == 0)
				count++;
		}
	}
	p = (double)(total - count) / total;
	//	DbgPrint("%d %d %d %d",count,total,(int)(p*100), (int)(sim*100));
	if (p<sim)
		return 0;
	else
		return 1;
}

int CBmpFind::ColorCompare(int colorB, int colorS, int deltacolor)
{
	if (abs(GetRValue((COLORREF)colorB) - GetRValue((COLORREF)colorS) > GetRValue((COLORREF)deltacolor))
		|| abs(GetGValue((COLORREF)colorB) - GetGValue((COLORREF)colorS) > GetGValue((COLORREF)deltacolor))
		|| abs(GetBValue((COLORREF)colorB) - GetBValue((COLORREF)colorS) > GetBValue((COLORREF)deltacolor))){
		return 0;
	}
	return 1;
}

//从矩形x1,y1,x2,y2内查找与位图lpszName条件匹配的起始点
//若有匹配的,(fx,fy)为图片第一次出现的左上角顶点,并返回匹配个数
//忽略位图backcolor颜色点，颜色偏差deltacolor，匹配精度sim
int CBmpFind::FindPic(int x1, int y1, int x2, int y2, LPCTSTR lpszName, int backcolor, int deltacolor, double sim, int &fx, int &fy)
{
	Sleep(50);
	int x = 0, y = 0, color = -1;
	FromWndPoint(x1, y1, x2, y2);
	if (!GetBmpData((sPath + lpszName).c_str()))
		return 0;

	//OutputDebugString((sPath + lpszName).c_str());
	GetScreenData(x2, y2);
	for (y = 0; y < bm.bmHeight - 1; y++)
	{
		for (x = 0; x<bm.bmWidth - 1; x++)
			//if(GetBmpPixel(x,y)!=backcolor)
		{
			color = GetBmpPixel(x, y);
			break;
		}
		if (color>0)break;
	}
	Points *head, *tail, *p, *q;
	head = tail = p = q = NULL;
	int xtmp = x1, countP = 0, countB = 0;
	for (; y1 < y2; y1++)
	for (x1 = xtmp; x1 <= x2; x1++)
	if (GetScrPixel(x1, y1) == color)
	{
		countP++;
		p = (struct Points *)malloc(sizeof(struct Points));
		(p->pts).x = x1 - x;
		(p->pts).y = y1 - y;
		if (head == NULL)q = head = tail = p;
		else
			tail = tail->next;
		tail->next = p;
	}
	if (p)
		p->next = NULL;
	while (q)
	{
		if (CompareEx((q->pts), backcolor, deltacolor, sim))
		{
			countB++;
			if (countB == 1)
			{
				if (hWnd)ScreenToClient(hWnd, &q->pts);
				fx = (q->pts).x;
				fy = (q->pts).y;
			}
		}
		p = q->next;
		free(q);
		q = p;
	}

	ReleaseLP();
	//char shuchu[50] = { 0 };
	//_stprintf_s(shuchu, "%d", countB);
	//OutputDebugString(shuchu);
	return countB;

}

void CBmpFind::FromWndPoint(int &x1, int &y1, int &x2, int &y2)
{
	if (hWnd)
	{
		RECT rc;
		GetWindowRect(hWnd, &rc);
		x1 = rc.left;
		y1 = rc.top;
		x2 = rc.right;
		y2 = rc.bottom;
	}
}