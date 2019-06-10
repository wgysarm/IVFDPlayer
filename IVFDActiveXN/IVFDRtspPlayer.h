#ifndef IVFDRTSPPLAYER_H
#define IVFDRTSPPLAYER_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDebug>
#include <QTimer>


extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
	//2017.8.9---lizhen
#include "libavutil/time.h"
#include "libavutil/mathematics.h"

}

#include <stdio.h>
#include<iostream>

class IVFDRtspPlayer : public QObject
{
	Q_OBJECT

public:
	IVFDRtspPlayer(QObject *parent);
	explicit IVFDRtspPlayer(QObject *parent, QString url, qlonglong timeStamp);
	~IVFDRtspPlayer();
	// void setFileName(QString path){mFileName = path;}
	QThread *m_rtspThread;
	QTimer *m_timer;
	QString m_url;
	qlonglong m_timeStamp;

signals:
	void SIG_GetOneFrame(QImage); //没获取到一帧图像 就发送此信号
	void SIG_StartToPlay0();
	void SIG_StartToPlay1();
	void SIG_StartTryPlay();
	//void SIG_ThreadFinished();


public slots:
	void SLOT_StartToPlay();
	void SLOT_StartToPlay1();
	void SLOT_StartTryPlay();
	
private:
	QString mFileName;
};

#endif // IVFDRTSPPLAYER_H
