#include <QtCore>
#include <QtGui>
#include "win_data.h"
#include "myview.h"

Win_data::Win_data(QWidget *parent,Qt::WindowFlags flags )
		:QWidget(parent,flags|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint)
{
	QFont font;
	font.setFamily(("simsun"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	//set the font and encode

	view = new MyView(this);	
	view->setGeometry(10,10,280,380);

	radioButton_allSelect = new QRadioButton(this);
	radioButton_noneSelect = new QRadioButton(this);
	radioButton_allSelect->setText(QObject::tr("全选"));
	radioButton_noneSelect->setText(QObject::tr("全不选"));
	radioButton_allSelect->setGeometry(300,10,80,30);
	radioButton_noneSelect->setGeometry(300,50,80,30);

	button_delete = new QPushButton(this);
	button_delete->setText(QObject::tr("删除"));
	button_delete->setGeometry(300,90,80,30);
	button_copy = new QPushButton(this);
	button_copy->setText(QObject::tr("导出"));
	button_copy->setGeometry(300,130,80,30);
	button_close = new QPushButton(this);
	button_close->setText(QObject::tr("退出"));
	button_close->setGeometry(300,170,80,30);

	msgLabel = new QLabel(this);
	msgLabel->setGeometry(10,150,300,80);
	msgLabel->setAlignment(Qt::AlignHCenter);


	QObject::connect(button_delete,SIGNAL(clicked()),this,SLOT(deleteSelect()));
	QObject::connect(button_copy,SIGNAL(clicked()),view,SLOT(copySelected()));
	QObject::connect(button_close,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(radioButton_allSelect,SIGNAL(toggled(bool)),view,SLOT(AllSelect(bool)));
	QObject::connect(radioButton_noneSelect,SIGNAL(toggled(bool)),view,SLOT(NoneSelect(bool)));
	QObject::connect(view,SIGNAL(signalShow(QString,Qt::GlobalColor)),this,SLOT(slotShow(QString,Qt::GlobalColor)));
	QObject::connect(view,SIGNAL(signalClearShow()),this,SLOT(slotClearShow()));

}

void Win_data::update()
{
	radioButton_noneSelect->setChecked(true);
	view->NoneSelect(true);
	view->update();
}

void Win_data::slotShow(QString text,Qt::GlobalColor color)
{
	msgLabel->setWindowFlags(Qt::WindowStaysOnTopHint);
	msgLabel->setText(text);
	QPalette pa;
	pa.setColor(QPalette::WindowText,color);
	msgLabel->show();
	QApplication::processEvents();
}

void Win_data::slotClearShow()
{
	msgLabel->hide();
	QApplication::processEvents();
}

void Win_data::deleteSelect()
{
	int tmp = QMessageBox::warning(this,tr("警告"),tr("您确认删除选中文件？"),QMessageBox::Yes,QMessageBox::No);
	if(tmp == QMessageBox::Yes)
 	{
  		view->deleteSelected();
	}
}
