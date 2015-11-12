/*written by Wang Wenbin,time:2010-7-3
  this file is the top interface,used to arrange others, waiting for improvment!!
*/

#ifndef WIN_SYSTEM_H
#define WIN_SYSTEM_H

#include <QWidget>
#include <string>
#include "win_data.h"
class Data;
class QPushButton;
class QLabel;

class Win_system:public QWidget
{
	Q_OBJECT
public:
	Win_system(QWidget *parent = 0,Qt::WindowFlags flags = 0);
private:
	QPushButton *button_shutdown;
	QPushButton *button_reboot;
	QPushButton *button_correct;
	QPushButton *button_return;
	QPushButton *button_data;
	QPushButton *button_update;

	Win_data *winData;

	QLabel *label_title;
public slots:
	void shutdown();
	void reboot();
	void correct();
	void update();
};

#endif
