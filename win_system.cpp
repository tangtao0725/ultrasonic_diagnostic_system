#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QTextCodec>
#include <QMovie>
#include <QtGui>
#include <string>
#include <fstream>
#include "myview.h"
#include "win_system.h"
#include <iostream>
using namespace std;

Win_system::Win_system(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint)
{
	QFont font;
	font.setFamily(("simsun"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	QPalette palette;
	QBrush brushDark(QColor(0,0,0,255));
	QBrush brushBrown(QColor(114,117,118,255));
	QBrush brushWhite(QColor(255,255,255,255));
	palette.setBrush(QPalette::Active,QPalette::Window,brushDark);
	palette.setBrush(QPalette::Inactive,QPalette::Window,brushDark);
	palette.setBrush(QPalette::Disabled,QPalette::Window,brushDark);
	palette.setBrush(QPalette::Active,QPalette::Button,brushBrown);
	palette.setBrush(QPalette::Inactive,QPalette::Button,brushBrown);
	palette.setBrush(QPalette::Disabled,QPalette::Button,brushBrown);
	palette.setBrush(QPalette::Active,QPalette::ButtonText,brushWhite);
	palette.setBrush(QPalette::Inactive,QPalette::ButtonText,brushWhite);
	palette.setBrush(QPalette::Disabled,QPalette::ButtonText,brushWhite);
	palette.setBrush(QPalette::Active,QPalette::WindowText,brushWhite);
	palette.setBrush(QPalette::Inactive,QPalette::WindowText,brushWhite);
	palette.setBrush(QPalette::Disabled,QPalette::WindowText,brushWhite);
	setPalette(palette);
	

	font.setPointSize(100);
	label_title = new QLabel(this);
//	label_title->setGeometry(QRect(200,10,400,40));
	label_title->setGeometry(QRect(200,40,400,40));
	label_title->setText(QObject::tr("系统选项"));
	label_title->setFont(font);
	label_title->setAlignment(Qt::AlignHCenter);

	font.setPointSize(60);

	button_reboot = new QPushButton(this);
	//button_reboot->setGeometry(QRect(80,120,250,60));
	button_reboot->setGeometry(QRect(92,120,250,60));
	button_reboot->setText(QObject::tr("系统重启"));
	button_reboot->setFont(font);

	button_shutdown = new QPushButton(this);
	button_shutdown->setGeometry(QRect(92,220,250,60));
	button_shutdown->setText(QObject::tr("系统关机"));
	button_shutdown->setShortcut(QKeySequence(Qt::ALT + Qt::Key_H));
	button_shutdown->setFont(font);

	button_correct= new QPushButton(this);
	button_correct->setGeometry(QRect(92,320,250,60));
	button_correct->setText(QObject::tr("屏幕矫正"));
	button_correct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_J));
	button_correct->setFont(font);

	button_return = new QPushButton(this);
	button_return->setGeometry(QRect(482,220,250,60));
	button_return->setText(QObject::tr("返回"));
	button_return->setShortcut(QKeySequence(Qt::ALT + Qt::Key_R));
	button_return->setFont(font);

	button_data = new QPushButton(this);
	button_data->setGeometry(QRect(482,120,250,60));
	button_data->setText(QObject::tr("数据管理"));
	button_data->setShortcut(QKeySequence(Qt::ALT + Qt::Key_M));
	button_data->setFont(font);

	button_update = new QPushButton(this);
	button_update->setGeometry(QRect(482,320,250,60));
	button_update->setText(QObject::tr("更新程序"));
	button_update->setShortcut(QKeySequence(Qt::ALT + Qt::Key_U));
	button_update->setFont(font);
	
	winData = new Win_data();
	winData->setGeometry(QRect(200,30,400,400));

	
	QObject::connect(button_shutdown,SIGNAL(clicked()),this,SLOT(shutdown()));
	QObject::connect(button_reboot,SIGNAL(clicked()),this,SLOT(reboot()));
	QObject::connect(button_correct,SIGNAL(clicked()),this,SLOT(correct()));
	QObject::connect(button_return,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(button_data,SIGNAL(clicked()),winData,SLOT(show()));
	QObject::connect(button_data,SIGNAL(clicked()),winData,SLOT(update()));
	QObject::connect(button_update,SIGNAL(clicked()),this,SLOT(update()));
}

void Win_system::shutdown()
{
	int tmp = QMessageBox::warning(this,tr("警告"),tr("您确认关闭系统?"),QMessageBox::Yes,QMessageBox::No);
	if(tmp == QMessageBox::Yes)
	{
		system("shutdown -h now");
	}
}

void Win_system::reboot()
{
	int tmp = QMessageBox::warning(this,tr("警告"),tr("您确认重启系统?"),QMessageBox::Yes,QMessageBox::No);
	if(tmp == QMessageBox::Yes)
	{
		system("reboot");
	}
}

void Win_system::correct()
{
	int tmp = QMessageBox::warning(this,tr("警告"),tr("屏幕校正需要重启系统\n确认?"),QMessageBox::Yes,QMessageBox::No);
	if(tmp == QMessageBox::Yes)
	{
	system("/usr/local/tslib/bin/ts_calibrate");
	system("reboot");
	}
}


void Win_system::update()
{
	int tmp = QMessageBox::warning(this,tr("警告"),tr("更新程序并重启系统\n确认?"),QMessageBox::Yes,QMessageBox::No);
	if(tmp == QMessageBox::Yes)
	{
		string name = "sd";
		string name1;
		QLabel *label = new QLabel(this);
		label->setGeometry(300,200,200,80);
		label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
		label->setText(QObject::tr("正在检测优盘，请稍候"));
		QPalette pa;
		pa.setColor(QPalette::WindowText,Qt::green);
		label->show();
		sleep(3);
		QApplication::processEvents();
		QStringList filters;
		filters << "sd*";
		QDir dir("/dev");
		dir.setNameFilters(filters);
		dir.setFilter(QDir::System);
		QStringList filelist = dir.entryList();
		if(filelist.empty())
		{
			label->setText(QObject::tr("优盘未检测，请检查端口"));
			label->show();
			QApplication::processEvents();
			sleep(1);
			label->hide();
			return;
		}
		else
		{
			label->setText(QObject::tr("正在复制，请稍候"));
			label->show();
			QApplication::processEvents();
		/*msgLabel->setText(QObject::tr("正在复制，请稍候"));
		QPalette pa;
		pa.setColor(QPalette::WindowText,Qt::green);
		msgLabel->setPalette(pa);
		msgLabel->show();
		QApplication::processEvents();
*/		
			string tmp = "mount /dev/" + filelist.first().toStdString() + " /mnt";
			system(tmp.c_str());
			system("sleep 1");

			system("cp -rf /mnt/project /usr/local");

			label->setText(QObject::tr("程序更新成功"));
			label->show();
			QApplication::processEvents();
			sleep(1);
			system("umount /mnt");
			system("sleep 0.2");
			system("reboot");
		}
	}
}

