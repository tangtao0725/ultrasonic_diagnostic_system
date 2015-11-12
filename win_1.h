//win_1.h
/*written by Wang Wenbin.time:2010-7-3
This file is used to deal with the parameters input, and realize the function of input(ie. keyboard and Chinese input).waiting for improvement.
*/

#ifndef WIN_1_H
#define WIN_1_H

#include <QWidget>
#include <string>
#include <iostream>
#include "parameter.h"
using namespace std;


class QLabel;
class QPushButton;
class QSpinBox;
class QRadioButton;
class QDoubleSpinBox;
class QGroupBox;
class QLineEdit;


class Win_1:public QWidget
{
	Q_OBJECT
public:
	Win_1(QWidget *parent = 0,Qt::WindowFlags flags = 0);
public slots:
	void setPrm();
	void slotSaveParameterSuccess();
	void slotClearData();
signals:
	void button_conform_click(string name_tmp,string number_tmp,sex_bool sex_tmp,int height_tmp,double weight_tmp,int age_tmp);
	void signalBeginMeasure();
	
private:
	QGroupBox *box_top;

	QLabel *label_title;
	QLabel *label_name;
	QLabel *label_number;
	QLabel *label_sex;
	QLabel *label_height;
	QLabel *label_weight;
	QLabel *label_age;
	
	QLineEdit *edit_name;
	QLineEdit *edit_number;
	QRadioButton *radio_male;
	QRadioButton *radio_female;
	QSpinBox *spin_height;
	QSpinBox *spin_age;
	QDoubleSpinBox *doublespin_weight;

	QPushButton *button_conform;
	QPushButton *button_back;
};
#endif
