#include "StdAfx.h"
#include <windows.h>
#include <mmsystem.h>
#include "BitMap.h"
#include "WinPing.h"
/* 将位图装入控件*/
void CNEWBMP::SetWindHand(HWND h)
{
	hwnd = h;
	return;
}

HWND CNEWBMP::GetWindHand()
{

	return hwnd;
}
//把图形清除(是清除OK,NG bmp)
void CNEWBMP::ClearBmp(HWND hMap)
{
	HDC   pDC;
	RECT rect;
	//HWND hMap;
	//hMap = GetDlgItem(hDlg,IDC_WIFI_BITMAP);
	pDC = GetWindowDC((HWND)hMap);//这个用法不错,学习一下.
	rect.left = 0;               //我一般不会这么用的.
	rect.top = 0;
	rect.right = 80;
	rect.bottom = 46;
	FillRect(pDC, &rect, (HBRUSH)COLOR_WINDOW);
}

/*
// uBmpResource：图片ID; uCtrlResource: Picture控件ID
void CNEWBMP::SetBitmap(UINT uBmpResource, UINT uCtrlResource)
{
    HBITMAP hBitmap;
    CStatic *pStatic = (CStatic *)GetDlgItem(uCtrlResource);
    hBitmap = (HBITMAP)LoadImage(
        AfxGetInstanceHandle(),
        MAKEINTRESOURCE(uBmpResource),
        IMAGE_BITMAP,
        0,
        0,
        LR_LOADMAP3DCOLORS);
    pStatic->ModifyStyle(0xF, SS_BITMAP);
    pStatic->SetBitmap(hBitmap);
}
*/

void CNEWBMP::DrawBmp(DWORD id)
{/**/
	HINSTANCE g_hInstance = GetGlobalInstance();

	if (hwnd != NULL)
	{
#if 0
		HBITMAP   hBitmap;
		hBitmap = (HBITMAP)LoadBitmap(g_hInstance, MAKEINTRESOURCE(id));//,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);   

		HBITMAP   hOldBitmap;
		HDC   MemDC;
		RECT  rect;
		HDC   pDC = GetDC(hwnd);
		MemDC = CreateCompatibleDC(pDC);
		GetWindowRect(hwnd, &rect);
		//	GetClientRect(hDlg,&rect);
		hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);

		int width = rect.right - rect.left;
		int Height = (rect.bottom - rect.top);
		BitBlt(pDC, 0, 0, width, Height, MemDC, 0, 0, SRCCOPY);
		SelectObject(MemDC, hOldBitmap);

		DeleteDC(MemDC);
#endif
#if 1
		//		HDC   pDC=BeginPaint(hwnd,&ps);   //不闪烁

		HDC   pDC = GetWindowDC(hwnd);

		HBITMAP   hBitmap;
		hBitmap = (HBITMAP)LoadBitmap(g_hInstance, MAKEINTRESOURCE(id));//,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);   

		HBITMAP   hOldBitmap;
		HDC   MemDC;
		RECT  rect;

		MemDC = CreateCompatibleDC(pDC);
		//	GetClientRect(hDlg,&rect);
		hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);

		GetWindowRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int Height = (rect.bottom - rect.top);
		//		fprintf(stdout, "width=%d\r\n",width);
		//		fprintf(stdout, "Height=%d,\r\n",Height);

		BitBlt(pDC, 0, 0, width, Height, MemDC, 0, 0, SRCCOPY);

		SelectObject(MemDC, hOldBitmap);

		ReleaseDC(hwnd, pDC);

		DeleteObject(hBitmap);

		DeleteObject(hOldBitmap);
		DeleteDC(MemDC);


		//	pDC=BeginPaint(hwnd,&ps);   //del all WM_PAINT
		//	EndPaint(hwnd,&ps);
#endif
	}
	return;

}


