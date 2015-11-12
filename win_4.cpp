#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextCodec>
#include <fstream>
#include <string>
#include <time.h>
#include "win_4.h"
#include "win_video.h"

Win_4::Win_4(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint)
{
	QFont font;
	font.setPointSize(80);
	font.setFamily(("simsun"));
	font.setBold(true);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

	QPalette palette;
	QBrush brushDark(QColor(0,0,0,255));
	QBrush brushBrown(QColor(114,117,118,255));
	QBrush brushWhite(QColor(255,255,255,255));
	QBrush brushGreen(QColor(170,255,0,255));
	QBrush brushBlue(QColor(0,0,255,255));
	palette.setBrush(QPalette::Active,QPalette::Base,brushDark);
	palette.setBrush(QPalette::Inactive,QPalette::Base,brushDark);
	palette.setBrush(QPalette::Disabled,QPalette::Base,brushDark);
	palette.setBrush(QPalette::Active,QPalette::Window,brushDark);
	palette.setBrush(QPalette::Inactive,QPalette::Window,brushDark);
	palette.setBrush(QPalette::Disabled,QPalette::Window,brushDark);
	palette.setBrush(QPalette::Active,QPalette::Button,brushBrown);
	palette.setBrush(QPalette::Inactive,QPalette::Button,brushBrown);
	palette.setBrush(QPalette::Disabled,QPalette::Button,brushBrown);
	palette.setBrush(QPalette::Active,QPalette::ButtonText,brushBlue);
	palette.setBrush(QPalette::Inactive,QPalette::ButtonText,brushBlue);
	palette.setBrush(QPalette::Disabled,QPalette::ButtonText,brushBlue);
	palette.setBrush(QPalette::Active,QPalette::Shadow,brushWhite);
	palette.setBrush(QPalette::Inactive,QPalette::Shadow,brushWhite);
	palette.setBrush(QPalette::Active,QPalette::Shadow,brushWhite);
	palette.setBrush(QPalette::Inactive,QPalette::Text,brushGreen);
	palette.setBrush(QPalette::Active,QPalette::Text,brushGreen);
	palette.setBrush(QPalette::Active,QPalette::Text,brushGreen);
	palette.setBrush(QPalette::Active,QPalette::WindowText,brushWhite);
	palette.setBrush(QPalette::Inactive,QPalette::WindowText,brushWhite);
	palette.setBrush(QPalette::Disabled,QPalette::WindowText,brushWhite);
	setPalette(palette);

	QPalette palette2;
	palette2.setBrush(QPalette::Inactive,QPalette::Text,brushGreen);
	palette2.setBrush(QPalette::Active,QPalette::Text,brushGreen);
	palette2.setBrush(QPalette::Active,QPalette::Text,brushGreen);


	label = new QLabel(this);
//	label->setGeometry(QRect(200,10,400,40));
	label->setGeometry(QRect(200,30,400,40));
	label->setText(QObject::tr("帮助文档"));
	label->setFont(font);
	label->setAlignment(Qt::AlignHCenter);

	button = new QPushButton(this);
	button->setGeometry(20,80,150,50);
//	button->setGeometry(20,60,150,50);
	button->setText(QObject::tr("产品介绍"));
	font.setPointSize(60);
	button->setFont(font);

	button_2 = new QPushButton(this);
	button_2->setGeometry(20,140,150,50);
//	button_2->setGeometry(20,120,150,50);
	button_2->setText(QObject::tr("测量步骤"));
	button_2->setFont(font);

	button_3 = new QPushButton(this);
	button_3->setGeometry(20,200,150,50);
//	button_3->setGeometry(20,180,150,50);
	button_3->setText(QObject::tr("返回"));
	button_3->setFont(font);

	button_4 = new QPushButton(this);
	button_4->setGeometry(20,260,150,50);
//	button_4->setGeometry(20,240,150,50);
	button_4->setText(QObject::tr("视频演示"));
	button_4->setFont(font);

	textBrowser_2 = new QTextBrowser(this);
//	textBrowser_2->setGeometry(QRect(180,60,600,410));
	textBrowser_2->setGeometry(QRect(180,80,600,390));
	add_help_use();
	textBrowser_2->setFont(font);
	textBrowser_2->setPalette(palette2);

	
	textBrowser = new QTextBrowser(this);
//	textBrowser->setGeometry(QRect(180,60,600,410));
	textBrowser->setGeometry(QRect(180,80,600,390));
	add_help_products();
	textBrowser->setFont(font);
	textBrowser->setPalette(palette2);

	m = new Win_video(this);
//	m->setGeometry(180,60,600,410);
	m->setGeometry(180,80,600,390);
	m->close();

	QObject::connect(button_3,SIGNAL(clicked()),this,SLOT(close()));


	QObject::connect(button,SIGNAL(clicked()),this,SLOT(button_clicked()));

	QObject::connect(button_2,SIGNAL(clicked()),this,SLOT(button_2_clicked()));

	QObject::connect(button_4,SIGNAL(clicked()),m,SLOT(show()));
	QObject::connect(button_4,SIGNAL(clicked()),textBrowser,SLOT(close()));
	QObject::connect(button_4,SIGNAL(clicked()),textBrowser_2,SLOT(close()));
}

void Win_4::add_help_products()
{
	std::ifstream fin("/usr/local/help/help_products.cfg");
	std::string text;
	while(!fin.eof())
	{
		getline(fin,text);
		if(fin.eof())
			break;
		textBrowser->append(QObject::tr(text.c_str()));
	}
}

void Win_4::add_help_use()
{
	std::ifstream fin("/usr/local/help/help_use.cfg");
	std::string text;
	while(!fin.eof())
	{
		getline(fin,text);
		if(fin.eof())
			break;
		textBrowser_2->append(QObject::tr(text.c_str()));
	}
}

void Win_4::button_clicked()
{
	textBrowser_2->close();
	m->slotStop();
	m->close();
//	repaint(180,60,600,410);
	repaint(180,80,600,390);
	textBrowser->show();
//	repaint(180,60,600,410);
	repaint(180,80,600,390);
}

void Win_4::button_2_clicked()
{
	textBrowser->close();
	m->slotStop();
	m->close();
//	repaint(180,60,600,410);
	repaint(180,80,600,390);
	textBrowser_2->show();
	repaint(180,80,600,390);
//	repaint(180,60,600,410);
}
