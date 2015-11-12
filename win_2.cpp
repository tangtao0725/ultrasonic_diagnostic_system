#include <QtGui>
#include <QTextCodec>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include "win_2.h"
#include "plotter.h"
#include "parameter.h"

Win_2::Win_2(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint),isDynamicDisplay(false)
{
	QFont font;
	font.setPointSize(55);
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
	palette2.setBrush(QPalette::Disabled,QPalette::Text,brushDark);
	palette2.setBrush(QPalette::Active,QPalette::WindowText,brushWhite);
	palette2.setBrush(QPalette::Inactive,QPalette::WindowText,brushWhite);
	palette2.setBrush(QPalette::Disabled,QPalette::WindowText,brushWhite);

	groupBox = new QGroupBox(QObject::tr("超声发射频率频率"),this);
	//groupBox->setGeometry(10,40,231,230);
	groupBox->setGeometry(22,40,231,230);
	groupBox->setFont(font);

	radioButton = new QRadioButton(groupBox);
	radioButton->setGeometry(10,30,86,27);
	radioButton->setText(QObject::tr("1.0MHz"));
	radioButton->setFont(font);
	radioButton->setChecked(true);
	radioButton->setPalette(palette2);

	radioButton_1 = new QRadioButton(groupBox);
	radioButton_1->setGeometry(10,65,86,27);
	radioButton_1->setText(QObject::tr("2.25MHz"));
	radioButton_1->setFont(font);
	radioButton_1->setPalette(palette2);

	radioButton_2= new QRadioButton(groupBox);
	radioButton_2->setGeometry(10,100,86,27);
	radioButton_2->setText(QObject::tr("3.5MHz"));
	radioButton_2->setFont(font);
	radioButton_2->setPalette(palette2);

	radioButton_3 = new QRadioButton(groupBox);
	radioButton_3->setGeometry(10,135,86,27);
	radioButton_3->setText(QObject::tr("5.0MHz"));
	radioButton_3->setFont(font);
	radioButton_3->setPalette(palette2);

	radioButton_4 = new QRadioButton(groupBox);
	radioButton_4->setGeometry(10,170,86,27);
	radioButton_4->setText(QObject::tr("6.0MHz"));
	radioButton_4->setFont(font);
	radioButton_4->setPalette(palette2);
	
	button = new QPushButton(this);
	button->setGeometry(22,280,111,45);
	button->setText(QObject::tr("开始测量"));
	button->setShortcut(QKeySequence(Qt::ALT + Qt::Key_B));
	//button->setAutoDefault(1);	//20130107
	button->setFont(font);

	button_1 = new QPushButton(this);
	button_1->setGeometry(142,340,111,45);
	button_1->setText(QObject::tr("退出"));
	button_1->setShortcut(QKeySequence(Qt::ALT + Qt::Key_R));
	button_1->setFont(font);

	button_2 = new QPushButton(this);
	button_2->setGeometry(22,340,111,45);
	button_2->setText(QObject::tr("历史数据"));
	button_2->setShortcut(QKeySequence(Qt::ALT + Qt::Key_H));
	button_2->setFont(font);

	button_3 = new QPushButton(this);
	button_3->setGeometry(142,280,111,45);
	button_3->setText(QObject::tr("信号频谱"));
	button_3->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F));
	button_3->setFont(font);

	button_4 = new QPushButton(this);
	button_4->setGeometry(22,400,111,45);
	button_4->setText(QObject::tr("保存数据"));
	button_4->setShortcut(QKeySequence(Qt::ALT + Qt::Key_S));
	button_4->setFont(font);

	//setPalette(palette2);


	plotter_1= new Plotter(this);//,true);

	plotter_2 = new Plotter(this);
	plotter_2->isSignal = false;


	filelist = new FilelistView();

	ifstream fin("/usr/local/coef.txt");
	double tmp = 0;
	while(!fin.eof())
	{
		fin >> tmp;
		if(fin.fail())
			break;
		Coef.push_back(tmp);
	}

	//退出win_2
	QObject::connect(button_1,SIGNAL(clicked()),this,SIGNAL(signalQuit()),Qt::DirectConnection);//button_1按下表示退出，退出之前要关闭参数如入的界面
	QObject::connect(button_1,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(button_1,SIGNAL(clicked()),this,SIGNAL(signalClearData()));	//通过main.cpp传递给dt(data.cpp)
	QObject::connect(button_1,SIGNAL(clicked()),this,SLOT(slotEndMeasure()));	//通过main.cpp传递给dt(data.cpp)
	//开始测量
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(slotBeginMeasure()));	//"开始测量"按下，开始测量
	QObject::connect(button_3,SIGNAL(clicked()),this,SLOT(slotShowSpectrum()));
	QObject::connect(plotter_1,SIGNAL(signalZoomOut()),this,SLOT(slotSignalZoomOut()));
	QObject::connect(plotter_2,SIGNAL(signalZoomOut()),this,SLOT(slotSpectrumZoomOut()));
	QObject::connect(plotter_1,SIGNAL(signalZoomIn()),this,SLOT(slotSignalZoomIn()));
	QObject::connect(plotter_2,SIGNAL(signalZoomIn()),this,SLOT(slotSpectrumZoomIn()));
	QObject::connect(plotter_1,SIGNAL(signalToolButtonClick()),this,SLOT(slotBeginMeasure()));
	QObject::connect(button_4,SIGNAL(clicked()),this,SLOT(slotSaveData()));
	QObject::connect(button_2,SIGNAL(clicked()),this,SLOT(slotLoadData()));
	QObject::connect(filelist,SIGNAL(signalFilename(const QString&)),this,SIGNAL(signalPassFileName(const QString&)));
}



