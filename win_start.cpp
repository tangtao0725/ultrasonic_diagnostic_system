#include <QtCore>
#include <QtGui>

#include "win_start.h"

Win_Start::Win_Start(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint)
{
	QPalette palette;
	QBrush brushBlue(QColor(0,0,255,255));
	palette.setBrush(QPalette::Active,QPalette::Window,brushBlue);
	palette.setBrush(QPalette::Inactive,QPalette::Window,brushBlue);
	palette.setBrush(QPalette::Disabled,QPalette::Window,brushBlue);
	setPalette(palette);
	setFixedSize(800,480);
	QFont font;
	font.setFamily("simsun");
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

	pb = new QProgressBar(this);
	font.setPointSize(45);
	pb->setFont(font);
	pb->setStyleSheet("QProgressBar::chunk:horizontal{background:qlineargradient(x1:0,y1:0.5,x2:1,y2:0.5,stop: 0 green,stop: 1 white);}");
	pb->setGeometry(200,300,400,16);
	pb->setRange(0,100);


	label1 = new QLabel(this);
	label1->setGeometry(200,340,400,50);
	label1->setText(QObject::tr("正在启动..."));
	label1->setAlignment(Qt::AlignHCenter);

	label2 = new QLabel(this);
	label2->setMovie(new QMovie("a.gif"));
	label2->show();
	label2->movie()->start();

	this->show();
}

void Win_Start::valueSet(int num)
{
	pb->setValue(num);
}

