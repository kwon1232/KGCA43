#pragma once
#include "TComponent.h"
class TContext
{
	TComponent* sA = nullptr; // ��ü����
public:
	void Set(TComponent* s)
	{
		sA = s;
	}
	void operation()// �������� ���� ��Ģ
	{
		return sA->Operation();
	}
	void operation(TComponent* s)// �������� ���� ��Ģ
	{
		return s->Operation();
	}
	TContext(TComponent* s) : sA(s)
	{

	}
	TContext() = default;
};

