#pragma once
#include "Net_Client.cpp"
#include <list>
SOCKET sock;
HWND g_hWnd;
bool g_bConnect = false;
HWND g_hEdit;
HWND g_hButton;
HWND g_hList;

std::list<std::string> g_szMsgList;
static int Recvwork()
{
    char recvbuf[256] = { 0, };
    int iRecvByte = recv(sock, recvbuf, 256, 0);
    if (iRecvByte == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {

            return -1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        //printf("[����]%s\n", recvbuf);
        OutputDebugStringA(recvbuf);
        g_szMsgList.push_back(recvbuf);
        if (g_szMsgList.size() > 20)
        {
            g_szMsgList.pop_front();
        }
        SendMessageA(g_hList, LB_RESETCONTENT, 0, 0);
        for (auto msg : g_szMsgList)
        {
            SendMessageA(g_hList, LB_ADDSTRING, 0, (LPARAM)msg.c_str());
        }
    }
    return 1;
}
static int sendwork(const char* buf, int iLen)
{
    int iSendByte;
    iLen = strlen(buf);
    iSendByte = send(sock, buf, iLen, 0);
    if (iSendByte == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {

            return -1;
        }
        else
        {
            return 0;
        }
    }
    printf("%d����Ʈ�� �����߽��ϴ�", iSendByte);
    return iLen;

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 200://button
        {
            char buffer[MAX_PATH] = { 0, };
            SendMessageA(g_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
            sendwork(buffer, strlen(buffer));
        }break;
        }
    }break;
    case NETWORK_MSG:
    {
        if (WSAGETSELECTERROR(lParam) != 0)
        {
            PostQuitMessage(0);
        }
        WORD dwSelect = WSAGETSELECTEVENT(lParam);
        switch (dwSelect)
        {
        case FD_CONNECT:
        {
            g_bConnect = true;
        }break;
        case FD_CLOSE: 
        {
            g_bConnect = false;
        }break;
        case FD_READ: 
        {
            int iRet =Recvwork();
            // �ͺ��ŷ ����.
            if (iRet == -1)
            {
                g_bConnect = false;
                closesocket(sock);
            }
            if (iRet == 0)
            {
                PostMessage(g_hWnd, NETWORK_MSG, sock, FD_READ);
            }
        };
        case FD_WRITE: 
        {
            //sendwork();
        }break;
        }
    }break;
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);

//HINSTANCE : �ü���� �����ϴ� ������ ���μ���ID
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    if (AllocConsole() == TRUE)
    {
        FILE* console = nullptr;
        if (freopen_s(&console, "conin$", "rt", stdin))
        {
            return 0;
        }
        if (freopen_s(&console, "conin$", "wt", stdout))
        {
            return 0;
        }
        if (freopen_s(&console, "conin$", "wt", stderr))
        {
            return 0;
        }
    }

    //1�� ) ������ �������� Ŭ���� ���
    MyRegisterClass(hInstance);
    //2�� ) ������ ����
    InitInstance(hInstance, nCmdShow);
    /// <summary>
    ///  ä�� UI
    /// </summary>
    DWORD style = WS_CHILD | WS_VISIBLE;
    g_hList = CreateWindow(L"listbox", NULL, style, 10, 10, 400, 500,
        g_hWnd, (HMENU)100, hInstance, NULL);
    g_hButton = CreateWindow(L"button", L"����", style, 640, 10, 100, 100,
        g_hWnd, (HMENU)200, hInstance, NULL);
    style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
    g_hEdit = CreateWindow(L"edit", NULL, style, 430, 10, 200, 100,
        g_hWnd, (HMENU)300, hInstance, NULL);
    


    // ��Ʈ��ũ �ʱ�ȭ
    mainNetwork();
    SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)L"ü�ý���!");

    //3�� ) ������ ���ν��� �۾�
    MSG msg;
    // �⺻ �޽��� �����Դϴ�:
    //WM_QUIT : FALSE
    //while (GetMessage(&msg, nullptr, 0, 0))
    while(1)
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
            // ���ӷ���
        }
    }
    closesocket(sock);
    // )  ���� ����    
    WSACleanup();

    return (int)msg.wParam;    
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindowW(L"KGCA_WINDOWS", 
        L"ü�����α׷�", WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600, nullptr, nullptr, 
        hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    g_hWnd = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = L"KGCA_WINDOWS";
    //wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
    wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    return RegisterClassExW(&wcex);
}
