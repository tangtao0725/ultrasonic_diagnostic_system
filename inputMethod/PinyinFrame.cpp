/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphyTalk PinYin Chinese input method,
** which is for the Qtopia Environment.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**
** murphytalk@gmail.com
** $Date: 2004/07/31 17:20:19 $
**
**********************************************************************/
#include "PinyinFrame.h"

#include <QPainter>
#include <QList>
#include <QBitmap>
#include <QFontMetrics>
#include <QLayout>
#include <QPalette>
#include <QPushButton>
#include <QMessageBox>
#include <QColor>
#include <QPen>
#include <QWSServer>
#include <QTimer>
#include <QApplication>
#include <QEvent>
#include <QColorGroup>
#include <QMouseEvent>
#include <QDesktopWidget>

#include "public.h"
#include "Config.h"

extern QApplication* App;

const char table_file[]="/usr/local/InputMethod/pinyin_table.txt";
const char phrase_index_file[]="/usr/local/InputMethod/murphytalk_phrase_idx.txt";
const char config_file[]="/usr/local/InputMethod/murphytalk.conf";


/* XPM */
static const char * pix_prev[] = {
"19 18 3 1",
" 	c #C0C0C0C0C0C0",
"#	c #000000000000",
".	c #00000000FFFF",
"                   ",
"                   ",
"             #     ",
"            ##     ",
"           #.#     ",
"          #..#     ",
"         #...#     ",
"        #....#     ",
"       #.....#     ",
"      #......#     ",
"       #.....#     ",
"        #....#     ",
"         #...#     ",
"          #..#     ",
"           #.#     ",
"            ##     ",
"             #     "
"                   ",
"                   ",};

static const char * pix_next[] = {
"19 18 3 1",
" 	c #C0C0C0C0C0C0",
"#	c #000000000000",
".	c #00000000FFFF",
"                   ",
"                   ",
"     #             ",
"     ##            ",
"     #.#           ",
"     #..#          ",
"     #...#         ",
"     #....#        ",
"     #.....#       ",
"     #......#      ",
"     #.....#       ",
"     #....#        ",
"     #...#         ",
"     #..#          ",
"     #.#           ",
"     ##            ",
"     #             ",
"                   ",
"                   ",};

const int PINYIN_X = 2;
const int PINYIN_Y = 2;

const unsigned short ZHONG = 0x4e2d;   //Hanzi : Chinese 
const unsigned short YING  = 0x82f1;   //Hanzi : English

const char ABOUT[]="About";
static const char * kb_xpm[] = {
"28 13 4 1",
"       c None",
".      c #4C4C4C",
"+      c #FFF7DD",
"@      c #D6CFBA",
" .......................... ",
" .+++.+++.+++.+++.+++.++++. ",
" .+@@.+@@.+@@.+@@.+@@.+@@@. ",
" .......................... ",
" .+++++.+++.+++.+++.++++++. ",
" .+@@@@.+@@.+@@.+@@.+@@@@@. ",
" .......................... ",
" .++++++.+++.+++.+++.+++++. ",
" .+@@@@@.+@@.+@@.+@@.+@@@@. ",
" .......................... ",
" .++++.++++++++++++++.++++. ",
" .+@@@.+@@@@@@@@@@@@@.+@@@. ",
" .......................... "};
const char MAKEPHRASE_OPEN [] = "\xe3\x80\x90\xe9\x80\xa0\xe8\xaf\x8d\x20";
const char MAKEPHRASE_CLOSE[] = "\xe3\x80\x91";

//the following marks encoded in ucs-2(unicode 16 big endian)
const int COMMA_MARK	=0xff0c;
const int PERIOD_MARK	=0x3002;
const int BOOK_MARK1	=0x300a;
const int BOOK_MARK2	=0x300b;
const int QUESTION_MARK	=0xff1f;
const int COLON_MARK	=0xff1a;

QPinyinFrame::QPinyinFrame(QWidget* parent,Qt::WindowFlags flags) :
	QFrame(parent,flags|Qt::WindowStaysOnTopHint),m_engine(table_file,phrase_index_file),
	m_bEnglishMode(true),m_bMakingPhrase(false),m_bShowKeyboard(true),
	keyHeight(30),PickHeight(60),xoffs(0),pressedKey(-1),
	shift(0),ctrl(0),alt(0),lock(0),useLargeKeys(TRUE),
	useOptiKeys(0),modifiers(0),pressed(0),
	pressTid(0),qkeycode(0),unicode(-1)
{
	(new QHBoxLayout(this))->setEnabled(TRUE);
	Config config(config_file);

	String font_name=config.get_as_string("font");
	int    font_size=config.get_as_int("font-size");
	m_bShowKeyboard = true;//config.get_as_int("keyboard");
	printX86("from config -> font=%s\n",font_name.c_str());
	printX86("from config -> font-size=%d\n",font_size);
	printX86("from config -> keyboard=%d\n",m_bShowKeyboard);
	
	/*if(font_name.size()>0){
		if(font_size<=0){
			font_size=font().pointSize();
		}
		setFont(QFont(font_name.c_str(),font_size));
	}*/	
	QFont font;
	font.setPointSize(60);
	setFont(font);
	screen_rect=QApplication::desktop()->rect(); 
	defaultKeyWidth=screen_rect.width()/15;
	m_kbd_rect=QRect(0,0,screen_rect.width(),480);
	repeatTimer = new QTimer((QFrame*)this);

	QObject::connect ( repeatTimer, SIGNAL(timeout()),(QFrame*)this,SLOT(repeat()));
	setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred));
}

QPinyinFrame::~QPinyinFrame()
{
	printf("ime killed\n");	
}

void QPinyinFrame::resizeEvent(QResizeEvent*)
{
   int nk;
   if ( useOptiKeys ) {
       nk = 15;
   } else if ( useLargeKeys ) {
       nk = 15;
   } else {
       nk = 19;
   }
   defaultKeyWidth = screen_rect.width()/nk;
   xoffs = (screen_rect.width()-defaultKeyWidth*nk)/2;
}

