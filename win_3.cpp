#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>
#include <QTextCodec>
#include <QString>
#include "win_3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "sqlite3.h"
#include <stdio.h>	//以下头文件后续添加
#include <string.h>
#include <string>
#include "unicode_to_gbk.h"
#include <math.h>
#include <unistd.h>



static int sequence = 0;
Win_3::Win_3(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint)
{
	QFont font;
	font.setPointSize(80);
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

	QPalette palette3;
	palette3.setBrush(QPalette::Inactive,QPalette::WindowText,brushGreen);
	palette3.setBrush(QPalette::Active,QPalette::WindowText,brushGreen);
	palette3.setBrush(QPalette::Active,QPalette::WindowText,brushGreen);

	label = new QLabel(this);
//	label->setGeometry(260,10,200,31);
	label->setGeometry(260,30,200,31);
	label->setText(QObject::tr("诊断结果"));
	font.setBold(true);
	label->setAlignment(Qt::AlignHCenter);
	label->setFont(font);

	widget = new QWidget(this);
	widget->setGeometry(QRect(30,35,240,151));
	

	label_2 = new QLabel(widget);
	label_2->setGeometry(QRect(0,10,130,31));
	label_2->setText(QObject::tr("您的个人信息"));
	label_2->setPalette(palette3);
	font.setPointSize(55);
	label_2->setFont(font);

	textBrowser = new QTextBrowser(widget);
	textBrowser->setGeometry(QRect(0,40,240,111));
	textBrowser->setPalette(palette2);
	textBrowser->setFont(font);

	widget_2 = new QWidget(this);
	widget_2->setGeometry(QRect(30,180,240,280));

	label_3 = new QLabel(widget_2);
	label_3->setGeometry(QRect(0,10,160,21));
	label_3->setText(QObject::tr("超声参数诊断结果"));
	label_3->setPalette(palette3);
	label_3->setFont(font);
	label_3->setFont(font);

	textBrowser_2 = new QTextBrowser(widget_2);
	textBrowser_2->setGeometry(QRect(0,40,240,81));
	textBrowser_2->setPalette(palette2);
	textBrowser_2->setFont(font);

	button_5 = new QPushButton(widget_2);
	button_5->setGeometry(QRect(0,140,115,40));
	button_5->setText(QObject::tr("导入结果"));
	button_5->setFont(font);

	button_1 = new QPushButton(widget_2);			//打印键，2012/02/26
	button_1->setGeometry(QRect(125,140,115,40));
	button_1->setText(QObject::tr("打印"));
	button_1->setFont(font);

	
	button = new QPushButton(widget_2);
	button->setGeometry(QRect(0,190,115,40));
	button->setText(QObject::tr("BC参数查看"));
	button->setFont(font);

	button_2 = new QPushButton(widget_2);
	button_2->setGeometry(QRect(0,240,115,40));
	button_2->setText(QObject::tr("AIB参数查看"));
	button_2->setFont(font);

	button_3 = new QPushButton(widget_2);
	button_3->setGeometry(QRect(125,190,115,40));
	button_3->setText(QObject::tr("SCS参数查看"));
	button_3->setFont(font);

	button_4 = new QPushButton(widget_2);
	button_4->setGeometry(QRect(125,240,115,40));
	button_4->setText(QObject::tr("返回"));
	button_4->setShortcut(QKeySequence(Qt::ALT + Qt::Key_R));
	button_4->setFont(font);

	dw = new Draw(this);
	dw->minX = 20;
	dw->maxX = 100;
	dw->minY = -50;
	dw->maxY = 0;
	dw->numXTicks = 9;
	dw->numYTicks = 5;	//20120717,from '11' to '5'
	dw->display();
	dw->setGeometry(QRect(290,70,500,391));


	QObject::connect(button_4,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(button_4,SIGNAL(clicked()),dw,SLOT(slotClearData()));
	QObject::connect(button_1,SIGNAL(clicked()),this,SLOT(slotPrint()));	//打印键，2012/02/26

	QObject::connect(button,SIGNAL(clicked()),this,SLOT(slotShowBCPoints()));
	QObject::connect(button_2,SIGNAL(clicked()),this,SLOT(slotShowAIBPoints()));
	QObject::connect(button_3,SIGNAL(clicked()),this,SLOT(slotShowSCSPoints()));
	QObject::connect(button_5,SIGNAL(clicked()),this,SLOT(slotImportResult()));

	
}

void Win_3::showPersonalImfo()
{
	emit getPersonalImfo();
}

void Win_3::slotImportResult()
{
	emit signalImportResult();
}
void Win_3::slotImportSuccess()
{
	QMessageBox::warning(this,tr("提示"),tr("参数导入成功！"),QMessageBox::Ok);
}
void Win_3::slotRepeatImportError()
{
	QMessageBox::warning(this,tr("提示"),tr("参数导入失败！重复导入或无有效参数"),QMessageBox::Ok);
}
void Win_3::sendPersonalImfoACK(const Data& dt)
{
	QString sex_tmp = "";
	if(dt.sex == male)
		sex_tmp = QObject::tr("男\n");
	else
		sex_tmp = QObject::tr("女\n");
	
	
	QString tmp = QObject::tr("姓名:");
	tmp.append(QObject::tr(dt.name.c_str()));
	tmp.append("\n");
	tmp.append(QObject::tr("编号:"));
	tmp.append(QObject::tr(dt.number.c_str()));
	tmp.append("\n");
	tmp.append(QObject::tr("性别:"));
	tmp.append(sex_tmp);
	tmp.append(QObject::tr("年龄:"));
	tmp.append(QString::number(dt.age,10));
	tmp.append("\n");
	tmp.append(QObject::tr("身高:"));
	tmp.append(QString::number(dt.height,10));
	tmp.append("(cm)");
	tmp.append("\n");
	tmp.append(QObject::tr("体重:"));
	tmp.append(QString::number(dt.weight,'g',2));
	tmp.append("(kg)");
	tmp.append("\n");
	tmp.append(QObject::tr("超声探头中心频率:"));
	tmp.append(QString::number(dt.udFeq,'g',2));
	tmp.append("MHz");
	cen_freq=dt.udFeq;	//2012.05.28, 探头中心频率，以1,2,3,5,6表示
	textBrowser->setText(tmp);
	QString tmp1 = QObject::tr("AIB参数：");
	tmp1.append(QString::number(dt.prmAIB,'g',4));
	tmp1.append("\n");
	tmp1.append(QObject::tr("BC参数: "));
	tmp1.append(QString::number(dt.prmBC,'g',4));
	tmp1.append("\n");
	tmp1.append(QObject::tr("SCS参数："));
	tmp1.append(QString::number(dt.prmSCS,'g',4));
	tmp1.append("KHz");
	textBrowser_2->setText(tmp1);

	dw->age = dt.age;
	dw->bc_value = dt.prmBC;
	dw->aib_value = dt.prmAIB;
	dw->scs_value = dt.prmSCS;

	
	
}

void Win_3::getInfoprinted(const Data &dt)	//20120310 by houli
{
	QString sexy="";
	if(dt.sex == male)
		sexy = "男";
	else
		sexy = "女";
	str_value0=QString(dt.name.c_str());
	str_value1=sexy;
	str_value2="左";
	//str_value<<QString(dt.name.c_str())<<sexy<<"左";
	values[0]=date.c_str();				
	values[1]=dt.number.c_str();			//number
	
	age_str=QString::number(dt.age,10);	//age
	age_ba=(age_str.toLatin1());
	values[2]=age_ba.data();
	age_value=dt.age;
	
	height_str=QString::number(dt.height,10);	//height
	height_ba=(height_str.toLatin1());
	values[3]=height_ba.data();

	weight_str=QString::number(dt.weight,'g',4);	//weight
	weight_ba=weight_str.toLatin1();
	values[4]=weight_ba.data();

	ratioper_str=QString::number(189,10);	//ratioper
	ratioper_ba=ratioper_str.toLatin1();
	values[5]=ratioper_ba.data();
	
	T_str=QString::number(0.2,'g',3);	//value T
	T_ba=T_str.toLatin1();
	values[6]=T_ba.data();
	
	ratioage_str=QString::number(98,10);	//ratioper
	ratioage_ba=ratioage_str.toLatin1();
	values[7]=ratioage_ba.data();
	
	Z_str=QString::number(0.7,'g',3);	//value Z
	Z_ba=(Z_str.toLatin1());
	values[8]=Z_ba.data();
	
	BC_str=QString::number(dt.prmBC,'g',4);	//value BC
	BC_ba=BC_str.toLatin1();
	values[9]=BC_ba.data();
	BCvalue=dt.prmBC;	//20120621
	
	AIB_str=QString::number(dt.prmAIB,'g',4);	//value AIB
	AIB_ba=AIB_str.toLatin1();
	values[10]=AIB_ba.data();
	AIBvalue=dt.prmAIB;	//20120621

	SCS_str=QString::number(dt.prmSCS,'g',4);	//value SCS
	SCS_ba=SCS_str.toLatin1();
	values[11]=SCS_ba.data();
	SCSvalue=dt.prmSCS;	//20120621
}

int callback(void *pData_tmp,int argc,char **argv,char **azColName)
{
	std::stringstream ss;
	for(int i = 0; i < argc; ++i)
	{
		double *pData = (double *)pData_tmp;
		int matchData = (int)(*pData);
		string tmp1 = azColName[i];
		string tmp2;
		if(matchData == 1)
			tmp2 = "BC";
		else if(matchData == 2)
			tmp2 = "AIB";
		else if(matchData == 3)
			tmp2 = "SMS";
		else
			tmp2 = "";
		if(tmp1 == tmp2)
		{
			static int j = 1;
			pData = (double*)(pData_tmp)+j;
			j += 1;
			if(j == 100)
				j = 1;
			ss << argv[i];
			ss >> *pData;
		}
	}
	return 0;
}

int callback1(void *pData_tmp,int argc,char **argv,char **azColName)
{
	std::stringstream ss;
	double *pData = (double*)pData_tmp;
	pData = (double*)pData_tmp+sequence;
	string tmp1 = azColName[0];
	ss << argv[0];
	ss >> *pData;
	std::cout << tmp1 << ":\t" << *pData << std::endl;
	++sequence;
	return 0;
}


//in the following three function, every one query the database and select the data, but how to decide which coloum to be selected
//here I set a flag at the beginning of the datatmp, if datatmp[0] is 1,then query the column BC, if datatmp[0] is 2,then query the column AIB
//and if datatemp[0] is 2,then column SCS
void Win_3::slotShowBCStandedCurve()
{
	char* errMsg;
	sqlite3 * pdb;
	dw->data.clear();
	int res = sqlite3_open("/usr/local/data/data.db",&pdb);
	if(res){
		std::cout << "Can't open database:" << sqlite3_errmsg(pdb);
		sqlite3_close(pdb);
	}
	double *datatmp = new double[105];
	datatmp[0]  = 1;
	for(int i = 1;i < 105; ++i)
		datatmp[i] = 0;
	string strSQL = "select * from maleDatabase;";
	res = sqlite3_exec(pdb,strSQL.c_str(),callback,datatmp,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}
	for(int k = 1; k < 100; ++k)
	{
		dw->data.push_back(*(datatmp + k));
	}
	sqlite3_close(pdb);
	delete [] datatmp;
	dw->showCurves();
}

void Win_3::slotShowAIBStandedCurve()
{
	char* errMsg;
	sqlite3 * pdb;
	dw->data.clear();
	int res = sqlite3_open("/usr/local/data/data.db",&pdb);
	if(res){
		std::cout << "Can't open database:" << sqlite3_errmsg(pdb);
		sqlite3_close(pdb);
	}
	double *datatmp = new double[105];
	datatmp[0]  = 2;
	for(int i = 1;i < 105; ++i)
		datatmp[i] = 0;
	string strSQL = "select * from maleDatabase;";
	res = sqlite3_exec(pdb,strSQL.c_str(),callback,datatmp,&errMsg);
	for(int k = 1; k < 100; ++k)
	{
		dw->data.push_back(*(datatmp + k));
	}

	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}
	sqlite3_close(pdb);
	delete [] datatmp;
	dw->showCurves();
}

