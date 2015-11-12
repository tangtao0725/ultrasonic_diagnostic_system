/*Written by Wang Wenbin,2010.7.3
  This file is to define some parameter used in the programmer, you can adjust them by yourself.
  */
#ifndef PARAMETER_H
#define PARAMETER_H
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

const double pi = 3.1415926;
const unsigned long sampleNum = 512 * 32;//采样点个数
const double vel = 1.5;//超声传播速度
const double deep = 8;//表层皮质骨厚度
const int displayPoint = 512;
const int len = 2048;
const int sampleFeq = 32.0;
const double gate = 0.1;
const string dataPath = "/usr/local/data/data.txt";
const string driverFilePath = "/dev/boneDensityDevice";
enum sex_bool{male = 0,female = 1};
enum curve{BC = 0,AIB = 1,SCS = 2};

struct boneParameter
{
	boneParameter(double lhs,double mhs,double rhs):BC(lhs),AIB(mhs),SCS(rhs){}
	double BC;
	double AIB;
	double SCS;
};

struct compx
{
	compx(double real_input = 0,double img_input = 0):real(real_input),img(img_input){}
	double real;
	double img;

	friend compx operator *(const compx &lhs,const compx &rhs)
	{
		compx tmp;
		tmp.real = lhs.real*rhs.real-lhs.img*rhs.img;
		tmp.img = lhs.real*rhs.img+lhs.img*rhs.real;
		return tmp;
	}

	friend ostream& operator <<(ostream & os,const compx & rhs)
	{
		os << rhs.real << "+" << rhs.img << "j";
		return os;
	}

	double abs()
	{
		double tmp;
		tmp = sqrt(real*real+img*img);
		return tmp;
	}
};//定义复数数据结构和复数乘法
typedef struct frequence_range
{
	frequence_range(double lhs = 0,double mhs = 0,double rhs = 0):f_min(lhs),f_max(mhs),f_center(rhs){}
	double f_min;
	double f_max;
	double f_center;
}frequence_range;
static map<double,frequence_range> _init_static_list()
{
	map<double,frequence_range> ret;
//	1号板
	ret[1.0] = frequence_range(0.6758,1.138,0.9061);	
	ret[2.25] = frequence_range(1.935,2.953,2.4393);
	ret[3.5] = frequence_range(2.748,3.792,3.2664);
	ret[5.0] = frequence_range(3.581,4.838,4.2034);
	ret[6.0] = frequence_range(3.5,8.10,6.0);
	

//	2号板 
/*	ret[1.0] = frequence_range(0.6914,1.128,0.9094);
	ret[2.25] = frequence_range(1.377,2.947,2.2034);
	ret[3.5] = frequence_range(2.511,3.57,3.0339);
	ret[5.0] = frequence_range(3.417,5.646,4.4112);
	ret[6.0] = frequence_range(3.5,8.10,6.0);
*/	


/*
//3号板

	ret[1.0] = frequence_range(0.679,1.051,0.863);	
	ret[2.25] = frequence_range(1.89,2.687,2.2859);
	ret[3.5] = frequence_range(2.65,3.652,3.1468);
	ret[5.0] = frequence_range(3.332,5.924,4.6242);
	ret[6.0] = frequence_range(3.5,8.10,6.0);


*/	
//4号板	
/*
	ret[1.0] = frequence_range(0.667,1.085,0.8758);	
	ret[2.25] = frequence_range(1.35,2.979,2.1866);
	ret[3.5] = frequence_range(2.46,3.533,2.9943);
	ret[5.0] = frequence_range(3.055,5.788,4.2617);
	ret[6.0] = frequence_range(3.5,8.10,6.0);


*/	
	return ret;

};
static map<double,frequence_range> freq_info = _init_static_list();
extern void debug(const char*);
extern void fft(compx*,unsigned long);
extern vector<double> filter_IIR(const vector<double>& input,const vector<long double>& a,const vector<long double>& b);
extern vector<double> filter_FIR(vector<double>&,vector<double>&);
extern bool absLess(double elem1,double elem2);
extern struct boneParameter parameter_calculate(vector<double>& data,double,vector<double>& spectrum);
extern void calculate_spectrum_ref(double feq);
#endif