QSize QPinyinFrame::sizeHint() const
{
	QPinyinFrame *This=const_cast<QPinyinFrame*>(this);
	int height=This->init_gui_dimention();
	//return QSize(maximumWidth(), height);
	return QSize(QApplication::desktop()->width(), height);
}

/*
 * return frame height 返回窗体的高度
 */
int QPinyinFrame::init_gui_dimention()//初始化GUI的尺寸
{
	static int sHeight = -1;
	if(sHeight>0) 
	{
		if(m_bShowKeyboard) 
		{
			return sHeight + 160;
		}
		else
		{
			return sHeight + 160;
		}
	}
//	qDebug("sHeight : %d",sHeight);
	const int pix_width=19,pix_height=18;

	//QPainter painter(this);//qt4中不允许这么使用

	QFontMetrics metric=App->fontMetrics();
	QRect rect = metric.boundingRect(QChar(YING));

	//int width=640;
	int width=QApplication::desktop()->width();
//	qDebug("text metric is %d %d %d,%d",rect.x(),rect.y(),rect.width(),rect.height());

	int x=PINYIN_X;
	int y=PINYIN_Y;
//	drawText(x,rect.height(),QChar(m_bEnglishMode?YING:ZHONG));
	m_indicator_rect=QRect(x,y,rect.width(),rect.height());
//	qDebug("indicator rect(%d,%d,%d,%d)",m_indicator_rect.left(),m_indicator_rect.top(),m_indicator_rect.width(),m_indicator_rect.height());

		//horizonal line which seperates pinyin and hanzi candidates
		y+=rect.height()+2;
		QRect about = metric.boundingRect(QString(ABOUT));
		m_about_rect = QRect(width-2-about.width(),PINYIN_Y,about.width(),about.height());
		m_kbdbtn_rect=QRect(width-2-about.width()-24,PINYIN_Y,28,13);
		printX86("about rect(%d,%d,%d,%d)\n",m_about_rect.left(),m_about_rect.top(),m_about_rect.width(),m_about_rect.height());

	//the second row --  hanzi candicates
	y+=12;
//	qDebug("y1:%d",y);
	m_leftbtn_rect=QRect(x,y+2,pix_width,pix_height);		
//	qDebug("left btn rect(%d,%d,%d,%d)",m_leftbtn_rect.left(),m_leftbtn_rect.top(),m_leftbtn_rect.width(),
//		       m_leftbtn_rect.height());

	m_rightbtn_rect=QRect(width-2-pix_width,y+2,pix_width,pix_height);		
//	qDebug("right btn rect(%d,%d,%d,%d)",m_rightbtn_rect.left(),m_rightbtn_rect.top(),m_rightbtn_rect.width(),
//		       m_rightbtn_rect.height());

		m_cand_rect=QRect(0,y,width-2,rect.height()+2);
		printX86("candidate rect(%d,%d,%d,%d)\n",m_cand_rect.left(),m_cand_rect.top(),m_cand_rect.width(),m_cand_rect.height());
	sHeight=y+rect.height()+5;

	return sHeight+160;
}

bool QPinyinFrame::filter (int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat)
{   
	//qDebug("filter!");
	qDebug("qkeycode:%d;Key_F1:%d,Key_F12:%d\n",keycode,Qt::Key_F1,Qt::Key_F12);
	if (!isHidden()&& isPress) {
		if(m_bEnglishMode){
		//	if(unicode == 9 && keycode == Qt::Key_Tab){
			if(unicode == 0xffff && keycode == Qt::Key_Control){
				m_bEnglishMode = false;
				update();			
			}
			else if(Qt::Key_F1<=keycode<=Qt::Key_F12)
			{
				return false;
			}
			else{
				SendKey(unicode,keycode); 
			}
			return true;		
		}

		if (GetKey (unicode, keycode))
			return true;
		else
			return false;
	} 
	else
		return false;
}   
//上面的filter函数是一个QWSInputMethod的重载函数，定义它时，处理来自键盘或者模拟键盘的按键输入，键盘输入容易理解，模拟键盘输入是如何捕获的呢？这是通过
//处理鼠标事件时，emit key()信号来让filter函数捕获


