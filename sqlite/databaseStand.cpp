#include <QTextCodec>
#include <QObject>
#include <QMessageBox>
#include <QSqlError>
#include "databaseStand.h"
#include <iostream>

DatabaseStand::DatabaseStand(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint)
{
	QFont font;
	font.setFamily(("simsun"));
	font.setPointSize(30);

	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("/usr/local/data/data.db");
	db.open();
	QSqlQuery query;
	query.exec(QObject::tr("create table if not exists maleDatabase(年龄 INTEGER PRIMARY KEY AUTOINCREMENT,BC参数 REAL,AIB参数 REAL,SMS参数 REAL)"));
	query.exec(QObject::tr("insert into maleDatabase values(1,0,0,0)"));
	query.exec(QObject::tr("create table if not exists femaleDatabase(年龄 INTEGER PRIMARY KEY AUTOINCREMENT,BC参数 REAL,AIB参数 REAL,SMS参数 REAL)"));
	query.exec(QObject::tr("insert into femaleDatabase values(1,0,0,0)"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	table = new QTableView(this);
	table->setGeometry(30,90,361,351);
	model = new QSqlTableModel(this);
	model->setTable("maleDatabase");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setFilter(QObject::tr("年龄 > 0"));
	model->select();
	table->setModel(model);
	
	label = new QLabel(this);
	label->setGeometry(240,20,201,51);
	font.setPointSize(15);
	label->setText(QObject::tr("标注曲线数据库"));
	label->setFont(font);
	
	button_male = new QPushButton(this);
	button_male->setGeometry(430,100,50,30);
	button_male->setText(QObject::tr("男"));
	button_male->setFont(font);
	
	button_female = new QPushButton(this);
	button_female->setGeometry(490,100,50,30);
	button_female->setText(QObject::tr("女"));
	button_female->setFont(font);
	
	button_insert = new QPushButton(this);
	button_insert->setGeometry(430,140,111,31);
	button_insert->setText(QObject::tr("插入记录"));
	button_insert->setFont(font);
	
	button_delete = new QPushButton(this);
	button_delete->setGeometry(430,180,111,31);
	button_delete->setText(QObject::tr("删除所在行"));
	button_delete->setFont(font);
	
	conform_modify = new QPushButton(this);
	conform_modify->setGeometry(430,220,111,31);
	conform_modify->setText(QObject::tr("确认修改"));
	conform_modify->setFont(font);
	
	button_back = new QPushButton(this);
	button_back->setGeometry(430,400,111,31);
	button_back->setText(QObject::tr("返回"));
	button_back->setFont(font);

	QObject::connect(button_back,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(button_insert,SIGNAL(clicked()),this,SLOT(insertRecord()));
	QObject::connect(button_delete,SIGNAL(clicked()),this,SLOT(deleteRecord()));
	QObject::connect(conform_modify,SIGNAL(clicked()),this,SLOT(conformModify()));
	QObject::connect(button_male,SIGNAL(clicked()),this,SLOT(maleData()));
	QObject::connect(button_female,SIGNAL(clicked()),this,SLOT(femaleData()));
}

void DatabaseStand::insertRecord()
{
	int curRow = table->currentIndex().row();
	bool ok = model->insertRow(curRow);
	if(!ok)
	{
		std::cout << model->lastError().text().toStdString() << std::endl;
	}
	model->submitAll();
}

void DatabaseStand::deleteRecord()
{
	int curRow = table->currentIndex().row();
	bool tmp  = model->removeRow(curRow);
	if(!tmp)
	{
		std::cout << model->lastError().text().toStdString() << std::endl;
	}

	if(model->rowCount() == 1)
	{
		QMessageBox::warning(this,tr("出错！"),tr("这已经是最后一组数据了"));
		return;
	}

	int ok = QMessageBox::warning(this,tr("删除当前行！"),tr("你确定删除当前行吗?"),QMessageBox::Yes,QMessageBox::No);
	if(ok == QMessageBox::No)
	{
		model->revertAll();
	}
	else
	{
		model->submitAll();
	}
}

void DatabaseStand::conformModify()
{
	model->database().transaction();
	if(model->submitAll()){
		model->database().commit();
	}
	else{
		model->database().rollback();
		QMessageBox::warning(this,tr("tableModel"),
				tr("数据库错误:%1").arg(model->lastError().text()));
	}
}

void DatabaseStand::display()
{
	model->setFilter(QObject::tr("年龄 > 0"));
	model->select();
	table->setModel(model);
	showMaximized();
}

void DatabaseStand::maleData()
{
	model->setTable("maleDatabase");
	model->setFilter(QObject::tr("年龄 > 0"));
	model->select();
	display();
}

void DatabaseStand::femaleData()
{
	model->setTable("femaleDatabase");
	model->setFilter(QObject::tr("年龄 > 0"));
	model->select();
	display();
}