void Win_2::slotBeginMeasure()
{
	double tmp;
	if(radioButton->isChecked())
		tmp = 1.0;
	else if(radioButton_1->isChecked())
		tmp = 2.25;
	else if(radioButton_2->isChecked())
		tmp = 3.5;
	else if(radioButton_3->isChecked())
		tmp = 5.0;
	else if(radioButton_4->isChecked())
		tmp = 6.0;
	isDynamicDisplay = !isDynamicDisplay;	//由"0"到"1"，动态显示
	plotter_1->toolButtonFlag = !plotter_1->toolButtonFlag;
	if(isDynamicDisplay)
	{
		button->setText(QObject::tr("确定"));
        	button->setShortcut(QKeySequence(Qt::ALT + Qt::Key_B));
	}
	else
	{
		button->setText(QObject::tr("开始测量"));
		button->setShortcut(QKeySequence(Qt::ALT + Qt::Key_B));
	}
	emit signalBeginMeasure(isDynamicDisplay,tmp);	//见main.cpp,传递给dt(data.cpp)
}

void Win_2::slotAutoStop()
{
	isDynamicDisplay = false;
	button->setText(QObject::tr("开始测量"));
    button->setShortcut(QKeySequence(Qt::ALT + Qt::Key_B));
	plotter_1->toolButtonFlag = !plotter_1->toolButtonFlag;
	plotter_1->toolButtonSetting();
}

void Win_2::slotEndMeasure()
{
	plotter_1->toolButtonFlag = true;
	if(isDynamicDisplay)
	{
		isDynamicDisplay = false;
		button->setText(QObject::tr("开始测量"));
    	button->setShortcut(QKeySequence(Qt::ALT + Qt::Key_B));
		emit signalEndMeasure();
	}
}

void Win_2::slotLoadData()
{
	if(isDynamicDisplay)
	{
		QMessageBox::warning(this,tr("错误"),tr("请停止捕获信号"),QMessageBox::Ok);
		return;
	}
	plotter_1->display();
	plotter_2->display();
/*	QString fileName = QFileDialog::getOpenFileName(this,tr("选择要打开文件的路径"),".",tr("datafile txt(*.txt)"));
	std::cout << "文件名" << std::endl;
	emit signalPassFileName(fileName);
*/ 
	filelist->model->setDirPath(QString("/usr/data"));
	filelist->move(200,100);
	filelist->show();
}