void QPinyinFrame::paintEvent(QPaintEvent* e)
{
	QFrame::paintEvent(e);
	
	QPainter painter(this);
	painter.setClipRect (e->rect());

	//FIXME: how to get screen size
	//unsigned int width  = 640;
	unsigned int width  = QApplication::desktop()->width();
	///////////////////////////////////////////////
	//the first row - pinyin inputed 

	QFontMetrics metric=App->fontMetrics();//painter.fontMetrics();
	QRect rect = metric.boundingRect(QChar(ZHONG));

	int x=PINYIN_X;
	int y=PINYIN_Y;
	painter.drawText(x,rect.height(),QChar(m_bEnglishMode?YING:ZHONG));

	QPen penGray (QColor(128,128,128));
	QPen penWhite(QColor(255,255,255));
	QPen penText (QColor(0,0,0));
	QPen penPage (QColor(0,0,255));

	//vertical line which seperates chinese/english indicator with pinyin inputed
	painter.setPen(penGray);
	painter.drawLine(x+rect.width()+2,y,x+rect.width()+2,y+rect.height());
	painter.setPen(penWhite);
	painter.drawLine(x+rect.width()+3,y,x+rect.width()+3,y+rect.height());

	//making phrase?
	int X=x+rect.width()+7;
	int Y=rect.height();
	y += rect.height()+2;
	if(m_bMakingPhrase){
		QRect temp;

	    QString s=QString::fromUtf8(MAKEPHRASE_OPEN);
		painter.setPen(penText);
		painter.drawText(X,Y,s);
		temp = metric.boundingRect(s);
		X+=temp.width();

		painter.setPen(penPage);
		painter.drawText(X,Y,m_ime_info.phrase);
		temp = metric.boundingRect(m_ime_info.phrase);
		X+=temp.width();
 
		s=QString::fromUtf8(MAKEPHRASE_CLOSE);
		painter.setPen(penWhite);
		painter.drawText(X,Y,s);			
		temp = metric.boundingRect(s);
		X+=temp.width()+2;
	}
	//pinyin inputed by user
	painter.setPen(penWhite);
	painter.drawText(X,Y,QString(m_engine.get_formatted_pinyin().c_str()));

	//horizonal line which seperates pinyin and hanzi candidates
	painter.setPen(penGray);
	painter.drawLine(x,y,x+width,y);
	painter.setPen(penWhite);
	painter.drawLine(x,y+1,x+width,y+1);


	//about
	painter.setPen(penPage);
	QRect about = metric.boundingRect(QString(ABOUT));
	painter.drawText(width-2-about.width(),rect.height(),QString(ABOUT));
	painter.drawLine(width-2-about.width(),rect.height()+1,width-2,rect.height()+1);
	
	painter.drawPixmap(width-2-about.width()-28,rect.height()-10,QPixmap((const char**)kb_xpm));

	///////////////////////////////////////////////
	//the second row --  hanzi candicates
	y+=12;

	//horizonal line at the bottom
	Y=y+rect.height();
	painter.setPen(penWhite);
	painter.drawLine(x,Y,x+width,Y);

	if(m_ime_info.prev_page_available()){
		QPixmap prev((const char **)pix_prev);
		painter.drawPixmap(x,y,prev);
		x+=prev.width()+2;
	}

	//hanzi candidates
	
	m_ime_info.candidates_on_page=0;	
	QString str;
	QRect hanzi;
	painter.setPen(penWhite);
	for(unsigned int i=m_ime_info.first_visible,xx=x,yy=Y-3;m_ime_info.candidates_on_page<10&&i<m_ime_info.candidates_count;i++){
		str.sprintf("%d.",(i-m_ime_info.first_visible+1)%10);//1. 2. ... 9. 0.
     		if(m_engine.isPhrase()){
			str+=get_phrase(i);
		}
		else{
			str+=get_char(i);
		}
		hanzi=metric.boundingRect(str);
		cand_width=hanzi.width();
		if(xx+cand_width>=(unsigned int)m_rightbtn_rect.left()){//no space for this hanzi and right button
			if(i<m_ime_info.candidates_count-1){ //but we need another page
				break; //so give up to draw this hanzi
			}
			else{
				//this is the last page,do not need to show next button
				//so just check if the left space is enough to show this hanzi
				if(xx+cand_width>width){
					break;//Ooops
				}			
			}			
		}
		
		painter.drawText(xx,yy,str);		
		xx+=cand_width+30;
		m_ime_info.candidates_on_page++;
	}

	if(m_ime_info.next_page_available()){
		QPixmap next((const char **)pix_next);
		painter.drawPixmap(width-2-next.width(),y,next);
	}
	if(m_bShowKeyboard)
	{
		drawKeyboard(painter,-1);
	}
}

bool QPinyinFrame::prev_page()
{
	if(m_ime_info.prev_page_available()){		
		//qDebug("prev page,firt visible index %d->",m_ime_info.first_visible);	
		m_ime_info.first_visible-=m_ime_info.candidates_on_prev_page;
		//qDebug("prev page,firt visible index %d->",m_ime_info.first_visible);
		update();	
		return true;		
	}	
	else{
		return false;
	}
}

bool QPinyinFrame::next_page()
{
	if(m_ime_info.next_page_available()){		
		//qDebug("next page,firt visible index %d->",m_ime_info.first_visible);	
		m_ime_info.candidates_on_prev_page=m_ime_info.candidates_on_page;
		m_ime_info.first_visible+=m_ime_info.candidates_on_page;
		//qDebug("%d\n",m_ime_info.first_visible);	
		update();	
		return true;		
	}	
	else{
		return false;
	}	
}


int QPinyinFrame::get_charunicode(unsigned int nIndexOnPage)
{
	unsigned int index = m_ime_info.first_visible + nIndexOnPage;
	qDebug("get unicode:first visible index %d,current index %d,global index %d",
		m_ime_info.first_visible,nIndexOnPage,index);
	return (m_engine.get_char(index)).unicode();	
}

/*
  return true if need to update UI
 */
bool QPinyinFrame::commit_selection(int k)
{
	bool bUpdate = false;
	unsigned int index = ((k-'0')+9)%10;
	if(index<m_ime_info.candidates_on_page){
		m_engine.hit(m_ime_info.first_visible+index);
		if(m_engine.isPhrase()){
			QString phrase=get_phrase(m_ime_info.first_visible+index);
			if(m_bMakingPhrase){
				m_ime_info.phrase+=phrase;
			}
			else{
				for(/*unsigned*/ int i=0;i<phrase.length();i++){
					qDebug("%04X,",phrase[i].unicode());
					SendKey(phrase[i].unicode()); 
				}
				//qDebug("\n");
			}
		}
		else{
			
			if(m_bMakingPhrase){
				m_ime_info.phrase+=get_char(m_ime_info.first_visible+index);
			}
			else{
				SendKey(get_charunicode(index)); 
			}
		}
		resetState();
		bUpdate=true;
	}
	return bUpdate;
}

bool QPinyinFrame::send_hanzi_mark(int ascii_mark)
{
	int unicode = 0;	
	switch(ascii_mark){
		case ',':
			unicode = COMMA_MARK;
			break;
		case '.':
			unicode = PERIOD_MARK;
			break;
		case '<':
			unicode = BOOK_MARK1;
			break;
		case '>':
			unicode = BOOK_MARK2;
			break;
		case '?':
			unicode = QUESTION_MARK;
			break;
		case ':':
			unicode = COLON_MARK;
			break;
	}
	if(unicode!=0){
		SendKey(unicode);
		return true;
	}
	else{
		return false;
	}
}

