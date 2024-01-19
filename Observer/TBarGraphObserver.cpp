#include "TBarGraphObserver.h"
#include "TSubject.h"
void TBarGraphObserver::update()
{
	m_pUpdateList->m_ScoreListBar.clear();

	for (auto iter = m_pScoreData->m_ScoreListBar.begin();
		iter != m_pScoreData->m_ScoreListBar.end();
		iter++)
	{
		//TScoreCard* pScore = iter->get();
		m_pUpdateList->m_ScoreListBar.push_back(*iter);
	}
}

void   TBarGraphObserver::print()
{
	for( auto iter = m_pUpdateList->m_ScoreListBar.begin();
		 iter != m_pUpdateList->m_ScoreListBar.end();
		 iter++)
	{
		TScoreCard* pScore = (TScoreCard*)*iter;
		std::wcout << pScore->name << std::endl;
	}
}

TBarGraphObserver::TBarGraphObserver(TSubject* pScores)
{
	pScores->observes.push_back(this);
	// ��Ʈ�� ������ ȣ��
	m_pUpdateList = std::make_shared<TSubject>(L"bb");		
	//�⺻ ��������� ȣ��
	m_pScoreData = std::make_shared<TSubject>(*pScores);	
	// �����͸� �޴� �����ڰ� �־�� �Ѵ�.
	//m_pScoreData = std::make_shared<TSubject>(pScores);
	
}