void Win_3::slotShowSCSStandedCurve()
{
	char* errMsg;
	sqlite3 * pdb;
	dw->data.clear();
	int res = sqlite3_open("/usr/local/data/data.db",&pdb);
	if(res){
		std::cout << "Can't open database:" << sqlite3_errmsg(pdb);
		sqlite3_close(pdb);
	}
	double *datatmp = new double[105];
	datatmp[0]  = 3;
	for(int i = 1;i < 105; ++i)
		datatmp[i] = 0;
	string strSQL = "select * from maleDatabase;";
	res = sqlite3_exec(pdb,strSQL.c_str(),callback,datatmp,&errMsg);
	for(int k = 1; k < 100; ++k)
	{
		dw->data.push_back(*(datatmp + k));
	}

	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}
	sqlite3_close(pdb);
	delete [] datatmp;
	dw->showCurves();
}

void Win_3::slotShowBCPoints()
{
	dw->numXTicks = 9;	//2012.07.17,用于指定x.y的坐标个数
	dw->numYTicks = 6;

	char* errMsg;
	sqlite3 *pdb;
	dw->points.clear();
	dw->current_value.clear();
	int res = sqlite3_open("/usr/local/data/result.db",&pdb);
	if(res){
		std::cout << "can't open database:" << sqlite3_errmsg(pdb);
		sqlite3_close(pdb);
	}
	double *dataAge = new double[1000];
	double *dataBC = new double[1000];
	memset(dataAge,0,sizeof(double)*1000/sizeof(int));
	memset(dataBC,0,sizeof(double)*1000/sizeof(int));
	res = sqlite3_exec(pdb,"select age from maletable",callback1,dataAge,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}
	sequence = 0;
	res = sqlite3_exec(pdb,"select parameterBC from maletable",callback1,dataBC,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}	
	/*for(int i = 0;dataAge[i] != 0;++i)
	{
		std::cout << "dataAge[i]:\t" << dataAge[i] << std::endl;
		std::cout << "dataBC[i]:\t" << dataBC[i] << std::endl;
		dw->points.push_back(points(int(dataAge[i]),dataBC[i]));
	}*/
	dw->current_value.push_back(points(int(dw->age),dw->bc_value));	

	dw->type = bc;
	dw->showPoints();
	sqlite3_close(pdb);
	sequence = 0;	
	delete [] dataAge;
	delete [] dataBC;
}


void Win_3::slotShowAIBPoints()
{
	double cur_AIB_test[80]={-9.0759, -8.90295, -8.74823, -8.6112, -8.49135, -8.38815, -8.30108, -8.22962, -8.17325, -8.13145, -8.10368, -8.08944, -8.08821, -8.09944, -8.12264, -8.15727, -8.20281,  -8.25874, -8.32453, -8.39968, -8.48365, -8.57592, -8.67597, -8.78328, -8.89733, -9.01759, -9.14354, -9.27467, -9.41044, -9.55034, -9.69384, -9.84043, -9.98958, -10.1408, -10.2935, -10.4472, -10.6013, -10.7555, -10.909, -11.0615, -11.2123, -11.361, -11.5071, -11.65, -11.7891, -11.9241, -12.0543, -12.1792, -12.2984, -12.4112, -12.5172, -12.6158, -12.7066, -12.7889, -12.8624, -12.9264, -12.9804, -13.024, -13.0565, -13.0775, -13.0864, -13.0828, -13.0661, -13.0357, -12.9913, -12.9321, -12.8578, -12.7677, -12.6615, -12.5385, -12.3982, -12.2401, -12.0637, -11.8684, -11.6538, -11.4193, -11.1645, -10.8886, -10.5914, -10.2721};
	
	dw->numXTicks = 9;	//2012.07.17
	dw->numYTicks = 5;	
	char* errMsg;
	sqlite3 *pdb;
	dw->points.clear();
	dw->current_value.clear();
	int res = sqlite3_open("/usr/local/data/result.db",&pdb);
	if(res){
		std::cout << "can't open database:" << sqlite3_errmsg(pdb);
		sqlite3_close(pdb);
	}
	double *dataAge = new double[1000];
	double *dataAIB = new double[1000];
	memset(dataAge,0,sizeof(double)*1000/sizeof(int));
	memset(dataAIB,0,sizeof(double)*1000/sizeof(int));
	res = sqlite3_exec(pdb,"select age from maletable",callback1,dataAge,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}
	sequence = 0;
	res = sqlite3_exec(pdb,"select parameterAIB from maletable",callback1,dataAIB,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}	
	/*for(int i = 0;dataAge[i] != 0;++i)		//20120716
	{
		std::cout << "dataAge[i]:\t" << dataAge[i] << std::endl;
		std::cout << "dataAIB[i]:\t" << dataAIB[i] << std::endl;
		dw->points.push_back(points(int(dataAge[i]),dataAIB[i]));
	}*/
	
	for(int i=0; i<80; i++)		//2012.07.16,插入参考曲线
	{
		dw->points.push_back(points(int(i+20),cur_AIB_test[i]));
	}
	
	dw->current_value.push_back(points(int(dw->age),dw->aib_value));

	dw->type = aib;
	dw->showPoints();
	sqlite3_close(pdb);
	sequence = 0;	
	delete [] dataAge;
	delete [] dataAIB;
}


