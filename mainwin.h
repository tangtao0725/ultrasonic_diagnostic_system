/*written by Wang Wenbin,time:2010-7-3
  this file is the top interface,used to arrange others, waiting for improvment!!
*/

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <string>
class Data;
class QPushButton;
class QLabel;

class Mainwin:public QWidget
{
	Q_OBJECT
public:
	Mainwin(QWidget *parent = 0,Qt::WindowFlags flags = 0);
private:
//	QPushButton *button_win_1;//参数输入按钮
	QPushButton *button_win_2;//开始测量按钮
	QPushButton *button_win_3;//诊断报告按钮
	QPushButton *button_win_4;//帮助文档按钮
	QPushButton *button_database;

	QPushButton *button_system;

	QLabel *label_title;
	QLabel *label_gif;
signals:
//	void button_win_1_click();
	void button_win_2_click();
	void button_win_3_click();
	void button_win_4_click();
	void button_database_click();
	void button_system_click();
public slots:
};
#endif