#if 0
void CNEWBMP::DrawBmp(LPCTSTR szFileName)
{/**/
	if (hwnd != NULL)
	{
#if 1
		//		HDC   pDC=BeginPaint(hwnd,&ps);   //不闪烁

		HDC   pDC = GetWindowDC(hwnd);

		HBITMAP   hBitmap;
		//hBitmap=(HBITMAP)LoadBitmap(g_hInstance,MAKEINTRESOURCE(id));//,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);   
		hBitmap = SHLoadDIBitmap(szFileName);

		HBITMAP   hOldBitmap;
		HDC   MemDC;
		RECT  rect;

		MemDC = CreateCompatibleDC(pDC);
		//	GetClientRect(hDlg,&rect);
		hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);

		GetWindowRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int Height = (rect.bottom - rect.top);
		//		fprintf(stdout, "width=%d\r\n",width);
		//		fprintf(stdout, "Height=%d,\r\n",Height);

		BitBlt(pDC, 0, 0, width, Height, MemDC, 0, 0, SRCCOPY);

		SelectObject(MemDC, hOldBitmap);

		ReleaseDC(hwnd, pDC);

		DeleteObject(hBitmap);

		DeleteObject(hOldBitmap);
		DeleteDC(MemDC);


		//	pDC=BeginPaint(hwnd,&ps);   //del all WM_PAINT
		//	EndPaint(hwnd,&ps);
#endif
	}
	return;

}
#endif


void CNEWBMP::DrawBmp2(DWORD id)
{/**/
	HINSTANCE g_hInstance = GetGlobalInstance();
	if (hwnd != NULL)
	{
		//	HDC   pDC=GetDC(hwnd);   
		HDC   pDC = BeginPaint(hwnd, &ps);
		HBITMAP   hBitmap;
		hBitmap = (HBITMAP)LoadBitmap(g_hInstance, MAKEINTRESOURCE(id));//,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);   

		HBITMAP   hOldBitmap;
		HDC   MemDC;
		RECT  rect;
		MemDC = CreateCompatibleDC(pDC);
		hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);

		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int Height = (rect.bottom - rect.top);

		BitBlt(pDC, 0, 0, width, Height, MemDC, 0, 0, SRCCOPY);

		SelectObject(MemDC, hOldBitmap);

		//		ReleaseDC(hwnd,pDC);
		EndPaint(hwnd, &ps);

		DeleteObject(hBitmap);

		DeleteObject(hOldBitmap);
		DeleteDC(MemDC);



	}
	return;

}

void CNEWBMP::SetBackgroundDC(HDC h)
{
	BackgroundDC = h;
	return;
}

extern HINSTANCE hMainInst;
void  CNEWBMP::DrawBackgroundBMP(DWORD id)
{
	HINSTANCE g_hInstance = GetGlobalInstance();
	if (BackgroundDC != 0)
	{
		HDC   pDC = BackgroundDC;
		HBITMAP   hBitmap;
		SetLastError(NO_ERROR);
		hBitmap = (HBITMAP)LoadBitmap(g_hInstance, MAKEINTRESOURCE(id));//,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		//		hBitmap=SHLoadDIBitmap(szFileName);
		int i = GetLastError();
		HBITMAP   hOldBitmap;
		HDC   MemDC;
		RECT  rect;
		MemDC = CreateCompatibleDC(pDC);
		hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);

		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int Height = (rect.bottom - rect.top);

		BitBlt(pDC, 0, 0, width, Height, MemDC, 0, 0, SRCCOPY);

		SelectObject(MemDC, hOldBitmap);


		DeleteObject(hBitmap);

		DeleteObject(hOldBitmap);
		DeleteDC(MemDC);
	}
	return;
}

#if 0
void  CNEWBMP::DrawBackgroundBMP(/*DWORD id*/LPCTSTR szFileName)
{
	if (BackgroundDC != 0)
	{
		HDC   pDC = BackgroundDC;
		HBITMAP   hBitmap;
		//		hBitmap=(HBITMAP)LoadBitmap(g_hInstance,MAKEINTRESOURCE(id));//,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);   
		hBitmap = SHLoadDIBitmap(szFileName);
		HBITMAP   hOldBitmap;
		HDC   MemDC;
		RECT  rect;
		MemDC = CreateCompatibleDC(pDC);
		hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);

		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int Height = (rect.bottom - rect.top);

		BitBlt(pDC, 0, 0, width, Height, MemDC, 0, 0, SRCCOPY);

		SelectObject(MemDC, hOldBitmap);


		DeleteObject(hBitmap);

		DeleteObject(hOldBitmap);
		DeleteDC(MemDC);
	}
	return;
}
#endif

void  CNEWBMP::SetStaticHand(HWND h)
{
	hStatic = h;
}

void  CNEWBMP::SetStaticBmp(DWORD id)
{
	HBITMAP   hBitmap;
	HINSTANCE g_hInstance = GetGlobalInstance();
	hBitmap = (HBITMAP)LoadBitmap(g_hInstance, MAKEINTRESOURCE(id));
	SendMessage(hStatic, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)hBitmap);
	DeleteObject(hBitmap);
}