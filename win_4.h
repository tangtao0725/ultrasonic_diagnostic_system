/*written by Wang Wenbin,time: 2010-7-3
  this file is designed to do some help files,waiting for improvement!!
*/

#ifndef WIN_4_H
#define WIN_4_H

#include <QWidget>

class QLabel;
class QPushButton;
class QTextBrowser;
class Win_video;

class Win_4:public QWidget
{
	Q_OBJECT
public:
	Win_4(QWidget *parent = 0,Qt::WindowFlags flags = 0);
	void add_help_products();
	void add_help_use();
public slots:
	void button_clicked();
	void button_2_clicked();
private:
	QLabel *label;
	QPushButton *button;
	QPushButton *button_2;
	QPushButton *button_3;
	QPushButton *button_4;
	QTextBrowser *textBrowser;
	QTextBrowser *textBrowser_2;
	Win_video *m;

public slots:
//	void play();
};

#endif

