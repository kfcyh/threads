#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <Windows.h>
#include <queue>
#include <time.h>
#include <stdio.h>
using namespace std;

int r = 3;
mutex mu;
mutex m_cout;
struct DiningTable {
	int fork[5] = { 0,0,0,0,0 };
	mutex my_mutex[5];
	condition_variable cv[5];
};

void think(int numth)
{
	m_cout.lock();
	cout << "哲学家" << numth << "正在思考...\n";
	m_cout.unlock();
	srand(time(NULL));
	Sleep(rand() % 10000);
}

void hungry(int numth)
{
	m_cout.lock();
	cout << "哲学家" << numth << "饿了\n";
	m_cout.unlock();
}

bool down()
{
	if (r < 0)
		return false;
	r--;
	return true;
}

void philosopher(int numth, DiningTable* table)
{
	think(numth);
	hungry(numth);
	mu.lock();
	while (!down()) {}
	//bool ready = down();
	mu.unlock();

	//while (!table->my_mutex[numth].try_lock()) 
	//{ 
	//	//cout << "哲学家" << numth << "等待叉子" << numth << endl;
	//}
	unique_lock<mutex> lck1(table->my_mutex[numth]);
	if (table->fork[numth] == 1)
		table->cv[numth].wait(lck1);
	table->fork[numth] = 1;
	m_cout.lock();
	cout << "哲学家" << numth << "获得叉子" << numth << endl;
	m_cout.unlock();
	//while (!table->my_mutex[(numth+1)%5].try_lock()) 
	//{ 
	//	//cout << "哲学家" << (numth + 1) % 5 << "等待叉子" << (numth + 1) % 5 << endl;
	//}
	unique_lock<mutex> lck2(table->my_mutex[(numth + 1) % 5]);
	if (table->fork[(numth + 1) % 5] == 1)
		table->cv[(numth + 1) % 5].wait(lck2);
	table->fork[(numth + 1) % 5] = 1;
	m_cout.lock();
	cout << "哲学家" << numth << "获得叉子" << (numth + 1) % 5 << "开始进餐..." << endl;
	m_cout.unlock();
	//srand(time(NULL));
	Sleep(10000);
	m_cout.lock();
	cout << "哲学家" << numth << "进餐完毕放回叉子" << numth << "和" << (numth + 1) % 5 << "继续思考\n";
	m_cout.unlock();
	table->fork[numth] = 0;
	table->fork[(numth + 1) % 5] = 0;
	table->cv[(numth + 1) % 5].notify_all();
	table->cv[numth].notify_all();
	r++;
}

int main()
{
	DiningTable* table = new DiningTable;
	thread t[5];
	for (int i = 0; i < 5; i++)
	{
		t[i] = thread(philosopher, i, table);
	}

	for (int i = 0; i < 5; i++)
	{
		t[i].join();
	}

	getchar();
}
