#include <QtCore>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include "thread.h"
#include <time.h>
#include "parameter.h"
using namespace std;

Thread::Thread(bool lhs)
{
	isFilter = false;
	flag = lhs;
	fd = open(driverFilePath.c_str(),O_RDWR);	//利用了parameter.h中的driverFilePath
/*	ifstream fin("/usr/local/coef.txt");
	double tmp = 0;
	while(!fin.eof())
	{
		fin >> tmp;
		if(fin.fail())
			break;
		coef.push_back(tmp);
	}
*/	data = vector<double>(2048,0);
}

void Thread::mutexLock()
{
	mutex.lock();
}

void Thread::mutexUnlock()
{
	mutex.unlock();
}

void Thread::run()
{
	static double tmp = 0;
	static unsigned short dataTmp[len] = {0};
	cout << "thread start!\n" << endl;
	if(write(fd,&feq,2) != 2)
		cout << "write feq error\n" << endl;
	while(!flag)
	{
		data.clear();			
		if(read(fd,dataTmp,4096) != 4096)
			cout << "read data error\n" << endl;
		for(int i = 0;i < len;++i)
			data.push_back((dataTmp[i]-2048)/2048.0);
		//data = filter_FIR(data,coef);
		emit signalThreadData(data);	//把接收到的时域信号发送出去
		usleep(200000);
	}
}

void Thread::stop()
{
	flag = true;
}
void Thread::begin()
{
	flag = false;
}

void Thread::setFeq(double input)	//设置探头的中心频率
{
	cout << "input:" << input << endl;
	if(abs(input - 1.0) < 0.1)
		feq = 1;
	else if(abs(input - 2.25) < 0.1)
		feq = 2;
	else if(abs(input - 3.5) < 0.1)
		feq = 3;
	else if(abs(input - 5) < 0.1)
		feq = 4;
	else 
		feq = 5;
	
}

void Thread::setFilter(bool lhs)
{
	isFilter = lhs;
}

ThreadForStart::ThreadForStart(QProgressBar* rhs):p(rhs){}

void ThreadForStart::run()
{
	for(int i = 0; i < 100; ++i)
	{
		p->setValue(i);
		usleep(50000);
		QApplication::processEvents();
	}
	emit signalStartOk();
}
