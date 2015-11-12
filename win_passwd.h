/*written by wenbin Wang,time:2010-08-10
  this file provide the function of set password for database 
  need improvment
  */
#ifndef WIN_PASSWD_H
#define WIN_PASSWD_H

#include <QWidget>

class QPushButton;
class QLabel;
class QLineEdit;

class Win_passwd:public QWidget
{
	Q_OBJECT
public:
	Win_passwd(QWidget *parent = 0,Qt::WindowFlags flags = 0);
private:
	QLabel *label1;
	QLabel *label2;
	QLabel *label3;
	QPushButton *button1;
	QPushButton *button2;
	QLineEdit *lineedit1;
	QLineEdit *lineedit2;
public slots:
	void processToSure();
signals:
	void inputRight();
};
#endif
