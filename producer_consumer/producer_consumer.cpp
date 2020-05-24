#include<iostream>
#include <mutex>
#include <condition_variable>
#include <windows.h>
#include <thread>

using namespace std;

static const int buffer_size = 10; // 缓存大小
static const int item_total = 100; //总共要生产 item_total个item

								   // 缓存结构体, 使用循环队列当做缓存
struct Buffer
{
	int buffer[buffer_size];
	size_t read_position; // 当前读位置
	size_t write_position; // 当前写位置
	mutex mtx; // 读写互斥
			   //条件变量
	condition_variable not_full;
	condition_variable not_empty;
}buffer_res;

typedef struct Buffer Buffer;

void porduce_item(Buffer *b, int item)
{
	unique_lock<mutex> lock(b->mtx);//设置互斥锁

	while (((b->write_position + 1) % buffer_size) == b->read_position) {
		//当前缓存已经满了
		cout << "buffer is full now, producer is wating....." << endl;
		(b->not_full).wait(lock); // 等待缓存非full
	}
	// 向缓存中添加item
	(b->buffer)[b->write_position] = item;
	(b->write_position)++;

	// 若到达最后一个, 写位置置位0
	if (b->write_position == buffer_size)
		b->write_position = 0;

	(b->not_empty).notify_all();
	//lock.unlock();
}

int consume_item(Buffer *b)
{
	int data;
	unique_lock <mutex> lock(b->mtx);
	while (b->write_position == b->read_position)
	{   // 当前buffer 为空
		cout << "buffer is empty , consumer is waiting....." << endl;
		(b->not_empty).wait(lock);
	}

	data = (b->buffer)[b->read_position];
	(b->read_position)++;

	if (b->read_position >= buffer_size)
		b->read_position = 0;

	(b->not_full).notify_all();
	//lock.unlock();

	return data;
}

//生产者任务
void producer() {
	for (int i = 1; i <= item_total; i++) {
		cout << "prodece the " << i << "^th item ..." << endl;
		porduce_item(&buffer_res, i);
	}
}

//消费者任务
void consumer()
{
	static int cnt = 0;
	while (1) {
		Sleep(1);
		int item = consume_item(&buffer_res);
		cout << "consume the " << item << "^th item" << endl;
		if (++cnt == item_total)
			break;
	}
}

//初始化 buffer
void init_buffer(Buffer *b)
{
	b->write_position = 0;
	b->read_position = 0;
}

int main()
{
	init_buffer(&buffer_res);
	thread prodece(producer);
	thread consume(consumer);
	prodece.join();
	consume.join();
	getchar();
}