void Win_3::slotShowSCSPoints()
{


	double cur_SCS_test[80]={-457.662, -458.1, -458.38, -458.505, -458.482, -458.317, -458.013, -457.576, -457.013, -456.327, -455.525, -454.612, -453.592, -452.472, -451.256, -449.95, -448.56, -447.089, -445.545, -443.931, -442.254, -440.519, -438.73, -436.894, -435.015, -433.099, -431.151, -429.177, -427.182, -425.17, -423.148, -421.12, -419.092, -417.07, -415.058, -413.062, -411.087, -409.139, -407.222, -405.342, -403.505, -401.715, -399.978, -398.299, -396.684, -395.138, -393.666, -392.273, -390.965, -389.747, -388.624, -387.602, -386.686, -385.881, -385.193, -384.626, -384.187, -383.88, -383.71, -383.684, -383.806, -384.082, -384.516, -385.115, -385.883, -386.826, -387.949, -389.258, -390.757, -392.452, -394.348, -396.452, -398.767, -401.299, -404.054, -407.036, -410.252, -413.706, -417.403, -421.35};	//2012.07.16

	dw->numXTicks = 9;	//2012.07.17
	dw->numYTicks = 5;

	char* errMsg;
	sqlite3 *pdb;
	dw->points.clear();
	dw->current_value.clear();
	int res = sqlite3_open("/usr/local/data/result.db",&pdb);
	if(res){
		std::cout << "can't open database:" << sqlite3_errmsg(pdb);
		sqlite3_close(pdb);
	}
	double *dataAge = new double[1000];
	double *dataSCS = new double[1000];
	memset(dataAge,0,sizeof(double)*1000/sizeof(int));
	memset(dataSCS,0,sizeof(double)*1000/sizeof(int));
	res = sqlite3_exec(pdb,"select age from maletable",callback1,dataAge,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}
	sequence = 0;
	res = sqlite3_exec(pdb,"select parameterSCS from maletable",callback1,dataSCS,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "execute SQL error" << errMsg << std::endl;
		sqlite3_close(pdb);
	}	
	/*for(int i = 0;dataAge[i] != 0;++i)		//2012.07.16
	{
		std::cout << "dataAge[i]:\t" << dataAge[i] << std::endl;
		std::cout << "dataBC[i]:\t" << dataSCS[i] << std::endl;
		dw->points.push_back(points(int(dataAge[i]),dataSCS[i]));
	}*/

	for(int i=0; i<80; i++)		//2012.07.16,插入参考曲线
	{
		dw->points.push_back(points(int(i+20),cur_SCS_test[i]));
	}

	dw->current_value.push_back(points(int(dw->age),dw->scs_value));
	dw->type = scs;
	dw->showPoints();
	sqlite3_close(pdb);
	sequence = 0;	
	delete [] dataAge;
	delete [] dataSCS;
}




//打印键，2012/02/26

void Win_3::slotPrint()
{

		//后面为正式格式

		QStringList str;
		str<<"姓名"
		   <<"编号"
		   <<"年龄"
		   <<"性别"
		   <<"身高"                
		   <<"体重"
		   <<"左右脚"
		   <<"年轻成人"
	    	   <<"值"
		   <<"相应年龄"
		   <<"值";
		QString str0 = "骨超声诊断报告";
		//QStringList str_value;
		//char * index[9]={"%","T","%","Z","BC:","AIB:","SCS:","(Years)",":"};  
		char * index[12]={"%","T","%","Z","BC:","AIB:","SCS:","(Years)",":","BC","AIB","SCS"};     //2012.05.24

		char * letter[4] = {"123456789012345678901234567890", "107", "12/18/2011","3:57 PM"};
		//str_value<<"某某人"<<"男"<<"左"<<"亚洲";
		//const char * values[12]={"10/12/2011", "92", "5", "175", "65", "189", "0.2","98", "0.7", "117", "62","134"};
		

		print_start();  //启动打印机
		printf("%c",0x1b);usleep(100);   //初始化打印机，否者打印汉字出现问题
		printf("%c",0x40);usleep(100);

				               
                printf("%c",0x1b);usleep(100);  //禁止反向打印
                printf("%c",0x63);usleep(100);
		printf("%c",0x00);usleep(100);

		printf("%c",0x1b);usleep(100);	//放大2倍
		printf("%c",0x57);usleep(100);
		printf("%c",0x02);usleep(100);
		
		print_space(2);			//title
		print_chinese(str0);
		printf("%c",0x0a);usleep(100);

		printf("%c",0x1b);usleep(100);   //初始化打印机，否者打印汉字出现问题
		printf("%c",0x40);usleep(100);

				               
                printf("%c",0x1b);usleep(100);  //禁止反向打印
                printf("%c",0x63);usleep(100);
		printf("%c",0x00);usleep(100);
		
		printf("%c",0x1b);usleep(100);	//放大1倍
		printf("%c",0x57);usleep(100);
		printf("%c",0x01);usleep(100);
		
		printf("%c",0x1b);usleep(100);	//行间距为4
		printf("%c",0x31);usleep(100);
		printf("%c",0x04);usleep(100);
		
		print_space(12);
		const char *str_date=date.c_str();	//日期打印
		print_chs(str_date);
		printf("%c",0x0a);usleep(100);
		
		
		/*for(int i=0; i<12; i++){
			print_chs(values[i]);
			print_line(1);
		}
		for(int i=0; i<3; i++){
			print_chinese(str_value.at(i));
			print_line(1);
		}*/

		
		print_line(1);
		print_space(4);		//第1行 	name	num
		print_chinese(str.at(0));
		print_chs(index[8]);
		/*if(str_value.at(0).length()>7)
			print_space(9-str_value.at(0).length());
		else 
			print_space(8-str_value.at(0).length());*/
		print_space(6-str_value0.length());
		//print_chinese(str_value.at(0));	//20120307
		QByteArray name_ba=str_value0.toLatin1();
		const char * name_chs=name_ba.data();		
		print_chs(name_chs);

		print_space(3);
		print_chinese(str.at(1));
		print_chs(index[8]);
		print_space(5-strlen(values[1]));
		print_chs(values[1]);
		printf("%c",0x0a);usleep(100);

		print_space(4);		//第2行 	age	sex
		print_chinese(str.at(2));
		print_chs(index[8]);
		print_space(5-strlen(values[2]));
		print_chs(values[2]);
		print_space(4);
		print_chinese(str.at(3));
		print_chs(index[8]);
		print_space(6-str_value1.length());
		print_chinese(str_value1);
		printf("%c",0x0a);usleep(100);
		
		print_space(4);		//第3行 	height	weight
		print_chinese(str.at(4));
		print_chs(index[8]);
		print_space(5-strlen(values[3]));
		print_chs(values[3]);
		print_space(4);
		print_chinese(str.at(5));
		print_chs(index[8]);
		print_space(5-strlen(values[4]));
		print_chs(values[4]);
		printf("%c",0x0a);usleep(100);
		
		/*print_space(4);		//第4行 	foot   20120621
		print_chinese(str.at(6));
		print_chs(index[8]);
		print_space(5-str_value2.length());
		print_chinese(str_value2);
		printf("%c",0x0a);usleep(100);*/
		
		print_line(1);

		int m;
		/*for(int i=7; i<11; i++){
			if (i==7|i==9)
				print_space(4);
			else  	
				print_space(2);
			print_chs(index[i-7]);		//4 about ratio
			print_chinese(str.at(i));
			print_chs(index[8]);		
			if(i==7||i==9)
			{
				m=3-strlen(values[i-2]);		
				print_space(m);			
			}
			else 
			{
				m=5-strlen(values[i-2]);		
				print_space(m);			
			}
			print_chs(values[i-2]);
			if(i==8|i==10)
				printf("%c",0x0a);usleep(100);
			//if(i==9||i==11)	
			//	print_line(1);
		}*/
		
		print_space(2);				//3 prameters
		/*for(int i=0; i<3; i++)
		{
			print_chs(index[i+4]);
			print_space(6-strlen(values[i+9]));
			print_chs(values[i+9]);
			print_space(1);
		}*/
		print_chs(index[4]);
		print_space(6-strlen(values[9]));
		print_chs(values[9]);
		print_space(1);
		
		print_chs(index[5]);
		print_space(5-strlen(values[10]));
		print_chs(values[10]);
		print_space(1);

		print_chs(index[6]);
		print_space(5-strlen(values[11]));
		print_chs(values[11]);
		print_space(1);
		
		printf("%c",0x0a);usleep(100);
		print_line(1);
		

		printf("%c",0x1b);usleep(100);	//间距恢复
		printf("%c",0x31);usleep(100);
		printf("%c",0x00);usleep(100);

	

		//gen_diag();
		//print_diag(50,104);
		gen_diag();	//打印3个图表
		print_space(5);
		print_chs(index[9]);
		printf("%c",0x0a);usleep(100);
		//print_diag(age_value,-9);
		print_diag(age_value,BCvalue);
		print_line(1);

		print_space(5);
		print_chs(index[10]);
		printf("%c",0x0a);usleep(100);
		//print_diag2(age_value,-15);
		print_diag2(age_value,AIBvalue);
		print_line(1);

		print_space(5);
		print_chs(index[11]);
		printf("%c",0x0a);usleep(100);
		//print_diag3(age_value,-392);
		print_diag3(age_value,SCSvalue);
		print_line(1);

	

                //print_chs(letter[0]);    //用作标尺      
		//print_chinese(str.at(0)); 
                //printf("%c",0x0a);usleep(100);
		
		
		
		print_line(4);
	        
		print_end();  //关闭打印机
	





	
		                
}


