#include "TServer.h"

void TServer::ChatMsg(T_Packet& t)
{
	m_PacketBroadcasttingPool.Add(t);
}
void TServer::tPACKET_MOVE_CHARACTER(T_Packet& t)
{
    // ���� �۾��� ��
    T_Packet SendPacket;
    SendPacket.m_pUser = t.m_pUser;
    m_PacketBroadcasttingPool.Add(SendPacket);
}
void TServer::tPACKET_CREATE_CHARACTER(T_Packet& t)
{
    // �ش� ��Ŷ�� ó���� ����� �ش� �������� ����.
    // MAKE Packet 
    UPACKET SendPacket;
    t.m_pUser->list.push_back(SendPacket);
}