bool QPinyinFrame::GetKey(int u, int k/*,int m*/)/*int unicode, int keycode, int modifiers*/
{
#if 0
	if(m_bEnglishMode){
		if(u == 9 && k == Qt::Key_Tab){
			m_bEnglishMode = false;
			update();			
		}
		else{
			SendKey(u,k); 
		}
		return true;		
	}
#endif
	switch(k)
	{
		//case Qt::Key_F1:
			//return true;
		case Qt::Key_F2:
			return true;
		case Qt::Key_F3:
			return true;
		case Qt::Key_F4:
			return true;
		case Qt::Key_F5:
			return true;
		case Qt::Key_F6:
			return true;
		case Qt::Key_F7:
			return true;
		case Qt::Key_F8:
			return true;
		case Qt::Key_F9:
			return true;
		case Qt::Key_F10:
			return true;
		case Qt::Key_F11:
			return true;
		case Qt::Key_F12:
			return true;
//		case Qt::Key_Control:
//			return true;
		case Qt::Key_NumLock:
			return true;
		case Qt::Key_CapsLock:
			return true;
//		case Qt::Key_Alt:
//			return true;
	}

	bool bUpdate         = false;
	bool bKeyProcessed   = true;
	
        if( k >= '0' && k <= '9'){
commit:
		bUpdate=commit_selection(k);
		if(!bUpdate){
			bKeyProcessed=false;
		}
	}
	else if(k >= 'a' && k <= 'z'){
input:		
		m_ime_info.pinyin+=(char)k;
		search();
		qDebug("arrived here");
		bUpdate=true;	     
	}
	else if(k >= 'A' && k <= 'Z'){
		k = k - 'A'+ 'a';
		goto input;
	}
	else if(k == Qt::Key_Space){
		if(m_ime_info.candidates_on_page>0){
			k='1';
			goto commit;
		}
		else{
			bKeyProcessed=false;
		}
	}
	else if(k == Qt::Key_Right||k == Qt::Key_Down){
		if(!next_page()){
			bKeyProcessed=false;
		}					
	}
	else if(k == Qt::Key_Left||k == Qt::Key_Up){
		if(!prev_page()){
			bKeyProcessed=false;
		}					
	}
	else if( u == 8 && k == Qt::Key_Backspace){
		if(m_ime_info.pinyin.size()>0){			
			String::iterator pos=m_ime_info.pinyin.end()-1;
			m_ime_info.pinyin.erase(pos);
			search();
			bUpdate=true;
		}
		else{
			SendKey(u,k);
		}
		
	}
	else if(k == Qt::Key_Shift){
		if(m_bMakingPhrase){
			m_bMakingPhrase=false;
			m_engine.append_phrase(m_ime_info.phrase,m_making_phrase_pinyin.c_str());
			m_engine.save_table();	
			m_ime_info.phrase="";
			resetState();
			bUpdate=true;
		}
		else if(m_ime_info.pinyin.size()==0){
			m_making_phrase_pinyin="";
			m_bMakingPhrase=true;
			bUpdate=true;
		}
	}
//	else if(u == 9 && k == Qt::Key_Tab){
	else if(u == 0xffff && k == Qt::Key_Control){
		m_bEnglishMode=true;
		bUpdate=true;		
	}
	else if(  k == Qt::Key_Escape){
		if(m_bMakingPhrase){
			m_ime_info.phrase="";
			m_bMakingPhrase=false;
		}
		resetState();
		bUpdate=true;
	}
	else if(!send_hanzi_mark(k)){
		bKeyProcessed=false;
	}
	

	if(bUpdate){
		update();
	}
	
	if(!bKeyProcessed){
		SendKey(u,k);

	}


	return true;
}

void QPinyinFrame::show()
{
	QFrame::show ();
}

void QPinyinFrame::hide()
{
	QFrame::hide ();
	resetState();
	m_engine.save_table();	
}
    
bool hit_test_helper(int x,int y,QRect& rect)
{
	if(x>=rect.left()&&x<=rect.right()&&
           y>=rect.top() &&y<=rect.bottom()){
		return true;
	}
	else{
		return false;
	}
}

void QPinyinFrame::mouseReleaseEvent(QMouseEvent* m)
{
	bool bUpdate = false;
	int x=m->x();
	int y=m->y();

	if(hit_test_helper(x,y,m_about_rect)){
		//show about infomation
		QMessageBox::information(this,"About",
                                         "MurphyTalk Pinyin "VERSION"<br><br>"
                                         "Created by <font color=\"#0000FF\">MurphyTalk</font><br>"
					 "aka:<font color=\"#0000FF\">DeepWater</font>@<b>Hi-PDA</b> community<br>"
					 "contact me at <font color=\"#0000FF\">murphytalk@gmail.com</font><br><br>"
					 "This software is partially based on scim chinese<br>"
					 "written by James Su(suzhe@tsinghua.org.cn)<br><br>"
					 "This small piece of cake is released under GPL;)");
	}	
	else if(hit_test_helper(x,y,m_cand_rect)) {
		printX86("candrect x %d y %d width %d height %d",m_cand_rect.x(),m_cand_rect.y(),m_cand_rect.width(),m_cand_rect.height());
		int width=0,i=0;	
		if(m_ime_info.prev_page_available())
			width+=m_leftbtn_rect.width();
			
		if(m_ime_info.prev_page_available()&&
			hit_test_helper(x,y,m_leftbtn_rect)){
			prev_page();
		}else if(m_ime_info.next_page_available()&&
			hit_test_helper(x,y,m_rightbtn_rect)){
			next_page();
		}else { //we are hit the number area of the displayed candidates
			if((i=m_ime_info.candidates_on_page)>0){
			width=x-width;
			i=width/(cand_width+30);		
			printX86("index=%d,m_cand_rect.width()=%d,x=%d,m_ime_info.candidates_on_page=%d,cand_width=%d\n",i,width,x,m_ime_info.candidates_on_page,cand_width);
			commit_selection('1'+i);
			bUpdate=true;
			}
		}
	}
	else if(hit_test_helper(x,y,m_indicator_rect)){
		//switch between English/Chinese mode
		m_bEnglishMode=!m_bEnglishMode;
		bUpdate=true;
	}else if(hit_test_helper(x,y,m_kbdbtn_rect)) {
		m_bShowKeyboard=!m_bShowKeyboard;
		bUpdate=true;
	//	Global::hideInputMethod();
	//	Global::showInputMethod();
	}
	else if(m_bShowKeyboard&&hit_test_helper(x,y,m_kbd_rect)) {
		if(pressTid == 0)
			clearHighlight();
#if defined(Q_WS_QWS)|| defined(_WS_QWS_)
		if(unicode!=-1)	{
			//emit key(unicode,qkeycode,modifiers,false,false);
			repeatTimer->stop();
		}
#endif
		pressed = FALSE;
	}

	if(bUpdate){
		update();
	}
}

