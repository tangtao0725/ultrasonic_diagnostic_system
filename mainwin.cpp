#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QTextCodec>
#include <QMovie>
#include <QtGui>
#include <QtCore>
#include <string>
#include <fstream>
#include "mainwin.h"
#include <iostream>
//extern win_1 *w1;
using namespace std;

Mainwin::Mainwin(QWidget *parent,Qt::WindowFlags flags)
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
	

	font.setPointSize(120);
//	font.setPointSize(80);
	label_title = new QLabel(this);
//	label_title->setGeometry(QRect(200,10,400,40));
	label_title->setGeometry(QRect(212,40,400,40));
//	label_title->setGeometry(QRect(132,40,560,40));
	label_title->setText(QObject::tr("松质骨超声诊断仪"));
//	label_title->setText(QObject::tr("Ultrasound Bone Diagnosis Instrument"));
	label_title->setFont(font);
	label_title->setAlignment(Qt::AlignHCenter);

	font.setPointSize(70);

/*	
	button_win_1 = new QPushButton(this);
	//button_win_1->setGeometry(QRect(80,120,250,60));
	button_win_1->setGeometry(QRect(92,120,250,60));
	button_win_1->setText(QObject::tr("参数输入"));
//	button_win_1->setText(QObject::tr("Parameter"));
	button_win_1->setShortcut(QKeySequence(Qt::ALT + Qt::Key_P));
	button_win_1->setFont(font);
*/

	button_win_2 = new QPushButton(this);
	button_win_2->setGeometry(QRect(92,120,250,60));
	button_win_2->setText(QObject::tr("开始测量"));
//	button_win_2->setText(QObject::tr("Measurement"));
	button_win_2->setShortcut(QKeySequence(Qt::ALT + Qt::Key_B));
	button_win_2->setFont(font);

	button_win_3 = new QPushButton(this);
	button_win_3->setGeometry(QRect(92,220,250,60));
	button_win_3->setText(QObject::tr("诊断报告"));
//	button_win_3->setText(QObject::tr("Report"));
	button_win_3->setShortcut(QKeySequence(Qt::ALT + Qt::Key_R));
	button_win_3->setFont(font);

	button_win_4 = new QPushButton(this);
	button_win_4->setGeometry(QRect(92,320,250,60));
	button_win_4->setText(QObject::tr("帮助文档"));
//	button_win_4->setText(QObject::tr("Help"));
	button_win_4->setShortcut(QKeySequence(Qt::ALT + Qt::Key_H));
	button_win_4->setFont(font);
	
	button_database = new QPushButton(this);
	button_database->setGeometry(QRect(482,220,250,60));
	button_database->setText(QObject::tr("数据库管理"));
//	button_database->setText(QObject::tr("Database"));
	button_database->setShortcut(QKeySequence(Qt::ALT + Qt::Key_D));
	button_database->setFont(font);

	button_system = new QPushButton(this);
	button_system->setGeometry(QRect(482,120,250,60));
	button_system->setText(QObject::tr("系统选项"));
//	button_system->setText(QObject::tr("System"));
	button_system->setShortcut(QKeySequence(Qt::ALT + Qt::Key_S));
	button_system->setFont(font);

//	QObject::connect(button_win_1,SIGNAL(clicked()),this,SIGNAL(button_win_1_click()));
	QObject::connect(button_win_2,SIGNAL(clicked()),this,SIGNAL(button_win_2_click()));
	QObject::connect(button_win_3,SIGNAL(clicked()),this,SIGNAL(button_win_3_click()));
	QObject::connect(button_win_4,SIGNAL(clicked()),this,SIGNAL(button_win_4_click()));
	QObject::connect(button_system,SIGNAL(clicked()),this,SIGNAL(button_system_click()));
	QObject::connect(button_database,SIGNAL(clicked()),this,SIGNAL(button_database_click()));
}

