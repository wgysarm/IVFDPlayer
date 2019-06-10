#ifndef IVFDTCPSERVER_H
#define IVFDTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QLocalSocket>
#include <QLocalServer>
#include <QSocketNotifier>
#include <QSsl>
#include <QSslSocket>
#include <QUdpSocket>
#include <QThread>
#include <QByteArray>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QThreadPool>
#include <QTimer>


//typedef struct ALARMDATAINFO{
//	int alarmType;
//	long alarmTime;
//	int pTOPLx;
//	int pTOPLy;
//	int recHeight;
//	int recWidth;
//}ALARMDATAINFO;


class IVFDTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit IVFDTcpServer(QObject *parent = 0, QString ipAddr=QString("192.168.120.166"), quint16 tcpPort=59000);
    QTcpServer *m_tcpserver;
    QTcpSocket *m_tcpsocket;
    QThread *m_tcpserverThread;
    QHostAddress m_hostAddr;
    quint16 m_port;
    QByteArray m_ba;
	QTimer *m_timer;
	int m_count;

    ~IVFDTcpServer();
    void InitThis(QString ipaddr, quint16 portn);
    void XReadAll(QByteArray);
signals:
    void SIG_StartTcpServerThread();
    void SIG_StopTcpServerThread();
    void SIG_WriteToTcpSocket(QByteArray);
    void SIG_TellTop(int, QByteArray);// output signal to tell the top
	void SIG_GetStatus();
	void SIG_UpdateServerIP();

public slots:

    void SLOT_StartTcpServerThread();
    void SLOT_StopTcpServerThread();
    void SLOT_HasNewConnection();
    void SLOT_ReadyReadAll();
    void SLOT_WriteToTcpSocket(QByteArray);
    void SLOT_SocketError(QAbstractSocket::SocketError);
	void SLOT_TellStatus();
	void SLOT_TimeOut();
	void SLOT_GetServerIP();

};

#endif // IVFDMAINTCPSERVER_H
