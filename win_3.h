/*written by Wang Wenbin, time: 2010-7-3
  this file is used to display the result,waiting for improvement!!
*/

#ifndef WIN_3_H
#define WIN_3_H

#include <QWidget>
#include "data.h"
#include "draw.h"
#include <string>
#include <QtCore>
#include <QtGui>

class QLabel;
class QWidget;
class QTextBrowser;
class QPushButton;

class Win_3:public QWidget
{
	Q_OBJECT
	struct coordinate	//打印键
	{
		int x;
		int y;
	};
public:
	Win_3(QWidget *parent = 0,Qt::WindowFlags flags = 0);
	friend int callback(void *NotUsed,int argc,char **argv,char **azColName); 
	friend int callback1(void *NotUsed,int argc,char **argv,char **azColName); 
	void print_chs(const char ch[]);	//以下public函数，打印键，2012/02/26
	void print_chinese(QString chinese);
	void print_start();
	void print_end();
	void insert_mat(char *dest, char *src);
	void print_space(int x);
	void print_line(int x);
	void gen_diag(void);	//0301修改
	void print_diag(int x, int y);	//0301修改
	void print_diag2(int x, int y);	//添于2012.05.24pm
	void print_diag3(int x, int y);
	//void gen_curve();	//2012.05.28
	
	
signals:
	void getPersonalImfo();
	void signalImportResult();
	

	
public slots:
	void showPersonalImfo();
	void sendPersonalImfoACK(const Data &);
	void slotShowBCStandedCurve();
	void slotShowAIBStandedCurve();
	void slotShowSCSStandedCurve();
	void slotShowBCPoints();
	void slotShowAIBPoints();
	void slotShowSCSPoints();
	void slotImportResult();
	void slotRepeatImportError();
	void slotImportSuccess();
	void slotPrint();	//打印键，2012/03/01
	void getDate(string date_tmp);  //20120306 获取string类型的date
	void getInfoprinted(const Data &);
private:
	QLabel *label;
	QLabel *label_2;
	QLabel *label_3;
	QWidget *widget;
	QWidget *widget_2;
	QTextBrowser *textBrowser;
	QTextBrowser *textBrowser_2;
	QPushButton *button;
	QPushButton *button_2;
	QPushButton *button_3;
	QPushButton *button_4;
	QPushButton *button_5;
	QPushButton *button_1;	//打印键，2012/02/26
	Draw *dw;
	coordinate orient;	//以下private,打印键，2012/02/26
	coordinate matrix;
	char bit;
	struct coordinate zuobiao[41];
	char diag[24][280];
	char diag2[24][280];	//新增的2个diag
	char diag3[24][280];
	string date;	
	const char * values[12];	//打印建
	QString str_value0;
	QString str_value1;
	QString str_value2;
	int cen_freq;	//用于记录中心频率整数部分
	//curve 表示BC, AIB, SCS打印图表中的标准曲线。
	
	double curve[15][80];   //具体数据请见gen_diag函数


	QString age_str;	//age,20120310
	QByteArray age_ba;
	int age_value;
	QString height_str;	//height
	QByteArray height_ba;
	QString weight_str;	//weight
	QByteArray weight_ba;
	QString ratioper_str;	//ratioper
	QByteArray ratioper_ba;
	QString T_str;	//T	
	QByteArray T_ba;
	QString ratioage_str;	//ratioage
	QByteArray ratioage_ba;
	QString Z_str;	//Z
	QByteArray Z_ba;
	QString BC_str;		//BC
	QByteArray BC_ba;
	int BCvalue;
	QString AIB_str;	//AIB
	QByteArray AIB_ba;
	int AIBvalue;
	QString SCS_str;	//SCS
	QByteArray SCS_ba;
	int SCSvalue;
	
};

#endif
