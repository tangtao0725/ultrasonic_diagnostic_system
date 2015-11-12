#ifndef WIN_DATA_H
#define WIN_DATA_H

#include <QtCore>
#include <QtGui>
#include "myview.h"
class Win_data:public QWidget
{
	Q_OBJECT
public:
	Win_data(QWidget *parent = 0,Qt::WindowFlags flags = 0);

public slots:
	void update();
	void slotShow(QString,Qt::GlobalColor);
	void slotClearShow();
	void deleteSelect();

private:
	QPushButton *button_delete;
	QPushButton *button_copy;
	QRadioButton *radioButton_allSelect;
	QRadioButton *radioButton_noneSelect;
	QPushButton *button_close;
	QLabel *msgLabel;
	MyView *view;

};

#endif
