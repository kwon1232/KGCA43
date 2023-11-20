#include "TNetStd.h"

TCHAR*          g_pFileBuffer = nullptr;
LARGE_INTEGER   g_filesize;

HANDLE g_hWF;
HANDLE g_hRF;
OVERLAPPED g_hReadOverlapped;
OVERLAPPED g_hWriteOverlapped;
void ASyncLoad(std::wstring filename);
void ASyncCopy(std::wstring filename);

//SleepEx �Լ��� ȣ���� �����尡 I/O �Լ��� ȣ���� ������� ������ ��쿡�� �߻��Ѵ�.
VOID WINAPI COMPLETION_ROUTINE(
        DWORD dwErrorCode,
        DWORD dwNumberOfBytesTransfered,
     LPOVERLAPPED lpOverlapped
    )
{
    ASyncCopy(L"DXSDK_Jun10.exeCopy.exe");
    PrintA("����Ϸ�\n");

    CloseHandle(g_hWF);
    CloseHandle(g_hRF);
    delete[] g_pFileBuffer;
    return;
}
void Load(std::wstring filename)
{
    HANDLE g_hRF = ::CreateFile(filename.c_str(), GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (g_hRF != INVALID_HANDLE_VALUE)
    {
        ::GetFileSizeEx(g_hRF, &g_filesize);
        g_pFileBuffer = new TCHAR[g_filesize.QuadPart];
        DWORD dwRead;
        __int64 dwLength = g_filesize.QuadPart;
        BOOL ret = ReadFile(g_hRF, g_pFileBuffer, dwLength, &dwRead, NULL);
        if (ret == TRUE)
        {
            //PrintW(L"%s", buf);
            PrintA("�ε�Ϸ�!");
        }
    }
    CloseHandle(g_hRF);
}
void Copy(std::wstring filename)
{
    HANDLE g_hWF = ::CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_hWF != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten;
        DWORD dwLength = g_filesize.LowPart;
        BOOL ret = WriteFile(g_hWF, g_pFileBuffer, dwLength, &dwWritten, NULL);
        if (ret == TRUE)
        {
            PrintA("��¿Ϸ�!");
        }
    }
    CloseHandle(g_hWF);
}
void SyncRun()
{
    Load(L"sql.ISO");
    Copy(L"DemoCopy.iso");

    delete[] g_pFileBuffer;
}

void ASyncLoad(std::wstring filename)
{
    ZeroMemory(&g_hReadOverlapped, sizeof(g_hReadOverlapped));
    g_hReadOverlapped.hEvent = (HANDLE)111;
    HANDLE g_hRF = ::CreateFile(filename.c_str(), GENERIC_READ| GENERIC_WRITE, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED  /*|
        FILE_FLAG_WRITE_THROUGH *//* | FILE_FLAG_NO_BUFFERING */ , NULL);
    if (g_hRF != INVALID_HANDLE_VALUE)
    {
        ::GetFileSizeEx(g_hRF, &g_filesize);
        g_pFileBuffer = new TCHAR[g_filesize.QuadPart];
        DWORD dwRead;
        __int64 dwLength = g_filesize.QuadPart;
        BOOL ret = ReadFileEx(g_hRF, g_pFileBuffer, dwLength,  
            &g_hReadOverlapped, COMPLETION_ROUTINE );
        BOOL bPending = FALSE;
        if (ret == FALSE)
        {
            DWORD dwError = GetLastError();
            if (dwError == ERROR_IO_PENDING)
            {
                bPending = TRUE;
                PrintW(L"ERROR_IO_PENDING");
            }            
            PrintW(L"ERROR_IO_ERROR");
        }
        if (ret == TRUE)
        {
            PrintA("�ε�Ϸ�!");
            return;
        }
        
        DWORD dwTrans;
        while (bPending)
        {
            ret = GetOverlappedResult(g_hRF, &g_hReadOverlapped, &dwTrans, FALSE);
            if (ret == TRUE)
            {
                PrintA("�ε�Ϸ�!");
                bPending = FALSE;
            }
            else
            {
                DWORD dwError = GetLastError();
                if (dwError == ERROR_IO_INCOMPLETE)
                {
                    PrintW(L"%ld<->%ld\n", g_hReadOverlapped.Internal, dwTrans);
                }
                else
                {
                    PrintW(L"ERROR_IO_ERROR");
                }
            }
            Sleep(1);
        }
    }
    CloseHandle(g_hRF);
}
void ASyncCopy(std::wstring filename)
{
    HANDLE g_hWF = ::CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL |
        FILE_FLAG_WRITE_THROUGH, NULL);
    if (g_hWF != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten;
        DWORD dwLength = g_filesize.LowPart;
        BOOL ret = WriteFile(g_hWF, g_pFileBuffer, dwLength, &dwWritten, &g_hWriteOverlapped);
        BOOL bPending = FALSE;
        if (ret == FALSE)
        {
            DWORD dwError = GetLastError();
            if (dwError == ERROR_IO_PENDING)
            {
                bPending = TRUE;
                PrintW(L"ERROR_IO_PENDING");
            }
            PrintW(L"ERROR_IO_ERROR");
        }
        if (ret == TRUE)
        {
            PrintA("��¿Ϸ�!");
            CloseHandle(g_hWF);
            return;
        }

        DWORD dwTrans;
        while (bPending)
        {
            ret = GetOverlappedResult(g_hWF, &g_hWriteOverlapped, &dwTrans, FALSE);
            if (ret == TRUE)
            {
                PrintA("��¿Ϸ�!");
                bPending = FALSE;
            }
            else
            {
                DWORD dwError = GetLastError();
                if (dwError == ERROR_IO_INCOMPLETE)
                {
                    PrintW(L"%ld<->%ld\n", g_hWriteOverlapped.Internal, dwTrans);
                }
                else
                {
                    PrintW(L"ERROR_IO_ERROR");
                }
            }
            Sleep(1);
        }
    }
    CloseHandle(g_hWF);
}
void ASyncRun()
{
    PrintA("�ε�����!\n");
    ASyncLoad(L"DXSDK_Jun10.exe");
   /* PrintA("�ε��Ϸ�\n");
    ASyncCopy(L"DXSDK_Jun10.exeCopy.exe");
    PrintA("����Ϸ�\n");

    delete[] g_pFileBuffer;*/
}

