#include <QtCore>
#include <QtGui>
#include <string>
#include <iostream>
using namespace std;
#include "myview.h"

MyView::MyView(QWidget* parent):QListView(parent)
{
	model = new MyModel;
	QDir dir("/usr/data");
	QStringList filters;
	filters << "*.txt";
	dir.setNameFilters(filters);
	list = dir.entryList();
	model->setStringList(list);
	setModel(model);

}

void MyView::getData()
{
	if(!selectionModel()->selectedIndexes().empty())
	{
		int index = selectedIndexes().first().row();
		QString filename = list.value(index);
		std::cout << filename.toStdString() << std::endl;
		std::cout << index << std::endl;
	}
}

void MyView::update()
{
	QDir dir("/usr/data");
	QStringList filters;
	filters << "*.txt";
	dir.setNameFilters(filters);
	list = dir.entryList();
	model->setStringList(list);
}

void MyView::copySelected()
{
	string name = "sd";
	string name1;
	emit signalShow(QObject::tr("正在检测优盘，请稍候"),Qt::green);
	sleep(3);
	QStringList filters;
	filters << "sd*";
	QDir dir("/dev");
	dir.setNameFilters(filters);
	dir.setFilter(QDir::System);
	QStringList filelist = dir.entryList();
	if(filelist.empty())
	{
		emit signalShow(QObject::tr("优盘未检测，请检查端口"),Qt::red);
		return;
	}
	else
	{
		emit signalShow(QObject::tr("正在复制，请稍候"),Qt::green);
		/*msgLabel->setText(QObject::tr("正在复制，请稍候"));
		QPalette pa;
		pa.setColor(QPalette::WindowText,Qt::green);
		msgLabel->setPalette(pa);
		msgLabel->show();
		QApplication::processEvents();
*/		
		string tmp = "mount /dev/" + filelist.first().toStdString() + " /mnt";
		system(tmp.c_str());
		system("sleep 1");

		for(QSet<QModelIndex>::iterator ptr = model->checkedIndexes.begin();ptr != model->checkedIndexes.end();++ptr)
		{
			int tmp = ptr->row();
			QList<QString>::iterator ptr1 = list.begin();
			for(int i = 0;i < tmp;++i)
			{
				++ptr1;
			}
			QString x = "cp -rf /usr/data/"+*ptr1 + " /mnt";
			std::cout << x.toStdString() << std::endl;
			system(x.toStdString().c_str());

                        QString y = "cp -rf /usr/data/"+*ptr1 + " /udisk";	//20140219 by 侯黎，增加U盘适用范围，将文件复制到udisk
			std::cout << y.toStdString() << std::endl;		//
			system(y.toStdString().c_str());			//
		}

		system("umount /mnt");
		system("sleep 0.2");
		emit signalShow(QObject::tr("数据复制成功"),Qt::green);
		sleep(1);
		emit signalClearShow();
	}
}

void MyView::deleteSelected()
{
	for(QSet<QModelIndex>::iterator ptr = model->checkedIndexes.begin();ptr != model->checkedIndexes.end();++ptr)
	{
		int tmp = ptr->row();
		QList<QString>::iterator ptr1 = list.begin();
		for(int i = 0;i < tmp;++i)
		{
			++ptr1;
		}
		QString x = "rm -rf /usr/data/"+*ptr1;
		system(x.toStdString().c_str());
	}
	update();
	model->checkedIndexes.clear();
}

void MyView::AllSelect(bool flag)
{
	if(flag)
	{
		model->AllSelect(true,list.count());
		update();
	}
}

void MyView::NoneSelect(bool flag)
{
	if(flag)
	{
		model->AllSelect(false,list.count());
		update();
	}
}

