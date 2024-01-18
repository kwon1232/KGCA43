#pragma once
#include "TStrategy.h"
class TContext
{
	TStrategy* sA = nullptr; // ��ü����
public:
	void Set(TStrategy* s)
	{
		sA = s;
	}
	int operation()// �������� ���� ��Ģ
	{
		return sA->algorithm();
	}
	int operation(TStrategy* s)// �������� ���� ��Ģ
	{
		return s->algorithm();
	}
	TContext(TStrategy* s) : sA(s)
	{

	}
	TContext() = default;
};

