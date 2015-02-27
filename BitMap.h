#ifndef _BITMAP_H_
#define _BITMAP_H_
#include <windows.h>
#include <mmsystem.h>

class CNEWBMP
{
public:
	CNEWBMP(){ hwnd = NULL; };
	~CNEWBMP(){};
	/* 将位图装入控件*/
	void SetWindHand(HWND h);
	HWND GetWindHand();

    //void SetBitmap(UINT uBmpResource, UINT uCtrlResource);
	void DrawBmp(DWORD id);
	//void DrawBmp(LPCTSTR szFileName);
	void DrawBmp2(DWORD id);

	void SetBackgroundDC(HDC h);
	void DrawBackgroundBMP(DWORD id);
	//void DrawBackgroundBMP(LPCTSTR szFileName);
	void SetStaticHand(HWND h);
	void SetStaticBmp(DWORD id);
	void ClearBmp(HWND hMap);
protected:
private:
	HWND hwnd;
	PAINTSTRUCT ps;
	HDC BackgroundDC;
	HWND hStatic;

	//	virtual
};
//extern CNEWBMP g_TestStatusbmp;

#endif