#pragma once
#include "TNetStd.h"

#define MAX_RECV_STREAM_SIZE (PACKET_MAX_SIZE*10)

class TUser;

class TStreamPacket 
{
public:	
	UPACKET*    m_pPacketStart;
	PVOID       m_pWritePos;
	PVOID 		m_pPacket;
	char		m_RecvBuffer[MAX_RECV_STREAM_SIZE];
	// 64��Ʈ������,   volatile(�����Ϸ� ����ȭ ����)
	// const int a = 0;
	// if( 0 ) {} // ����ȭ
	long long volatile	m_iStartPos;
	long long volatile	m_iWritePos;
	long long volatile	m_iReadPos;
public:

	bool    Put(char* redvBuffer, int iSize, TUser* = NULL);
	bool    GetPacket(TUser& user);
	bool	ParityCheck();
	void    Reset();
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};
