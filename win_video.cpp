#include <QtGui>

#include "win_video.h"
#include "player.h"

Win_video::Win_video(QWidget *parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint)
{
	resize(600,410);

	playButton = new QPushButton(QIcon(tr(":/images/play.png")),tr(""),this);
	playButton->setGeometry(100,350,80,60);
	connect(playButton,SIGNAL(clicked()),this,SLOT(slotPlay()));

	stopButton = new QPushButton(QIcon(tr(":/images/stop.png")),tr(""),this);
	stopButton->setGeometry(400,350,80,60);
	connect(stopButton,SIGNAL(clicked()),this,SLOT(slotStop()));

	player = new Player();
	player->setGeometry(10,10,580,350);	
}

Win_video::~Win_video()
{
}

void Win_video::slotPlay()
{
	player->play("/mplayer/bin/test.AVI");
}

void Win_video::slotStop()
{
	player->controlCmd("quit\n");
}
