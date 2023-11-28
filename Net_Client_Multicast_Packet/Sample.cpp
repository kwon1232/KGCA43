#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <string>
#include "TProtocol.h"
#include "tchar.h" // _tcscpy
#include <list>

int   g_Mode = MCAST_EXCLUDE;// MCAST_INCLUDE;// MCAST_EXCLUDE; // MCAST_INCLUDE

static void LogErrorA(const char* fmt, ...)
{
    void* lpMsgBuf;
    DWORD dwError = WSAGetLastError();
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char*)&lpMsgBuf, 0, NULL);
    LocalFree(lpMsgBuf);

    va_list ap;
    char buf[MAX_PATH];

    va_start(ap, fmt);
    vsprintf_s(buf, fmt, ap);
    va_end(ap);

    std::string msg = buf;
    msg += "\n";
    msg += "ERRCode[";
    msg += std::to_string(dwError);
    msg += "]:";
    msg += (char*)lpMsgBuf;
    //TLog::Get().log(msg);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWriten;
    WriteConsoleA(handle, msg.c_str(), msg.size(), &dwBytesWriten, 0);
}
static std::string wtm(std::wstring data)
{
    char retData[4096] = { 0 };
    // �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
    int iLength = WideCharToMultiByte(CP_ACP, 0, data.c_str(), -1, 0, 0, NULL, NULL);
    int iRet = WideCharToMultiByte(CP_ACP, 0,
        data.c_str(), -1,  //  �ҽ�
        retData, iLength, // ���
        NULL, NULL);
    return retData;
}
static std::wstring mtw(std::string data)
{
    WCHAR retData[4096] = { 0 };
    // �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
    int iLength = MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, 0, 0);
    int iRet = MultiByteToWideChar(CP_ACP, 0,
        data.c_str(), -1,  //  �ҽ�
        retData, iLength); // ���
    return retData;
}

static void ConsolePrintW(const wchar_t* fmt, ...)
{
    va_list ap;
    wchar_t buf[MAX_PATH];

    va_start(ap, fmt);
    vswprintf_s(buf, fmt, ap);
    va_end(ap);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWriten;
    WriteConsoleW(handle, buf, wcslen(buf), &dwBytesWriten, 0);
}
static void ConsolePrintA(const char* fmt, ...)
{
    va_list ap;
    char buf[MAX_PATH];

    va_start(ap, fmt);
    vsprintf_s(buf, fmt, ap);
    va_end(ap);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWriten;
    WriteConsoleA(handle, buf, strlen(buf), &dwBytesWriten, 0);
}

#define PrintA(fmt,...)         ConsolePrintA( fmt, __VA_ARGS__ )
#define PrintDetailA(fmt,...)   ConsolePrintA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define PrintW(fmt,...)         ConsolePrintW( fmt, __VA_ARGS__ )
#define PrintDetailW(fmt,...)   ConsolePrintW( L"[%s %d] : " fmt, __FILEW__,__LINE__, ##__VA_ARGS__ )
#define LogErrorA(fmt,...)		LogErrorA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )


SOCKET      g_hSendSocket;
SOCKADDR_IN g_SendAddress;

SOCKET      g_hRecvSocket;
SOCKADDR_IN g_RecvAddress;
HWND        g_hWnd;

SOCKADDR_IN g_hMulticastAddr;

