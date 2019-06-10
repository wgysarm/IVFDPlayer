#include "IVFDTcpServer.h"
#include <QNetworkProxyFactory>

IVFDTcpServer::IVFDTcpServer(QObject *parent, QString ipAddr, quint16 tcpPort) : QObject(parent)
{
    InitThis(ipAddr, tcpPort);
	QNetworkProxyFactory::setUseSystemConfiguration(false);
}

IVFDTcpServer::~IVFDTcpServer()
{

}


void IVFDTcpServer::SLOT_StartTcpServerThread()
{
    m_tcpserverThread->start(QThread::HighestPriority);
    moveToThread(m_tcpserverThread);
}

void IVFDTcpServer::SLOT_StopTcpServerThread()
{

    m_tcpserverThread->quit();

}

void IVFDTcpServer::SLOT_HasNewConnection()
{
	emit SIG_UpdateServerIP();//是否保留？
    while(m_tcpserver->hasPendingConnections())
    {
        m_tcpsocket = m_tcpserver->nextPendingConnection();
		if(m_tcpsocket->peerAddress() == QHostAddress(m_hostAddr)){//属于变通的做法
			QObject::connect(m_tcpsocket, SIGNAL(readyRead()), this, SLOT(SLOT_ReadyReadAll()));
			QObject::connect(m_tcpsocket, SIGNAL(disconnected()), m_tcpsocket, SLOT(deleteLater()));
			QObject::connect(m_tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SLOT_SocketError(QAbstractSocket::SocketError)));
		}
    }
}

void IVFDTcpServer::SLOT_ReadyReadAll()
{
	while(m_tcpsocket->bytesAvailable())
	{
		m_ba = m_tcpsocket->readAll();
		m_count++;
	}

    XReadAll(m_ba);

	m_tcpsocket->close();

}

void IVFDTcpServer::SLOT_SocketError(QAbstractSocket::SocketError err)
{
    qDebug() << "5 IVFDMainTcpServer::SLOT_SocketError:" << err;

}

void IVFDTcpServer::SLOT_WriteToTcpSocket(QByteArray ba)
{
    qDebug() << "1 IVFDMainTcpServer::SLOT_WriteToTcpSocket:"  << m_tcpsocket->peerAddress() << "-----current id---" << QThread::currentThread();
    qDebug() << "1 IVFDMainTcpServer::SLOT_WriteToTcpSocket:"  << m_tcpsocket->peerPort();
}


void IVFDTcpServer::XReadAll(QByteArray ba)
{
	emit SIG_TellTop(m_count, ba);//tell the top outside

}

void IVFDTcpServer::InitThis(QString ipaddr, quint16 portn)
{
	QString strip = ipaddr;
    m_hostAddr = QHostAddress(strip);
    m_port = portn;
	m_tcpserver = new QTcpServer(this);//这里一改为0就会down掉。
	m_tcpsocket = NULL;
	//m_tcpserver->listen(m_hostAddr, m_port);
	m_timer = new QTimer(this);
	m_count = 0;

	if(m_tcpserver != NULL)
	{
		bool islisten = m_tcpserver->listen(QHostAddress::Any, 59000);//这里应该改成m_hostAddr，可以试一下是否可以。
		//bool islisten = m_tcpserver->listen(QHostAddress(m_hostAddr), 59000);//经测试，这里没有监听成功
		if(islisten){
			m_count = 0;
			emit SIG_TellTop(m_count, (QString::number(0)).toLatin1());
		}else{
			m_count = 2999;
			emit SIG_TellTop(m_count, (QString::number(2999)).toLatin1());
		}
	}


	m_tcpserverThread = new QThread;

    QObject::connect(m_tcpserverThread, SIGNAL(finished()), this, SLOT(deleteLater()));
    QObject::connect(m_tcpserver, SIGNAL(newConnection()), this, SLOT(SLOT_HasNewConnection()));
	QObject::connect(this, SIGNAL(SIG_GetStatus()), this, SLOT(SLOT_TellStatus()));

    QObject::connect(this, SIGNAL(SIG_StartTcpServerThread()), this, SLOT(SLOT_StartTcpServerThread()));
    QObject::connect(this, SIGNAL(SIG_StopTcpServerThread()), this, SLOT(SLOT_StopTcpServerThread()));
    QObject::connect(this, SIGNAL(SIG_WriteToTcpSocket(QByteArray)), this, SLOT(SLOT_WriteToTcpSocket(QByteArray)));

}
void IVFDTcpServer::SLOT_TellStatus()
{
	unsigned int num = (unsigned int )QThread::currentThreadId();
	emit SIG_TellTop(m_count, (QString::number(num)).toLatin1());

}

void IVFDTcpServer::SLOT_TimeOut()
{
	m_count++;
	emit SIG_TellTop(m_count, QString(" is the count!").toLatin1());
}


void IVFDTcpServer::SLOT_GetServerIP(){
	//m_hostAddr

}