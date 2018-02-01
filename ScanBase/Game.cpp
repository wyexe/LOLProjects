// Game.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanBase.h"
#include "Game.h"
#include "afxdialogex.h"
#include <vector>
#include "SearchBase.h"

using namespace std;

// CGame 对话框

IMPLEMENT_DYNAMIC(CGame, CDialogEx)

CGame::CGame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGame::IDD, pParent)
{

}

CGame::~CGame()
{
}

void CGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGame, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CGame::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGame 消息处理程序
BOOL CGame::OnInitDialog()
{
	return TRUE;
}

/*

union Base  
{
	DWORD   address;
	BYTE    data[4];
};

DWORD ScanAddress(HANDLE process, char *markCode,DWORD distinct = 1, DWORD findMode = 1,LPDWORD offset = NULL)
{

	const DWORD beginAddr = 0x00400000;

	const DWORD endAddr = 0x7FFFFFFF;

	const DWORD pageSize = 4096;

	if (strlen(markCode) % 2 != 0) return 0;
	int len = strlen(markCode) / 2;

	BYTE *m_code = new BYTE[len];
	for (int i = 0; i < len; i++){
		char c[] = {markCode[i*2], markCode[i*2+1], '\0'};
		m_code[i] = (BYTE)::strtol(c, NULL, 16);
	}
	/////////////////////////查找特征码/////////////////////
	BOOL _break = FALSE;
	//用来保存在第几页中的第几个找到的特征码
	int curPage = 0;
	int curIndex = 0;
	Base base;
	//每页读取4096个字节
	BYTE page[pageSize];
	DWORD tmpAddr = beginAddr;
	while (tmpAddr <= endAddr - len)
	{
		::ReadProcessMemory(process, (LPCVOID)tmpAddr, &page, pageSize, 0);
		//在该页中查找特征码
		for (int i = 0; i < pageSize; i++)
		{
			for (int j = 0; j < len; j++)
			{
				//只要有一个与特征码对应不上则退出循环
				if (m_code[j] != page[i + j])
				{
					break;
				}
				//找到退出所有循环
				if (j == len - 1)
				{
					_break = TRUE;
					if (!findMode)

					{
						curIndex = i;
						base.data[0] = page[curIndex-distinct-4];
						base.data[1] = page[curIndex-distinct-3];
						base.data[2] = page[curIndex-distinct-2];
						base.data[3] = page[curIndex-distinct-1];
					}
					else
					{
						curIndex = i + j;
						base.data[0] = page[curIndex+distinct+1];
						base.data[1] = page[curIndex+distinct+2];
						base.data[2] = page[curIndex+distinct+3];
						base.data[3] = page[curIndex+distinct+4];
					}
					break;
				}
			}
			if (_break)
			{
				break;
			}
		}
		if (_break)
		{
			break;
		}
		curPage++;
		tmpAddr += pageSize;
	}
	if(offset != NULL)
	{
		*offset = curPage * pageSize + curIndex + beginAddr;
	}
	//return tmpAddr + curIndex;
	return base.address;
}

/ ************************************************************************ /
/ * 函数说明：根据特征码扫描call地址
/ * 参数一：process 要查找的进程
/ * 参数二：markCode 特征码字符串,不能有空格
/ * 参数三：特征码离基址的距离，默认距离：1
/ * 参数四：findMode 扫描方式，找到特征码后，默认为：1
/ *                  0：往上找基址
/ *                  1：往下找基址
/ ************************************************************************ /
DWORD ScanCall(HANDLE process, char *markCode,
	DWORD distinct = 1, DWORD findMode = 1,DWORD * retvalue = 0)
{
	DWORD offset;
	DWORD call = ScanAddress(process, markCode, distinct, findMode, &offset);
	*retvalue = offset;
	call += offset;
	if(findMode) call = call + 5 + distinct;
	else call = call - distinct;
	return call;
}
vector<CString> FileList;
typedef struct _ScanCode
{
	BOOL IsBase;//是否基址
	CString Title;//标题
	CString FeatureCode;//特征码
	int Offset;//偏移
	BOOL IsDown;//是否向下搜索
	BOOL IsConst;//是否常量
	int FindCount;//查找次数
	DWORD addr;
}ScanCode;
ScanCode scBuff[100];
void ReadFile()
{
	CStdioFile file_src;
	FileList.clear();
	if (file_src.Open("1.txt",CFile::typeBinary | CFile::modeRead))
	{
		CString strTemp;
		while(file_src.ReadString(strTemp))
		{
			FileList.push_back(strTemp);
		}

		file_src.Close();
	}
}

CString GetParm(CString & str)
{
	int l = str.Find(",");
	CString retStr;
	if (l >= 0)
	{
		retStr = str.Mid(0,l);
		str = str.Mid(l + 1,str.GetLength());
	}
	else
	{
		//最后一个参数
		retStr = str.Mid(0,str.GetLength());
		str = "";
	}
	return retStr;
}

void ConvertStruct()
{
	for (int i = 0;i < (int)FileList.size(); i++)
	{
		//开始转换成结构
		//人物基址_,00FFD08bf83bfd0f84,+1,4B,79
		CString strTemp = FileList[i];
		scBuff[i].IsBase = GetParm(strTemp) == "Base" ? TRUE : FALSE;
		scBuff[i].Title = GetParm(strTemp);
		scBuff[i].FeatureCode = GetParm(strTemp);
		CString Temp = GetParm(strTemp);//+1,-1
		if (Temp.Find("+") >= 0)
		{
			scBuff[i].IsDown = TRUE;//向下搜索
		}
		else
		{
			scBuff[i].IsDown = FALSE;
		}
		int Offset = atoi(Temp.Mid(1,Temp.GetLength()));
		CString Temp1 = GetParm(strTemp);
		CString Temp2 = GetParm(strTemp);
		char * cstr;
		int dwValue2 = (int)strtol(Temp1.GetBuffer(),&cstr,16);
		int dwValue1 = (int)strtol(Temp2.GetBuffer(),&cstr,16);
		int Sum = abs(dwValue2 - dwValue1);
		if (scBuff[i].IsDown)
		{
			Sum += Offset;
		}
		else
		{
			Sum -= Offset;
		}
		scBuff[i].Offset = Sum;
		scBuff[i].IsConst = FALSE;
		if (strTemp.Trim() != "")
		{
			OutputDebugString(strTemp);
			scBuff->IsConst = TRUE;
			scBuff[i].Title += "\t" + GetParm(strTemp);
		}
	}
}*/

