#include <QtGui>
#include <QTextCodec>
#include <QFont>

#include "win_dateinput.h"

using namespace std;

Win_dateinput::Win_dateinput(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint)
{
	QFont font;
	font.setFamily(("simsun"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

	QPalette palette;
	QBrush brushDark(QColor(0,0,0,255));
	QBrush brushBrown(QColor(114,117,118,255));
	QBrush brushWhite(QColor(255,255,255,255));
	QBrush brushGreen(QColor(170,255,0,255));
	QBrush brushBlue(QColor(0,0,255,255));
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
	palette2.setBrush(QPalette::Inactive,QPalette::Text,brushDark);
	palette2.setBrush(QPalette::Active,QPalette::Text,brushDark);
	palette2.setBrush(QPalette::Active,QPalette::Text,brushDark);

	font.setPointSize(80);
	label_title = new QLabel(this);
	label_title->setGeometry(QRect(212,40,400,40));
	label_title->setText(QObject::tr("日期输入"));
	label_title->setFont(font);
	label_title->setAlignment(Qt::AlignHCenter);
	
	font.setPointSize(60);
	box_top = new QGroupBox(this);
	box_top->setGeometry(QRect(32,70,760,180));
	box_top->setTitle(QObject::tr("请输入今日日期:"));
	box_top->setFont(font);

	edit_date = new QDateEdit(box_top);		//20120305修改，日期输入窗口
	edit_date->setGeometry(QRect(25,45,240,60));
	//edit_date->setCalendarPopup(true);
	edit_date->setDisplayFormat("yyyy.MM.dd");
	edit_date->setMinimumDate(QDate(2012, 03, 06));
	edit_date->setPalette(palette2);
	edit_date->setFont(font);
	
	button_confirm = new QPushButton(this);
	button_confirm->setGeometry(QRect(620,190,91,41));
	button_confirm->setText(QObject::tr("确定"));
	button_confirm->setFont(font);
	button_confirm->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C));
	QObject::connect(button_confirm,SIGNAL(clicked()),this,SLOT(setDate()));//按下"确定", 调用setDate函数
	//QObject::connect(button_confirm,SIGNAL(clicked()),this,SLOT(close()));
	
}


void Win_dateinput::setDate()
{
	QString date_qs=(edit_date->date()).toString("yyyy.MM.dd");		//20120306 by houli
	QByteArray date_ba=date_qs.toUtf8();	
	string date_tmp = string(date_ba);

	
	if(date_tmp=="")
	{
		QMessageBox::warning(this,tr("错误"),tr("请输入正确的日期"),QMessageBox::Ok);
		return;
	}
	emit button_confirm_clicked(date_tmp);
	emit signalDateok();
}

