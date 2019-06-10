#ifndef CHANELWIDGET_H
#define CHANELWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QImage>
#include <QPaintEvent>
#include <QByteArray>
#include <QTimer>
#include <QDateTime>
#include <QPainter>
#include <QFont>
#include <QPixmap>
#include <QStringList>
#include <QRect>
#include <QList>
#include <QDateTime>

#include "IVFDRtspPlayer.h"
#include "ui_ChanelWidget.h"
//#include "IVFDTcpServer.h"

namespace Ui {
	class Form;
}


typedef struct ALARMDATAINFO
{
	int alarmType;
	long alarmTime;
	int pTOPLx;
	int pTOPLy;
	int recHeight;
	int recWidth;
}ALARMDATAINFO;

class ChanelWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ChanelWidget(QWidget *parent, QString url, qlonglong createTime/*, quint16 tcpPort*/);
	ChanelWidget(QWidget *parent = 0);
	~ChanelWidget();
	QPushButton *pb;
	qlonglong m_createTime;
	qlonglong m_deltaTime;

	IVFDRtspPlayer *m_player;

	QImage mImage; //记录当前的图像
	QString m_CurrentSnap;


	QString m_url;

	static QString GetFrame_TimeStamp();
	QString m_msgAlgProcess;
	int m_typeAlgProcess;
	int m_count;
	int m_reverseFlag;
	int m_onwatchFlag;
	bool m_alarmComeFlag;
	bool m_modeTenFlag;
	bool m_calibrateFlag;
	int m_frameCounter;
	int m_flagsnap;
	QList<QRect> m_recList;
	int m_offsetX;
	int m_offsetY;
	ALARMDATAINFO *m_alarmData;

	void SetOffsetXY(int offSetX, int offSetY);

protected:
	void paintEvent(QPaintEvent *event);
	void closeEvent();

private:
	Ui::Form *ui;
signals:
	void SIG_StartPlay();
	void SIG_RtspDisconnected();
	void SIG_DoAlgProcessMsg(int, QByteArray);
	void SIG_Test(int);
	void SIG_ClearAlarm();
	void SIG_OffsetXY(int, int);
	void SIG_UpdateOnWatch(int);
	void SIG_Snap(int);
	void SIG_NotifyChannel();
	//void SIG_GetAlgOutFromTcpServer(int, QByteArray);

	private slots:
		void slotGetOneFrame(QImage img);
		void SLOT_Play();
		void SLOT_RtspThreadQuit();
		void SLOT_TimerOut();
		void SLOT_ClearAlarm();
		void SLOT_Snap(int);
		void SLOT_NotifyChannel();
		//void SLOT_GetAlgOutFromTcpServer(int, QByteArray);
		//void SLOT_StartTcpServer();

public slots:
	void SLOT_DoAlgProcessMsg(int, QByteArray);
	void SLOT_Test(int);
	void SLOT_OffsetXY(int, int);
	void SLOT_UpdateOnWatch(int);
};

#endif // CHANELWIDGET_H
