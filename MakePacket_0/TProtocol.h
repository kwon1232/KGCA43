#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4
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
	char           msg[2048];
}UPACKET;

typedef struct
{
	char   szName[13];  //1+1	
	int    iID;        // 4
	char   szMsg[1024];   // 4+4
}CHAT_MSG;
typedef struct
{
	char   szName[13];
	int    iID;
	int    iData[10];
}ITEM_CREATE;
#pragma pack(pop)
//
//enum TPACKET_TYPE
//{
//	PACKET_VERIFICATION_REQ = 1,
//	PACKET_VERIFICATION_ACK,
//
//	PACKET_CHAT_NAME_REQ = 1000, // ��ȭ�� �Է¿�û	
//	PACKET_CHAT_NAME_ACK, // ��ȭ�� �Է� ����
//	PACKET_CHAT_MSG, // ���� ä�� �޼���.
//	PACKET_LOGIN_REQ, // �α��ο�û
//	PACKET_LOGOUT_REQ, // �α׾ƿ���û
//	PACKET_LOGIN_ACK, // �α�������
//	PACKET_LOGOUT_ACK, // �α׾ƿ�����
//	PACKET_LOGOUT_PLAYER, // �α׾ƿ� �÷��̾�
//
//	PACKET_CREATE_ACCOUNT_REQ = 2000, // ����������û
//	PACKET_DELETE_ACCOUNT_REQ, // ����������û
//	PACKET_CREATE_ACCOUNT_ACK, // ������������
//	PACKET_DELETE_ACCOUNT_ACK, // ������������
//
//	PACKET_USER_POSITION = 3000, // �ɸ��� ��ġ �̵� ����
//	PACKET_STOP_POSITION,// �ɸ��� ����
//	PACKET_LOBBY_USER_INFO, // ���� ��������Ʈ ����
//
//
//	PACKET_CREATE_CHARACTER_REQ = 4000, // �ɸ��� ���� ��û
//	PACKET_DELETE_CHARACTER_REQ, // �ɸ��� ���� ��û
//	PACKET_CREATE_CHARACTER_ACK, // �ɸ��� ���� ����
//	PACKET_DELETE_CHARACTER_ACK, // �ɸ��� ���� ����
//
//	PACKET_HIT_CHARACTER = 5000, // �ɸ��� �ǰ� ����
//	PACKET_HIT_NPC, // NPC �ǰ� ����
//	PACKET_ATTACK_CHARACTER, // �ɸ��� ���� ����
//	PACKET_ATTACK_NPC, // NPC ���� ����
//	PACKET_DEAD_CHARACTER, // �ɸ��� ����
//	PACKET_DEAD_NPC, // NPC ����
//	PACKET_DAMAGE_CHARACTER, // �ɸ��� ������ ����
//	PACKET_DAMAGE_NPC, // NPC ������ ����
//	PACKET_SPAWN_CHARACTER, // �ɸ��� ����
//	PACKET_SPAWN_NPC, // NPC ����
//	PACKET_SPAWN_NPC_LIST, // 
//	PACKET_SYNC_CHARACTER, // �ɸ��� ����ȭ
//	PACKET_SYNC_NPC_LIST, // NPC ����ȭ	
//	PACKET_NPC_TARGETMOVE,  // NPC Ÿ���̵�	
//
//
//	PACKET_ZONE_ENTRY_REQ = 6000,//������ ���� ��û
//	PACKET_ZONE_ENTRY_ACK, // ���� �� ���� ����
//	PACKET_ZONE_NEW_PLAYER, // ���� �� ���� �ɸ��� ���� ����
//	PACKET_ZONE_PLAYERS_INFO, // ���� ���� ������ �������� �÷��� �ɸ��� ���� ���� 
//	PACKET_ZONE_USER_INFO,// ������ ��������Ʈ ����(�÷��̾�+��Ÿ(������ ��))
//	PACKET_ZONE_TARGET_REQ, // �� Ÿ�� ��ġ ��û  
//	PACKET_ZONE_TARGET_ACK, // �� Ÿ�� ��ġ ����
//};

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