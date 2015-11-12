#include "parameter.h"
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <deque>
#include <map>
using namespace std;

static vector<double> spectrum_ref;	//用于存放参考信号的频谱

void calculate_spectrum_ref(double feq)
{
	spectrum_ref.clear();
	ifstream fin;
	if(abs(feq-1.0)<1E-6)
	{
		fin.open("/usr/local/reference/100.txt");
	}
	else if(abs(feq-2.25)<1E-6)
	{
		fin.open("/usr/local/reference/225.txt");
	}
	else if(abs(feq-3.5)<1E-6)
	{
		fin.open("/usr/local/reference/350.txt");
	}
	else if(abs(feq-5.0)<1E-6)
	{
		fin.open("/usr/local/reference/500.txt");
	}
	else
	{
		cout << "open file error" << endl;
	}
	if(!fin.is_open())
		return;
	compx reference[sampleNum];//for loading the referecnce data
	double tmp;
	int i = 0;
	while(!fin.eof())
	{
		fin >> tmp;
		if(fin.fail())
			break;
		reference[i].real = tmp;
		reference[i].img = 0;
		++i;
	}
	for(int j = i;j < sampleNum;++j)
	{
		reference[j].real = tmp;
		reference[j].img = 0;	
	}
	fft(reference,sampleNum);
	for(int k = 0; k < sampleNum/2;++k)
	{
		double tmp = reference[k].abs();
		spectrum_ref.push_back(tmp);
	}

}

void fft(compx *s,unsigned long N)
{
	if(N&(N-1)!=0)
	{
		exit(0);
	}
	long unsigned int f,m,LH,nm,i,k,j,L;
	double p,ps;
	long int le,B,ip;
	compx v,w,t;
	LH=N/2;
	f=N;
	for(m=1;(f=f/2)!=1;m++){;}
	nm=N-2;	
	j=N/2;
	//变址运算
	for(i=1;i<=nm;i++)
	{
		if(i<j){t=s[j];s[j]=s[i];s[i]=t;}
		k=LH;
		while(j>=k){j=j-k;k=k/2;}
		j=j+k;
	}
	for(L=1;L<=m;L++)
	{
		le=pow(2,L);
		B=le/2;
		for(j=0;j<=B-1;j++)
		{
			p=pow(2,m-L)*j;
			ps=2*pi/N*p;
			w.real=cos(ps);
			w.img=-sin(ps);
			for(i=j;i<=N-1;i=i+le)
			{
				ip=i+B;
				t=s[ip]*w;
				s[ip].real=s[i].real-t.real;
				s[ip].img=s[i].img-t.img;
				s[i].real=s[i].real+t.real;
				s[i].img=s[i].img+t.img;
			}
		}
	}
	return;
}
//this is IIR filter of butterworth, but there is something wrong that it dosn't work
vector<double> filter_IIR(const vector<double>& input,const vector<long double>& a,const vector<long double>& b)
{
	deque<long double> data1(input.begin(),input.end());
	int size = data1.size();
	int size1 = a.size();
	int size2 = b.size();
	for(int i = 0;i < size2-1;++i)
		data1.push_front(0);
	vector<long double> dataOut(size1-1,0);
	for(int j = 0;j < size;++j)
	{
		long double tmp1 = 0;
		long double tmp2 = 0;
		for(int k = 0;k < size1;++k)
		{
			tmp1 += data1[j+k]*b[size1-1-k];
		}
		for(int n = 0;n < size2 - 1;++n)
		{
			tmp2 += dataOut[j+n]*a[size2-1-n];
		}
		long double tmp = tmp1 - tmp2;
		dataOut.push_back(tmp);
	}
	vector<double> dataFilter(dataOut.begin()+size1-1,dataOut.end());
	return dataFilter;
}

//FIR滤波器
vector<double> filter_FIR(vector<double>& input,vector<double>& filterCoef)
{
	deque<double> tmp(input.begin(),input.end());
	int len_input = input.size();
	input.clear();
	int len = filterCoef.size() - 1;
	for(int i = 0;i < len;++i)
	{
		tmp.push_front(0);
		tmp.push_back(0);
	}
	for(deque<double>::iterator item = tmp.begin();item != (tmp.end()-len);++item)
	{
		double tmp = 0;
		deque<double>::iterator item1 = item;
		for(vector<double>::reverse_iterator item2 = filterCoef.rbegin();item2 != filterCoef.rend();++item2)
		{
			tmp += (*item1)*(*item2);
			++item1;
		}
		input.push_back(tmp);
	}
	return vector<double>(input.begin(),input.begin()+len_input);
}

bool absLess(double elem1,double elem2)
{
	return abs(elem1) < abs(elem2);
}

struct boneParameter parameter_calculate(vector<double>& data,double udFeq,vector<double>& spectrum)
{
	spectrum.clear();
	vector<double> bcPrm;
	vector<double>::iterator ptrToSignal = data.begin();
	//iterator points to the start of the data;	
	int offSet = 60;
	//set the 60th point of the data as the offSet of signal;	//设置数据起始位
	ptrToSignal += offSet;
	//remove the effect of offset

