/***************************************************************************
This file is used to generate the stand curve data for BC,AIB and SCS parameter
written by Wenbin Wang,2010-07-27
more comment need to be added!
*****************************************************************************/
#ifndef DATABASESTAND_H
#define DATABASESTAND_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>

class DatabaseStand:public QWidget
{
	Q_OBJECT
public:
	DatabaseStand(QWidget *parent = 0,Qt::WindowFlags flags = 0);
	QTableView *table;
	QLabel *label;
	QSqlTableModel *model;
	QPushButton *button_male;
	QPushButton *button_female;
	QPushButton *button_insert;
	QPushButton *button_delete;
	QPushButton *button_back;
	QPushButton *conform_modify;
public slots:
	void insertRecord();
	void deleteRecord();
	void conformModify();
	void display();
	void maleData();
	void femaleData();
};
#endif
