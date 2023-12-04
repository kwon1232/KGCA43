#include "TWindow.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        int k = 0;
    }break;
    case WM_CREATE: break;
    case WM_DESTROY:
        PostQuitMessage(0);// WM_QUIT
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL TWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindowW(L"KGCA_WINDOWS",
        L"CreateDevice1 Projects", WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600, nullptr, nullptr,
        hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    m_hWnd = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
ATOM TWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = L"KGCA_WINDOWS";
    //wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    return RegisterClassExW(&wcex);
}
void TWindow::SetWindow(HINSTANCE hInstance, int    nCmdShow)
{
    m_hInstance = hInstance;
    //1�� ) ������ �������� Ŭ���� ���
    MyRegisterClass(m_hInstance);
    //2�� ) ������ ����
    InitInstance(hInstance, nCmdShow);
    //3�� ) ������ ���ν��� �۾�
}

void TWindow::Run()
{
    Init(); 
    MSG msg;
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            Render();
        }
    }
    Release();
}

void TWindow::ReleaseAll()
{
}
