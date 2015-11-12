/*这个类是为了绘制三个标准曲线
  
*/
#ifndef DRAW_H
#define DRAW_H
#include <vector>
#include <QWidget>
using namespace std;
struct points
{
	points(int lhs = 0,double rhs = 0):age(lhs),value(rhs){}
	int age;
	double value;
};

enum valuetype {bc,aib,scs};

class Draw:public QWidget
{
	Q_OBJECT
public:
	Draw(QWidget *parent = 0,Qt::WindowFlags flags = 0);
	void setPlotSettings();
	int minX,maxX;
	int minY,maxY;
	int numXTicks,numYTicks;
	int pointAge;
	double pointValue;
	vector<double> data;
	vector<struct points> points;
	int age;
	double bc_value,aib_value,scs_value;
	vector<struct points> current_value;
	valuetype type;
public slots:
	void display();
	void showCurves();
	void showPoints();
	void slotClearData();
protected:
	void paintEvent(QPaintEvent *event);
private:
	void drawGrid(QPainter *painter);
	void drawCurves(QPainter *painter);
	void drawPoint(QPainter *painter);
	enum{Margin = 10};
};
#endif
