#include "ChanelWidget.h"
#include <QPushButton>
#include <QString>
#include <QPainter>
#include <QInputDialog>
#include <QDebug>
#include <QThread>
#include <iostream>
#include <QFont>
#include <QColor>
#include <QRgb>


static void XParseAlarmInfo(QByteArray msg)
{


}



ChanelWidget::ChanelWidget(QWidget *parent)
	: QWidget(parent), ui(new Ui::Form)
{
    ui->setupUi(this);
	m_player = new IVFDRtspPlayer(0);


	m_alarmData = (ALARMDATAINFO *)malloc(sizeof(ALARMDATAINFO));
	m_alarmData->alarmTime = 0;
	m_alarmData->alarmType = 0;
	m_alarmData->pTOPLx = 50;
	m_alarmData->pTOPLy = 50;
	m_alarmData->recHeight = 100;
	m_alarmData->recWidth = 100;
	m_recList << QRect(0,0,-1,-1);

	QObject::connect(m_player,SIGNAL(SIG_GetOneFrame(QImage)),this,SLOT(slotGetOneFrame(QImage)),Qt::QueuedConnection);	
	QObject::connect(this, SIGNAL(SIG_StartPlay()), m_player, SIGNAL(SIG_StartToPlay0()), Qt::QueuedConnection);
	QObject::connect(m_player->m_rtspThread, SIGNAL(finished()), this, SLOT(SLOT_RtspThreadQuit()), Qt::QueuedConnection);
	//QObject::connect(m_tcpserver, SIGNAL(SIG_TellTop(int, QByteArray)), this, SIGNAL(SIG_GetAlgOutFromTcpServer(int, QByteArray)),Qt::QueuedConnection);
	//QObject::connect(this, SIGNAL(SIG_GetAlgOutFromTcpServer(int, QByteArray)), this, SLOT(SLOT_GetAlgOutFromTcpServer(int, QByteArray)));
	QObject::connect(this, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)), this, SLOT(SLOT_DoAlgProcessMsg(int, QByteArray)));

	SLOT_Play();
}


ChanelWidget::ChanelWidget(QWidget *parent, QString url, qlonglong createTime/*, quint16 tcpPort*/)
	: QWidget(parent), ui(new Ui::Form)
{
	ui->setupUi(this);
	qlonglong stamp = createTime;
	m_createTime = createTime;
	qDebug() << "createTime = " << createTime;
	QString getFrameTime = GetFrame_TimeStamp();
	QStringList timelist = getFrameTime.split("#");
	QString timeval = timelist.at(0);

	m_typeAlgProcess = 5;
	m_msgAlgProcess = QString(" no any algout!");
	m_count = 0;
	m_reverseFlag = Qt::Unchecked;
	m_onwatchFlag = 0;
	m_alarmComeFlag = false;
	m_modeTenFlag = false;
	m_calibrateFlag = false;
	m_flagsnap = 0;

	m_alarmData = (ALARMDATAINFO *)malloc(sizeof(ALARMDATAINFO));
	m_alarmData->alarmTime = 0;
	m_alarmData->alarmType = 0;
	m_alarmData->pTOPLx = 50;
	m_alarmData->pTOPLy = 50;
	m_alarmData->recHeight = 100;
	m_alarmData->recWidth = 100;
	m_frameCounter = 0;

	m_offsetX = 0;
	m_offsetY = 0;

	qDebug() << "current local timestamp val = " << timeval << "----" << timeval.toLongLong()*1000000;
	m_deltaTime = m_createTime - timeval.toLongLong()*1000000;
	qDebug() << QString::number(m_deltaTime);
	qDebug() << "m_createTime = " << m_createTime << " m_deltaTime = " << m_deltaTime; 
	m_player = new IVFDRtspPlayer(0 ,url, stamp);

	QObject::connect(m_player,SIGNAL(SIG_GetOneFrame(QImage)),this,SLOT(slotGetOneFrame(QImage)),Qt::QueuedConnection);	
	QObject::connect(this, SIGNAL(SIG_StartPlay()), m_player, SIGNAL(SIG_StartToPlay0()), Qt::QueuedConnection);
	QObject::connect(m_player->m_rtspThread, SIGNAL(finished()), this, SLOT(SLOT_RtspThreadQuit()), Qt::QueuedConnection);
	//QObject::connect(this, SIGNAL(SIG_GetAlgOutFromTcpServer(int, QByteArray)), this, SLOT(SLOT_GetAlgOutFromTcpServer(int, QByteArray)));
	QObject::connect(this, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)), this, SLOT(SLOT_DoAlgProcessMsg(int, QByteArray)));
	QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_Test(int)));
	QObject::connect(this, SIGNAL(SIG_ClearAlarm()), this, SLOT(SLOT_ClearAlarm()));
	QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), this, SLOT(SLOT_OffsetXY(int, int)));
	QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), this, SLOT(SLOT_UpdateOnWatch(int)));
	QObject::connect(this, SIGNAL(SIG_Snap(int)), this, SLOT(SLOT_Snap(int)));
	QObject::connect(this, SIGNAL(SIG_NotifyChannel()), this, SLOT(SLOT_NotifyChannel()));

	SLOT_Play();
	//SLOT_StartTcpServer();
}

