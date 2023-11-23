#include "TNetStd.h"
#include "TUser.h"
#define MAX_WORKER_THREAD 2

const short port = 10000;
std::list<TUser*>  g_userlist;
SOCKET          listensock;

HANDLE	        g_hKillEvent;
HANDLE          g_hWorkerThread[MAX_WORKER_THREAD];
HANDLE          g_hIOCP;
int SendPacket(TUser* pUser, UPACKET& packet)
{
    char* sendbuffer = (char*)&packet;

    pUser->wsaSendBuffer.buf = (char*)&packet;
    pUser->wsaSendBuffer.len = packet.ph.len;
    pUser->ovSend.flag = OVERLAPPED2::MODE_SEND;

    int iSendByte = 0;
    int iTotalSendByte = 0;
    DWORD dwSendByte;
    int iRet= WSASend(pUser->sock, &pUser->wsaSendBuffer, 1,
            &dwSendByte, 0, (LPOVERLAPPED)&pUser->ovSend,   NULL);
    if (iRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            return -1;
        }
    }
    return packet.ph.len;
}
bool Broadcastting(UPACKET& packet)
{
    for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
        iterSend != g_userlist.end();
        iterSend++)
    {
        TUser* pUser = (*iterSend);
        if (pUser->bConneted == false) continue;
        int iSendByte = SendPacket(pUser, packet);
        if (iSendByte == SOCKET_ERROR)
        {         
            pUser->bConneted = false;
            continue;
        }        
    }
    return true;
}
DWORD WINAPI WorkerThread(LPVOID param)
{
    DWORD dwTransfer; // �а�,���� �� ����Ʈ 
    ULONG_PTR KeyValue;
    OVERLAPPED*  ov;
    
    while (1)
    {
        if (WaitForSingleObject(g_hKillEvent, 0) == WAIT_OBJECT_0)
        {
            break;
        }
        // �񵿱� ����¿� ����� IOCP ť�� ����ȴ�.
        //GetQueuedCompletionStatus�� IOCPť���� �Ϸ� ����� �����´�.
       BOOL bReturn = ::GetQueuedCompletionStatus(g_hIOCP, &dwTransfer, &KeyValue, &ov,
           0 );       

       if (bReturn == TRUE)
       {
           TUser* pUser = (TUser*)KeyValue;
           if (pUser != nullptr)
           {
               pUser->Dispatch(dwTransfer, ov);
           }
       }
       else
       {
           //���α׷� �����ͺ��̽��� ���� ���� ������ �߻��߽��ϴ�. ��ũ ������ ��������, ��ΰ� �߸��Ǿ����� �Ǵ� ������ ������ Ȯ���Ͻʽÿ�.
           DWORD dwError = GetLastError();
           if (dwError == WAIT_TIMEOUT)
           {
               continue;
           }
           if (ERROR_HANDLE_EOF == dwError)
           {
               ::SetEvent(g_hKillEvent);
               break;
           }
           //#define ERROR_HANDLE_EOF                 38L   
           E_MSG("error");
           PrintDetailA("Error!");
           ::SetEvent(g_hKillEvent);
           break;
       }
    }
    return 0;
}

DWORD WINAPI WorkerAcceptThread(LPVOID param)
{
    SOCKADDR_IN clientaddr;
    while(1)
    {
        int addlen = sizeof(clientaddr);
        SOCKET clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addlen);
        if (clientsock == SOCKET_ERROR)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                break;
            }
        }
        else
        {
            TUser* user = new TUser(clientsock, clientaddr);                 
            user->bind(g_hIOCP);
            user->recv();
            g_userlist.push_back(user);

            PrintA("Ŭ���̾�Ʈ ���� ip=%s, Port:%d\n",
                inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port));       

            
        }
    }
    return 1;
}
void IOCPRun()
{
    g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);   
    g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0,0,0);

    // 1) ���� �ʱ�ȭ
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return ;
    listensock = socket(AF_INET, SOCK_STREAM, 0);
  
    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.12");
    sa.sin_port = htons(10000);

    iRet = bind(listensock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return ;

    iRet = listen(listensock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return ;

    DWORD dwThreadID;
    CloseHandle(::CreateThread(0, 0, WorkerAcceptThread, nullptr, 0, &dwThreadID));

    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        g_hWorkerThread[iThread] = ::CreateThread(0, 0, WorkerThread, nullptr, 0, &dwThreadID);
    }

  
}


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
    IOCPRun();

    //1�� ) ������ �������� Ŭ���� ���
    MyRegisterClass(hInstance);
    //2�� ) ������ ����
    InitInstance(hInstance, nCmdShow);
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
            for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
                iterSend != g_userlist.end();
                iterSend++)
            {
                TUser* pUser = (*iterSend);
                if (pUser->bConneted == false) continue;
                for (auto& data : pUser->list)
                {
                    if (!Broadcastting(data))
                    {
                        pUser->bConneted = false;                       
                    }
                }  
                pUser->list.clear();
            }

            //post
            for (std::list<TUser*>::iterator iterSend = g_userlist.begin();
                iterSend != g_userlist.end();
                )
            {
                TUser* pUser = (*iterSend);
                if (pUser->bConneted == false) 
                {
                    pUser->Close();
                    iterSend = g_userlist.erase(iterSend);
                }  
                else
                {
                    iterSend++;
                }                 
            }
        }
    }

    WaitForMultipleObjects(MAX_WORKER_THREAD, g_hWorkerThread, TRUE, INFINITE);

   
    CloseHandle(g_hKillEvent);

    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        CloseHandle(g_hWorkerThread[iThread]);
    }

    return (int)msg.wParam;    
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindowW(L"KGCA_WINDOWS", 
        L"Sample Projects", WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600, nullptr, nullptr, 
        hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

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
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    return RegisterClassExW(&wcex);
}
