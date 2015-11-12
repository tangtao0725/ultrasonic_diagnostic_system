#ifndef MYVIEW_H
#define MYVIEW_H
#include <QtCore>
#include <QtGui>
#include <iostream>
#include "mymodel.h"

class MyView:public QListView
{
	Q_OBJECT
public:
	MyView(QWidget* parent);
	void update();
	void deleteSelected();
public slots:
	void getData();
	void copySelected();
	void AllSelect(bool);
	void NoneSelect(bool);
signals:
	void signalShow(QString,Qt::GlobalColor);
	void signalClearShow();
private:
//	QStringListModel *model;
	MyModel *model;
	QStringList list;
	QStringList checkedList;
};
#endif

