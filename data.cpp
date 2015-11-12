#include "data.h"
#include "parameter.h"
#include "thread.h"
#include <algorithm>
#include <functional>
#include <QMessageBox>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <map>
#include <sstream>
#include "sqlite3.h"
using namespace std;
Data::Data()
{
	sex = male;
	height = 0;
	weight = 0.0;
	age = 0;
	
	prmBC = 0.0;
	prmAIB = 0.0;
	prmSCS = 0.0;

	prmBCLast = 0.0;
	prmAIBLast = 0.0;
	prmSCSLast = 0.0;
	
	udFeq = 1.0;
	calculate_spectrum_ref(udFeq);

	QObject::connect(&thread,SIGNAL(signalThreadData(const vector<double>)),this,SLOT(slotThreadData(const vector<double>)),Qt::DirectConnection);

}

void Data::keyToSure(string name_tmp,string number_tmp,sex_bool sex_tmp,int height_tmp,double weight_tmp,int age_tmp)	//接收来至Win_1(参数输入)输入的的"个人信息"
{
	name = name_tmp;
	number = number_tmp;
	sex = sex_tmp,
	height = height_tmp;
	weight = weight_tmp;
	age = age_tmp;
	prmBC = 0.0;
	prmAIB = 0.0;
	prmSCS = 0.0;
	if(age > 0)
		flagAgeChange = true;
	emit signalSaveParameterSuccess();
}


void Data::slotThreadData(const vector<double> lhs)	//接收来至“Thread”中的时域信号，进行处理
{

	data.clear();
	data = vector<double>(lhs.begin(),lhs.begin()+2048);//.toStdVector();
	emit signalPassWaveform(data);	//见main.cpp，将收集到的时域波形传递给Win_2进行显示
	struct boneParameter tmp = parameter_calculate(data,udFeq,spectrum);	//相关参数计算的核心部分，见parameter.cpp
	prmBC = tmp.BC;
	prmAIB = tmp.AIB;
	prmSCS = tmp.SCS;
	if((abs(prmAIB-prmAIBLast) < 0.02*abs(prmAIB)))// && (abs(prmSCS-prmSCSLast) < 0.1*abs(prmSCS)))
	{
		prmAIBLast = 0.0;
		prmBCLast = 0.0;
		prmSCSLast = 0.0;
		/*add for test, need to remove*/  	//20120621 by wangwenbin
		/*prmAIB = -9+(int)(3.0*rand()/(RAND_MAX+1.0));
		prmBC = -15.02+(int)(4.0*rand()/(RAND_MAX+1.0));
		prmSCS = -430+(int)((30.0*rand())/(RAND_MAX+1.0));*/
		//-----------------------------
		thread.stop();
		usleep(100000);
		flagParameterChange = true;
		emit(signalAutoStop());
	}

	prmAIBLast = prmAIB;
	prmBCLast = prmBC;
	prmSCSLast = prmSCS;
	std::cout << "prmBC\t" << prmBC << std::endl;
	std::cout << "prmAIB\t" << prmAIB << std::endl;
	std::cout << "prmSCS\t" << prmSCS << std::endl;
}

void Data::slotShowSpectrum()
{
	emit signalPassSpectrum(spectrum);
}

void Data::slotPassFileName(const QString& fileName)
{
	reset(data,2048);
	spectrum.clear();
	string filepath = "/usr/data/"+fileName.toStdString();
	ifstream fin(filepath.c_str());
	if(!fin.is_open())
		cout << "fileName is invalid" << endl;
	string tmp;
	int x = 0;
	double tmp1,tmp2;
	vector<double>::iterator item = data.begin();
	while(!fin.eof())
	{
		getline(fin,tmp);
		if(fin.fail())
			break;
		++x;
		if(x > 10 && x < 2059)
		{
			stringstream stream;
			stream << tmp;
			stream >> tmp1 >> tmp2;
			*item = tmp2;
			item++;
		}
	}
	emit signalPassWaveform(data);
	struct boneParameter prm = parameter_calculate(data,udFeq,spectrum);
	prmBC = prm.BC;
	prmAIB = prm.AIB;
	prmSCS = prm.SCS;
}

