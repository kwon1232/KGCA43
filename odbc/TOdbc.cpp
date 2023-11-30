#include "TOdbc.h"
bool TOdbc::Connect()
{
	// �ش����̹��� ����Ͽ� �ش� �����ͺ��̽��� ����.
	TCHAR inConnect[255] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	_stprintf(inConnect,
		L"DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\cigarette.accdb;", dir);

	TCHAR outConnect[255] = { 0, };
	SQLSMALLINT   cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL,
		inConnect, _countof(inConnect),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		return false;
	}

	hr = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc,
		&g_hStmt);
	if (hr != SQL_SUCCESS)
	{
		return false;
	}
	return true;
}
bool TOdbc::Init()
{
	SQLRETURN hr = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE,
				&g_hEnv);
	if (hr == SQL_SUCCESS)
	{
		hr = SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, 
						(SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER);
		if (hr != SQL_SUCCESS)
		{
			return false;
		}

		hr = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv,
			&g_hDbc);
		if (hr == SQL_SUCCESS)
		{
			return true;
		}		
	}
	return false;
}
bool TOdbc::Release()
{
	if (g_hStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	}
	if (g_hDbc)
	{
		SQLDisconnect(g_hDbc);
	}
	if(g_hDbc)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	}
	if (g_hEnv)
	{
		SQLFreeEnv(g_hEnv);
		SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
	}
	return true;
}