ChanelWidget::~ChanelWidget()
{
	//if(m_player != NULL)
	//{

	//	delete m_player;
	//}
	//if(m_alarmData != NULL)
	//{
	//	free(m_alarmData);
	//	m_alarmData = NULL;
	//}

}

//void ChanelWidget::SLOT_StartTcpServer()
//{
//	//emit m_tcpserver->SIG_StartTcpServerThread();
//}
void ChanelWidget::paintEvent(QPaintEvent *event)
{
	QPainter m_painter(this);
	QFont m_font;
	m_font.setPointSize(16);
	m_painter.setFont(m_font);
	QPixmap bgPic;
	bgPic.load(":/image/dark.png");
	m_painter.drawPixmap(0, 0, this->width(), this->height(), bgPic);
	if (mImage.size().width() <= 0) return;

	quint16 m_width = mImage.size().width();
	quint16 m_heigth = mImage.size().height();
	///将图像按比例缩放成和窗口一样大小
	if(m_alarmComeFlag/* && m_modeTenFlag*/)
	{
		//对img进行报警框的处理
		int t_width = mImage.width();
		int t_height = mImage.height();
		int tplx = (m_alarmData->pTOPLx + m_offsetX);
		int tply = (m_alarmData->pTOPLy + m_offsetY);
		int recWidth = m_alarmData->recWidth;
		int recHeight = m_alarmData->recHeight;

		QPainter t_painter(&mImage);
		QPen t_pen;
		QFont t_font;

		t_pen.setStyle(Qt::SolidLine);
		QColor mycolor = mImage.pixel(tplx+recWidth/2,tply+recHeight/2);
		QRgb myrgb = qRgb(255-mycolor.red(),255-mycolor.green(),255-mycolor.blue());
		QColor invertclolr = QColor(myrgb);
		t_pen.setColor(invertclolr);
		t_pen.setWidth(5);

		t_font.setPointSize(48);
		t_painter.setFont(t_font);
		t_painter.setPen(t_pen);

		t_painter.drawEllipse(tplx, tply, recWidth, recHeight);
		t_painter.drawLine(tplx+recWidth/2-40,tply+recHeight/2,tplx+recWidth/2+40,tply+recHeight/2);
		t_painter.drawLine(tplx+recWidth/2,tply+recHeight/2-40,tplx+recWidth/2,tply+recHeight/2+40);
		t_painter.end();
	}
	if(m_calibrateFlag){
		QPen pen;
		pen.setStyle(Qt::SolidLine);
		pen.setColor(Qt::red);
		pen.setWidth(4);
		QPainter c_painter(&mImage);
		c_painter.setPen(pen);
		c_painter.drawRect(this->x(), this->y(), this->width()-2, this->height()-2);
		c_painter.drawRect(960, 540, 100, 100);
	}
	if(m_reverseFlag == Qt::Checked)
	{
		mImage = mImage.mirrored(false, true);
	}

	//if(m_onwatchFlag == 0){
	//	QPainter t_painter(&mImage);
	//	QPen t_pen;
	//	QFont t_font;
	//	t_pen.setStyle(Qt::SolidLine);
	//	t_painter.begin(this);
	//	t_painter.setPen(QPen(Qt::blue,4,Qt::SolidLine));//设置画笔形式 
	//	t_painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));//设置画刷形式 
	//	t_painter.drawEllipse(1800,30,48,48);

	//	t_painter.end();
	//}else{
	//	QPainter t_painter(&mImage);
	//	QPen t_pen;
	//	QFont t_font;

	//	t_pen.setStyle(Qt::SolidLine);
	//	t_painter.begin(this);
	//	t_painter.setPen(QPen(Qt::blue,4,Qt::SolidLine));//设置画笔形式 
	//	t_painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));//设置画刷形式 
	//	t_painter.drawEllipse(1800,30,48,48);

	//	t_painter.end();
	//}
	
	QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);
	int x = this->width() - img.width();
	int y = this->height() - img.height();
	x /= 2;
	y /= 2;
	float mscale;
	if(m_width == 1280)
	{
		mscale = img.width()/1280.0;
	}else if(m_width == 1920)
	{
		mscale = img.width()/1920.0;
	}

	m_painter.drawImage(QPoint(x,y),img); //画出图像

	int tplx = (m_alarmData->pTOPLx + m_offsetX);
	int tply = (m_alarmData->pTOPLy + m_offsetY);
	int recWidth = m_alarmData->recWidth;
	int recHeight = m_alarmData->recHeight;

	QString timeStamp = GetFrame_TimeStamp();

	QStringList qlstr = timeStamp.split("#");
	qlonglong curStamp = qlstr.at(1).toLongLong()*1000000 + m_deltaTime;

	if(m_alarmComeFlag/* && m_modeTenFlag*/)
	{
		//2017.8.7---wanggy
		QPen pen;
		pen.setStyle(Qt::DashDotLine);
		pen.setColor(Qt::red);
		pen.setWidth(1);
		//painter.setPen(QPen(Qt::red,1));
		m_painter.setPen(pen);

		pen.setStyle(Qt::SolidLine);
		pen.setColor(Qt::red);
		pen.setWidth(2);
		m_painter.setPen(pen);
		m_painter.drawRect(this->x(), this->y(), this->width()-2, this->height()-2);
		m_painter.drawRect(this->x()+ (this->width()-img.width())/2 + tplx*mscale, this->y()+(this->height()-img.height())/2 + tply*mscale, recWidth*mscale, recHeight*mscale);
	}

	m_painter.end();

	QWidget::paintEvent(event);
}