void Data::slotClearData()
{
	data.clear();
	reset(spectrum,2048);
}

void Data::slotSaveData()
{
	if(number == "")
	{
		emit signalParameterSaveError();
		return;
	}
	else
	{

		//save the raw data
		stringstream ss;
		ss << udFeq;
		string tmp;
		ss >> tmp;
		string fileName = "/usr/data/"+number+"-"+ tmp +"M.txt";
		ofstream fout(fileName.c_str());
		fout << "个人信息" << "\n";
		fout << "姓名:\t" << name << "\n";
		fout << "编号:\t" << number << "\n";
		fout << "性别:\t" << sex << "\n";
		fout << "年龄:\t" << age << "\n";
		fout << "身高:\t" << height << "\n";
		fout << "体重:\t" << weight << "\n";
		fout << "探头中心频率" << udFeq << "\n";
		fout << "\n\n";
		fout << "信号数据(raw data)" << "\n";
		int i = 0;
		for(vector<double>::iterator item = data.begin();item != data.end(); ++item)
		{
			fout << i/32000000.0 << "\t" << *item << "\n";
			++i;
		}
		fout << endl;

		//save parameter
		ofstream fout1("/usr/data/result.txt",ios::app);
		fout1 << "姓名:\t" << name << "\n";
		fout1 << "编号:\t" << number << "\n";
		fout1 << "性别:\t" << sex << "\n";
		fout1 << "年龄:\t" << age << "\n";
		fout1 << "身高:\t" << height << "\n";
		fout1 << "体重:\t" << weight << "\n";
		fout1 << "BC参数:" << prmBC << "AIB参数:\t" << prmAIB << "SCS参数:\t" << prmSCS;
		fout1 << "\n\n";
		fout1 << endl;

		emit signalSaveDataSuccess();
	}
}

void Data::beginMeasure(bool isDymcDsp,double feq)
{
	if(abs(udFeq - feq) > 1E-6)
	{
		udFeq = feq;	//设置超声探头发射频率	
		calculate_spectrum_ref(udFeq);
	}
	if(isDymcDsp)
	{
		data.clear();
		reset(spectrum,2048);
		thread.begin();
		thread.setFeq(udFeq);	//设置探头的中心频率
		thread.start();
	}
	else
	{
		thread.stop();
		usleep(100000);
		flagParameterChange = true;
	}

}

void Data::slotEndMeasure()
{
	spectrum.clear();
	thread.stop();
}

void Data::getPersonalImfoACK()
{
	emit sendPersonalImfo(*this);
}

void Data::slotImportResult()
{
	if(!(flagParameterChange&flagAgeChange))
	{
		flagParameterChange = false;
		emit signalRepeatImportError();
		return;
	}
	static int id = 1;
	char * errMsg;
	sqlite3 * pdb;
	int res = sqlite3_open("/usr/local/data/result.db",&pdb);
	if(res){
		std::cout << "Can't open database:" << sqlite3_errmsg(pdb);
		sqlite3_close(pdb);
	}
	res = sqlite3_exec(pdb,"create table if not exists maletable(age integer ,parameterBC real,parameterAIB real,parameterSCS real)",NULL,NULL,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "create error" << std::endl;
		sqlite3_close(pdb);
	}
	char s[100];
	memset(s,'\0',sizeof(s));

	sprintf(s,"insert into maletable values(%d,%f,%f,%f)",age,prmBC,prmAIB,prmSCS);
	res = sqlite3_exec(pdb,s,NULL,NULL,&errMsg);
	if(res != SQLITE_OK)
	{
		std::cout << "insert error" << std::endl;
		sqlite3_close(pdb);
	}
	id += 1;
	flagParameterChange = false;
	emit signalImportSuccess();
}
/*
void Data::readData()
{
}

void Data::fft()
{
}

void displayData()
{}

void displaySpectrum()
{}

void displayBC()
{}

void displayAIB()
{}

void displaySCS()
{}
*/

void reset(vector<double>& data,int n)
{
	data.clear();
	data.insert(data.begin(),n,0);
}

