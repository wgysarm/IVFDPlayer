#include "IVFDTcpClient.h"


typedef struct CmdARG_{
    int cmdtype;
    int argnum;
    int arg1;
    int arg2;
    int arg3;
    int arg4;
}CmdARG_;

IVFDTcpClient::IVFDTcpClient(QObject *parent, quint16 portn, QString hostAddr) : QObject(parent)
{
    qDebug() << "1 IVFDTcpClient::IVFDTcpClient IVFDTcpClient create";
    m_portn = portn;
    m_tcpAddress = QHostAddress(hostAddr);
    m_tcpClientSocket = new QTcpSocket;
    m_socketisconnectted = false;
    ///* * //create the thread;
    m_threadClient = new QThread;
    if(m_threadClient != NULL)
    {
        m_threadClient->start();
    }
    this->moveToThread(m_threadClient);

    QObject::connect(m_threadClient, SIGNAL(finished()), this, SLOT(deleteLater()));
    QObject::connect(this, SIGNAL(SIG_StopClientThread()), m_threadClient, SLOT(quit()));
    //*/
    QObject::connect(this, SIGNAL(SIG_SendMsgToServer(QString, quint16, QByteArray, QByteArray)), this, SLOT(SLOT_SendMsgToServer(QString, quint16, QByteArray, QByteArray)));//from outside signal

    QObject::connect(m_tcpClientSocket, SIGNAL(readyRead()), this, SLOT(SLOT_ReadMsg()));
    QObject::connect(m_tcpClientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SLOT_DisplayError(QAbstractSocket::SocketError)));
    QObject::connect(m_tcpClientSocket, SIGNAL(connected()), this, SLOT(SLOT_ClientConnected()));
    QObject::connect(this, SIGNAL(SIG_ConnectToHost(QString, quint16)), this, SLOT(SLOT_NewConnect(QString, quint16)));
    QObject::connect(this, SIGNAL(SIG_TcpSocketSendMsg(QByteArray, QByteArray)), this, SLOT(SLOT_TcpSocketSendMsg(QByteArray, QByteArray)));
}

void IVFDTcpClient::SLOT_ReadMsg()
{
    QByteArray ba;
    while(m_tcpClientSocket->bytesAvailable())
    {
        ba = m_tcpClientSocket->readAll();
    }
//    m_msgFromServer = QString(ba);
//    qDebug() <<  "1 IVFDTcpClient::SLOT_ReadMsg():" << m_msgFromServer;
//    m_tcpClientSocket->disconnectFromHost();
//    m_socketisconnectted = false;
    //qDebug() << "1 IVFDTcpClient::SLOT_ReadMsg():connect status is :" << m_socketisconnectted << "current thread:" << QThread::currentThread();
}

void IVFDTcpClient::SLOT_NewConnect(QString addr, quint16 portn)
{
    //qDebug() << "1 IVFDTcpClient::SLOT_NewConnect(): " << addr << "@" << portn;
    m_blockSize = 0;
    m_tcpClientSocket->abort();
    m_tcpClientSocket->connectToHost(QHostAddress(addr), portn);
    //qDebug() << "1 IVFDTcpClient::SLOT_NewConnect() end";
}
void IVFDTcpClient::SLOT_ClientConnected()
{
    //qDebug() << "1 IVFDTcpClient::SLOT_ClientConnected: connected to dip or webbroswer's' tcpserver" ;
    m_socketisconnectted = true;

}

void IVFDTcpClient::SLOT_DisplayError(QAbstractSocket::SocketError)
{
    //qDebug() << "1 In IVFDTcpClient displayError: " << m_tcpClientSocket->errorString();

}

void IVFDTcpClient::SLOT_ClearClient()
{

}

void IVFDTcpClient::SLOT_HeartBeat()
{

}

void IVFDTcpClient::SLOT_SendMsgToServer(QString addr, quint16 portn, QByteArray cmd, QByteArray val)
{
    //qDebug() << "1 IVFDTcpClient::SLOT_SendMsgToServer current thread :" << QThread::currentThread();

    if(!m_socketisconnectted)
    {
        //qDebug() << "1 IVFDTcpClient::SLOT_SendMsgToServer: start new connect to local host:" << addr << "@" << portn;
        emit SIG_ConnectToHost(addr, portn);
        if(m_tcpClientSocket != NULL)
        {
//            m_socketisconnectted = true;
            //qDebug() << "1 IVFDTcpClient::SLOT_SendMsgToServer: m_tcpClientSocket != NULL: " << addr << "@" << portn;
            emit SIG_TcpSocketSendMsg(cmd, val);
        }
    }else
    {
        //qDebug() << "1 IVFDTcpClient::SLOT_SendMsgToServer: tcpsocket is connected:" << addr << "@" << portn;
        emit SIG_TcpSocketSendMsg(cmd, val);
    }
}

void IVFDTcpClient::SLOT_TcpSocketSendMsg(QByteArray cmd , QByteArray val)
{
    //qDebug() << "1 IVFDTcpClient::SLOT_TcpSocketSendMsg:";
    if(m_tcpClientSocket != NULL)
    {
        m_tcpClientSocket->write(val);
		QThread::msleep(20);
        m_tcpClientSocket->disconnectFromHost();
        m_tcpClientSocket->waitForDisconnected(3);
        m_socketisconnectted = false;
    }
	QThread::msleep(30);
}

void IVFDTcpClient::SLOT_Test()
{
    while(1)
    {
        qDebug() << "test";
        QThread::msleep(1000);
    }
}