int SetMulticast(SOCKET s,  int opt, int val, int af= IPPROTO_IP)
{
    int rc = ::setsockopt(g_hRecvSocket, af,
        opt, (char*)&val, sizeof(val));
    if (rc == SOCKET_ERROR)
    {
        LogErrorA("IP_MULTICAST_LOOP");
    }
    return rc;
}
void UDPSocketInit()
{
    // 1) ���� �ʱ�ȭ
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    g_hSendSocket = socket(AF_INET, SOCK_DGRAM, 0);
    g_hRecvSocket = socket(AF_INET, SOCK_DGRAM, 0);

    int loopval = 1;
    SetMulticast(g_hRecvSocket, IP_MULTICAST_LOOP, loopval);

    BOOL optval2 = TRUE;
    // ip & port ����
    setsockopt(g_hRecvSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval2, sizeof(optval2));
    // ���� ������ �纻 ����.
    bool loop = true;
    setsockopt(g_hRecvSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&loop, sizeof(loop));
    int ttl = 3;
    setsockopt(g_hRecvSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl));
    // ��Ƽ�ɽ�Ʈ �����͸� ������ ���� �������̽��� �����Ѵ�.
    //ULONG addr = inet_addr("192.168.0.12");
    //setsockopt(g_hRecvSocket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&addr, sizeof(addr));

    g_RecvAddress.sin_family = AF_INET;
    g_RecvAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    g_RecvAddress.sin_port = htons(9000);
    int rc = ::bind(g_hRecvSocket,(SOCKADDR*)&g_RecvAddress, sizeof(g_RecvAddress));
    if (rc == SOCKET_ERROR)
    {
        LogErrorA("bind");
    }
    else
    {
        //LogErrorA("bind");
    }
    
    g_hMulticastAddr.sin_family = AF_INET;
    g_hMulticastAddr.sin_addr.s_addr = inet_addr("235.7.8.9");
    g_hMulticastAddr.sin_port = htons(9000);

    struct ip_mreq mreq;
    struct ip_mreq_source mreqsrc;
    
    int g_iSouceCnt = 1;
    if (g_Mode == MCAST_INCLUDE)
    {
        for (int i = 0; i < g_iSouceCnt; i++)
        {
            //IP_ADD_SOURCE_MEMBERSHIP �����ϰ� Ư�� �ּҸ� ���� ���.
            //IP_DRAP_SOURCE_MEMBERSHIP ���� ��ϵ� ����Ʈ ����.
            mreqsrc.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
            mreqsrc.imr_interface.s_addr = inet_addr("192.168.0.12");
            rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
                IP_ADD_SOURCE_MEMBERSHIP, (char*)&mreqsrc, sizeof(mreqsrc));
            if (rc == SOCKET_ERROR)
            {
                LogErrorA("bind");
            }
        }
    }
    else if( g_Mode == MCAST_EXCLUDE)
    {
        //IP_ADD_MEMBERSHIP  �����Ѵ�.
        //IP_DROP_MEMBERSHIP Ż���Ѵ�.
        mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
        mreq.imr_interface.s_addr = inet_addr("192.168.0.12");
        rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
            IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
        if (rc == SOCKET_ERROR)
        {
            LogErrorA("bind");
        }
        for (int i = 0; i < g_iSouceCnt; i++)
        {
            //IP_BLOCK_SOURCE    : �����ϰ� Ư�� �ּҷκ��� ���۵� ������ �����ϰ� ����.
            //IP_UNBLOCK_SOURCE  : ������ �ּ��� ����Ʈ���� Ư�� �ּҸ� �߰��Ѵ�.
            mreqsrc.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
            mreqsrc.imr_interface.s_addr = inet_addr("192.168.0.12");
            mreqsrc.imr_sourceaddr.s_addr = inet_addr("192.168.0.54");
            rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
                IP_BLOCK_SOURCE, (char*)&mreqsrc, sizeof(mreqsrc));
            if (rc == SOCKET_ERROR)
            {
                LogErrorA("bind");
            }
        }
    }   
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {    
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

    int iSize = sizeof(CHAT_MSG);

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

    UDPSocketInit();

    
    char RecvBuf[2048] = { 0, };

    auto sendthread = []()
    {
        char SendBuf[2048] = "KGCA";

        while (1)
        {
            int iSendRet = sendto(g_hSendSocket, SendBuf, strlen(SendBuf), 0,
                (SOCKADDR*)&g_hMulticastAddr,
                sizeof(g_hMulticastAddr));
            Sleep(1000);
        }
    };
    std::thread t(sendthread);

    //1�� ) ������ �������� Ŭ���� ���
    MyRegisterClass(hInstance);
    //2�� ) ������ ����
    InitInstance(hInstance, nCmdShow);
    
    //3�� ) ������ ���ν��� �۾�
    MSG msg;
    SOCKADDR_IN PeerAddr;
    int addlen;

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
            char RecvBuf[2048] = { 0, };
            addlen = sizeof(PeerAddr);
            int recvRet = recvfrom(g_hRecvSocket, RecvBuf, 2048, 0,
                (SOCKADDR*)&PeerAddr, &addlen);
            PrintA(RecvBuf);            
        }
    }
    closesocket(g_hSendSocket);
    closesocket(g_hRecvSocket);
    // )  ���� ����    
    WSACleanup();

    //t.join();
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
