#include "TPacketPool.h"

void    TPacketPool::Add(T_Packet& tPacket)
{
	// write ����
	{
		std::lock_guard<std::shared_mutex> lock(m_sMutex);
		// �Ӱ豸��
		list.push_back(tPacket);
	}
	//m_sMutex.lock();
	//	// �Ӱ豸��
	//	list.push_back(tPacket);
	//m_sMutex.unlock();
}

void    TPacketPool::Process(T_Packet& tPacket)
{
	// read ����
	{
		std::shared_lock<std::shared_mutex> lock(m_sMutex);
		// ����
		for (auto& packet : list)
		{
		}
	}
	//m_sMutex.lock_shared();
	//	// ����
	//	for (auto& packet : list)
	//	{

	//	}
	//m_sMutex.unlock_shared();
}