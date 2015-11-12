#include <QtGui>
#include <QPainter>
#include <iostream>
#include "draw.h"

using namespace std;

Draw::Draw(QWidget* parent,Qt::WindowFlags flags)
	:QWidget(parent,flags|Qt::FramelessWindowHint)
{
	setBackgroundRole(QPalette::Link);
	setAutoFillBackground(true);
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	setFocusPolicy(Qt::NoFocus);

	setPlotSettings();
}

void Draw::setPlotSettings()
{
	update();
	updateGeometry();
}

void Draw::display()
{
	points.clear();
	data.clear();
	current_value.clear();
	setPlotSettings();
	showMaximized();
}

void Draw::slotClearData()
{
	points.clear();
	data.clear();
	current_value.clear();
}
void Draw::showCurves()
{
	setPlotSettings();
}

void Draw::showPoints()
{
	setPlotSettings();
	
}

void Draw::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.initFrom(this);
	drawGrid(&painter);
//	drawCurves(&painter);
	drawPoint(&painter);
}

void Draw::drawGrid(QPainter *painter)
{
	QRect rect(Margin+40,Margin+20,width()-2*Margin-50,height()-2*Margin-50);
	if(!rect.isValid())
		return;
	QPen quiteDark = palette().dark().color().light();
	QPen light = palette().light().color();
	QFont font;
	font.setPointSize(50);
	font.setFamily(("simsun"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));	
	for(int i = 0;i < numXTicks;++i){
		int x =rect.left()+(i*(rect.width()-1)/(numXTicks-1));
		double label = i*(maxX-minX)/(numXTicks-1)+minX;
		painter->setPen(quiteDark);
		painter->drawLine(x,rect.top(),x,rect.bottom());
		painter->setPen(light);
		painter->setFont(font);
		int tick = (rect.width()-1)/(5*(numXTicks-1));
		for(int j = 1;j < 5;++j)
		{
			if(i == (numXTicks-1))
				break;
			painter->drawLine(x+(j*tick),rect.bottom(),x+(j*tick),rect.bottom()-3);
		}
		painter->drawText(x-20,rect.bottom()+3,40,20,Qt::AlignHCenter|Qt::AlignTop,QString::number(label));
	}
	for(int j = 0;j < numYTicks;++j)
	{
		int y=rect.top()+(j*(rect.height()-1)/(numYTicks-1));
		double label;
		if(type == scs)
		{
			maxY = -350;	//2012.07.16
			minY = -550;	//2012.07.16
			label = -j*(maxY-minY)/(numYTicks-1) + maxY;
		}
		else if(type == bc)
		{
			maxY = 0;
			minY = -50;
			label = -j*(maxY-minY)/(numYTicks-1) + maxY;
		}
		else 
		{
			maxY=0;
			minY=-20;
			label =	-j*(maxY-minY)/(numYTicks-1) + maxY;
		}
		painter->setPen(quiteDark);
		painter->setFont(font);
		painter->drawLine(rect.left(),y,rect.right(),y);
		painter->setPen(light);
		painter->drawLine(rect.left()-3,y,rect.left(),y);
		int tick = (rect.height()-1)/(5*(numYTicks-1));		//于2012.07.17
		for(int k = 1;k < 5;++k)	//2012.07.17,添加纵轴的小横线，参考横轴的小竖线
		{
			if(j == (numYTicks-1))
				break;
			painter->drawLine(rect.left(),y+(k*tick),rect.left()+3,y+(k*tick));
		}
		painter->drawText(rect.left()-35,y-10,35,20,Qt::AlignRight|Qt::AlignVCenter,QString::number(label));
	}
	painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Draw::drawCurves(QPainter *painter)
{
	QRect rect(Margin+40,Margin+20,width()-2*Margin-50,height()-2*Margin-50);
	if(!rect.isValid())	
	return;	
	std::cout << rect.top() << std::endl;
	QPolygonF polyline(data.size());
	for (int j = 0;j < data.size();++j){
		double dx = j;
		double dy = data[j];
		double x = rect.left() + ((dx-minX) * (rect.width() - 1)/(maxX-minX));
		double y;
		if(type == scs)
		{
			maxY = -350;	//2012.07.16
			minY = -550;	//2012.07.16
        	y = rect.top() - ((dy-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		else if(type == bc)
		{
			maxY = 0;
			minY =-50;
        	y = rect.top() - ((dy-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		else
		{
			maxY = 0;
			minY = -20;
		y = rect.top() - ((dy-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		polyline[j] = QPointF(x,y);
	}
	QPen pen;
	pen.setWidth(4);
	pen.setColor(Qt::red);
	painter->setPen(pen);
	painter->drawPolyline(polyline);
}

void Draw::drawPoint(QPainter *painter)
{
	QRect rect(Margin+40,Margin+20,width()-2*Margin-50,height()-2*Margin-50);
	if(!rect.isValid())	
	return;	

	QPen pen1;
	pen1.setWidth(4);
	pen1.setColor(Qt::red);
	painter->setPen(pen1);
	for(int j = 0;j < points.size();++j){
		double dx = points[j].age;
		double dy = points[j].value;
		std::cout << "dy:" << dy << std::endl;
		double x = rect.left() + ((dx-minX) * (rect.width() - 1)/(maxX-minX));
		double y;
		if(type == scs)
		{
			maxY = -350;	//20120716,原始数据：1000
			minY = -550;	//20120716,原始数据: 0
        	y = rect.top() - ((dy-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		else if(type == bc)
		{
			maxY = 0;
			minY = -50;
        	y = rect.top() - ((dy-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		else
		{
			maxY = 0;
			minY = -20;
		y = rect.top() - ((dy-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		painter->drawEllipse(x,y,4,4);
	}
	pen1.setColor(Qt::green);
	painter->setPen(pen1);
	if(current_value.size() != 0)
	{
		double x = rect.left() + ((current_value[0].age-minX) * (rect.width()-1)/(maxX-minX));
		double y;
		if(type == scs)
		{
			maxY = -350;	//20120716
			minY = -550;	//20120716
        	y = rect.top() - ((current_value[0].value-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		else if(type == bc)
		{
			maxY = 0;	//20120716
			minY = -50;	//20120716
        	y = rect.top() - ((current_value[0].value-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		else
		{
			maxY = 0;
			minY = -20;
		y = rect.top() - ((current_value[0].value-maxY) * (rect.height() - 1)/(maxY-minY));
		}
		painter->drawEllipse(x,y,4,4);
	}

	pen1.setWidth(6);
	pen1.setColor(Qt::yellow);
	painter->setPen(pen1);


	rect = QRect(0,0,width(),height());
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	QFont font;
	font.setPointSize(60);
	font.setFamily(("simsun"));
	painter->setFont(font);
	if(type == bc)
	{
		painter->drawText(180,rect.bottom()-40,180,38,Qt::AlignHCenter|Qt::AlignBottom,QObject::tr("Age/y"));
		painter->rotate(270);
		painter->drawText(-250,0,100,30,Qt::AlignHCenter|Qt::AlignBottom,QObject::tr(" BC(dB)"));
		painter->rotate(-270);
	}
	else if(type == aib)
	{
		painter->drawText(180,rect.bottom()-40,180,38,Qt::AlignHCenter|Qt::AlignBottom,QObject::tr("Age/y"));
		painter->rotate(270);
		painter->drawText(-250,0,100,30,Qt::AlignLeft|Qt::AlignBottom,QObject::tr(" AIB(dB)"));
		painter->rotate(-270);
	}
	else if(type == scs)
	{
		painter->drawText(180,rect.bottom()-40,180,38,Qt::AlignHCenter|Qt::AlignBottom,QObject::tr("Age/y"));
		painter->rotate(270);
		painter->drawText(-250,0,100,30,Qt::AlignLeft|Qt::AlignBottom,QObject::tr(" SCS(KHz)"));
		painter->rotate(-270);
	}
	else{}	
}



