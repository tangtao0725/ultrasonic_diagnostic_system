#include <QtGui>

#include "player.h"

Player::Player(QWidget * parent)
	: QWidget(parent)
{
	renderTarget = new QWidget(this);
	renderTarget->setAttribute(Qt::WA_OpaquePaintEvent);

	mplayerProcess = new QProcess(this);
	
	/*
	QStringList args;
	args << tr("-slave");
	args << "-quiet";
	args << "-zoom";
	args << tr("-wid") << QString::number(renderTarget->winId());
	args << tr("/home/hdc/视频/02-112009中国香港动作大片风云2 风云2DVD-RMVB国语中字 1024x434高清晰 无水印版.rmvb");
	mplayerProcess->start(tr("/usr/bin/mplayer"), args);
	*/
}

Player::~Player()
{}
	
void Player::play(const QString &fileName)
{
	QStringList args;
	args << tr("-vf");
	args << tr("scale=320:240");
	args << fileName;//fileName;//tr("/home/hdc/视频/02-112009中国香港动作大片风云2 风云2DVD-RMVB国语中字 1024x434高清晰 无水印版.rmvb");

	mplayerProcess->start(tr("/mplayer/bin/mplayer"), args);
}

void Player::controlCmd(const QString &cmd)
{
	mplayerProcess->write(cmd.toLatin1());
}

void Player::resizeEvent(QResizeEvent * event)
{
	renderTarget->resize(event->size());
}

/*
void Player::slotStarted(const QString &fileName)
{
	qDebug() << "mplayer started ...";

}
*/

//void Player::slotError(QProcess::ProcessError /*error*/)
//{
//	qDebug() << "player error.";
//}
//void Player::slotFinished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
//{
//	qDebug() << "mplayer finished.";
//}
/*	
void Player::slotBackMessage()
{
	while( mplayerProcess->canReadLine() )
	{
		QString message(mplayerProcess->readLine());
		// message 即为读取的信息，我们可以根据需要取我们要的信息如：
		// 文件总时间为：ANS_LENGTH=23.00
		// 当前时间为：ANS_TIME_POSITION=23.00
		qDebug() << message;
	}
}
*/
