/**************睡眠的理发师************/
#include "queue.h"
#include <iostream>
#include <stdlib.h>
#include <mutex>
#include <condition_variable>
#include <Windows.h>
#include <vector>
using namespace std;

mutex qmutex;
condition_variable cvhair;
queue<int> qhair;
mutex cout_mutex;
int count_consumer=0;
int n = 40;

void barber()
{
	while (1)
	{
		unique_lock<mutex> lck(qmutex);
		if (qhair.empty())
		{
			cout_mutex.lock();
			cout << "无人理发，理发师开始睡眠...\n";
			cout_mutex.unlock();
			cvhair.wait(lck, [] {return !qhair.empty(); });
		}
		else
		{
			int k = qhair.front();
			cout_mutex.lock();
			cout << "理发师为今日第" << k << "位顾客理发...";
			cout_mutex.unlock();
			qhair.pop();
			cout_mutex.lock();
			cout << "目前排队等待顾客" << qhair.size() << "人"<<endl;
			cout_mutex.unlock();
			lck.unlock();
			Sleep(4000);
			cout_mutex.lock();
			cout << "理发师为今日第" << k << "位顾客理发完毕...\n";
			cout_mutex.unlock();

		}
	}
}

void consumer()
{
	while (n>0)
	{
		srand(time(NULL));
		Sleep(rand() % 1000);
		count_consumer++;
		unique_lock<mutex> lck(qmutex);
		if (qhair.size() < 10)
		{
			qhair.push(count_consumer);
			cout_mutex.lock();
			cout << "今日第" << count_consumer << "位顾客等待理发...当前等待人数" << qhair.size() << endl;
			cout_mutex.unlock();
			cvhair.notify_all();
		}
		else
		{
			cout_mutex.lock();
			cout << "等待理发人数过多,今日第" << count_consumer << "位顾客离开理发店\n";
			cout_mutex.unlock();
			cvhair.notify_all();
			//lck.unlock();
		}
		n--;

	}
}

int main()
{
	thread _barber(barber);
	/*vector<thread> vec;
	for (int i = 0; i < 20; i++)
	{
		vec.push_back(thread(consumer));
	}
	*/
	thread _consumer(consumer);
	_barber.join();
	_consumer.join();
	getchar();

}