void ChanelWidget::slotGetOneFrame(QImage img)
{
	mImage = img;
	m_frameCounter++;
	if(m_frameCounter < 360)
	{
		if((m_frameCounter % 90 < 45))
		{
			m_modeTenFlag = true;
		}else{
			m_modeTenFlag = false;
		}
	}else
	{
		m_frameCounter = 0;
	}

	if(m_flagsnap == 1){
		m_flagsnap = 0;
		qDebug() << "Save a image";
		qDebug() << "Save success" << mImage.save("D:\\develop_c++\\ffmpeg2010good\\IVFDPlayer\\IVFDActiveXN\\Win32\\Debug\\record\\image.jpg", "JPG", 100);

	}

	update(); //调用update将执行 paintEvent函数
}

void ChanelWidget::SetOffsetXY(int offSetX, int offSetY)
{
	m_offsetX = offSetX;
	m_offsetY = offSetY;
}

void ChanelWidget::SLOT_OffsetXY(int type, int val)
{
	switch (type)
	{
	case 0:
		{
			m_offsetX += 10;
			break;
		}
	case 1:
		{
			m_offsetX -= 10;
			break;
		}
	case 2:
		{
			m_offsetY += 10;
			break;
		}
	case 3:
		{
			m_offsetY -= 10;
			break;
		}
	default:
		{
			m_offsetX +=0;
			m_offsetY +=0;
		}
	SetOffsetXY(m_offsetX, m_offsetY);

	}


}
//void MainWindow::SLOT_Replay()
//{
//
//	qDebug() << "the mPlayer is quit!";
//QThread::sleep(10000);

//if(mPlayer == NULL)
//{
//	return;
//}
//if(mPlayer->isRunning())
//{
//	qDebug() << "mPlayer didnt release";
//	return;
//}

//mPlayer->startPlay();


//}

void ChanelWidget::SLOT_Play()
{
	emit SIG_StartPlay();

}


