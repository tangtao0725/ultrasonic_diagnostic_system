#include <QtGui>
#include <QtCore>
 #include "filelistview.h"
 #include <iostream>
 using namespace std;

 FilelistView::FilelistView(QWidget *parent,Qt::WindowFlags flags)
     : QWidget(parent,flags|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint)
 {
     model = new FileListModel(this);
     model->setDirPath(QLibraryInfo::location(QLibraryInfo::PrefixPath));
	 
	 model->setDirPath(QString("/usr/data"));

	 QFont font;
	 font.setFamily(("simsun"));
	 QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

     QLabel *label = new QLabel(tr("&Directory:"));
	 QPushButton *button = new QPushButton(tr("确定"));
	 QPushButton *button1 = new QPushButton(tr("退出"));

     view = new QListView;
     view->setModel(model);
	 //view->setViewMode(QListView::IconMode);
	 
     QGridLayout *layout = new QGridLayout;
     layout->addWidget(label, 0, 0);
     layout->addWidget(view, 1, 0, 1, 2);
	 layout->addWidget(button,2,0,1,1);
	 layout->addWidget(button1,2,1,1,1);

     setLayout(layout);
     setWindowTitle(tr("Fetch More Example"));
	 
	 QObject::connect(button,SIGNAL(clicked()),this,SLOT(getData()));
	 QObject::connect(button1,SIGNAL(clicked()),this,SLOT(close()));
 }
 
 void FilelistView::getData()
 {
	 
	if(!view->selectionModel()->selectedIndexes().empty())
	{
		int index = view->selectionModel()->selectedIndexes().first().row();
		filename = model->fileList.value(index);
		emit signalFilename(filename);
		close();
	}
 }

void FilelistView::fileFresh()
{
}
