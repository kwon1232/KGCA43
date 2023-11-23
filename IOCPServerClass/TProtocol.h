#pragma once
#include <winsock2.h>
#include <windows.h>
#define PACKET_HEADER_SIZE 4
#define PACKET_MSG_BUFFER_SIZE 2048
#define PACKET_MAX_SIZE (PACKET_HEADER_SIZE+PACKET_MSG_BUFFER_SIZE)

#pragma pack(push, 1)
// ��Ŷ ���� ���
typedef struct
{
	WORD  len;
	WORD  type;	
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER  ph;
	char           msg[PACKET_MSG_BUFFER_SIZE];
}UPACKET;

typedef struct
{
	char   szName[13];  //1+1	
	int    iID;        // 4
}CHAT_HEADER;
typedef struct
{
	CHAT_HEADER header;
	char   szMsg[1024];   // 4+4
}CHAT_MSG;
typedef struct
{
	byte   szName[13];
	int    iID;
	int    iData[10];
}ITEM_CREATE;

#pragma pack(pop)

// ��Ŷ Ÿ��
#define PACKET_CHAT_MSG			1000
#define PACKET_GAME_START		2000
#define PACKET_GAME_END			3000
#define PACKET_CREATE_CHARACTER 4000
#define PACKET_MOVE_CHARACTER   5000

/*
* UPACKET sendpacket;
* sendpacket.len = PACKET_HEADER_SIZE + "�ȳ��ϼ���";
* sendpacket.type = PACKET_CHAT_MSG;
* sendpacket.msg = "�ȳ��ϼ���";
* char * sendbuffer = (char*)sendpacket;
* 
* UPACKET recvpacket;
* memcpy(recvpacket, sendbuffer, sizeof(sendbuffer));
  CHAT_MSG msg;
  memcpy(msg, recvpacket.msg, sizeof(CHAT_MSG));
*/