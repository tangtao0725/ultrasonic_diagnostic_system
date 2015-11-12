#include <QtGui>
#include <QTextCodec>
#include <QFont>

#include "win_1.h"

using namespace std;

Win_1::Win_1(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint)
{
	QFont font;
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

	font.setPointSize(80);
	label_title = new QLabel(this);
//	label_title->setGeometry(QRect(200,10,400,40));
	label_title->setGeometry(QRect(212,40,400,40));
	label_title->setText(QObject::tr("参数输入"));
	label_title->setFont(font);
	label_title->setAlignment(Qt::AlignHCenter);

	font.setPointSize(60);
	box_top = new QGroupBox(this);
//	box_top->setGeometry(QRect(20,40,760,180));
	box_top->setGeometry(QRect(32,70,760,180));
	box_top->setTitle(QObject::tr("个人信息输入:"));
	box_top->setFont(font);

	label_name = new QLabel(box_top);
	label_name->setGeometry(QRect(25,45,55,20));
	label_name->setText(QObject::tr("姓名:"));
	label_name->setAlignment(Qt::AlignRight);
	label_name->setFont(font);

	edit_name = new QLineEdit(box_top);
	edit_name->setGeometry(QRect(85,40,160,30));
	edit_name->setPalette(palette2);
	edit_name->setFont(font);
	
	label_number = new QLabel(box_top);
	label_number->setGeometry(QRect(25,90,55,20));
	label_number->setText(QObject::tr("编号:"));
	label_number->setAlignment(Qt::AlignRight);
	box_top->setFont(font);

	edit_number = new QLineEdit(box_top);
	QRegExp regExp("[A-Za-z0-9_.]{1,7}");
	edit_number->setValidator(new QRegExpValidator(regExp,this));
	edit_number->setGeometry(QRect(85,85,160,30));
	edit_number->setPalette(palette2);
	edit_number->setFont(font);

	label_sex = new QLabel(box_top);
	label_sex->setGeometry(QRect(25,135,55,20));
	label_sex->setText(QObject::tr("性别:"));
	label_sex->setFont(font);
	label_sex->setAlignment(Qt::AlignRight);
	
	radio_male = new QRadioButton(box_top);
	radio_male->setGeometry(QRect(90,135,55,20));
	radio_male->setText(QObject::tr("男"));
	radio_male->setChecked(true);
	radio_male->setPalette(palette2);
	radio_male->setFont(font);

	radio_female = new QRadioButton(box_top);
	radio_female->setGeometry(QRect(165,135,55,20));
	radio_female->setText(QObject::tr("女"));
	radio_female->setPalette(palette2);
	radio_female->setFont(font);

	label_height = new QLabel(box_top);
	label_height->setGeometry(QRect(270,45,100,25));
	label_height->setText(QObject::tr("身高(cm):"));
	label_height->setFont(font);
	label_height->setAlignment(Qt::AlignRight);

	spin_height = new QSpinBox(box_top);
	spin_height->setGeometry(QRect(380,40,80,30));
	spin_height->setMaximum(200);
	spin_height->setMinimum(0);
	spin_height->setValue(0);
	spin_height->setPalette(palette2);
	spin_height->setFont(font);

	label_weight = new QLabel(box_top);
	label_weight->setGeometry(QRect(270,90,100,25));
	label_weight->setText(QObject::tr("体重(kg):"));
	label_weight->setFont(font);
	label_weight->setAlignment(Qt::AlignRight);
	
	doublespin_weight = new QDoubleSpinBox(box_top);
	doublespin_weight->setGeometry(QRect(380,85,80,30));
	doublespin_weight->setMinimum(0);
	doublespin_weight->setValue(0);
	doublespin_weight->setPalette(palette2);
	doublespin_weight->setFont(font);

	label_age = new QLabel(box_top);
	label_age->setGeometry(QRect(270,135,100,25));
	label_age->setText(QObject::tr("年龄:"));
	label_age->setFont(font);
	label_age->setAlignment(Qt::AlignRight);

	spin_age = new QSpinBox(box_top);
	spin_age->setGeometry(QRect(380,130,80,30));
	spin_age->setMaximum(100);
	spin_age->setMinimum(0);
	spin_age->setValue(0);
	spin_age->setPalette(palette2);
	spin_age->setFont(font);

	button_conform = new QPushButton(this);
	button_conform->setGeometry(QRect(580,190,91,41));
	button_conform->setText(QObject::tr("下一步"));
	button_conform->setFont(font);
	button_conform->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C));
	QObject::connect(button_conform,SIGNAL(clicked()),this,SLOT(setPrm()));//“下一步“按钮按下，调用setprm函数
	
	button_back = new QPushButton(this);
	button_back->setGeometry(QRect(680,190,91,41));
	button_back->setText(QObject::tr("返回"));
	button_back->setFont(font);
	button_back->setShortcut(QKeySequence(Qt::ALT + Qt::Key_R));
	QObject::connect(button_back,SIGNAL(clicked()),this,SLOT(close()));
}
void Win_1::setPrm()
{
	QByteArray tmp = (edit_name->text()).toUtf8();
	string name_tmp = string(tmp);
	string number_tmp = (edit_number->text()).toStdString();
	sex_bool sex_tmp = radio_male->isChecked()==true?male:female;
	int height_tmp = spin_height->value();
	double weight_tmp = doublespin_weight->value();
	double age_tmp = spin_age->value();
//	std::cout << age_tmp << std::endl;
	if((name_tmp=="")||(number_tmp == "" )||(height_tmp == 0)||(weight_tmp == 0)||(age_tmp==0))
	{
		QMessageBox::warning(this,tr("错误"),tr("请输入完整的个人信息"),QMessageBox::Ok);
		return;
	}
	emit button_conform_click(name_tmp,number_tmp,sex_tmp,height_tmp,weight_tmp,age_tmp);
}

void Win_1::slotSaveParameterSuccess()
{
	emit signalBeginMeasure();
	//QMessageBox::warning(this,tr("提示"),tr("参数保存成功!"),QMessageBox::Ok);
}

void Win_1::slotClearData()
{
	edit_name->clear();
	edit_number->clear();
	radio_male->setChecked(true);
	spin_height->setValue(0);
	doublespin_weight->setValue(0);
	spin_age->setValue(0);
}
