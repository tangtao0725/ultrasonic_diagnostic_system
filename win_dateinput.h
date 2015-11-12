/*
Written by houli, for date inputing! 2012\03\06
*/

#ifndef WIN_DATEINPUT_H
#define WIN_DATEINPUT_H

#include <QWidget>
#include <string>
#include <iostream>
using namespace std;

class QDateEdit;
class QGroupBox;
class QLabel;
class QPushButton;

class Win_dateinput:public QWidget
{
	Q_OBJECT
public:
	Win_dateinput(QWidget *parent = 0, Qt::WindowFlags flags = 0);

public slots:
	void setDate();
signals:
	void button_confirm_clicked(string date);
	void signalDateok();
		
private:
	QGroupBox *box_top;
	QLabel *label_title;
	QDateEdit *edit_date;
	QPushButton *button_confirm;
	
};
#endif
