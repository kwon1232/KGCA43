#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("RaceCrit");
HANDLE g_hMutex; // ��ȣ����
HANDLE g_hEvent;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	HANDLE hMutex = CreateMutex(NULL, FALSE, L"KGCA");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, L"�̹� Ŭ���̾�Ʈ�� ���� ���Դϴ�.!", L"�ߺ�����", MB_OK);
		CloseHandle(hMutex);
		return 1;
	}
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	CloseHandle(g_hEvent);
	return (int)Message.wParam;
}

int X;
CRITICAL_SECTION critA;
CRITICAL_SECTION critB;
DWORD WINAPI ThreadFunc1(LPVOID Param)
{
	WaitForSingleObject(g_hEvent, INFINITE);
	HDC hdc;
	hdc = GetDC(hWndMain);
	for (int i = 0; i < 100; i++) {
		// ��ȣ�����̸� �����Ѵ�.
		//EnterCriticalSection(&critA);// �Ӱ迵��
		//EnterCriticalSection(&critB);// �Ӱ迵��
		WaitForSingleObject(g_hMutex, INFINITE);
		
			// ��ȣ���°� �ɶ� ���� ���Ѵ��
			X = 100;
			Sleep(1);// ����Ī
			TextOutA(hdc, X, 100, "������", 6);
	
		//LeaveCriticalSection(&critB);// �Ӱ迵��
		//LeaveCriticalSection(&critA);
		ReleaseMutex(g_hMutex);
	}
	ReleaseDC(hWndMain, hdc);
	return 0;
}
void Fun()
{
	
}
DWORD WINAPI ThreadFunc2(LPVOID Param)
{
	WaitForSingleObject(g_hEvent, INFINITE);
	HDC hdc;
	hdc = GetDC(hWndMain);
	for (int i = 0; i < 100; i++) {
		// �����(��������): deadrock
		//EnterCriticalSection(&critB);// �Ӱ迵��
		//EnterCriticalSection(&critA);// �Ӱ迵��
		WaitForSingleObject(g_hMutex, INFINITE);
			X = 200;
			Sleep(1);// ����Ī
			TextOutA(hdc, X, 200, "�����", 6);
		//LeaveCriticalSection(&critA);
		//LeaveCriticalSection(&critB);
		ReleaseMutex(g_hMutex);
	}
	ReleaseDC(hWndMain, hdc);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	DWORD ThreadID;
	HANDLE hThread;

	switch (iMessage) {
	case WM_CREATE:
		InitializeCriticalSection(&critA);
		InitializeCriticalSection(&critB);
		hWndMain = hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		SetEvent(g_hEvent);
		// FALSE�̸� ����Լ��� ȣ���ϴ� �Լ��� �����Ѵ�.
		// TRUE�̸� CreateMutex�� ȣ���� �����尡 �����Ѵ�.
		// ����ȭ ��ü�� �ƴϸ� FALSE �д�.
		g_hMutex = CreateMutex(NULL, FALSE, NULL);		
		hThread = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		return 0;
	case WM_DESTROY:
		DeleteCriticalSection(&critA);
		DeleteCriticalSection(&critB);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