void Win_3::insert_mat(char *dest, char *src)
{
	for(int i=0; i<3; i++)
	{
		memcpy(dest+i*280, src+i*12, 12);   //dest*280换行,src*12换行
	}
}


void Win_3::print_space(int x)
{
	printf("%c",0x1b);usleep(100);
	printf("%c",0x66);usleep(100);
	printf("%c",0x00);usleep(100);
	printf("%c",x);usleep(100);
}


void Win_3::print_line(int x)
{
	printf("%c",0x1b);usleep(100);
	printf("%c",0x66);usleep(100);
	printf("%c",0x01);usleep(100);
	printf("%c",x);usleep(100);
}


void Win_3::print_chs(const char ch[])
{               
		
		for(int i = 0; i < strlen(ch);++i)
		{       
			printf("%c",ch[i]);
			usleep(100);
		}
		//printf("%c",0x0a);           //打印完后换行已被侯黎注释掉
		//usleep(100);
}



void Win_3::print_chinese(QString chinese)
{
		QByteArray ba = chinese.toAscii () ;
		char *ch = ba.data();
		int len = strlen(ch);
		unsigned char *str2 = (unsigned char *)ch;
		unsigned short temp1[len/3+1];
		unsigned char temp2[2*(len/3+1)];
		int a = utf8_to_unicode(str2, temp1, len, 0);
		int res = unicode_to_gbk(temp1, temp2, 2*len/3, 0);
		//print_start();  
		for(int i = 0; i < 2*len/3;++i)
		{
			printf("%c",temp2[i]);
			usleep(100);
		}
		//printf("%c",0x0a);                    //打印完后换行已被侯黎注释掉
		//usleep(100);
                		
                //print_end();  //关闭打印机
		
}

void Win_3::print_start()
{
		printf("%c",0x03);
		usleep(100);
                printf("%c",0x55);
		usleep(100);
}

void Win_3::print_end()
{
		printf("%c",0x03);
		usleep(100);
                printf("%c",0xaa);
		usleep(100);
		printf("%c",'\n');
		usleep(100);
}




