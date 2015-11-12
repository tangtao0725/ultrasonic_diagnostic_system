/*这个类是用来显示波形和显示频谱 */
#ifndef PLOTTER_H
#define PLOTTER_H
#include <vector>
#include <QWidget>
#include <QtGui>
using namespace std;

class Plotter:public QWidget
{
	Q_OBJECT
public:
	Plotter(QWidget *parent = 0,Qt::WindowFlags flags = 0);
	void setPlotSettings();
	void toolButtonSetting();
	double minX,maxX;
	double minY,maxY;
	int numXTicks,numYTicks;
	vector<double> data;
	bool isSignal;
	QToolButton *waveformButton;
	bool toolButtonFlag;
public slots:
	void display();
	void showCurves(bool);
	void slotToolButtonClick();
signals:
	void signalZoomOut();
	void signalZoomIn();
	void signalToolButtonClick();
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
private:
	void drawGrid(QPainter *painter);
	void drawCurves(QPainter *painter);
	enum{Margin = 10};
	enum{Margin1 = 40};
	bool isZoomIn; 
};
#endif
