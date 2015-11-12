#ifndef __PLAYER_H_
#define __PLAYER_H_

#include <QWidget>
#include <QProcess>

class QString;
class QPushButton;
class QProcess;

class Player : public QWidget
{
	Q_OBJECT

public:
	Player(QWidget * parent = 0);
	~Player();

	void play(const QString &fileName);
	void controlCmd(const QString &cmd);


protected:
	void resizeEvent(QResizeEvent * event);

private:
	QWidget *renderTarget;
	QProcess *mplayerProcess;
};

#endif // __PLAYER_H_