void Win_3::gen_diag(void)
{
	memset(&diag[0][0],0,sizeof(char)*24*280);  //全部置为0
	memset(&diag2[0][0],0,sizeof(char)*24*280); 
	memset(&diag3[0][0],0,sizeof(char)*24*280); 


	char one[3][12]={{0x00,0x00,0x00,0x20,0x20,0x60,0xff,0x00,0x00,0x00,0x00,0x00},		//ok
			{0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00},
			{0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0x00,0x00}};

	char two[3][12]={{0x00,0x18,0x20,0x40,0x80,0x80,0x80,0x80,0x41,0x22,0x18,0x00},		//ok
			{0x00,0x00,0x00,0x01,0x06,0x08,0x10,0x60,0x80,0x00,0x00,0x00},
			{0x00,0x30,0x50,0x90,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00}};

	char three[3][12]={{0x00,0x00,0x20,0x40,0x00,0x80,0x80,0x00,0x40,0x21,0x1e,0x00},	//2012.05.24
	   		  {0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x00,0x30,0x0f,0x00},
			  {0x00,0x00,0x60,0x00,0x10,0x10,0x10,0x10,0x00,0x60,0x80,0x00}};

	char eight[3][12]={{0x00,0x1e,0x21,0x40,0x00,0x80,0x80,0x00,0x40,0x21,0x1e,0x00},	//ok
	   		  {0x00,0x0f,0x30,0x00,0x40,0x40,0x40,0x40,0x00,0x30,0x0f,0x00},
			  {0x00,0x80,0x60,0x00,0x10,0x10,0x10,0x10,0x00,0x60,0x80,0x00}};

	char zero[3][12]={{0x00,0x1f,0x20,0x40,0x80,0x80,0x80,0x80,0x40,0x20,0x1f,0x00},	//ok
			 {0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00},
	 		 {0x00,0x80,0x40,0x20,0x10,0x10,0x10,0x10,0x20,0x40,0x80,0x00}};

	char four[3][12]={{0x00,0x00,0x00,0x00,0x03,0x06,0x18,0x30,0xff,0x00,0x00,0x00},	//ok
	  		 {0x00,0x0e,0x32,0xc2,0x02,0x02,0x02,0x02,0xff,0x02,0x02,0x00},   
			 {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x00,0x00,0x00}};

	char five[3][12]={{0x00,0xff,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00},	//ok
			 {0x00,0xe0,0x40,0x40,0x80,0x80,0x80,0x80,0x40,0x40,0x3f,0x00},
			 {0x00,0x40,0x20,0x20,0x10,0x10,0x10,0x10,0x20,0x20,0xc0,0x00}};

	char six[3][12]={{0x00,0x1f,0x20,0x40,0x00,0x80,0x80,0x00,0x40,0x20,0x10,0x00},		//ok
			{0x00,0xff,0x30,0x00,0x40,0x40,0x40,0x40,0x00,0x30,0x0f,0x00},
			{0x00,0x80,0x60,0x00,0x10,0x10,0x10,0x10,0x00,0x60,0x80,0x00}};

	char seven[3][12]={{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x8d,0xe0,0x00},	//2012.05.24
			{0x00,0x00,0x00,0x00,0x00,0x01,0x0e,0x70,0x80,0x00,0x00,0x00},
			{0x00,0x00,0x00,0x00,0x30,0xd0,0x00,0x00,0x00,0x00,0x00,0x00}};
	
	char blank[3][12]={{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	//ok
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	
	//curve数组中存放着15条曲线，0-4表示5个不同频率对应的BC标准曲线，5-9 对应5条AIB标准曲线，10-14 对应SCS标准曲线,2012.05.28 from houli
	double curve[15][80]={ {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-9.0759, -8.90295, -8.74823, -8.6112, -8.49135, -8.38815, -8.30108, -8.22962, -8.17325, -8.13145, -8.10368, -8.08944, -8.08821, -8.09944, -8.12264, -8.15727, -8.20281,  -8.25874, -8.32453, -8.39968, -8.48365, -8.57592, -8.67597, -8.78328, -8.89733, -9.01759, -9.14354, -9.27467, -9.41044, -9.55034, -9.69384, -9.84043, -9.98958, -10.1408, -10.2935, -10.4472, -10.6013, -10.7555, -10.909, -11.0615, -11.2123, -11.361, -11.5071, -11.65, -11.7891, -11.9241, -12.0543, -12.1792, -12.2984, -12.4112, -12.5172, -12.6158, -12.7066, -12.7889, -12.8624, -12.9264, -12.9804, -13.024, -13.0565, -13.0775, -13.0864, -13.0828, -13.0661, -13.0357, -12.9913, -12.9321, -12.8578, -12.7677, -12.6615, -12.5385, -12.3982, -12.2401, -12.0637, -11.8684, -11.6538, -11.4193, -11.1645, -10.8886, -10.5914, -10.2721},
 {-9.0759, -8.90295, -8.74823, -8.6112, -8.49135, -8.38815, -8.30108, -8.22962, -8.17325, -8.13145, -8.10368, -8.08944, -8.08821, -8.09944, -8.12264, -8.15727, -8.20281, -8.25874, -8.32453, -8.39968, -8.48365, -8.57592, -8.67597, -8.78328, -8.89733, -9.01759, -9.14354, -9.27467, -9.41044, -9.55034, -9.69384, -9.84043, -9.98958, -10.1408, -10.2935, -10.4472, -10.6013, -10.7555, -10.909, -11.0615, -11.2123, -11.361, -11.5071, -11.65, -11.7891, -11.9241, -12.0543, -12.1792, -12.2984, -12.4112, -12.5172, -12.6158, -12.7066, -12.7889, -12.8624, -12.9264, -12.9804, -13.024, -13.0565, -13.0775, -13.0864, -13.0828, -13.0661, -13.0357, -12.9913, -12.9321, -12.8578, -12.7677, -12.6615, -12.5385, -12.3982, -12.2401, -12.0637, -11.8684, -11.6538, -11.4193, -11.1645, -10.8886, -10.5914, -10.2721},
 {-9.0759, -8.90295, -8.74823, -8.6112, -8.49135, -8.38815, -8.30108, -8.22962, -8.17325, -8.13145, -8.10368, -8.08944, -8.08821, -8.09944, -8.12264, -8.15727, -8.20281, -8.25874, -8.32453, -8.39968, -8.48365, -8.57592, -8.67597, -8.78328, -8.89733, -9.01759, -9.14354, -9.27467, -9.41044, -9.55034, -9.69384, -9.84043, -9.98958, -10.1408, -10.2935, -10.4472, -10.6013, -10.7555, -10.909, -11.0615, -11.2123, -11.361, -11.5071, -11.65, -11.7891, -11.9241, -12.0543, -12.1792, -12.2984, -12.4112, -12.5172, -12.6158, -12.7066, -12.7889, -12.8624, -12.9264, -12.9804, -13.024, -13.0565, -13.0775, -13.0864, -13.0828, -13.0661, -13.0357, -12.9913, -12.9321, -12.8578, -12.7677, -12.6615, -12.5385, -12.3982, -12.2401, -12.0637, -11.8684, -11.6538, -11.4193, -11.1645, -10.8886, -10.5914, -10.2721},
 {-9.0759, -8.90295, -8.74823, -8.6112, -8.49135, -8.38815, -8.30108, -8.22962, -8.17325, -8.13145, -8.10368, -8.08944, -8.08821, -8.09944, -8.12264, -8.15727, -8.20281, -8.25874, -8.32453, -8.39968, -8.48365, -8.57592, -8.67597, -8.78328, -8.89733, -9.01759, -9.14354, -9.27467, -9.41044, -9.55034, -9.69384, -9.84043, -9.98958, -10.1408, -10.2935, -10.4472, -10.6013, -10.7555, -10.909, -11.0615, -11.2123, -11.361, -11.5071, -11.65, -11.7891, -11.9241, -12.0543, -12.1792, -12.2984, -12.4112, -12.5172, -12.6158, -12.7066, -12.7889, -12.8624, -12.9264, -12.9804, -13.024, -13.0565, -13.0775, -13.0864, -13.0828, -13.0661, -13.0357, -12.9913, -12.9321, -12.8578, -12.7677, -12.6615, -12.5385, -12.3982, -12.2401, -12.0637, -11.8684, -11.6538, -11.4193, -11.1645, -10.8886, -10.5914, -10.2721},
 {-9.0759, -8.90295, -8.74823, -8.6112, -8.49135, -8.38815, -8.30108, -8.22962, -8.17325, -8.13145, -8.10368, -8.08944, -8.08821, -8.09944, -8.12264, -8.15727, -8.20281, -8.25874, -8.32453, -8.39968, -8.48365, -8.57592, -8.67597, -8.78328, -8.89733, -9.01759, -9.14354, -9.27467, -9.41044, -9.55034, -9.69384, -9.84043, -9.98958, -10.1408, -10.2935, -10.4472, -10.6013, -10.7555, -10.909, -11.0615, -11.2123, -11.361, -11.5071, -11.65, -11.7891, -11.9241, -12.0543, -12.1792, -12.2984, -12.4112, -12.5172, -12.6158, -12.7066, -12.7889, -12.8624, -12.9264, -12.9804, -13.024, -13.0565, -13.0775, -13.0864, -13.0828, -13.0661, -13.0357, -12.9913, -12.9321, -12.8578, -12.7677, -12.6615, -12.5385, -12.3982, -12.2401, -12.0637, -11.8684, -11.6538, -11.4193, -11.1645, -10.8886, -10.5914, -10.2721}, 
 {-457.662, -458.1, -458.38, -458.505, -458.482, -458.317, -458.013, -457.576, -457.013, -456.327, -455.525, -454.612, -453.592, -452.472, -451.256, -449.95, -448.56, -447.089, -445.545, -443.931, -442.254, -440.519, -438.73, -436.894, -435.015, -433.099, -431.151, -429.177, -427.182, -425.17, -423.148, -421.12, -419.092, -417.07, -415.058, -413.062, -411.087, -409.139, -407.222, -405.342, -403.505, -401.715, -399.978, -398.299, -396.684, -395.138, -393.666, -392.273, -390.965, -389.747, -388.624, -387.602, -386.686, -385.881, -385.193, -384.626, -384.187, -383.88, -383.71, -383.684, -383.806, -384.082, -384.516, -385.115, -385.883, -386.826, -387.949, -389.258, -390.757, -392.452, -394.348, -396.452, -398.767, -401.299, -404.054, -407.036, -410.252, -413.706, -417.403, -421.35}, 
 {-457.662, -458.1, -458.38, -458.505, -458.482, -458.317, -458.013, -457.576, -457.013, -456.327, -455.525, -454.612, -453.592, -452.472, -451.256, -449.95, -448.56, -447.089, -445.545, -443.931, -442.254, -440.519, -438.73, -436.894, -435.015, -433.099, -431.151, -429.177, -427.182, -425.17, -423.148, -421.12, -419.092, -417.07, -415.058, -413.062, -411.087, -409.139, -407.222, -405.342, -403.505, -401.715, -399.978, -398.299, -396.684, -395.138, -393.666, -392.273, -390.965, -389.747, -388.624, -387.602, -386.686, -385.881, -385.193, -384.626, -384.187, -383.88, -383.71, -383.684, -383.806, -384.082, -384.516, -385.115, -385.883, -386.826, -387.949, -389.258, -390.757, -392.452, -394.348, -396.452, -398.767, -401.299, -404.054, -407.036, -410.252, -413.706, -417.403, -421.35}, 
 {-457.662, -458.1, -458.38, -458.505, -458.482, -458.317, -458.013, -457.576, -457.013, -456.327, -455.525, -454.612, -453.592, -452.472, -451.256, -449.95, -448.56, -447.089, -445.545, -443.931, -442.254, -440.519, -438.73, -436.894, -435.015, -433.099, -431.151, -429.177, -427.182, -425.17, -423.148, -421.12, -419.092, -417.07, -415.058, -413.062, -411.087, -409.139, -407.222, -405.342, -403.505, -401.715, -399.978, -398.299, -396.684, -395.138, -393.666, -392.273, -390.965, -389.747, -388.624, -387.602, -386.686, -385.881, -385.193, -384.626, -384.187, -383.88, -383.71, -383.684, -383.806, -384.082, -384.516, -385.115, -385.883, -386.826, -387.949, -389.258, -390.757, -392.452, -394.348, -396.452, -398.767, -401.299, -404.054, -407.036, -410.252, -413.706, -417.403, -421.35}, 
 {-457.662, -458.1, -458.38, -458.505, -458.482, -458.317, -458.013, -457.576, -457.013, -456.327, -455.525, -454.612, -453.592, -452.472, -451.256, -449.95, -448.56, -447.089, -445.545, -443.931, -442.254, -440.519, -438.73, -436.894, -435.015, -433.099, -431.151, -429.177, -427.182, -425.17, -423.148, -421.12, -419.092, -417.07, -415.058, -413.062, -411.087, -409.139, -407.222, -405.342, -403.505, -401.715, -399.978, -398.299, -396.684, -395.138, -393.666, -392.273, -390.965, -389.747, -388.624, -387.602, -386.686, -385.881, -385.193, -384.626, -384.187, -383.88, -383.71, -383.684, -383.806, -384.082, -384.516, -385.115, -385.883, -386.826, -387.949, -389.258, -390.757, -392.452, -394.348, -396.452, -398.767, -401.299, -404.054, -407.036, -410.252, -413.706, -417.403, -421.35}, 
 {-457.662, -458.1, -458.38, -458.505, -458.482, -458.317, -458.013, -457.576, -457.013, -456.327, -455.525, -454.612, -453.592, -452.472, -451.256, -449.95, -448.56, -447.089, -445.545, -443.931, -442.254, -440.519, -438.73, -436.894, -435.015, -433.099, -431.151, -429.177, -427.182, -425.17, -423.148, -421.12, -419.092, -417.07, -415.058, -413.062, -411.087, -409.139, -407.222, -405.342, -403.505, -401.715, -399.978, -398.299, -396.684, -395.138, -393.666, -392.273, -390.965, -389.747, -388.624, -387.602, -386.686, -385.881, -385.193, -384.626, -384.187, -383.88, -383.71, -383.684, -383.806, -384.082, -384.516, -385.115, -385.883, -386.826, -387.949, -389.258, -390.757, -392.452, -394.348, -396.452, -398.767, -401.299, -404.054, -407.036, -410.252, -413.706, -417.403, -421.35}};
	

	//插入数字 for diag
	insert_mat(&diag[0][36],&zero[0][0]);
	
	insert_mat(&diag[6][24],&one[0][0]);
	insert_mat(&diag[6][36],&six[0][0]);

	insert_mat(&diag[12][24],&three[0][0]);
	insert_mat(&diag[12][36],&two[0][0]);

	insert_mat(&diag[18][24],&four[0][0]);
	insert_mat(&diag[18][36],&eight[0][0]);

	insert_mat(&diag[21][59],&two[0][0]);   //横坐标数字,20120717 for adult
	insert_mat(&diag[21][71],&zero[0][0]); 
	insert_mat(&diag[21][104],&four[0][0]); 
	insert_mat(&diag[21][116],&zero[0][0]); 
	insert_mat(&diag[21][149],&six[0][0]); 
	insert_mat(&diag[21][161],&zero[0][0]); 
	insert_mat(&diag[21][194],&eight[0][0]); 
	insert_mat(&diag[21][206],&zero[0][0]); 
	insert_mat(&diag[21][233],&one[0][0]); 	//2012.07.17, for adult版本
	insert_mat(&diag[21][245],&zero[0][0]); 
	insert_mat(&diag[21][257],&zero[0][0]); 

	
	for(int i=0; i<12; i++)       //插入16\32\48前面的两个负号
	{
		diag[7][12+i]=0x20;
		diag[13][12+i]=0x20;
		diag[19][12+i]=0x20;
	}

	//画横线
	for(int i=0; i<200; i++)    //3条长横线
	{
		diag[0][i+60]=0x02;
		diag[9][i+60]=0x02;
		diag[18][i+60]=0x02;
	}
	for(int i=0; i<180; i++)    //稍短的一条横线
	{
		diag[14][i+70]=0x20;
	}
	for(int i=0; i<10; i++)     //8条短横线
	{
		diag[3][i+60]=0x02;
		diag[3][i+250]=0x02;
		diag[6][i+60]=0x02;
		diag[6][i+250]=0x02;
		diag[12][i+60]=0x02;
		diag[12][i+250]=0x02;
		diag[15][i+60]=0x02;
		diag[15][i+250]=0x02;
	}

	//2长竖线和7条短竖线
	diag[0][70]=0x03;
	diag[0][250]=0x03;
	for(int i=1; i<21; i++)
	{
		diag[i][70]=0xff;
		diag[i][250]=0xff;
	}
	for(int i=18; i<20; i++)
	{
		if(i==18){
			for(int j=92; j<229; j=j+45)
				diag[i][j]=0x03;
			for(int j=115; j<207; j=j+45)
				diag[i][j]=0x03;
		}
		else{
			for(int j=92; j<229; j=j+45)
				diag[i][j]=0xff;
			for(int j=115; j<207; j=j+45)
				diag[i][j]=0xff;
		}
	}


	//2种不同的散点
	for(int j=80; j<250; j=j+10)
		diag[10][j]=0x04;
	for(int j=75; j<250; j=j+10)
		diag[11][j]=0x08;
	for(int j=80; j<250; j=j+10)
		diag[12][j]=0x10;
	for(int j=75; j<250; j=j+10)
		diag[13][j]=0x20;

	for(int j=76; j<250; j=j+6)
		diag[14][j]=0x21;
	for(int j=73; j<250; j=j+6)
		diag[15][j]=0x08;
	for(int j=76; j<250; j=j+6)
		diag[16][j]=0x40;
	for(int j=73; j<250; j=j+6)
		diag[16][j]=0x02;
	for(int j=76; j<250; j=j+6)
		diag[17][j]=0x10;
	for(int j=73; j<250; j=j+6)
		diag[18][j]=0x82;
	for(int j=76; j<250; j=j+6)
		diag[18][j]=0x06;


	//*******************************************************************
	//制造diag2和diag3原始图形,横坐标和图表通过复制diag1得到

	for(int i=0; i<24; i++)
	{
		for(int j=0; j<280; j++)
		{
			diag2[i][j]=diag[i][j];
			diag3[i][j]=diag[i][j];
		}	
	}
	
	//插入对应纵坐标数字 for diag2
	insert_mat(&diag2[0][36],&zero[0][0]);
	
	insert_mat(&diag2[6][12],&blank[0][0]);	
	insert_mat(&diag2[6][24],&blank[0][0]);
	insert_mat(&diag2[6][36],&six[0][0]);
	
	insert_mat(&diag2[12][24],&one[0][0]);
	insert_mat(&diag2[12][36],&two[0][0]);

	insert_mat(&diag2[18][24],&one[0][0]);
	insert_mat(&diag2[18][36],&eight[0][0]);

	
	for(int i=0; i<12; i++)       //插入6\12\18前面的两个负号
	{
		diag2[7][24+i]=0x20;
		diag2[13][12+i]=0x20;
		diag2[19][12+i]=0x20;
	}



	//插入对应纵坐标数字 for diag3
	insert_mat(&diag3[0][12],&three[0][0]);
	insert_mat(&diag3[0][24],&five[0][0]);
	insert_mat(&diag3[0][36],&zero[0][0]);
	
	insert_mat(&diag3[6][12],&four[0][0]);
	insert_mat(&diag3[6][24],&one[0][0]);
	insert_mat(&diag3[6][36],&zero[0][0]);
	
	insert_mat(&diag3[12][12],&four[0][0]);
	insert_mat(&diag3[12][24],&seven[0][0]);
	insert_mat(&diag3[12][36],&zero[0][0]);

	insert_mat(&diag3[18][12],&five[0][0]);
	insert_mat(&diag3[18][24],&three[0][0]);
	insert_mat(&diag3[18][36],&zero[0][0]);
	
	for(int i=0; i<12; i++)       //插入350\410\470\530前面的两个负号
	{
		diag3[1][i]=0x20;
		diag3[7][i]=0x20;
		diag3[13][i]=0x20;
		diag3[19][i]=0x20;
	}
	



	//下面开始为diag, diag2, diag3 分别加上标准曲线，根据探头中心频率
	//gen_curve();    //填入曲线数据！	
	struct coordinate curvedot;
	/*for(int i=0; i<80; i++)		//diag,BC
	{
		curvedot.y=70+2.25*(i+20-20);
		switch (cen_freq)
		{
			case 1: curvedot.x=6+3*(-curve[0][i]);
			case 2: curvedot.x=6+3*(-curve[1][i]);
			case 3: curvedot.x=6+3*(-curve[2][i]);
			case 5: curvedot.x=6+3*(-curve[3][i]);
			case 6: curvedot.x=6+3*(-curve[4][i]);
		}
		matrix.x=(curvedot.x)/8;
		matrix.y=curvedot.y;
		int temp;
		temp=7-(curvedot.x)%8;
		bit=0x1;
		if(temp>0){
		   for(int i=0;i<temp;i++)
			bit=2*bit;
		}
		diag[matrix.x][matrix.y]=((diag[matrix.x][matrix.y])|bit);
	}*/

	for(int i=0; i<80; i++)		//diag2,AIB
	{
		curvedot.y=70+2.25*(i+20-20);	//2012.07.17 for adult版
		switch (cen_freq)
		{
			case 1: curvedot.x=6.0+8.0*(-curve[5][i]);
			case 2: curvedot.x=6.0+8.0*(-curve[6][i]);
			case 3: curvedot.x=6.0+8.0*(-curve[7][i]);
			case 5: curvedot.x=6.0+8.0*(-curve[8][i]);
			case 6: curvedot.x=6.0+8.0*(-curve[9][i]);
		}
		matrix.x=(curvedot.x)/8;
		matrix.y=curvedot.y;
		int temp;
		temp=7-(curvedot.x)%8;
		bit=0x1;
		if(temp>0){
		   for(int i=0;i<temp;i++)
			bit=2*bit;
		}
		diag2[matrix.x][matrix.y]=((diag2[matrix.x][matrix.y])|bit);
	}

	for(int i=0; i<80; i++)		//diag3, SCS
	{
		curvedot.y=70+2.25*(i+20-20);	//2012.07.17 for adult版
		switch (cen_freq)
		{
			case 1: curvedot.x=6+0.8*(-350-curve[10][i]);
			case 2: curvedot.x=6+0.8*(-350-curve[11][i]);
			case 3: curvedot.x=6+0.8*(-350-curve[12][i]);
			case 5: curvedot.x=6+0.8*(-350-curve[13][i]);
			case 6: curvedot.x=6+0.8*(-350-curve[14][i]);
		}
		matrix.x=(curvedot.x)/8;
		matrix.y=curvedot.y;
		int temp;
		temp=7-(curvedot.x)%8;
		bit=0x1;
		if(temp>0){
		   for(int i=0;i<temp;i++)
			bit=2*bit;
		}
		diag3[matrix.x][matrix.y]=((diag3[matrix.x][matrix.y])|bit);
	}

}


//***************************************************


void Win_3::print_diag(int x,int y)
{
	char temp_diag[24][280];
	
	
	for(int i=0; i<24; i++)
	{
		for(int j=0; j<280; j++)
			temp_diag[i][j]=diag[i][j];
	}


	orient.x=x;	//描点位置!!!!!
	orient.y=y;
	zuobiao[0].x=6+3*(-orient.y);    //第一步转换成图形上的点坐标
	zuobiao[0].y=70+2.25*(orient.x-20);	//20120621
	for(int i=1; i<6; i++)                 //横线
	{
		zuobiao[i].x=zuobiao[0].x+(i-6);
		zuobiao[i].y=zuobiao[0].y;
	}
	for(int i=6; i<11; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-5);
		zuobiao[i].y=zuobiao[0].y;
	}
	for(int i=11; i<16; i++)                //竖线
	{
		zuobiao[i].x=zuobiao[0].x;
		zuobiao[i].y=zuobiao[0].y+(i-16);
	}
	for(int i=16; i<21; i++)
	{
		zuobiao[i].x=zuobiao[0].x;
		zuobiao[i].y=zuobiao[0].y+(i-15);
	}
	for(int i=21; i<26; i++)                //斜率为1
	{
		zuobiao[i].x=zuobiao[0].x+(i-26);
		zuobiao[i].y=zuobiao[0].y+(i-26);
	}
	for(int i=26; i<31; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-25);
		zuobiao[i].y=zuobiao[0].y+(i-25);
	}
	for(int i=31; i<36; i++)                //斜率为-1
	{
		zuobiao[i].x=zuobiao[0].x+(i-36);
		zuobiao[i].y=zuobiao[0].y+(-i+36);
	}
	for(int i=36; i<41; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-35);
		zuobiao[i].y=zuobiao[0].y+(-i+35);
	}

	//从图形上的点坐标转换成矩阵坐标
	for(int i=0; i<41; i++)
	{
		matrix.x=(zuobiao[i].x)/8;
		matrix.y=zuobiao[i].y;
		int temp;
		temp=7-(zuobiao[i].x)%8;
		bit=0x1;
		if(temp>0){
		   for(int i=0;i<temp;i++)
			bit=2*bit;
		}
		temp_diag[matrix.x][matrix.y]=((temp_diag[matrix.x][matrix.y])|bit);

	}
	
	



	//print_diag();打印图形部分
	for(int i=0; i<24; i++)                   //共有24组数据
	{
		printf("%c",0x1b);usleep(100);
		printf("%c",0x36);usleep(100);		//选择字符集之后注意在程序开始前初始化打印机
		for(int k=0; k<8; k++)
			printf("%c",0x20);usleep(100);    //每行前放置8个8*6的空白符号

		printf("%c",0x1b);usleep(100);
		printf("%c",0x4b);usleep(100);
		printf("%c",0x18);usleep(100);
		printf("%c",0x01);usleep(100);
		for(int j=0; j<280; j++)
		{
			printf("%c",temp_diag[i][j]);
		}
		printf("%c",0x0d);usleep(100);
	}
	
	QString str="年龄";		//20120301
	const char* years="(Years)";
	printf("%c",0x1b);usleep(100);   //初始化打印机，否者打印汉字出现问题
	printf("%c",0x40);usleep(100);

        printf("%c",0x1b);usleep(100);  //禁止反向打印
        printf("%c",0x63);usleep(100);
	printf("%c",0x00);usleep(100);
	print_space(11);
	print_chinese(str);
	print_space(2);
	print_chs(years);
	printf("%c",0x0a);usleep(100);




}

