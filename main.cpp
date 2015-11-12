#include <QApplication>
#include <QFont>
#include <QSplashScreen>
#include "data.h"
#include "win_1.h"
#include "win_2.h"
#include "win_3.h"
#include "win_4.h"
#include "mainwin.h"
#include "win_passwd.h"
#include "win_system.h"
#include "win_start.h"
#include "win_dateinput.h"	//20120306 by houli 
#include "sqlite/databaseStand.h"
#include "inputMethod/PinyinFrame.h"
#include <time.h>
#include <stdlib.h>
	

using namespace std;

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);
	app.setFont(QFont("simsun",15));
	//利用Qt多线程新建启动界面进程
	Win_Start *ws = new Win_Start;
	ThreadForStart s(ws->pb);
	s.start();
	QApplication::processEvents();

	//设置系统字体相关
	QFont font;
	font.setPointSize(60);
	app.setFont(font);

	//新建各个界面并通过信号与槽进行相互调用
	Data *dt = new Data;

	Win_dateinput *w0 = new Win_dateinput();	//20120306 by houli 

	Win_1 *w1 = new Win_1();//参数输入界面
	Win_2 *w2 = new Win_2();//开始测量界面
	Win_3 *w3 = new Win_3();//诊断结果界面
	Win_4 *w4 = new Win_4();//帮助文档界面
	Win_system *system = new Win_system;//系统选项界面
	Win_passwd *passwd = new Win_passwd();//数据库管理的登录界面
	DatabaseStand *dts = new DatabaseStand();//数据库界面
	Mainwin *mw = new Mainwin();//主界面

	//设置输入法界面，并将注册系统输入法
	QPinyinFrame *m_pinyin_frame=new QPinyinFrame(w1);
	m_pinyin_frame->sizeHint();
	int pinyin_frame_y=QApplication::desktop()->height()-m_pinyin_frame->sizeHint().height();
	m_pinyin_frame->move(18,pinyin_frame_y);
	qwsServer->setCurrentInputMethod(m_pinyin_frame);
	
	QPinyinFrame *date_pinyin_frame=new QPinyinFrame(w0);	//20120307 by houli,日期输入键盘
	date_pinyin_frame->sizeHint();
	int date_pinyin_y=QApplication::desktop()->height()-date_pinyin_frame->sizeHint().height();
	date_pinyin_frame->move(18,date_pinyin_y);
	qwsServer->setCurrentInputMethod(date_pinyin_frame);

	//QWSServer::setCursorVisible(false);

	QObject::connect(w2,SIGNAL(signalSaveData()),dt,SLOT(slotSaveData()));

	QObject::connect(mw,SIGNAL(button_win_2_click()),w1,SLOT(showMaximized()));//mainwin中开始测量按钮按下，显示参数输入界面
	QObject::connect(mw,SIGNAL(button_win_2_click()),(QFrame*)m_pinyin_frame,SLOT(show()));//同时显示输入法界面

	QObject::connect(w1,SIGNAL(button_conform_click(string,string,sex_bool,int,double,int)),dt,SLOT(keyToSure(string,string,sex_bool,int,double,int)));	//传递个人信息
	QObject::connect(dt,SIGNAL(signalSaveParameterSuccess()),w1,SLOT(slotSaveParameterSuccess()));	//个人信息保存成功
//	QObject::connect(mw,SIGNAL(button_win_2_click()),w2,SLOT(show()));
	QObject::connect(w1,SIGNAL(signalBeginMeasure()),w2,SLOT(show()));	//参数保存成功后，Win_2出现
	QObject::connect(w2,SIGNAL(signalQuit()),w1,SLOT(slotClearData()));//开始测量退出按钮按下，参数输入界面已输入的参数清空。
	QObject::connect(w2,SIGNAL(signalQuit()),w1,SLOT(close()));//开始测量退出按钮按下，参数输入界面显示消失

	QObject::connect(w2,SIGNAL(signalBeginMeasure(bool,double)),dt,SLOT(beginMeasure(bool,double)));	//dt开始测量
	QObject::connect(w2,SIGNAL(signalEndMeasure()),dt,SLOT(slotEndMeasure()));	//Win_2按"退出"后结束测量
	QObject::connect(w2,SIGNAL(signalClearData()),dt,SLOT(slotClearData()));	//Win_2按"退出"后清除个人信息数据

	QObject::connect(w2,SIGNAL(signalPassFileName(const QString&)),dt,SLOT(slotPassFileName(const QString&)));
	QObject::connect(mw,SIGNAL(button_win_3_click()),w3,SLOT(showMaximized()));
	QObject::connect(mw,SIGNAL(button_win_3_click()),w3,SLOT(showPersonalImfo()));
	QObject::connect(mw,SIGNAL(button_win_4_click()),w4,SLOT(showMaximized()));
	QObject::connect(w3,SIGNAL(getPersonalImfo()),dt,SLOT(getPersonalImfoACK()));
	QObject::connect(mw,SIGNAL(button_system_click()),system,SLOT(showMaximized()));
	QObject::connect(w3,SIGNAL(signalImportResult()),dt,SLOT(slotImportResult()));
	QObject::connect(dt,SIGNAL(sendPersonalImfo(const Data&)),w3,SLOT(sendPersonalImfoACK(const Data&)));
	QObject::connect(dt,SIGNAL(sendPersonalImfo(const Data&)),w3,SLOT(getInfoprinted(const Data&)));	//20120308 by houli

	QObject::connect(mw,SIGNAL(button_database_click()),passwd,SLOT(show()));
	QObject::connect(passwd,SIGNAL(inputRight()),dts,SLOT(display()));

	QObject::connect(dt,SIGNAL(signalPassWaveform(const vector<double>&)),w2,SLOT(slotPassWaveform(const vector<double>&)),Qt::DirectConnection);	//dt将时域波形传递给Win_2
	QObject::connect(w2,SIGNAL(signalShowSpectrum()),dt,SLOT(slotShowSpectrum()));	//Win_2要求显示频谱,传递给dt
	QObject::connect(dt,SIGNAL(signalPassSpectrum(const vector<double>&)),w2,SLOT(slotPassSpectrum(const vector<double>&)));//dt算好后回传给Win_2进行显示

	QObject::connect(dt,SIGNAL(signalSaveDataSuccess()),w2,SLOT(slotSaveDataSuccess()));
	//QObject::connect(dt,SIGNAL(signalSaveParameterSuccess()),w1,SLOT(slotSaveParameterSuccess()));	//个人信息保存成功
	QObject::connect(dt,SIGNAL(signalParameterSaveError()),w2,SLOT(slotParameterSaveError()));
	QObject::connect(dt,SIGNAL(signalRepeatImportError()),w3,SLOT(slotRepeatImportError()));
	QObject::connect(dt,SIGNAL(signalImportSuccess()),w3,SLOT(slotImportSuccess()));
	QObject::connect(dt,SIGNAL(signalAutoStop()),w2,SLOT(slotAutoStop()));
	//QObject::connect(&s,SIGNAL(signalStartOk()),mw,SLOT(showMaximized()));
	QObject::connect(w0,SIGNAL(button_confirm_clicked(string)),w3,SLOT(getDate(string)));	//添加日期
	QObject::connect(w0,SIGNAL(signalDateok()),mw,SLOT(showMaximized()));
	QObject::connect(&s,SIGNAL(signalStartOk()),w0,SLOT(showMaximized()));

	return app.exec();
}
