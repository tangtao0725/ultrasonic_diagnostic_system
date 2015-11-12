#ifndef MYMODEL_H
#define MYMODEL_H
#include <QtCore>
#include <QtGui>
#include <iostream>

class MyModel : public QStringListModel
{
public:
	MyModel(){}
	Qt::ItemFlags flags(const QModelIndex & index)const
	{
		return QStringListModel::flags(index)|Qt::ItemIsUserCheckable;
	}
	QVariant data(const QModelIndex& index, int role)const
	{
		if(role == Qt::CheckStateRole)
			return checkedIndexes.contains(index)?Qt::Checked:Qt::Unchecked;
		else
			return QStringListModel::data(index,role);
	}

	bool setData(const QModelIndex &index,const QVariant &value, int role)
	{
		if(role == Qt::CheckStateRole)
		{
			if(value == Qt::Checked)
			{
				checkedIndexes.insert(index);
			}
			else
			{
				checkedIndexes.remove(index);
			}
			return true;
		}
		return QStringListModel::setData(index,value,role);
	}

	QSet<QModelIndex> checkedIndexes;

	void AllSelect(bool flag,int sum)
	{
		if(flag)//if flag is true; then select all
		{
			for(int i = 0;i < sum;++i)
			{
				QModelIndex tmp = QStringListModel::index(i,0);
				setData(tmp,Qt::Checked,Qt::CheckStateRole);				
			}
		}	
		else//select none
		{
			for(int i = 0;i < sum;++i)
			{
				QModelIndex tmp = QStringListModel::index(i,0);
				setData(tmp,Qt::Unchecked,Qt::CheckStateRole);	
			}
		}
	}

private:
};
#endif
