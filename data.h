/*written by Wang Wenbin, 2010.7.3
  This file is used to arrange the data and computer the parameter.
*/
#ifndef DATA_H
#define DATA_H

#include <QWidget>
#include <string>
#include <vector>
#include <QVector>
#include <fstream>
#include <iostream>
#include "parameter.h"
#include "thread.h"

using namespace std;

class Data:public QWidget
{
	Q_OBJECT
public:
	Data();
	string name;//存放病人姓名
	string number;//存放病人编号
	sex_bool sex;//存放病人性别
	int height;//存放病人身高
	double weight;//存放病人体重
	int age;//存放病人年龄

	vector<double> data;//存放超声信号
	vector<double> spectrum;//存放超声信号频谱
	vector<double> bcPrm;
	
	double udFeq;//存放超声探头频率
	double prmBC;
	double prmAIB;
	double prmSCS;
	double prmBCLast;
	double prmAIBLast;
	double prmSCSLast;
	Thread thread;
//	int fd;
public:
	void openDriver();
signals:
	void sendPersonalImfo(const Data&);
	void signalPassWaveform(const vector<double>&);
	void signalPassSpectrum(const vector<double>&);
	void signalSaveDataSuccess();
	void signalSaveParameterSuccess();
	void signalParameterSaveError();
	void signalRepeatImportError();
	void signalImportSuccess();
	void signalAutoStop();
public slots:
	void keyToSure(string name_tmp,string number_tmp,sex_bool sex_tmp,int height_tmp,double weight_tmp,int age_tmp);//确认设置各个参数
	void beginMeasure(bool isDyncDsp,double feq);//begin measure
	void getPersonalImfoACK();
	void slotShowSpectrum();
	void slotClearData();
	void slotSaveData();
	void slotEndMeasure();
	void slotPassFileName(const QString&);
	void slotThreadData(const vector<double>);
	void slotImportResult();
//	void displayData();//显示信号波形
//	void diaplaySpectrum(); //显示信号频谱
//	void displayBC();
//	void displayAIB();
//	void displaySCS();

};
class greaterAbs
{
public:
	greaterAbs(double lhs)
	{
		gate = lhs;
	}
	bool operator()(double input)
	{
		return abs(input) >=  gate;
	}
private:
	double gate;
};

void reset(vector<double>&,int n);

static bool flagParameterChange = false;
static bool flagAgeChange = false;

#endif