void Win_2::slotPassWaveform(const vector<double>& waveform)	//接收dt传来的数据，显示时域波形
{
//	cout << "show the data from the thread" << endl;
	vector<double> tmp(waveform.begin(),waveform.end());
	plotter_1->data = filter_FIR(tmp,Coef);
	plotter_1->showCurves(true);
}

void Win_2::slotPassSpectrum(const vector<double>& spectrum)
{
	plotter_2->data = spectrum;
	plotter_2->showCurves(false);
}

void Win_2::show()
{
	showMaximized();
	//plotter_1->move(260,40);
	plotter_1->move(272,40);
	plotter_1->minX = 0;
	plotter_1->maxX = 2048;
	plotter_1->numXTicks = 9;
	plotter_1->minY = -1;
	plotter_1->maxY = +1;
	plotter_1->numYTicks = 5;
	plotter_1->display();
	plotter_2->move(272,260);
	plotter_2->minX = 0;
	plotter_2->maxX = 256;
	plotter_2->numXTicks = 5;
	plotter_2->minY = 0;
	plotter_2->maxY = 4;
	plotter_2->numYTicks = 5;
	plotter_2->display();
}

void Win_2::slotSaveDataSuccess()
{
	QMessageBox::warning(this,tr("提示"),tr("保存数据成功！"),QMessageBox::Ok);
}

void Win_2::slotSaveData()
{
	if(isDynamicDisplay)
	{
		QMessageBox::warning(this,tr("错误"),tr("请停止捕获信号"),QMessageBox::Ok);
		return;
	}
	emit signalSaveData();
}

void Win_2::slotShowSpectrum()
{
	if(isDynamicDisplay)
	{
		QMessageBox::warning(this,tr("错误"),tr("请停止捕获信号"),QMessageBox::Ok);
		return;
	}
	emit signalShowSpectrum();
}

void Win_2::slotParameterSaveError()
{
	QMessageBox::warning(this,tr("提示"),tr("保存数据出错，请检查您的编号输入！"),QMessageBox::Ok);
}

/*void Win_2::slotReadDataSuccess()
{
	QMessageBox::warning(this,tr("提示"),tr("读取数据成功！"),QMessageBox::Ok);
}
*/

/*void Win_2::button_click()
{
	double tmp;
	if(radioButton->isChecked())
		tmp = 1.0;
	else if(radioButton_1->isChecked())
		tmp = 2.25;
	else if(radioButton_2->isChecked())
		tmp = 3.5;
	else if(radioButton_3->isChecked())
		tmp = 5.0;
	else if(radioButton_4->isChecked())
		tmp = 6.0;
	else
	{
		std::cout << "error! no radioButton is checked!" << std::endl;
		exit(0);
	}
//	std::cout << tmp << std::endl;
	emit beginMeasure(tmp);
}
*/

void Win_2::slotSignalZoomOut()
{
	plotter_1->move(0,0);
	plotter_1->hide();
	plotter_2->hide();
	plotter_1->toolButtonSetting();
	plotter_1->waveformButton->show();
	plotter_1->showMaximized();
}

void Win_2::slotSpectrumZoomOut()
{
	plotter_2->move(0,0);
	plotter_1->hide();
	plotter_2->hide();
	plotter_2->showMaximized();
}

void Win_2::slotSignalZoomIn()
{
	//plotter_1->move(260,40);
	plotter_1->move(272,40);
	plotter_1->hide();
	plotter_2->hide();
	plotter_2->show();
	plotter_1->waveformButton->hide();
	plotter_1->showMaximized();
}

void Win_2::slotSpectrumZoomIn()
{
	plotter_2->move(272,260);
	plotter_1->hide();
	plotter_2->hide();
	plotter_1->show();
	plotter_2->showMaximized();
}	 