void Win_3::print_diag2(int x,int y)
{
	char temp_diag[24][280];
	
	
	for(int i=0; i<24; i++)
	{
		for(int j=0; j<280; j++)
			temp_diag[i][j]=diag2[i][j];
	}


	orient.x=x;	//描点位置!!!!!
	orient.y=y;
	zuobiao[0].x=6+8*(-orient.y);    //第一步转换成图形上的点坐标,与前面不同
	zuobiao[0].y=70+2.25*(orient.x-20);	//20120621
	for(int i=1; i<6; i++)                 //横线
	{
		zuobiao[i].x=zuobiao[0].x+(i-6);
		zuobiao[i].y=zuobiao[0].y;
	}
	for(int i=6; i<11; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-5);
		zuobiao[i].y=zuobiao[0].y;
	}
	for(int i=11; i<16; i++)                //竖线
	{
		zuobiao[i].x=zuobiao[0].x;
		zuobiao[i].y=zuobiao[0].y+(i-16);
	}
	for(int i=16; i<21; i++)
	{
		zuobiao[i].x=zuobiao[0].x;
		zuobiao[i].y=zuobiao[0].y+(i-15);
	}
	for(int i=21; i<26; i++)                //斜率为1
	{
		zuobiao[i].x=zuobiao[0].x+(i-26);
		zuobiao[i].y=zuobiao[0].y+(i-26);
	}
	for(int i=26; i<31; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-25);
		zuobiao[i].y=zuobiao[0].y+(i-25);
	}
	for(int i=31; i<36; i++)                //斜率为-1
	{
		zuobiao[i].x=zuobiao[0].x+(i-36);
		zuobiao[i].y=zuobiao[0].y+(-i+36);
	}
	for(int i=36; i<41; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-35);
		zuobiao[i].y=zuobiao[0].y+(-i+35);
	}


	for(int i=0; i<41; i++)
	{
		matrix.x=(zuobiao[i].x)/8;
		matrix.y=zuobiao[i].y;
		int temp;
		temp=7-(zuobiao[i].x)%8;
		bit=0x1;
		if(temp>0){
		   for(int i=0;i<temp;i++)
			bit=2*bit;
		}
		temp_diag[matrix.x][matrix.y]=((temp_diag[matrix.x][matrix.y])|bit);

	}




		//print_diag();打印图形部分
	for(int i=0; i<24; i++)                   //共有24组数据
	{
		printf("%c",0x1b);usleep(100);
		printf("%c",0x36);usleep(100);		//选择字符集之后注意在程序开始前初始化打印机
		for(int k=0; k<8; k++)
			printf("%c",0x20);usleep(100);    //每行前放置8个8*6的空白符号

		printf("%c",0x1b);usleep(100);
		printf("%c",0x4b);usleep(100);
		printf("%c",0x18);usleep(100);
		printf("%c",0x01);usleep(100);
		for(int j=0; j<280; j++)
		{
			printf("%c",temp_diag[i][j]);
		}
		printf("%c",0x0d);usleep(100);
	}
	
	QString str="年龄";		//20120301
	const char* years="(Years)";
	printf("%c",0x1b);usleep(100);   //初始化打印机，否者打印汉字出现问题
	printf("%c",0x40);usleep(100);

        printf("%c",0x1b);usleep(100);  //禁止反向打印
        printf("%c",0x63);usleep(100);
	printf("%c",0x00);usleep(100);
	print_space(11);
	print_chinese(str);
	print_space(2);
	print_chs(years);
	printf("%c",0x0a);usleep(100);




}

