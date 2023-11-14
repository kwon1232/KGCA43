#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DownEvent");
static int Y = 0;
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HANDLE hEvent;
DWORD WINAPI ThreadDownLoad(LPVOID temp)
{
	int i,j;
	TCHAR str[256];
	HDC hdc=GetDC(hWndMain);
	int* iY = (int*)temp;
	int iYValue = *iY;
	for (i=0;i<100;i++) 
	{
		wsprintf(str,L"�ٿ�ε��� : %d%%�Ϸ�",i);
		for (j=0;j<i;j++)
			lstrcat(str,L"I");
		TextOut(hdc,10,100+(iYValue),str,lstrlen(str));
		Sleep(100);
		INT iRet = WaitForSingleObject(hEvent, 0);//INFINITE);
		if ( iRet== WAIT_TIMEOUT)
		{
			if (MessageBox(hWndMain,L"�ٿ�ε带 �����Ͻðڽ��ϱ�?",L"����",
				MB_YESNO) == IDYES) 
			{
				lstrcpy(str,L"����ڿ� �䱸�� ���� �ٿ�ε尡 ��ҵǾ����ϴ�");
				TextOut(hdc,10,100,str,lstrlen(str));
				ReleaseDC(hWndMain, hdc);
				return 0;
			} else {
				SetEvent(hEvent);
			}
		}
	}
	lstrcpy(str,L"�ٿ�ε带 �Ϸ��߽��ϴ�");
	TextOut(hdc,10,100,str,lstrlen(str));
	ReleaseDC(hWndMain, hdc);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	DWORD ThreadID;
	const TCHAR *Mes=L"���콺 ���� ��ư�� ������ �ٿ�ε带 �����ϸ� ������ ��ư�� "
		"������ �ٿ�ε带 �����մϴ�.";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
		return 0;
	case WM_LBUTTONDOWN:
		SetEvent(hEvent);
		
		Y += 30;
		CloseHandle(CreateThread(NULL, 0, ThreadDownLoad, &Y, 0, &ThreadID));
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_RBUTTONDOWN:
		ResetEvent(hEvent);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		CloseHandle(hEvent);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

