#include <QTextCodec>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include "win_passwd.h"

Win_passwd::Win_passwd(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::WindowStaysOnTopHint)
{
	setGeometry(200,60,360,250);
	setFixedSize(360,250);
	QFont font;
	font.setPointSize(60);
	font.setFamily("simsun");

	
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	
	label1 = new QLabel(this);
	label1->setGeometry(60,20,240,40);
	label1->setText(QObject::tr("请输入用户名与密码"));
	label1->setFont(font);
	label1->setAlignment(Qt::AlignHCenter);

	label2 = new QLabel(this);
	label2->setGeometry(40,80,100,40);
	label2->setText(QObject::tr("用户名："));
	label2->setFont(font);
	label2->setAlignment(Qt::AlignRight);

	lineedit1 = new QLineEdit(this);
	lineedit1->setGeometry(145,75,150,30);
	lineedit1->setFont(font);

	label3 = new QLabel(this);
	label3->setGeometry(40,130,100,40);
	label3->setText(QObject::tr("密码："));
	label3->setFont(font);
	label3->setAlignment(Qt::AlignRight);

	lineedit2 = new QLineEdit(this);
	lineedit2->setGeometry(145,125,150,30);
	lineedit2->setFont(font);

	button1 = new QPushButton(this);
        button1->setText(QObject::tr("确定"));
        button1->setGeometry(QRect(60, 185, 110, 35));
	button1->setFont(font);

	button2 = new QPushButton(this);
        button2->setText(QObject::tr("取消"));
        button2->setGeometry(QRect(190, 185, 110, 35));
	button2->setFont(font);	
	
	
	QObject::connect(button2,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(button1,SIGNAL(clicked()),this,SLOT(processToSure()));
}

void Win_passwd::processToSure()
{
	std::ifstream fin;
	fin.open("/usr/local/passwd/passwd");
	if(!fin.is_open())
	{
		std::cout << "open error()" << std::endl;
		return;
	}
	bool flag = false;
	std::string line;
	std::string arg[2];
	std::string username;
	std::string passwd;
	QTextCodec *codec = QTextCodec::codecForName("utf-8");
	std::string usernameInput = QString(codec->fromUnicode(lineedit1->text())).toStdString();
	std::string passwdInput = (lineedit2->text()).toStdString();
	while(!fin.eof())
	{
		fin >> line;
		if(fin.fail()) break;
		std::string::iterator item;
		for(item = line.begin();*item !='=';item++)
		{
			arg[0].push_back(*item);
		}
		for(std::string::iterator item1 = item+1;item1 != line.end();item1++)
		{
			arg[1].push_back(*item1);
		}

		if(arg[0] == "username")
			username = arg[1];
		else
			passwd = arg[1];
		if((username == "")||(passwd == ""))
		{
			arg[0].clear();
			arg[1].clear();
			continue;
		}
		std::cout << username << passwd << std::endl;
		std::cout << usernameInput << passwdInput << std::endl;
		if(username==usernameInput && passwd==passwdInput)
		{
			flag = true;
		}
		arg[0].clear();
		arg[1].clear();
		username.clear();
		passwd.clear();
	}
	if(flag)
	{
		close();
		emit inputRight();
	}
	else
	{
		//QMessageBox::warning(this,tr("出错!"),tr("用户名或密码错误\n请重新输入"));
		QMessageBox messageBox(QMessageBox::Warning,tr("出错!"),tr("用户名或密码错误\n请重新输入"),QMessageBox::Ok,this,0);
		messageBox.setGeometry(200,200,100,100);
		QFont font;
		font.setPointSize(40);
		messageBox.setFont(font);
		messageBox.exec();
	}
	fin.close();
}