void Win_3::print_diag3(int x,int y)
{
	char temp_diag[24][280];
	
	
	for(int i=0; i<24; i++)
	{
		for(int j=0; j<280; j++)
			temp_diag[i][j]=diag3[i][j];
	}


	orient.x=x;	//描点位置!!!!!
	orient.y=y;
	zuobiao[0].x=6+0.8*(-350-orient.y);    //第一步转换成图形上的点坐标,与前面也不同！
	zuobiao[0].y=70+2.25*(orient.x-20);	//20120621
	for(int i=1; i<6; i++)                 //横线
	{
		zuobiao[i].x=zuobiao[0].x+(i-6);
		zuobiao[i].y=zuobiao[0].y;
	}
	for(int i=6; i<11; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-5);
		zuobiao[i].y=zuobiao[0].y;
	}
	for(int i=11; i<16; i++)                //竖线
	{
		zuobiao[i].x=zuobiao[0].x;
		zuobiao[i].y=zuobiao[0].y+(i-16);
	}
	for(int i=16; i<21; i++)
	{
		zuobiao[i].x=zuobiao[0].x;
		zuobiao[i].y=zuobiao[0].y+(i-15);
	}
	for(int i=21; i<26; i++)                //斜率为1
	{
		zuobiao[i].x=zuobiao[0].x+(i-26);
		zuobiao[i].y=zuobiao[0].y+(i-26);
	}
	for(int i=26; i<31; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-25);
		zuobiao[i].y=zuobiao[0].y+(i-25);
	}
	for(int i=31; i<36; i++)                //斜率为-1
	{
		zuobiao[i].x=zuobiao[0].x+(i-36);
		zuobiao[i].y=zuobiao[0].y+(-i+36);
	}
	for(int i=36; i<41; i++)
	{
		zuobiao[i].x=zuobiao[0].x+(i-35);
		zuobiao[i].y=zuobiao[0].y+(-i+35);
	}


	for(int i=0; i<41; i++)
	{
		matrix.x=(zuobiao[i].x)/8;
		matrix.y=zuobiao[i].y;
		int temp;
		temp=7-(zuobiao[i].x)%8;
		bit=0x1;
		if(temp>0){
		   for(int i=0;i<temp;i++)
			bit=2*bit;
		}
		temp_diag[matrix.x][matrix.y]=((temp_diag[matrix.x][matrix.y])|bit);

	}




		//print_diag();打印图形部分
	for(int i=0; i<24; i++)                   //共有24组数据
	{
		printf("%c",0x1b);usleep(100);
		printf("%c",0x36);usleep(100);		//选择字符集之后注意在程序开始前初始化打印机
		for(int k=0; k<8; k++)
			printf("%c",0x20);usleep(100);    //每行前放置8个8*6的空白符号

		printf("%c",0x1b);usleep(100);
		printf("%c",0x4b);usleep(100);
		printf("%c",0x18);usleep(100);
		printf("%c",0x01);usleep(100);
		for(int j=0; j<280; j++)
		{
			printf("%c",temp_diag[i][j]);
		}
		printf("%c",0x0d);usleep(100);
	}
	
	QString str="年龄";		//20120301
	const char* years="(Years)";
	printf("%c",0x1b);usleep(100);   //初始化打印机，否者打印汉字出现问题
	printf("%c",0x40);usleep(100);

        printf("%c",0x1b);usleep(100);  //禁止反向打印
        printf("%c",0x63);usleep(100);
	printf("%c",0x00);usleep(100);
	print_space(11);
	print_chinese(str);
	print_space(2);
	print_chs(years);
	printf("%c",0x0a);usleep(100);




}


