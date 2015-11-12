#ifndef FILELISTMODEL_H
 #define FILELISTMODEL_H

 #include <QAbstractListModel>
 #include <QList>
 #include <QStringList>

 class FileListModel : public QAbstractListModel
 {
     Q_OBJECT

 public:
     FileListModel(QObject *parent = 0);

     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	 QStringList fileList;

 signals:
     void numberPopulated(int number);

 public slots:
     void setDirPath(const QString &path);

 protected:
     bool canFetchMore(const QModelIndex &parent) const;
     void fetchMore(const QModelIndex &parent);

 private:   
     int fileCount;
 };

 #endif