void QPinyinFrame::keyPressEvent(QKeyEvent*)
{
}

void QPinyinFrame::SendKey(int u , int c)
{
	QChar mychar(u);
	QString str(mychar);

	QKeyEvent *me=new QKeyEvent(QEvent::KeyPress,c,0,str,false);
	//qDebug("%s %d ",str.ascii(),str.length());
	QApplication::postEvent(App->focusWidget(),me);
}

//PC keyboard layout and scancodes

/*
  Format: length, code, length, code, ..., 0
  
  length is measured in half the width of a standard key.
  If code < 0x80, code gives the ASCII value of the key
  
  If code >= 0x80, the key is looked up in specialM[].
  
 */

static const uchar * const keyboard_opti[5] = { 
    (const uchar *const) "\001\223\003\240\002\20\002\41\002\26\002\62\002\56\002\45\002\54\003\200\001\223\002\226\002\235\002\234\002\236",
    (const uchar *const) "\001\223\003\201\004\207\002\30\002\24\002\43\004\207\003\203\001\223\006\002\002\065",
    (const uchar *const) "\001\223\003\202\002\60\002\37\002\23\002\22\002\36\002\21\002\55\003\203\001\223\006\005\002\055",
    (const uchar *const) "\001\223\003\205\004\207\002\27\002\61\002\40\004\207\003\204\001\223\006\010\002\014",
    (const uchar *const) "\001\223\003\206\002\44\002\31\002\57\002\42\002\46\002\25\002\207\003\204\001\223\002\013\002\064\002\015\002\230"
};


static const uchar * const keyboard_standard[5] = {

#ifdef USE_SMALL_BACKSPACE
    (const uchar *const)"\002\240\002`\0021\0022\0023\0024\0025\0026\0027\0028\0029\0020\002-\002=\002\200\002\223\002\215\002\216\002\217",
#else
    (const uchar *const)"\002\051\0021\0022\0023\0024\0025\0026\0027\0028\0029\0020\002-\002=\004\200\002\223\002\215\002\216\002\217",
#endif
    //~ + 123...+ BACKSPACE //+ INSERT + HOME + PGUP

    (const uchar *const)"\003\201\002q\002w\002e\002r\002t\002y\002u\002i\002o\002p\002[\002]\002\\\001\224\002\223\002\221\002\220\002\222",
    //TAB + qwerty..  + backslash //+ DEL + END + PGDN

    (const uchar *const)"\004\202\002a\002s\002d\002f\002g\002h\002j\002k\002l\002;\002'\004\203",
    //CAPS + asdf.. + RETURN 

    (const uchar *const)"\005\204\002z\002x\002c\002v\002b\002n\002m\002,\002.\002/\005\204\002\223\002\223\002\211",
    //SHIFT + zxcv... //+ UP

    (const uchar *const)"\003\205\003\206\022\207\003\206\003\205\002\223\002\212\002\213\002\214" 
    //CTRL + ALT + SPACE //+ LEFT + DOWN + RIGHT
    
};


struct ShiftMap {
    char normal;
    char shifted;
};


static const ShiftMap shiftMap[] = {
    { '`', '~' },
    { '1', '!' },
    { '2', '@' },
    { '3', '#' },
    { '4', '$' },
    { '5', '%' },
    { '6', '^' },
    { '7', '&' },
    { '8', '*' },
    { '9', '(' },
    { '0', ')' },
    { '-', '_' },
    { '=', '+' },
    { '\\', '|' },
    { '[', '{' },
    { ']', '}' },
    { ';', ':' },
    { '\'', '"' },
    { ',', '<' },
    { '.', '>' },
    { '/', '?' }
};


/* XPM */
static const char * const uparrow_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".........",
"....a....",
"...aaa...",
"..aaaaa..",
"....a....",
"....a....",
"....a....",
"....a....",
"........."};
/* XPM */
static const char * const leftarrow_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".........",
".........",
"...a.....",
"..aa.....",
".aaaaaaa.",
"..aa.....",
"...a.....",
".........",
"........."};
/* XPM */
static const char * const downarrow_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".........",
"....a....",
"....a....",
"....a....",
"....a....",
"..aaaaa..",
"...aaa...",
"....a....",
"........."};
/* XPM */
static const char * const rightarrow_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".........",
".........",
".....a...",
".....aa..",
".aaaaaaa.",
".....aa..",
".....a...",
".........",
"........."};
/* XPM */
static const char * const insert_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".........",
"a........",
"a.aaa.aaa",
"a.a.a.a..",
"a.a.a..a.",
"a.a.a...a",
"a.a.a.aaa",


