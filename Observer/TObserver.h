#pragma once
#include <iostream>
#include <vector>
#include <list>
// 1)������ - �̺�Ʈ�� �����ϰ� �ִٰ� ��ȭ�� ����� �˷��ִ� ��Ȱ.
// 2)       - �̺�Ʈ�� ���� �޾��� �� ������ �����͸� �������� ���·� ǥ���ϴ� ���.
class TObserver
{
public:
	virtual void update() = 0;
};