	double sampleGap = int(2*sampleFeq/vel*deep);
	//calculate the points in the signal that denotes twice the deepth of soft tissue and cortical bone;
	ptrToSignal += sampleGap;
	//set the iterator to the start of the effect signal.

	compx sample[sampleNum];
	double hamming[sampleNum/32];	//用于存储hamming窗
	//下面的for循环制造hamming窗,2012.07.20
	//***************************************************
	for (int m=0; m<sampleNum/32; m++)
	{
		hamming[m]=0.54-0.46*cos(2*pi*m/(sampleNum/32));
	}
	//***************************************************
	for(unsigned long i = 0; i < sampleNum; ++i)	//下面的for循环截取有效信号
	{
		if(i < sampleNum/32)
		{
			sample[i].real = *ptrToSignal;
			sample[i].real = sample[i].real*hamming[i];	//2012.07.20,乘以hanming窗
			sample[i].img = 0;
			++ptrToSignal;
			if(ptrToSignal == data.end())
				break;
		}
		else
		{
			sample[i].real = 0;
			sample[i].img = 0;
		}
	}
	//construct an array of complex number and fill it with effect signal, the real part the signal, and the img part is 0.
	fft(sample,sampleNum);
	//Do fft of the effect signal to get the spectrum.	//做有效信号的fft，求得频谱
	for(unsigned long j = 0; j < sampleNum/2;++j)
	{
		double tmp = sample[j].abs();
		spectrum.push_back(tmp);
	}
	//because the spectrum of signal is complex, calculate the absolute value to get the amptitude.	//取绝对值，得幅度谱
	/*
	for(vector<double>::iterator ptrToSpectrum = spectrum.begin();ptrToSpectrum != spectrum.end(); ++ptrToSpectrum)
	{
		if(ptrToSpectrum + 8 != spectrum.end())
		{
			double tmp = 0;
			for(int i = 0; i < 8;++i)
			{
				tmp += *(ptrToSpectrum + i);
			}
			*ptrToSpectrum = tmp/8.0;
		}	
	}
	*/
	//smooth the spectrum by averaging every point with the next 7 points.

	int indexMin = int(freq_info[udFeq].f_min/sampleFeq*sampleNum);
	int indexMax = int(freq_info[udFeq].f_max/sampleFeq*sampleNum);
	//specifies the index of the minimum and maximum of the frequency range in the known spectrum.
	int indexFeq = int(udFeq/sampleFeq*sampleNum);
	//specifies the index of the central frequency in the known spectrum.

	vector<double>::iterator ptrToRef = spectrum_ref.begin();	//参考信号频谱的指针
	for(vector<double>::iterator ptrToSpectrum = spectrum.begin();ptrToSpectrum != spectrum.end();++ptrToSpectrum,++ptrToRef)
	{
		double tmp = 8.68*log((*ptrToSpectrum)/(*ptrToRef));
		bcPrm.push_back(tmp);
	}
	//use the spectrum of signal and reference signal to calculate the BC parameter and save it in bcPrm.
	double prmBC = *(bcPrm.begin() + indexFeq);
	//get the BC parameter at the central frequency
	vector<double>::iterator ptrToBcprmMin = bcPrm.begin()+indexMin;
	vector<double>::iterator ptrToBcprmMax = bcPrm.begin()+indexMax;
	//set iterator to point the minimum and maximum of the freqency range in the bcPrm as we will use them in calculating AIB
	vector<double>::iterator ptrToSpecMin = spectrum.begin()+indexMin;
	vector<double>::iterator ptrToSpecMax = spectrum.begin()+indexMax;
	//set iterator to point the minimum and maximum of the frequency range in the spectrum as we will use them in calculating SCS.
	double sumAIB = 0;
	double sumSCS1 = 0;
	double sumSCS2 = 0;
	double curFeq = freq_info[udFeq].f_min;
	for(vector<double>::iterator ptrToBcprm = ptrToBcprmMin;ptrToBcprm <= ptrToBcprmMax;++ptrToBcprm)
	{
		sumAIB += *ptrToBcprm;
	}
	//AIB is the average spectrum value in frequency range, so add every point first and then devides the point number.	//计算带宽范围内的BC之和

	for(vector<double>::iterator ptrToSpectrum = ptrToSpecMin;ptrToSpectrum <= ptrToSpecMax;++ptrToSpectrum)
	{
		sumSCS1 += *ptrToSpectrum * curFeq;
		sumSCS2 += *ptrToSpectrum;
		curFeq += 32.0/sampleNum;
	}
	//SCS is the mass point bias of the spectrum, so multiply the freqency with spectrum value and then devide with the spectrum sum to get the frequency value.
	double prmAIB = sumAIB/(indexMax-indexMin);
	double prmSCS = 1000*(freq_info[udFeq].f_center - sumSCS1/sumSCS2)-450;   //20131104 by houli	"-450"!!!!
	return boneParameter(prmBC,prmAIB,prmSCS);
}