".........",
"........."};
/* XPM */
static const char * const delete_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".........",
"aa......a",
"a.a.aaa.a",
"a.a.a.a.a",
"a.a.aaa.a.",
"a.a.a...a",
"aaa.aaa.a",
".........",
"........."};
/* XPM */
static const char * const home_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
"....a....",
"...a.a...",
"..a...a..",
".a.....a.",
"aa.aaa.aa",
".a.a.a.a.",
".a.a.a.a.",
".aaaaaaa.",
"........."};
/* XPM */
static const char * const end_xpm[]={
"10 9 2 1",
"a c #000000",
". c None",
"..........",
"aa.......a",
"a..aaa.aaa",
"aa.a.a.a.a",
"a..a.a.a.a",
"a..a.a.a.a",
"aa.a.a.aaa",
"..........",
".........."};
/* XPM */
static const char * const pageup_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".aaa.aaa.",
".a.a.a.a.",
".aaa..aa.",
".a...aaa.",
".........",
".a.a.aaa.",
".a.a.a.a.",
".aaa.aaa.",
".....a..."};
/* XPM */
static const char * const pagedown_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".aaa.aaa.",
".a.a.a.a.",
".aaa..aa.",
".a...aaa.",
".........",
"...a.....",
".aaa.aaa.",
".a.a.a.a.",
".aaa.a.a."};
/* XPM */
static const char * const expand_xpm[]={
"4 9 2 1",
"a c #408040",
". c None",
"a...",
"aa..",
"aaa.",
"aaaa",
"aaaa",
"aaaa",
"aaa.",
"aa..",
"a..."};
/* XPM */
#ifdef USE_SMALL_BACKSPACE
static const char * const backspace_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".........",
".........",
"...a.....",
"..aa.....",
".aaaaaaaa",
"..aa.....",
"...a.....",
".........",
"........."};
#else
static const char * const backspace_xpm[]={
"21 9 2 1",
"a c #000000",
". c None",
".....................",
".....................",
".....aaa..a..........",
".a...a..a.a.a.aaa.aaa",
"aaaa.aaa..aa..aa..a.a",
".a...a..a.aaa..aa.a.a",
".....aaaa.a.a.aaa.aa.",
"..................a..",
"....................."};
#endif
/* XPM */
static const char * const escape_xpm[]={
"9 9 2 1",
"a c #000000",
". c None",
".........",
".........",
".aa.aa.aa",
".a..a..a.",
".aa.aa.a.",
".a...a.a.",
".aa.aa.aa",
".........",
"........."};


enum { BSCode = 0x80, TabCode, CapsCode, RetCode, 
       ShiftCode, CtrlCode, AltCode, SpaceCode, BackSlash,
       UpCode, LeftCode, DownCode, RightCode, Blank, Expand,
       Opti, ResetDict,
       Divide, Multiply, Add, Subtract, Decimal, Equal,
       Percent, Sqrt, Inverse, Escape };

typedef struct SpecialMap {
    int qcode;
    ushort unicode;
    const char * label;
    const char * const *xpm;
};


static const SpecialMap specialM[] = {
    {	Qt::Key_Backspace,	8,	"<",     backspace_xpm },
    {	Qt::Key_Tab,		9,	"Tab",   NULL }, // No tr
    {	Qt::Key_CapsLock,	0xffff,	"Caps",  NULL }, // No tr
    {	Qt::Key_Return,		13,	"Ret",   NULL }, // No tr
    {	Qt::Key_Shift,		0xffff,	"Shift", NULL }, // No tr
    {	Qt::Key_Control,	0xffff,	"Ctrl",  NULL }, // No tr
    {	Qt::Key_Alt,		0xffff,	"Alt",   NULL }, // No tr
    {	Qt::Key_Space,		' ',	"",      NULL },
    {	BackSlash,		43,	"\\",    NULL },

    // Need images?
    {	Qt::Key_Up,		0xffff,	"^",     uparrow_xpm },
    {	Qt::Key_Left,		0xffff,	"<",     leftarrow_xpm },
    {	Qt::Key_Down,		0xffff,	"v",     downarrow_xpm },
    {	Qt::Key_Right,		0xffff,	">",     rightarrow_xpm },
    {	Qt::Key_Insert,		0xffff,	"I",     insert_xpm },
    {	Qt::Key_Home,		0xffff,	"H",     home_xpm },
    {	Qt::Key_PageUp,		0xffff,	"U",     pageup_xpm },
    {	Qt::Key_End,		0xffff,	"E",     end_xpm },
    {	Qt::Key_Delete,		0xffff,	"X",     delete_xpm },
    {	Qt::Key_PageDown,	0xffff,	"D",     pagedown_xpm },
    {	Blank,			0,	" ",     NULL },
    {	Expand,			0xffff,	"->",    expand_xpm },
    {	Opti,			0xffff,	"#",     NULL },
    {	ResetDict,		0xffff,	"R",     NULL },
   
    // number pad stuff
    {	Divide,			0,	"/",     NULL },
    {	Multiply,		0,	"*",     NULL },
    {	Add,			0,	"+",     NULL },
    {	Subtract,		0,	"-",     NULL },
    {	Decimal,		0,	".",     NULL },
    {	Equal,			0,	"=",     NULL },
    {	Percent,		0,	"%",     NULL },
    {	Sqrt,			0,	"^1/2",	 NULL },
    {	Inverse,		0,	"1/x",	 NULL },

    {	Escape,			27,	"ESC",	 escape_xpm }
};


/*
  return scancode and width of first key in row \a j if \a j >= 0,
  or next key on current row if \a j < 0.
  
*/
int useOptiKeys=0;
int getKey( int &w, int j ) {
    static const uchar *row = 0;
    static int key_i = 0;
    static int scancode = 0;
    static int half = 0;
    
    if ( j >= 0 && j < 5 ) {
	if (useOptiKeys)
    	    row = keyboard_opti[j];
	else
    	    row = keyboard_standard[j];
	half=0;
    }

    if ( !row || !*row ) {
	return 0;    
    } else if ( row[1] >= 0x80 ) {
	scancode = row[1];
	w = (row[0] * w + (half++&1)) / 2;
	row += 2;
	return scancode;
    } else if ( key_i <= 0 ) {
	key_i = row[0]/2;
	scancode = row[1];
    }
    key_i--;
    if ( key_i <= 0 )
	row += 2;
    return scancode++;
}