DWORD WINAPI WorkThread(LPVOID lpParm)
{
	CSearchBase CSB;
	CSB.FindBnsKoreaBase();

	((CButton*)(((CGame*)lpParm)->GetDlgItem(IDC_BUTTON1)))->EnableWindow(TRUE);

	return 0;
}

void CGame::OnBnClickedButton1()
{
	((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkThread, this, NULL, NULL);

	//CGameScript CGS;
	//WCHAR szPath[MAX_PATH];
	//::GetCurrentDirectoryW(MAX_PATH,szPath);
	//CGS.ExcuteFile(szPath);
	/*ReadFile();

	//memset(scBuff,0,sizeof(scBuff));
	ConvertStruct();
	CString ss;
	ss.Format("FileList.size()=%d",FileList.size());
	OutputDebugString(ss);
	for (int i = 0; i < (int)FileList.size(); i++)
	{
		if (scBuff[i].IsConst)
		{
			OutputDebugString("不需要搜索");
			continue;
		}
		if (scBuff[i].IsBase)
		{
			DWORD retAddr = 0;
			CString str;

			//DWORD addr = ScanAddress(scBuff[i].FeatureCode.GetBuffer(),scBuff[i].Offset,scBuff[i].IsDown ? 1 : 0,&retAddr);
			DWORD addr = 0;
			str.Format("查找基址.特征码是:%s,偏移是:%d,方向是:%s,返回的地址是:%X,基址是:%X",scBuff[i].FeatureCode.GetBuffer(),scBuff[i].Offset,scBuff[i].IsDown ? "向下" : "向上",retAddr,addr);
			OutputDebugString(str);
			scBuff[i].addr = addr;
		}
		else
		{
			DWORD retAddr = 0x0;
			CString str;
			//DWORD addr = ScanCall(scBuff[i].FeatureCode.GetBuffer(),scBuff[i].Offset,scBuff[i].IsDown ? 1 : 0,&retAddr);
			DWORD addr = 0;
			str.Format("查找CALL.特征码是:%s,偏移是:%d,方向是:%s,返回的地址是:%X,CALL是:%X",scBuff[i].FeatureCode.GetBuffer(),scBuff[i].Offset,scBuff[i].IsDown ? "向下" : "向上",retAddr,addr);
			OutputDebugString(str);
			scBuff[i].addr = addr;
		}
	}*/
}
