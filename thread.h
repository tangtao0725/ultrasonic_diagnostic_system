#ifndef THREAD_H
#define THREAD_H
#include <vector>
#include <QVector>
#include <QThread>
#include <QMutex>
#include <QtGui>
using namespace std;
class Thread:public QThread
{
	Q_OBJECT
public:
	Thread(bool lhs = false);
	void stop();
	void begin();
	void mutexLock();
	void mutexUnlock();
	void setFeq(double input);
	void setFilter(bool);
protected:
	void run();
private:
	QMutex mutex;
	volatile bool flag;
	vector<double> data;
//	vector<double> coef;
	int fd;
	unsigned short feq;
	bool isFilter;
signals:
	void signalThreadData(const vector<double>);
};

class ThreadForStart:public QThread
{
	Q_OBJECT
public:
	ThreadForStart(QProgressBar*);
	void run();
private:
	QProgressBar *p;
signals:
	void signalStartOk();
};
#endif
