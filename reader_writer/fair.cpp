/***********读写竞争**********/
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <Windows.h>
#include <vector>
#include <shared_mutex>
using namespace std;

unsigned int numOfReader = 0;	//信号量，标记读者数量
unsigned int numOfWriter = 0;	//信号量，标记写者数量

shared_mutex my_mutex;				//读写锁
mutex cout_mutex;
void writer()
{
	numOfWriter++;
	int num = numOfWriter;

	unique_lock<shared_mutex> lck(my_mutex,std::defer_lock);
	if (!lck.try_lock())
	{
		cout_mutex.lock();
		cout << "当前读者或写者运行!写者"<<num<<"等待...\n";
		cout_mutex.unlock();
		while (!lck.try_lock()) {}
	}

	/*if (!my_mutex.try_lock())
	{
		cout_mutex.lock();
		cout << "当前读者或写者运行!写者" << num << "等待...\n";
		cout_mutex.unlock();
		while(!my_mutex.try_lock()){}
	}*/

	cout_mutex.lock();
	cout << "写者" << num << "运行...\n";
	cout_mutex.unlock();

	srand(time(NULL));
	Sleep(rand() % 4000 + 1);
	cout_mutex.lock();
	cout << "写者" << num << "退出!\n";
	cout_mutex.unlock();
	numOfWriter--;
	//my_mutex.unlock();

}

void reader()
{
	numOfReader++;
	int num = numOfReader;

	shared_lock<shared_mutex> lck(my_mutex, std::defer_lock);
	if (!lck.try_lock())
	{
		cout_mutex.lock();
		cout << "写者运行!读者"<<num<<"等待...\n";
		cout_mutex.unlock();
		while (!lck.try_lock()) {}
	}

	/*if (!my_mutex.try_lock_shared())
	{
		cout_mutex.lock();
		cout << "写者运行!读者" << num << "等待...\n";
		cout_mutex.unlock();
		while (!my_mutex.try_lock_shared()) {}
	}*/

	cout_mutex.lock();
	cout << "读者" << num << "运行...\n";
	cout_mutex.unlock();

	srand(time(NULL));
	Sleep(rand() % 4000 + 1);
	cout_mutex.lock();
	cout << "读者" << num << "退出!\n";
	cout_mutex.unlock();
	numOfReader--;
	//my_mutex.unlock_shared();
}

void choose(int num)
{
	switch (num)
	{
	case 0:
		reader();
		break;
	case 1:
		writer();
		break;
	default:
		break;
	}
}

int main()
{
	thread t[10];
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		t[i] = thread(choose, rand() % 2);
	}

	for (int i = 0; i < 10; i++)
	{
		t[i].join();
	}
	getchar();
}
