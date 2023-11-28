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

void UDPSocketInit()
{
    // 1) ���� �ʱ�ȭ
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    g_hSendSocket = socket(AF_INET, SOCK_DGRAM, 0);
    g_hRecvSocket = socket(AF_INET, SOCK_DGRAM, 0);

    int loopval = false;
    int optLevel = IPPROTO_IP;
    int option   = IP_MULTICAST_LOOP;
    char* optval = nullptr;
    optval = (char*)&loopval;
    int optlen = sizeof(loopval);
    /*::setsockopt(g_hRecvSocket, optLevel,
        option, optval, sizeof(optlen));*/
    int rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
        IP_MULTICAST_LOOP, (char*)&loopval, sizeof(loopval));
    if (rc == SOCKET_ERROR)
    {
        LogErrorA("IP_MULTICAST_LOOP");
    }
    else
    {
        //LogErrorA("IP_MULTICAST_LOOP");
    }

    g_RecvAddress.sin_family = AF_INET;
    g_RecvAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    g_RecvAddress.sin_port = htons(9000);
    rc = ::bind(g_hRecvSocket,(SOCKADDR*)&g_RecvAddress, sizeof(g_RecvAddress));
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

    //INCLUDE
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
    mreq.imr_interface.s_addr = inet_addr("192.168.0.12");
    rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
        IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
    if (rc == SOCKET_ERROR)
    {
        LogErrorA("bind");
    }

    //// EXCLUDE
    //struct ip_mreq_source mreqsrc;
    //mreqsrc.imr_interface = mreq.imr_interface;
    //mreqsrc.imr_multiaddr = mreq.imr_multiaddr;
    //mreqsrc.imr_sourceaddr.s_addr = inet_addr("192.168.0.33");
    //rc = ::setsockopt(g_hRecvSocket, IPPROTO_IP,
    //    IP_BLOCK_SOURCE, (char*)&mreqsrc, sizeof(mreqsrc));
    /*if (rc == SOCKET_ERROR)
    {
        LogErrorA("bind");
    }*/
    g_SendAddress.sin_family = AF_INET;
    g_SendAddress.sin_addr.s_addr = inet_addr("192.168.0.12");
    g_SendAddress.sin_port = htons(9000);
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