void ChanelWidget::SLOT_RtspThreadQuit()
{
	qDebug() << "----------in mainwindow----ChanelWidget::SLOT_RtspThreadQuit()--" << QThread::currentThread();

	QThread::msleep(1000);

	emit SIG_RtspDisconnected();


}

void ChanelWidget::SLOT_TimerOut()
{

	qDebug() << "----------";
}

QString ChanelWidget::GetFrame_TimeStamp()
{
	QDateTime time = QDateTime::currentDateTime();
	QDateTime utcTime = time.toUTC();
	quint64 secs = time.toMSecsSinceEpoch();
	quint64 ucDt = utcTime.toMSecsSinceEpoch();

	QDateTime testtime = QDateTime::fromMSecsSinceEpoch(1521036578065);
	QString testStr = testtime.toString("yyyy-MM-dd hh:mm:ss dddd");

	QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
	QString utcStr = utcTime.toString("yyyy-MM-dd hh:mm:ss dddd");

	QString ret1 = QString::number(ucDt);
	QString ret2 = QString::number(secs);

	return QString(ret1 + QString("#") + ret2);


}

void ChanelWidget::SLOT_DoAlgProcessMsg(int type, QByteArray ba)
{
	//qDebug() << " ChanelWidget::SLOT_GetAlgProcessMsg";
	m_typeAlgProcess = type;
	//m_msgAlgProcess = QString(ba);
	m_count++;

	int temp = m_count %10;
	ALARMDATAINFO *myRecvData = (ALARMDATAINFO *)ba.data();
	myRecvData->pTOPLx = (myRecvData->pTOPLx - temp) < 0 ? 0:(myRecvData->pTOPLx - temp); 
	myRecvData->pTOPLy = (myRecvData->pTOPLy - temp) < 0 ? 0:(myRecvData->pTOPLy - temp);
	
	//myRecvData->recWidth += 1;//这里容易宕机，是什么原因。会把t3也搞死。
	//myRecvData->recHeight += 1;


	//(myRecvData->recWidth + myRecvData->pTOPLx + temp) > 1280 ? 1280:(myRecvData->recWidth + myRecvData->pTOPLx + temp);
	//(myRecvData->recHeight+ myRecvData->pTOPLy + temp) > 720 ? 720:(myRecvData->recHeight+ myRecvData->pTOPLy + temp);
	//m_alarmData->alarmTime = myRecvData->alarmTime;
	if(myRecvData->alarmType == 3)
	{
		memcpy(m_alarmData, myRecvData, sizeof(ALARMDATAINFO));
		m_alarmComeFlag = true;
	}else if(myRecvData->alarmType == 6)//enum EOUTCOMETYPE{NOEVENT, TOOLIGHT, TOODARK, FIREDET, FIREDETEND, VLOST, RUNSTATUS};
	{

	};



	//memcpy(m_alarmData, myRecvData, sizeof(ALARMDATAINFO));
	//m_alarmData->alarmTime = myRecvData->alarmTime;
	//m_alarmData->alarmType = myRecvData->alarmType;
	//m_alarmData->pTOPLx = myRecvData->pTOPLx;
	//m_alarmData->pTOPLy = myRecvData->pTOPLy;
	//m_alarmData->recHeight = myRecvData->recHeight;
	//m_alarmData->recWidth = myRecvData->recWidth;

	//update();
}
//
//void ChanelWidget::SLOT_GetAlgOutFromTcpServer(int type, QByteArray ba)
//{
//
//	emit SIG_GetAlgProcessMsg(type, QString(ba));
//}

void ChanelWidget::closeEvent()
{
	//m_tcpserver->deleteLater();
	//if(m_tcpserver != NULL)
	//{

	//	m_tcpserver->m_tcpsocket->close();
	//	m_tcpserver->m_tcpserver->close();
	//}


}

void ChanelWidget::SLOT_Test(int flag)
{
	m_reverseFlag = flag;
}


void ChanelWidget::SLOT_ClearAlarm()
{
	m_alarmComeFlag = false;

}

void ChanelWidget::SLOT_UpdateOnWatch(int val)
{
	m_onwatchFlag = val;

}

void ChanelWidget::SLOT_Snap(int chanel)
{
	qDebug() << chanel;
	m_flagsnap = 1;

}
void ChanelWidget::SLOT_NotifyChannel()
{
	qDebug() << "got notify!";
	m_calibrateFlag = true;
}