static int keycode( int i2, int j, const uchar **keyboard )
{
    if ( j <0 || j >= 5 )
        return 0;
                                                                                
    const uchar *row = keyboard[j];
                                                                                
    while ( *row && *row <= i2 ) {
        i2 -= *row;
        row += 2;
    }
                                                                                
    if ( !*row ) return 0;
                                                                                
    int k;
    if ( row[1] >= 0x80 ) {
        k = row[1];
    } else {
        k = row[1]+i2/2;
    }
                                                                                
    return k;
}

    
/*
  Draw the keyboard.

  If key >= 0, only the specified key is drawn.
*/
void QPinyinFrame::drawKeyboard( QPainter &p, int key )
{
    const bool threeD = FALSE;
   // const QPalette& cg = QPalette();
    QColor keycolor = // cg.background();
		    	QColor(0,0,0);//QColor(240,240,230); // Beige! White
    QColor keycolor_pressed = QColor(240,0,0);//cg.mid(); Red
    QColor keycolor_lo = QColor(255,255,255);//QColor(0,240,0);//cg.dark(); GreenQColor(255,255,255);//
    QColor keycolor_hi = QColor(0,0,230);//cg.light();Blue
    QColor textcolor =  QColor(255,255,255);//QColor(0,0,0); // cg.text(); Dark

    int margin = threeD ? 1 : 0;
    
//    p.fillRect( 0, , kw-1, keyHeight-2, keycolor_pressed );

    for ( int j = 0; j < 5; j++ ) {
	int y = j * keyHeight + PickHeight + 1;
	int x = xoffs;
	int kw = defaultKeyWidth;
	int k= getKey( kw, j );
	while ( k ) {
	    if ( key < 0 || k == key ) {
		QString s;
		bool pressed = (k == pressedKey);
		bool blank = (k == 0223);
		const char * const * xpm = NULL;
		
		if ( k >= 0x80 ) {
		    s = specialM[k - 0x80].label;

		    xpm = specialM[k - 0x80].xpm;
			
		    if ( k == ShiftCode ) {
			pressed = shift;
		    } else if ( k == CapsCode ) {
			pressed = lock;
		    } else if ( k == CtrlCode ) {
			pressed = ctrl;
		    } else if ( k == AltCode ) {
			pressed = alt;
		    } 
		} else {
#if defined(Q_WS_QWS) || defined(_WS_QWS_)
/*
		    s = QChar( shift^lock ? QWSServer::keyMap()[k].shift_unicode : 
			       QWSServer::keyMap()[k].unicode);
*/
		    // ### Fixme, bad code, needs improving, whole thing needs to
		    // be re-coded to get rid of the way it did things with scancodes etc
		    char shifted = k;
		    if ( !isalpha( k ) ) {
			for ( unsigned i = 0; i < sizeof(shiftMap)/sizeof(ShiftMap); i++ )
			    if ( shiftMap[i].normal == k )
				shifted = shiftMap[i].shifted;
		    } else {
			shifted = toupper( k );
		    }
		    s = QChar( shift^lock ? shifted : k );
#endif
		}

		if (!blank) {
		    if ( pressed )
		    {
			p.fillRect( x+margin, y+margin, kw-margin, keyHeight-margin-1, keycolor_pressed );
		    }
		    else
		    {
			p.fillRect( x+margin, y+margin, kw-margin, keyHeight-margin-1, keycolor );
		    }	
		    if ( threeD ) {
			p.setPen(pressed ? keycolor_lo : keycolor_hi);
			p.drawLine( x, y+1, x, y+keyHeight-2 );
			p.drawLine( x+1, y+1, x+1, y+keyHeight-3 );
			p.drawLine( x+1, y+1, x+1+kw-2, y+1 );
		    } else if ( j == 0 ) {
			p.setPen(pressed ? keycolor_hi : keycolor_lo);
			p.drawLine( x, y, x+kw, y );
		    }

		    // right
		    p.setPen(pressed ? keycolor_hi : keycolor_lo);
		    p.drawLine( x+kw-1, y, x+kw-1, y+keyHeight-2 );

		    if ( threeD ) {
			p.setPen(keycolor_lo.light());
			p.drawLine( x+kw-2, y+keyHeight-2, x+kw-2, y+1 );
			p.drawLine( x+kw-2, y+keyHeight-2, x+1, y+keyHeight-2 );
		    }

		    if (xpm) {			
			p.drawPixmap( x + 1, y + 2, QPixmap((const char**)xpm) );
		    } else {
    			p.setPen(textcolor);
			p.drawText( x - 1, y, kw, keyHeight-2, Qt::AlignCenter, s );
		    }
	    
		    if ( threeD ) {
			p.setPen(keycolor_hi);
			p.drawLine( x, y, x+kw-1, y );
		    }

		    // bottom
		    p.setPen(keycolor_lo);
		    p.drawLine( x, y+keyHeight-1, x+kw-1, y+keyHeight-1 );
	    
		} else {
		    p.fillRect( x, y, kw, keyHeight, QColor(0,0,0)/*cg.background()*/ );
		}
	    }

	    x += kw;
	    kw = defaultKeyWidth;
	    k = getKey( kw ,-1);
	}
    }
}


void QPinyinFrame::clearHighlight()
{
    if (m_bShowKeyboard&&( pressedKey >= 0) ) {
	int tmp = pressedKey;
	pressedKey = -1;
	QPainter p(this);
	//drawKeyboard( p, tmp );
	update();
    }
}

void QPinyinFrame::timerEvent(QTimerEvent*e)
{
    if ( e->timerId() == pressTid ) {
	QFrame::killTimer(pressTid);
        pressTid = 0;
        if (!pressed )
	{
            clearHighlight();
	}
    }
}

