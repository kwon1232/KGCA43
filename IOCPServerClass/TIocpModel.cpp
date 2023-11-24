#include "TIocpModel.h"
#include "TUser.h"

DWORD WINAPI WorkerThread(LPVOID param)
{
    TIocpModel* iocp = (TIocpModel*)param;
    DWORD dwTransfer; // �а�,���� �� ����Ʈ 
    ULONG_PTR KeyValue;
    OVERLAPPED* ov;

    while (1)
    {
        if (WaitForSingleObject(iocp->g_hKillEvent, 0) == WAIT_OBJECT_0)
        {
            break;
        }
        // �񵿱� ����¿� ����� IOCP ť�� ����ȴ�.
        //GetQueuedCompletionStatus�� IOCPť���� �Ϸ� ����� �����´�.
        BOOL bReturn = ::GetQueuedCompletionStatus(iocp->g_hIOCP, &dwTransfer, &KeyValue, &ov,
            0);

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
                ::SetEvent(iocp->g_hKillEvent);
                break;
            }
            //#define ERROR_HANDLE_EOF                 38L   
            E_MSG("error");
            PrintDetailA("Error!");
            ::SetEvent(iocp->g_hKillEvent);
            break;
        }
    }
    return 0;
}

bool TIocpModel::Init()
{
    g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
    g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    DWORD dwThreadID;
    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        g_hWorkerThread[iThread] = ::CreateThread(0, 0, WorkerThread,
                                                    this, 0, &dwThreadID);
    }
    return true;
}