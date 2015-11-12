/*written by Wang Wenbin,time:2010-7-3
  This file is used to choose the ultrasouic frequency and display the signal and spectrum. waiting for improvement!!
*/

#ifndef WIN_2_H
#define WIN_2_H

class QGroupBox;
class QRadioButton;
class QPushButton;
class QLabel;
class Plotter;
class QToolButton;
class QCheckBox;

#include <QWidget>
#include <iostream>
#include <vector>
#include "filelistview.h"
using namespace std;

class Win_2:public QWidget
{
	Q_OBJECT
public:
	Win_2(QWidget *parent = 0,Qt::WindowFlags flags = 0);
signals:
	void signalShowSpectrum();
	void signalClearData();
	void signalSaveData();
	void signalPassFileName(const QString&);
	void signalBeginMeasure(bool,double);
	void signalEndMeasure();
	void signalFilter(bool);
	void signalQuit();
public slots:
	void slotPassWaveform(const vector<double>&);
	void slotPassSpectrum(const vector<double>&);
	void show();
	void slotSignalZoomIn();
	void slotSpectrumZoomIn();
	void slotSignalZoomOut();
	void slotSpectrumZoomOut();
	void slotLoadData();
	void slotAutoStop();
	void slotSaveDataSuccess();
	void slotBeginMeasure();
	void slotEndMeasure();
	void slotParameterSaveError();
	void slotShowSpectrum();
	void slotSaveData();
private:
	QGroupBox *groupBox;
	QRadioButton *radioButton;//1.0MHz
	QRadioButton *radioButton_1;//2.25MHz
	QRadioButton *radioButton_2;//3.5MHz
	QRadioButton *radioButton_3;//5.0MHz
	QRadioButton *radioButton_4;//6.0MHz
	QPushButton *button;
	QPushButton *button_1;
	QPushButton *button_2;
	QPushButton *button_3;
	QPushButton *button_4;
	Plotter *plotter_1;
	Plotter *plotter_2;
	QCheckBox *checkBox;
	bool isDynamicDisplay;	//是否动态显示标志
	vector<double> Coef;
	FilelistView *filelist;
};
#endif
