#ifndef IVFDTCPCLIENT_H
#define IVFDTCPCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QThread>
#include <QDataStream>
#include <QByteArray>
#include <QEventLoop>
#include <QTimer>
#include <QRunnable>
//#include "IVFDMainWidget.h"
class IVFDTcpClient : public QObject
{
	Q_OBJECT
public:
	explicit IVFDTcpClient(QObject *parent = 0, quint16 portn = 58892, QString hostAddr = QString("127.0.0.1"));
	quint16 m_portn;
	QTcpSocket *m_tcpClientSocket;
	QHostAddress m_tcpAddress;
	QString m_msgFromServer;
	quint16 m_blockSize;
	bool m_socketisconnectted;
	QThread *m_threadClient;

signals:
	void SIG_ConnectToHost(QString, quint16);
	void SIG_ClearClient();
	void SIG_HeartBeat();
	void SIG_SendMsgToServer(QString, quint16, QByteArray cmd ,QByteArray val);
	void SIG_StopClientThread();
	void SIG_TcpSocketSendMsg(QByteArray, QByteArray);


	public slots:
		void SLOT_ReadMsg();
		void SLOT_NewConnect(QString, quint16);
		void SLOT_DisplayError(QAbstractSocket::SocketError);
		void SLOT_ClearClient();
		void SLOT_HeartBeat();
		void SLOT_SendMsgToServer(QString, quint16, QByteArray cmd, QByteArray val);
		void SLOT_TcpSocketSendMsg(QByteArray, QByteArray);
		void SLOT_Test();
		void SLOT_ClientConnected();
};

#endif // IVFDTCPCLIENT_H
