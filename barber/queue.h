#pragma once
#include <cstdlib>
#define MAXSIZE 11  //循环队列容量
using namespace std;

template <typename T>
class queue
{
public:
	queue() :_front(0), _rear(0), _size(0) {};	//默认构造函数
	~queue() {}; //默认析构函数

	T back()const;	 //返回最后一个元素
	bool empty()const;	 //如果队列空则返回真
	T front()const;		//返回第一个元素
	void pop();		//删除第一个元素
	void push(T value);		//在末尾加入一个元素
	int size()const;	//返回队列中元素的个数

private:
	T data[MAXSIZE];	//队列数组
	int _front;	//队头游标
	int _rear;	//队尾游标
	unsigned int _size;	//队列元素数目
	const unsigned int capacity = MAXSIZE - 1;	//队列容量=MAXSIZE-1
};

template<typename T>
inline T queue<T>::back() const
{
	if (_front == _rear)
		return 0;
	return data[(_rear - 1 + MAXSIZE) % MAXSIZE];
}

template<typename T>
inline bool queue<T>::empty() const
{
	if (_front == _rear)
		return true;
	return false;

}

template<typename T>
inline T queue<T>::front() const
{
	if (_front == _rear)
		return 0;
	return data[_front];
}

template<typename T>
inline void queue<T>::pop()
{
	if (_front == _rear)
		return;
	_front = (_front + 1) % MAXSIZE;
	_size--;
}

template<typename T>
inline void queue<T>::push(T value)
{
	if ((_rear + 1) % MAXSIZE == _front)
		return;
	data[_rear] = value;
	_rear = (_rear + 1) % MAXSIZE;
	_size++;
}

template<typename T>
inline int queue<T>::size() const
{
	return _size;
}