int utf8_to_unicode(unsigned char *utf8_buf, unsigned short *unicode_buf, int max_unicode_buf_size, int endian)
{
	unsigned short *uni_ptr = unicode_buf;
	unsigned char *utf_ptr = utf8_buf;
	unsigned short word;
	unsigned char ch;
	unsigned int uni_ind = 0, utf_ind = 0, utf_num = 0;

	while(1)
	{
		ch = *(utf_ptr + utf_ind);

		if(ch == 0x00) //œáÊø·û
			break;
		
		if ((ch & 0x80) == 0) 
		{
			*(unicode_buf + uni_ind) = *(utf_ptr + utf_ind);
			uni_ind++;
			
			utf_ind++;
			utf_num++;
		} 
		else if((ch & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
		{
			word = (*(utf_ptr + utf_ind) & 0x3F) << 6;
			word |= (*(utf_ptr + utf_ind + 1) & 0x3F);

			*(unicode_buf + uni_ind) = word;
			uni_ind++;

			utf_ind += 2;
			utf_num++;
		}
		else if((ch & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
		{
			word = (*(utf_ptr + utf_ind) & 0x1F) << 12;
			word |= (*(utf_ptr + utf_ind + 1) & 0x3F) << 6;
			word |= (*(utf_ptr + utf_ind + 2) & 0x3F);

			*(unicode_buf + uni_ind) = word;
			uni_ind++;

			utf_ind += 3;
			utf_num++;
		} 
		else if((ch & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			word = (*(utf_ptr + utf_ind) & 0x0F) << 18;
			word = (*(utf_ptr + utf_ind + 1) & 0x3F) << 12;
			word |= (*(utf_ptr + utf_ind + 2) & 0x3F) << 6;
			word |= (*(utf_ptr + utf_ind + 3) & 0x3F);

			*(unicode_buf + uni_ind) = word;
			uni_ind++;

			utf_ind += 4;
			utf_num++;
		} 
		else   ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			word = (*(utf_ptr + utf_ind) & 0x07) << 24;
			word = (*(utf_ptr + utf_ind + 1) & 0x3F) << 18;
			word = (*(utf_ptr + utf_ind + 2) & 0x3F) << 12;
			word = (*(utf_ptr + utf_ind + 3) & 0x3F) << 6;
			word = (*(utf_ptr + utf_ind + 4) & 0x3F);

			*(unicode_buf + uni_ind) = word;
			uni_ind++;

			utf_ind += 5;
			utf_num++;
		}

		if(utf_num > max_unicode_buf_size - 1)
		{
			break;
		}
	}

	return utf_num;
}

int unicode_to_gbk(unsigned short *unicode_buf, unsigned char *gbk_buf, int max_gbk_buf_size, int endian)
{
	unsigned short word;
	unsigned short gbk_word;
	unsigned char ch;
    unsigned char *gbk_ptr = gbk_buf;
    unsigned short *uni_ptr = unicode_buf;
	unsigned int uni_ind = 0, gbk_ind = 0, gbk_num = 0;
    int word_pos;

    if( !gbk_buf || !unicode_buf )
        return -1;

	while(1)
	{
		word = *(uni_ptr + uni_ind);
		uni_ind++;

		if(word == 0x0000)  //字符串结束符
			break;
		
		if(endian == 1)  //大端
		{
			//高低位交换			
			ch = (unsigned char)word;
			word = word >> 8;
			word += ch << 8;
		}


		if(word < 0x80)  /*ASCII不用查表*/
		{
			*(gbk_ptr + gbk_ind) = (unsigned char)word;
			gbk_ind++;
		}
		else
		{
			word_pos = word - g_unicode_first_code;
			if(word >= g_unicode_first_code && word <= g_unicode_last_code && word_pos < g_unicode_to_gbk_buf_size)
			{
				gbk_word = g_unicode_to_gbk_buf[word_pos];

				*(gbk_ptr + gbk_ind) = (unsigned char)(gbk_word >> 8);
				gbk_ind++;
				*(gbk_ptr + gbk_ind) = (unsigned char)(gbk_word >> 0);
				gbk_ind++;

				gbk_num++;
			}
		}

		if(gbk_num > max_gbk_buf_size - 1)
			break;
	}

    return gbk_num;
}



void Win_3::getDate(string date_tmp)
{
	date=date_tmp;
}

/*void Win_3::gen_curve()
{
  
   curve[15][80]={ {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36,-37,-38,-39,-40,
  -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,
  -1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,
  -1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,
  -1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,
  -1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1},
 {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,
  -1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1}, {-352,-354,-356,-358,-360,-362,-364,-366,-368,-370,-372,-374,-376,-378,-380,-382,-384,-386,-388,-390,-392,-394,-396,-398,-400,-402,-404,-406,-408,-410,-412,-414,-416,-418,-420,-422,-424,-426,-428,-430,	
-432,-434,-436,-438,-440,-442,-444,-446,-448,-450,-452,-454,-456,-458,-460,-462,-464,-466,-468,-470,-472,-474,-476,-478,-480,-482,-484,-486,-488,-490,-492,-494,-496,-498,-500,-502,-504,-506,-508,-510 }, {-352,-354,-356,-358,-360,-362,-364,-366,-368,-370,-372,-374,-376,-378,-380,-382,-384,-386,-388,-390,-392,-394,-396,-398,-400,-402,-404,-406,-408,-410,-412,-414,-416,-418,-420,-422,-424,-426,-428,-430,	
-432,-434,-436,-438,-440,-442,-444,-446,-448,-450,-452,-454,-456,-458,-460,-462,-464,-466,-468,-470,-472,-474,-476,-478,-480,-482,-484,-486,-488,-490,-492,-494,-496,-498,-500,-502,-504,-506,-508,-510 }, {-352,-354,-356,-358,-360,-362,-364,-366,-368,-370,-372,-374,-376,-378,-380,-382,-384,-386,-388,-390,-392,-394,-396,-398,-400,-402,-404,-406,-408,-410,-412,-414,-416,-418,-420,-422,-424,-426,-428,-430,	
-432,-434,-436,-438,-440,-442,-444,-446,-448,-450,-452,-454,-456,-458,-460,-462,-464,-466,-468,-470,-472,-474,-476,-478,-480,-482,-484,-486,-488,-490,-492,-494,-496,-498,-500,-502,-504,-506,-508,-510 }, {-352,-354,-356,-358,-360,-362,-364,-366,-368,-370,-372,-374,-376,-378,-380,-382,-384,-386,-388,-390,-392,-394,-396,-398,-400,-402,-404,-406,-408,-410,-412,-414,-416,-418,-420,-422,-424,-426,-428,-430,	
-432,-434,-436,-438,-440,-442,-444,-446,-448,-450,-452,-454,-456,-458,-460,-462,-464,-466,-468,-470,-472,-474,-476,-478,-480,-482,-484,-486,-488,-490,-492,-494,-496,-498,-500,-502,-504,-506,-508,-510 }, {-352,-354,-356,-358,-360,-362,-364,-366,-368,-370,-372,-374,-376,-378,-380,-382,-384,-386,-388,-390,-392,-394,-396,-398,-400,-402,-404,-406,-408,-410,-412,-414,-416,-418,-420,-422,-424,-426,-428,-430,	
-432,-434,-436,-438,-440,-442,-444,-446,-448,-450,-452,-454,-456,-458,-460,-462,-464,-466,-468,-470,-472,-474,-476,-478,-480,-482,-484,-486,-488,-490,-492,-494,-496,-498,-500,-502,-504,-506,-508,-510 }};



}*/


