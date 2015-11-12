#include <QtGui>
#include <QPainter>
#include <iostream>
#include <algorithm>
#include "plotter.h"
#include "parameter.h"
using namespace std;

Plotter::Plotter(QWidget* parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint),isZoomIn(true),isSignal(true)
{
	resize(510,190);
	setBackgroundRole(QPalette::Dark);
	setAutoFillBackground(true);
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	setFocusPolicy(Qt::NoFocus);

	toolButtonFlag = true;
	waveformButton = new QToolButton(this);
	waveformButton->setIconSize(QPixmap(":/images/start.png").size());
	int x = width() - QPixmap(":/images/suspend.png").size().width()-15;		
	waveformButton->move(x,5);
	waveformButton->hide();

	setPlotSettings();

	QObject::connect(waveformButton,SIGNAL(clicked()),this,SLOT(slotToolButtonClick()));
}

void Plotter::setPlotSettings()
{
	update();
	updateGeometry();
}

void Plotter::toolButtonSetting()
{
	if(toolButtonFlag)
	{
		waveformButton->setIcon(QIcon(":/images/start.png"));
	}
	else
	{
		waveformButton->setIcon(QIcon(":/images/suspend.png"));
	}
}

void Plotter::display()
{
	data.clear();
	setPlotSettings();
	showMaximized();
}

void Plotter::showCurves(bool flag)
{
	if(flag)
	{
		double x = *max_element(data.begin(),data.end(),absLess)*1.1;
		maxY = int(abs(x)*10)/10.0;
		minY = -1*maxY;
	}
	else
	{
		double x = *max_element(data.begin(),data.end())*1.1;
		maxY = int(x*10)/10.0;
		minY = 0;
	}
	setPlotSettings();
	
}

void Plotter::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.initFrom(this);
	drawGrid(&painter);
	drawCurves(&painter);
}

void Plotter::drawGrid(QPainter *painter)
{
	QRect rect(Margin1,Margin,width()-Margin1-Margin,height()-3*Margin);
	if(!rect.isValid())
		return;
	QPen quiteDark = palette().dark().color().light();
	QPen light = palette().light().color();
	QFont font;
	font.setPointSize(50);
	font.setFamily("simsun");
	int x;
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	for(int i = 0;i < numXTicks-1;++i){
		x = rect.left()+(i*(rect.width()-1)/(numXTicks-1));
		int label = i*(maxX-minX)/(numXTicks-1);
		painter->setPen(quiteDark);
		painter->drawLine(x,rect.top(),x,rect.bottom());
		painter->setPen(light);
		painter->setFont(font);
		painter->drawLine(x,rect.bottom(),x,rect.bottom()+3);
		if(isSignal)
		{
			painter->drawText(x-20,rect.bottom()+3,40,30,Qt::AlignHCenter|Qt::AlignTop,QString::number(label/32));
		}
		else
		{
			painter->drawText(x-20,rect.bottom()+3,40,30,Qt::AlignHCenter|Qt::AlignTop,QString::number(label/16));
		}
	}

	//the last number on the axis does not display, and replaced with the scale as followed.
	x = rect.left()+(rect.width()-1);
	if(isSignal)
	{
		painter->drawText(x-20,rect.bottom()+3,40,30,Qt::AlignHCenter|Qt::AlignTop,QString("us"));
	}
	else
	{
		painter->drawText(x-20,rect.bottom()+3,40,30,Qt::AlignHCenter|Qt::AlignTop,QString("MHz"));
	}
	for(int j = 0;j < numYTicks; ++j){
		int y = rect.bottom()-(j*(rect.height()-1)/(numYTicks-1));
		double label = int(10*(minY+j*(maxY-minY)/double(numYTicks-1)))/10.0;
		painter->setPen(quiteDark);
		painter->drawLine(rect.left(),y,rect.right(),y);
		painter->setPen(light);
		painter->drawLine(rect.left()-3,y,rect.left(),y);
		painter->drawText(rect.left()-40,y-10,40,20,Qt::AlignRight|Qt::AlignVCenter,QString::number(label));
	}
	painter->drawRect(rect.adjusted(0,0,-1,-1));
}

void Plotter::drawCurves(QPainter *painter)
{
	QRect rect(Margin1,Margin,width()-Margin1-Margin,height()-3*Margin);
	if(!rect.isValid())
		return;
	QPolygonF polyline(data.size());
//	cout << data.size() << endl;
	for(int j = 0;j < data.size();++j){
		double dx = j;
		double dy = data[j];
		double x = rect.left() + (dx*(rect.width()-1)/(maxX-minX));
		double y = rect.bottom() - ((dy-minY)*(rect.height()-1)/(maxY-minY));
		polyline[j] = QPointF(x,y);
	}
	painter->setPen(Qt::green);
	painter->drawPolyline(polyline);

}

void Plotter::mousePressEvent(QMouseEvent *event)
{
	if(isZoomIn)
	{
		isZoomIn = false;
		resize(800,480);
		numXTicks = (numXTicks-1)*2+1;
		numYTicks = (numYTicks-1)*2+1;
		emit(signalZoomOut());
	}
	else
	{
		isZoomIn = true;
		resize(510,190);
		numXTicks = (numXTicks-1)/2+1;
		numYTicks = (numYTicks-1)/2+1;
		emit(signalZoomIn());
	}
}

void Plotter::slotToolButtonClick()
{
	toolButtonFlag = !toolButtonFlag;
	toolButtonSetting();
	toolButtonFlag = !toolButtonFlag;
	emit signalToolButtonClick();
}