void QPinyinFrame::repeat()
{
    repeatTimer->start( 1000 );
    if(m_bEnglishMode) {
    //	emit key( unicode, qkeycode, modifiers, true, true );
    } else
	GetKey(unicode,qkeycode);
}

void QPinyinFrame::mousePressEvent(QMouseEvent *e) 
{
    if(!m_bShowKeyboard) return;
    printX86("(%d,%d)-height=%d\n",e->x(),e->y(),PickHeight);
    if(e->y()<=PickHeight) return;                                                                        
    clearHighlight(); // typing fast?

    int i2 = ((e->x() - xoffs) * 2) / defaultKeyWidth;
    int j = (e->y() - PickHeight) / keyHeight;
                                                                                
    int k = keycode( i2, j, (const uchar **)((useOptiKeys) ? keyboard_opti : keyboard_standard) );
    bool need_repaint = FALSE;
    unicode = -1;
    qkeycode = 0;
    printX86("k=%d\n",k);
    if ( k >= 0x80 ) {
        if ( k == ShiftCode ) {
            shift = !shift;
	    if(!m_bEnglishMode)
	    {
	    	GetKey(unicode,Qt::Key_Shift);
	    }
            need_repaint = TRUE;
        } else if ( k == AltCode ){
            alt = !alt;
            need_repaint = TRUE;
        } else if ( k == CapsCode ) {
            lock = !lock;
            need_repaint = TRUE;
        } else if ( k == CtrlCode ) {
            ctrl = !ctrl;
	     need_repaint = TRUE;
        } else if ( k == 0224 /* Expand */ ) {
            useLargeKeys = !useLargeKeys;
            resizeEvent(0);
            update(); // need it to clear first
        } else if ( k == 0225 /* Opti/Toggle */ ) {
            useOptiKeys = !useOptiKeys;
            resizeEvent(0);
            update(); // need it to clear first
        } else {
            qkeycode = specialM[ k - 0x80 ].qcode;
            unicode = specialM[ k - 0x80 ].unicode;
        }
    } else {
        //due to the way the keyboard is defined, we know that
        //k is within the ASCII range, and can be directly mapped to
        //a qkeycode; except letters, which are all uppercase
        qkeycode = toupper(k);
        if ( shift^lock ) {
            if ( !isalpha( k ) ) {
            for ( unsigned i = 0; i < sizeof(shiftMap)/sizeof(ShiftMap); i++ )
                if ( shiftMap[i].normal == k ) {
                    unicode = shiftMap[i].shifted;
           qkeycode = unicode;
                    break;
                }
            } else {
                unicode = toupper( k );
            }
        } else {
            unicode = k;
        }
    }
    if  ( unicode != -1 ) {
        if ( ctrl && unicode >= 'a' && unicode <= 'z' )
            unicode = unicode - 'a'+1;
        modifiers = (shift ? Qt::Key_Shift : 0) | (ctrl ? Qt::Key_Control :
0) |
                  (alt ? Qt::Key_Alt : 0);
#if defined(Q_WS_QWS) || defined(_WS_QWS_)
	if(m_bEnglishMode) {
//	        emit key( unicode, qkeycode, modifiers, true, false );
//			if(unicode == 9 && qkeycode == Qt::Key_Tab){
			if(unicode == 0xffff && qkeycode == Qt::Key_Control){
				m_bEnglishMode = false;
				update();			
			}
			else if(Qt::Key_F1<=qkeycode && qkeycode<=Qt::Key_F12)
			{

			}
			else{
				SendKey(unicode,qkeycode); 
			}
		}
	 else 
	 {
		GetKey(unicode,qkeycode);
	 }
       // repeatTimer->start( 500 );
#endif
        need_repaint = shift || alt || ctrl;
        shift = alt = ctrl = FALSE;
	/*KeyboardConfig *dc = picks->dc;
                                                                                
        if (dc) {
            if (qkeycode == Qt::Key_Backspace) {
                dc->input.remove(dc->input.last()); // remove last input
                dc->decBackspaces();
            } else if ( k == 0226 || qkeycode == Qt::Key_Return ||
                        qkeycode == Qt::Key_Space ||
                        QChar(unicode).isPunct() ) {
                dc->input.clear();
                dc->resetBackspaces();
            } else {
                dc->add(QString(QChar(unicode)));
                dc->incBackspaces();
            }
        }*/
    }
    pressedKey = k;
    if ( need_repaint ) {
        
    } else {
        QPainter p(this);
        drawKeyboard( p, pressedKey );
    }

    pressTid = this->QFrame::startTimer(80);
    pressed = TRUE;
}


/*QSize QPinyinFrame::sizeHint() const
{
    QFontMetrics fm=fontMetrics();
    int keyHeight = fm.lineSpacing()+2;

    if (useOptiKeys)
    	keyHeight += 1;
    
    return QSize( 320, keyHeight * 5 + 32);
}*/


/*
 * Revision history
 * 
 * $Log: PinyinFrame.cpp,v $
 * Revision 1.7  2004/07/31 17:20:19  Lu Mu
 * support chinese punctuations
 *
 * Revision 1.6  2004/07/20 11:26:05  Lu Mu
 * (1)phrase frequency
 * (2)self define phrase
 *
 * Revision 1.5  2004/07/17 07:11:54  Lu Mu
 * 1)phrase support
 * 2)candidates sequence number,changed to 1,2,..,9,0
 * 3)space key to select first candidate
 *
 * Revision 1.4  2004/07/15 13:56:17  Lu Mu
 * fixed : hanzi candidates overlapped by system task bar while default font is not efont
 *
 * Revision 1.3  2004/07/10 16:58:11  Lu Mu
 * bug fix of displaying hanzi problem after the first page
 *
 * Revision 1.2  2004/07/10 15:02:23  Lu Mu
 * v0.0.1 released
 * TODO: phase support
 *
 * Revision 1.1.1.1  2004/07/07 16:24:13  Lu Mu
 * Created
 *
 *
 */
