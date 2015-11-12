#ifndef WIN_VIDEO_H
#define WIN_VIDEO_H

#include <QWidget>
#include <QProcess>

class QPushButton;
class QProcess;

class Player;

class Win_video : public QWidget
{
	Q_OBJECT

public:
	Win_video(QWidget *parent = 0,Qt::WindowFlags flags = 0);
	~Win_video();

public slots:
	void slotPlay();
	void slotStop();

private:
	Player *player;

	QPushButton *playButton;
	QPushButton *stopButton;

};
#endif
