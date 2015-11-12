#ifndef WIN_START_H
#define WIN_START_H

#include <QtGui>
#include <QtCore>

class Win_Start:public QWidget
{
	Q_OBJECT
public:
	Win_Start(QWidget *parent = 0,Qt::WindowFlags flags = 0);
public slots:
	void valueSet(int);
public:
	QProgressBar *pb;
	QLabel *label1;
	QLabel *label2;

};
#endif
