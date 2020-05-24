/***********写者优先***********/
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
unsigned int read = 0;			//信号量，标记写者数量

//bool isWrite = false;			//信号量，标记是否正在进行写操作
//bool isRead = false;			//信号量，标记是否正在进行写操作
mutex write_mutex;				//写线程锁
mutex cout_mutex;				//输出锁
//vector<int> vec;				//共享数据

void writer()
{
	numOfWriter++;
	int num = numOfWriter;
	if (read)
	{
		cout_mutex.lock();
		cout << "当前有读者运行!" << "写者" << num << "等待...\n";
		cout_mutex.unlock();
		while (read) {}
	}
	if (!write_mutex.try_lock())
	{
		cout_mutex.lock();
		cout << "当前有写者运行!" << "写者" << num << "等待\n";
		cout_mutex.unlock();
		while(!write_mutex.try_lock()){}
	}
	cout_mutex.lock();
	cout << "写者" << num << "运行...\n";
	cout_mutex.unlock();

	srand(time(NULL));
	Sleep(rand() % 4000 + 1);
	cout_mutex.lock();
	cout << "写者" << num << "退出!\n";
	cout_mutex.unlock();
	numOfWriter--;
	write_mutex.unlock();

}

void reader()
{
	numOfReader++;
	int num = numOfReader;
	if (numOfWriter)
	{
		cout_mutex.lock();
		cout << "读者" << num << "等待写者运行!无法进行读操作\n";
		cout_mutex.unlock();
		while (numOfWriter) {}
	}
	read++;
	cout_mutex.lock();
	cout << "当前无写者运行!读者" << num << "运行...\n";
	cout_mutex.unlock();
	srand(time(NULL));
	Sleep(rand() % 4000 + 1);
	
	cout_mutex.lock();
	cout << "读者" << num << "退出!\n";
	cout_mutex.unlock();
	read--;
	
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
