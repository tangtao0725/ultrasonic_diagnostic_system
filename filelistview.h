 #ifndef FILELISTVIEW_H
 #define FILELISTVIEW_H

 #include <QWidget>
 #include <QtGui>
 #include "filelistmodel.h"

 class QTextBrowser;

 class FilelistView : public QWidget
 {
     Q_OBJECT

 public:
     FilelistView(QWidget *parent = 0,Qt::WindowFlags flags = 0);
	 QListView *view;
	 FileListModel *model;
	 void fileFresh();

signals:
	 void signalFilename(const QString&);

 public slots:
	 void getData();
 private:
 	QString filename;
 };

 #endif
