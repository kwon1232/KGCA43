#pragma once
#include <iostream>
class TQueue
{
    int  MAX_SIZE = 10;
public:    
    TQueue(int iSize) // �⺻ ������
    {
        item = new int[iSize];
        MAX_SIZE = iSize;
        for (int i = 0; i < MAX_SIZE; i++)
        {
            item[i] = 0;
        }
    }
    TQueue(const TQueue& queue)
    {
        //item = queue.item; // ���� ����
        // ���� ���� �۾�.
        MAX_SIZE = queue.MAX_SIZE;
        item = new int[MAX_SIZE];
        for (int i = 0; i < MAX_SIZE; i++)
        {
            item[i] = queue.item[i];
        }        
        rear = queue.rear;
        front = queue.front;
    }
    TQueue() // �⺻ ������
    {
        MAX_SIZE = 10;
        item = new int[MAX_SIZE];
        for (int i = 0; i < MAX_SIZE; i++)
        {
            item[i] = 0;
        }
    }
    ~TQueue() // �⺻ ������
    {
        delete[] item;
    }
public:
    int*    item=nullptr;
    int     rear = -1;
    int     front = 0;
public:
    void    Enqueue(int data);
    int     Dequeue();
    int     Size();
    bool    IsEmpty();
    bool    IsFull();
    void    Display();

};