void ASyncCallbackLoad(std::wstring filename)
{
    ZeroMemory(&g_hReadOverlapped, sizeof(g_hReadOverlapped));
    g_hRF = ::CreateFile(filename.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED  /*|
        FILE_FLAG_WRITE_THROUGH *//* | FILE_FLAG_NO_BUFFERING */, NULL);
    if (g_hRF != INVALID_HANDLE_VALUE)
    {
        ::GetFileSizeEx(g_hRF, &g_filesize);
        g_pFileBuffer = new TCHAR[g_filesize.QuadPart];
        DWORD dwRead;
        __int64 dwLength = g_filesize.QuadPart;
        BOOL ret = ReadFileEx(g_hRF, g_pFileBuffer, dwLength,
            &g_hReadOverlapped, COMPLETION_ROUTINE);
        BOOL bPending = FALSE;
        if (ret == FALSE)
        {
            DWORD dwError = GetLastError();
            if (dwError == ERROR_IO_PENDING)
            {
                bPending = TRUE;
                PrintW(L"ERROR_IO_PENDING");
            }
            PrintW(L"ERROR_IO_ERROR");
        }
        // �񵿱� ���ν��� ȣ��(APC) ������ ť�� ����Ѵ�.
        // TRUE : �񵿱� �Ϸ� ������ ����Ѵ�
        SleepEx(1, TRUE);
    }   
    
}
void ASyncCallbackCopy(std::wstring filename)
{
    HANDLE g_hWF = ::CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL |
        FILE_FLAG_WRITE_THROUGH, NULL);
    if (g_hWF != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten;
        DWORD dwLength = g_filesize.LowPart;
        BOOL ret = WriteFile(g_hWF, g_pFileBuffer, dwLength, &dwWritten, &g_hWriteOverlapped);
        BOOL bPending = FALSE;
        if (ret == FALSE)
        {
            DWORD dwError = GetLastError();
            if (dwError == ERROR_IO_PENDING)
            {
                bPending = TRUE;
                PrintW(L"ERROR_IO_PENDING");
            }
            PrintW(L"ERROR_IO_ERROR");
        }
        if (ret == TRUE)
        {
            PrintA("��¿Ϸ�!");
            CloseHandle(g_hWF);
            return;
        }

        DWORD dwTrans;
        while (bPending)
        {
            ret = GetOverlappedResult(g_hWF, &g_hWriteOverlapped, &dwTrans, FALSE);
            if (ret == TRUE)
            {
                PrintA("��¿Ϸ�!");
                bPending = FALSE;
            }
            else
            {
                DWORD dwError = GetLastError();
                if (dwError == ERROR_IO_INCOMPLETE)
                {
                    PrintW(L"%ld<->%ld\n", g_hWriteOverlapped.Internal, dwTrans);
                }
                else
                {
                    PrintW(L"ERROR_IO_ERROR");
                }
            }
            Sleep(1);
        }
    }
    CloseHandle(g_hWF);
}
void ASyncCallbackRun()
{
    PrintA("�ε�����!\n");
    
    ASyncCallbackLoad(L"DXSDK_Jun10.exe");
   /* PrintA("�ε��Ϸ�\n");
    ASyncCallbackCopy(L"DXSDK_Jun10.exeCopy.exe");
    PrintA("����Ϸ�\n");
    delete[] g_pFileBuffer;*/    
}

void Run()
{
    HANDLE g_hWF = ::CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL, 
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,  NULL);
    if (g_hWF != INVALID_HANDLE_VALUE)
    {
        TCHAR buf[] = L"kgca";
        DWORD dwWritten;
        DWORD dwLength = sizeof(buf);
        BOOL ret = WriteFile(g_hWF, buf,  dwLength, &dwWritten, NULL);
        if (ret == TRUE)
        {
            PrintA("��¿Ϸ�!");
        }
    }
    CloseHandle(g_hWF);

    ZeroMemory(&g_hWriteOverlapped, sizeof(g_hWriteOverlapped));
    HANDLE g_hRF = ::CreateFile(L"test.txt", GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_hRF != INVALID_HANDLE_VALUE)
    {
        TCHAR buf[1024] = { 0, };
        DWORD dwRead;
        DWORD dwLength = sizeof(buf);
        BOOL ret = ReadFile(g_hRF, buf, dwLength, &dwRead, NULL);
        if (ret == TRUE)
        {
            PrintW(L"%s", buf);
            PrintA("�ε�Ϸ�!");
        }               
    } 
    CloseHandle(g_hRF);
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
    ASyncCallbackRun();

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
                   
        }
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
