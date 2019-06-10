#include "IVFDActiveXN.h"
#include "ChanelWidget.h"


IVFDActiveXN::IVFDActiveXN(QWidget *parent)
	: QWidget(parent),m_tcpserver(NULL), m_tcpclient(NULL), m_chanelWidget1(NULL), m_chanelWidget2(NULL)
{
	InitThis();

	emit SIG_StartTcpServerThread();//此处控件用movetoThread会有问题，经常传不上数据。所以改为在主线程里。
}

void IVFDActiveXN::InitThis()
{

	uitmp.setupUi(this);
	uitmp.lineEdit_ClientIP->installEventFilter(this);
	uitmp.lineEdit_DevIP->installEventFilter(this);
	ui.lineEdit_ClientIp = NULL;
	QObject::connect(uitmp.pushButton_initconfirm, SIGNAL(clicked()), this, SLOT(SLOT_Init()));
	QObject::connect(uitmp.pushButton_initreset, SIGNAL(clicked()), this, SLOT(SLOT_InitReset()));
}

void IVFDActiveXN::SetCurrentValid(int index)
{
	switch(index)
	{
	case 0:
		{
			m_tabWidget->setCurrentIndex(0);
			ui.pushButton_play1->setDisabled(false);
			ui.pushButton_reconnect1->setDisabled(false);
			ui.pushButton_snap1->setDisabled(false);
			ui.pushButton_stop1->setDisabled(false);

			ui.pushButton_play2->setDisabled(true);
			ui.pushButton_reconnect2->setDisabled(true);
			ui.pushButton_snap2->setDisabled(true);
			ui.pushButton_stop2->setDisabled(true);

			break;
		}
	case 1:
		{
			m_tabWidget->setCurrentIndex(1);
			ui.pushButton_play2->setDisabled(false);
			ui.pushButton_reconnect2->setDisabled(false);
			ui.pushButton_snap2->setDisabled(false);
			ui.pushButton_stop2->setDisabled(false);

			ui.pushButton_play1->setDisabled(true);
			ui.pushButton_reconnect1->setDisabled(true);
			ui.pushButton_snap1->setDisabled(true);
			ui.pushButton_stop1->setDisabled(true);
			break;
		}
	}
}

void IVFDActiveXN::SLOT_GetAlgOutFromTcpServer(int type, QByteArray ba)//all recv cmd here to clssify
{
	QString strtype = QString::number(type);
	ui.label_rcvcount->setText(strtype);
	//qDebug() << ba.size();
	if(ba.size() == 24){
		ALARMDATAINFO *myRecvData = (ALARMDATAINFO *)ba.data();
		if(myRecvData->alarmType == 3)
		{
			emit SIG_GetAlgProcessMsg(type, (ba));
			ui.label_status->setStyleSheet("QLabel{background:red}");
			ui.label_status->setText(QString::fromLocal8Bit("火警报告中"));
			ui.label_status->setAlignment(Qt::AlignCenter);
			ui.label_red->setStyleSheet("QLabel{border-image:url(:/image/redled.png)}");
			ui.label_buzzer->setStyleSheet("QLabel{border-image:url(:/image/ringon.png)}");
			ui.label_statuspic->setStyleSheet("QLabel{border-image:url(:/image/eyered.png)}");
			ui.textBrowser_eventinfo->append(QString("Alarm New:(%1,%2),Range(width=%3,height=%4)").arg(myRecvData->pTOPLx).arg(myRecvData->pTOPLy).arg(myRecvData->recWidth).arg(myRecvData->recHeight));
		}else if(myRecvData->alarmType == 6)//enum EOUTCOMETYPE{NOEVENT, TOOLIGHT, TOODARK, FIREDET, FIREDETEND, VLOST, RUNSTATUS};
		{

		}else if(myRecvData->alarmType == 4){
			emit SIG_GetAlgProcessMsg(type,(ba));
			ui.label_status->setStyleSheet("QLabel{background:green}");
			ui.label_status->setText(QString::fromLocal8Bit("火警结束，需手动消除"));
			ui.label_status->setAlignment(Qt::AlignCenter);
			ui.label_red->setStyleSheet("QLabel{border-image:url(:/image/ledoff.png)}");
			ui.label_buzzer->setStyleSheet("QLabel{border-image:url(:/image/ringoff.png)}");
			ui.label_statuspic->setStyleSheet("QLabel{border-image:url(:/image/eyegreen.png)}");
			ui.textBrowser_xyinfo->append(QString("Alarm:(%1,%2) End").arg(myRecvData->pTOPLx).arg(myRecvData->pTOPLy));
		};
	}else if(ba.size() == 136){	
		CmdActiveX *cmdrecv = (CmdActiveX *)ba.data();
		//QMessageBox::about(NULL,QString::fromLocal8Bit("调试1"),QString(cmdrecv->cmdstr));
		//QMessageBox::about(NULL,QString::fromLocal8Bit("调试1"),strtype);
		//QMessageBox::about(NULL,QString::fromLocal8Bit("调试2"),QString::number(cmdrecv->cmdtype));
		if(cmdrecv->cmdtype == 0){//发送普通注册命令后，设备回答。
			ui.textBrowser_statusinfo->setText(QString::fromLocal8Bit("新连接设备"));

			emit SIG_UpdateOnWatch(1);

			ui.label_status->setStyleSheet("QLabel{background:green}");
			ui.label_status->setText(QString::fromLocal8Bit("设备已连接"));
			ui.label_status->setAlignment(Qt::AlignCenter);
			//此处增加读取设备的设备id与ip写入到本地的记录文件中。格式为userid=userip
			QFile file("sysinfo.ini");
			if(file.exists()){
				QSettings myinisettings("sysinfo.ini",QSettings::IniFormat);
				myinisettings.setValue(QString("/remote/").append(ui.lineEdit_ServerIp->text()),QVariant(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
			}
			SLOT_ReconnectAll();
		}else if(cmdrecv->cmdtype == 17){//read info cmd
			if(cmdrecv->cmdval != 555)//555 means sysyinfo answer back
			{
				ui.label_readcam->setText(QString::number(cmdrecv->cmdval));//camera read back
			}else {
				ui.label_readproduct->setText(cmdrecv->cmdstr);
				if(QString(cmdrecv->cmdstr).contains("-I-")){
					QSettings myinisettings("sysinfo.ini",QSettings::IniFormat);
					myinisettings.setValue(QString("/device/").append(cmdrecv->cmdstr), QVariant(ui.lineEdit_ServerIp->text()));
				}
			}
		}else if(cmdrecv->cmdtype == 1){//客户端上线，发送请求服务命令，设备回答，收到后主动连接设备。 answer back
			ui.textBrowser_statusinfo->setText("get 1!");
			QString recvStr = QString(cmdrecv->cmdstr);
			//ui.textBrowser_eventinfo->append(recvStr);
			ui.textBrowser_eventinfo->append("now print the recv string!");
			ui.textBrowser_xyinfo->append(recvStr);
			//ui.textBrowser_statusinfo->setText(QString::number(cmdrecv->type));
			SLOT_ReconnectAll();
		}else if(cmdrecv->cmdtype == 2){//设备发送的心跳信息，收到后进行刷新。
			QString recvStr = QString(cmdrecv->cmdstr);
			QStringList strlist = recvStr.split("$");
			if(strlist.length() == 3){//确保收到的数据有效
				ui.label_hbcpu->setText(strlist.at(0));
				ui.label_hbmem->setText(strlist.at(1));
				ui.label_hbnet->setText(strlist.at(2));
				ui.textBrowser_statusinfo->append(recvStr);
				m_chartstatus->addData(strlist.at(0).split(":").at(1).split("%").at(0).toDouble());
			}
		}

	}else if(ba.size() == 10){//


	}else if(ba.size() == 20){
		GeneralMsg *msgrecv = (GeneralMsg *)ba.data();
		if(msgrecv->dest == 4){
			//ui.textBrowser_statusinfo->setText(QString::number(msgrecv->type));
			ui.label_server->setText(QString("type=%1 msgid=%2 val=%3").arg(msgrecv->type).arg(msgrecv->msgid).arg(msgrecv->val));
			if(msgrecv->val == 5){
				m_flagsuvey = 1;
				ui.pushButton_suvey->setText(QString::fromLocal8Bit("设备撤防"));
				ui.label_statuspic->setStyleSheet("QLabel{border-image:url(:/image/eyegreen.png)}");
			}else{
				m_flagsuvey = 0;
				ui.pushButton_suvey->setText(QString::fromLocal8Bit("设备布防"));
				ui.label_statuspic->setStyleSheet("QLabel{border-image:url(:/image/eyegrey.png)}");
			}
			SLOT_ReconnectAll();
		}
	}
	//emit SIG_ToJs(strtype);
	//ui.textBrowser_statusinfo->setText(strtype);
	//SLOT_SendToJs(type, QString(ba));
}

void IVFDActiveXN::SLOT_GetAlgProcessMsg(int type, QByteArray msg)
{
	//ui.textBrowser_statusinfo->setText(QString::number(type).append(msg));
	//打包数据

	//m_typeAlgProcess = type;
	//m_msgAlgProcess = msg;
	//update();
}

void IVFDActiveXN::SLOT_SendToJs()
{
	emit SIG_ToJs(QString("123456"));
}
void IVFDActiveXN::SLOT_ToJs(QString val)
{
	//qDebug() << val;

	//ui.textBrowser_statusinfo->append(QString::number(val));
	ui.textBrowser_statusinfo->append(val);
	ui.textBrowser_eventinfo->append(val);

}

void IVFDActiveXN::SLOT_ClearStatus()
{
	ui.textBrowser_statusinfo->clear();

}
void IVFDActiveXN::SLOT_ClearEvent()
{
	ui.textBrowser_eventinfo->clear();

}

void IVFDActiveXN::SLOT_ClearXY()
{
	ui.textBrowser_xyinfo->clear();

}

void IVFDActiveXN::SLOT_GetServerStatus()
{
	//ui.label_server->setText(QString::number(val).append(info));
	//ui.label_server->setText("");
}

void IVFDActiveXN::SLOT_Play1()
{
	m_serverip = ui.lineEdit_ServerIp->text();
	if(m_chanelWidget1 != NULL)
	{
		delete m_chanelWidget1;
		m_chanelWidget1 = NULL;
		rtspurl1 = QString("rtsp://%1/h264Video0").arg(m_serverip);
		m_chanelWidget1 = new ChanelWidget(this, rtspurl1, 0/*, listenport1*/);

		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget1, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget1, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget1, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget1, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(0, m_chanelWidget1,tr("video1"));
		m_currentVideoId = 0;
		m_tabWidget->setCurrentIndex(0);

		ui.pushButton_play1->setDisabled(true);
		ui.pushButton_reconnect1->setEnabled(true);
		ui.pushButton_stop1->setEnabled(true);
		ui.pushButton_snap1->setEnabled(true);

	}else
	{
		rtspurl1 = QString("rtsp://%1/h264Video0").arg(m_serverip);
		m_chanelWidget1 = new ChanelWidget(this, rtspurl1, 0/*, listenport1*/);

		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget1, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget1, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget1, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget1, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(0, m_chanelWidget1,tr("video1"));
		m_currentVideoId = 0;
		m_tabWidget->setCurrentIndex(0);
		ui.pushButton_play1->setDisabled(true);
		ui.pushButton_reconnect1->setEnabled(true);
		ui.pushButton_stop1->setEnabled(true);
		ui.pushButton_snap1->setEnabled(true);

	}
}

void IVFDActiveXN::SLOT_Reconnect1()
{
	m_serverip = ui.lineEdit_ServerIp->text();
	if(m_chanelWidget1 != NULL)
	{
		delete m_chanelWidget1;
		m_chanelWidget1 = NULL;
		rtspurl1 = QString("rtsp://%1/h264Video0").arg(m_serverip);

		m_chanelWidget1 = new ChanelWidget(this, rtspurl1, 0/*, listenport1*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget1, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget1, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget1, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget1, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(0, m_chanelWidget1,tr("video1"));
		m_currentVideoId = 0;
		m_tabWidget->setCurrentIndex(0);

		ui.pushButton_reconnect1->setDisabled(true);
		ui.pushButton_play1->setDisabled(false);
		ui.pushButton_snap1->setEnabled(true);
		ui.pushButton_stop1->setEnabled(true);

	}else
	{
		rtspurl1 = QString("rtsp://%1/h264Video0").arg(m_serverip);
		m_chanelWidget1 = new ChanelWidget(this, rtspurl1, 0/*, listenport1*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget1, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget1, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget1, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget1, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(0, m_chanelWidget1,tr("video1"));
		m_currentVideoId = 0;
		m_tabWidget->setCurrentIndex(0);

		ui.pushButton_play1->setDisabled(true);
		ui.pushButton_reconnect1->setEnabled(true);
		ui.pushButton_stop1->setEnabled(true);
		ui.pushButton_snap1->setEnabled(true);

	}
}

void IVFDActiveXN::SLOT_Snap1()
{
		//ui.label_server->setText("snap1!");
		//emit SIG_ToJs("XXXX");
	emit SIG_Snap(1);
}
void IVFDActiveXN::SLOT_StopPlay1()
{
	if(m_chanelWidget1 != NULL)
	{
		delete m_chanelWidget1;
		m_chanelWidget1 = new ChanelWidget(this);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));

		m_tabWidget->insertTab(0, m_chanelWidget1,tr("video1"));
		m_currentVideoId = 0;
		m_tabWidget->setCurrentIndex(0);

		ui.pushButton_reconnect1->setDisabled(false);
		ui.pushButton_play1->setDisabled(false);
		ui.pushButton_snap1->setEnabled(false);
		ui.pushButton_stop1->setEnabled(false);

	}else
	{
		m_chanelWidget1 = new ChanelWidget(this);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));

		m_tabWidget->insertTab(0, m_chanelWidget1,tr("video1"));
		m_currentVideoId = 0;
		m_tabWidget->setCurrentIndex(0);

		ui.pushButton_play1->setDisabled(false);
		ui.pushButton_reconnect1->setEnabled(true);
		ui.pushButton_stop1->setEnabled(false);
		ui.pushButton_snap1->setEnabled(false);
	}

}

void IVFDActiveXN::SLOT_CurrentVideoId(int id)
{
	qDebug() << id;
	m_currentVideoId = id;
	SetCurrentValid(m_currentVideoId);
}



void IVFDActiveXN::SLOT_Play2()
{
	m_serverip = ui.lineEdit_ServerIp->text();
	if(m_chanelWidget2 != NULL)
	{
		delete m_chanelWidget2;
		m_chanelWidget2 = NULL;
		rtspurl2 = QString("rtsp://%1/h264Video1").arg(m_serverip);
		m_chanelWidget2 = new ChanelWidget(this, rtspurl2, 0/*, listenport2*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget2, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget2, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget2, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget2, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget2, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(1, m_chanelWidget2, tr("video2"));
		m_currentVideoId = 1;
		m_tabWidget->setCurrentIndex(1);

		ui.pushButton_play2->setDisabled(true);
		ui.pushButton_reconnect2->setEnabled(true);
		ui.pushButton_stop2->setEnabled(true);
		ui.pushButton_snap2->setEnabled(true);

	}else
	{
		rtspurl2 = QString("rtsp://%1/h264Video1").arg(m_serverip);
		m_chanelWidget2 = new ChanelWidget(this, rtspurl2, 0/*, listenport2*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget2, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget2, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget2, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget2, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget2, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(1, m_chanelWidget2, tr("video2"));
		m_currentVideoId = 1;
		m_tabWidget->setCurrentIndex(1);

		ui.pushButton_play2->setDisabled(true);
		ui.pushButton_reconnect2->setEnabled(true);
		ui.pushButton_stop2->setEnabled(true);
		ui.pushButton_snap2->setEnabled(true);

	}
}

void IVFDActiveXN::SLOT_Reconnect2()
{
	m_serverip = ui.lineEdit_ServerIp->text();
	if(m_chanelWidget2 != NULL)
	{
		delete m_chanelWidget2;
		m_chanelWidget2 = NULL;
		rtspurl2 = QString("rtsp://%1/h264Video1").arg(m_serverip);
		m_chanelWidget2 = new ChanelWidget(this, rtspurl2, 0/*, listenport2*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget2, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget2, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget2, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget2, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget2, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(1, m_chanelWidget2, tr("video2"));
		m_currentVideoId = 1;
		m_tabWidget->setCurrentIndex(1);

		ui.pushButton_reconnect2->setDisabled(true);
		ui.pushButton_play2->setDisabled(true);
		ui.pushButton_snap2->setEnabled(true);
		ui.pushButton_stop2->setEnabled(true);
	}else
	{
		rtspurl2 = QString("rtsp://%1/h264Video1").arg(m_serverip);
		m_chanelWidget2 = new ChanelWidget(this, rtspurl2, 0/*, listenport2*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget2, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget2, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget2, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget2, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget2, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(1, m_chanelWidget2, tr("video2"));
		m_currentVideoId = 1;
		m_tabWidget->setCurrentIndex(1);

		ui.pushButton_play2->setDisabled(true);
		ui.pushButton_reconnect2->setEnabled(true);
		ui.pushButton_stop2->setEnabled(true);
		ui.pushButton_snap2->setEnabled(true);

	}
}

void IVFDActiveXN::SLOT_Snap2()
{

	emit SIG_Snap(2);
}
void IVFDActiveXN::SLOT_StopPlay2()
{
	if(m_chanelWidget2 != NULL)
	{
		delete m_chanelWidget2;
		m_chanelWidget2 = new ChanelWidget(this);
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));

		m_tabWidget->insertTab(1, m_chanelWidget2, tr("video2"));
		m_currentVideoId = 1;
		m_tabWidget->setCurrentIndex(1);

		ui.pushButton_reconnect2->setDisabled(false);
		ui.pushButton_play2->setDisabled(false);
		ui.pushButton_snap2->setEnabled(false);
		ui.pushButton_stop2->setEnabled(false);

	}else
	{
		m_chanelWidget2 = new ChanelWidget(this);
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));

		m_tabWidget->insertTab(1, m_chanelWidget2, tr("video2"));
		m_currentVideoId = 1;
		m_tabWidget->setCurrentIndex(1);

		ui.pushButton_play2->setDisabled(false);
		ui.pushButton_reconnect2->setEnabled(true);
		ui.pushButton_stop2->setEnabled(false);
		ui.pushButton_snap2->setEnabled(false);
	}
}

void IVFDActiveXN::SLOT_ReconnectAll(){
	m_serverip = ui.lineEdit_ServerIp->text();
	if(m_chanelWidget1 != NULL)
	{
		delete m_chanelWidget1;
		m_chanelWidget1 = NULL;
		rtspurl1 = QString("rtsp://%1/h264Video0").arg(m_serverip);
		m_chanelWidget1 = new ChanelWidget(this, rtspurl1, 0/*, listenport2*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget1, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget1, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget1, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget1, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(1, m_chanelWidget1, tr("video1"));
		m_currentVideoId = 0;
		m_tabWidget->setCurrentIndex(0);

		ui.pushButton_reconnect1->setDisabled(true);
		ui.pushButton_play1->setDisabled(true);
		ui.pushButton_snap1->setEnabled(true);
		ui.pushButton_stop1->setEnabled(true);
	}else
	{
		rtspurl1 = QString("rtsp://%1/h264Video0").arg(m_serverip);
		m_chanelWidget1 = new ChanelWidget(this, rtspurl1, 0/*, listenport2*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget1, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget1, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget1, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget1, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(0, m_chanelWidget1, tr("video1"));
		m_currentVideoId = 0;
		m_tabWidget->setCurrentIndex(0);

		ui.pushButton_play1->setDisabled(true);
		ui.pushButton_reconnect1->setEnabled(true);
		ui.pushButton_stop1->setEnabled(true);
		ui.pushButton_snap1->setEnabled(true);
	}

	if(m_chanelWidget2 != NULL)
	{
		delete m_chanelWidget2;
		m_chanelWidget2 = NULL;
		rtspurl2 = QString("rtsp://%1/h264Video1").arg(m_serverip);

		m_chanelWidget2 = new ChanelWidget(this, rtspurl2, 0/*, listenport1*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget2, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget2, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget2, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget2, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget2, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(1, m_chanelWidget2,tr("video2"));
		m_currentVideoId = 1;
		m_tabWidget->setCurrentIndex(1);

		ui.pushButton_reconnect2->setDisabled(true);
		ui.pushButton_play2->setDisabled(false);
		ui.pushButton_snap2->setEnabled(true);
		ui.pushButton_stop2->setEnabled(true);

	}else
	{
		rtspurl2 = QString("rtsp://%1/h264Video1").arg(m_serverip);
		m_chanelWidget2 = new ChanelWidget(this, rtspurl2, 0/*, listenport1*/);
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget2, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget2, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget2, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget2, SIGNAL(SIG_UpdateOnWatch(int)));
		QObject::connect(this, SIGNAL(SIG_Snap(int)), m_chanelWidget2, SIGNAL(SIG_Snap(int)));

		m_tabWidget->insertTab(1, m_chanelWidget2,tr("video2"));
		m_currentVideoId = 1;
		m_tabWidget->setCurrentIndex(1);

		ui.pushButton_play2->setDisabled(true);
		ui.pushButton_reconnect2->setEnabled(true);
		ui.pushButton_stop2->setEnabled(true);
		ui.pushButton_snap2->setEnabled(true);
	}
}

void IVFDActiveXN::SLOT_OffsetXinc()
{
	ui.label_server->setText("0,1");
	emit SIG_OffsetXY(0, m_offsetStep);

	if(ui.lineEdit_ClientIp->text().isEmpty()){
		QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("请输入本地ip地址"));
	}else{
		QString str = QString("xinc=%1").arg(m_offsetStep);
		QByteArray ba;
		int len;
		QMessageBox::about(NULL,QString::fromLocal8Bit("上传x+偏置"),m_serverip);
		CmdActiveX cmdsend;
		cmdsend.cmdtype = 3;//cmdtype = 3:modify xyoffset command
		cmdsend.cmdval = 0;
		memset(cmdsend.cmdstr, 0 ,128);

		QByteArray strba = str.toLatin1();
		len = strba.length() < 128?strba.length():128;
		memcpy(cmdsend.cmdstr, strba.data(), len);
		ba.resize(sizeof(cmdsend));
		memcpy(ba.data(), &cmdsend, sizeof(cmdsend));
		if(m_tcpclient != NULL){
			m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
		}
	}

}
void IVFDActiveXN::SLOT_OffsetXDec()
{
	emit SIG_OffsetXY(1, m_offsetStep);
	ui.label_server->setText("1,1");

	if(ui.lineEdit_ClientIp->text().isEmpty()){
		QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("请输入本地ip地址"));
	}else{
		QString str = QString("xdec=%1").arg(m_offsetStep);
		QByteArray ba;
		int len;
		QMessageBox::about(NULL,QString::fromLocal8Bit("上传x-偏置"),m_serverip);
		CmdActiveX cmdsend;
		cmdsend.cmdtype = 3;//cmdtype = 3:modify xyoffset command
		cmdsend.cmdval = 0;
		memset(cmdsend.cmdstr, 0 ,128);

		QByteArray strba = str.toLatin1();
		len = strba.length() < 128?strba.length():128;
		memcpy(cmdsend.cmdstr, strba.data(), len);
		ba.resize(sizeof(cmdsend));
		memcpy(ba.data(), &cmdsend, sizeof(cmdsend));
		if(m_tcpclient != NULL){
			m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
		}
	}
}

void IVFDActiveXN::SLOT_OffsetYinc()
{
	ui.label_server->setText("2,1");
	emit SIG_OffsetXY(2, m_offsetStep);
	if(ui.lineEdit_ClientIp->text().isEmpty()){
		QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("请输入本地ip地址"));
	}else{
		QString str = QString("yinc=%1").arg(m_offsetStep);
		QByteArray ba;
		int len;
		QMessageBox::about(NULL,QString::fromLocal8Bit("上传y+偏置"),m_serverip);
		CmdActiveX cmdsend;
		cmdsend.cmdtype = 3;//cmdtype = 3:modify xyoffset command
		cmdsend.cmdval = 0;
		memset(cmdsend.cmdstr, 0 ,128);

		QByteArray strba = str.toLatin1();
		len = strba.length() < 128?strba.length():128;
		memcpy(cmdsend.cmdstr, strba.data(), len);
		ba.resize(sizeof(cmdsend));
		memcpy(ba.data(), &cmdsend, sizeof(cmdsend));
		if(m_tcpclient != NULL){
			m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
		}
	}
}

void IVFDActiveXN::SLOT_OffsetYDec()
{
	emit SIG_OffsetXY(3, m_offsetStep);
	ui.label_server->setText("3,1");
	if(ui.lineEdit_ClientIp->text().isEmpty()){
		QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("请输入本地ip地址"));
	}else{
		QString str = QString("ydec=%1").arg(m_offsetStep);
		QByteArray ba;
		int len;
		QMessageBox::about(NULL,QString::fromLocal8Bit("上传y-偏置"),m_serverip);
		CmdActiveX cmdsend;
		cmdsend.cmdtype = 3;//cmdtype = 3:modify xyoffset command
		cmdsend.cmdval = 0;
		memset(cmdsend.cmdstr, 0 ,128);

		QByteArray strba = str.toLatin1();
		len = strba.length() < 128?strba.length():128;
		memcpy(cmdsend.cmdstr, strba.data(), len);
		ba.resize(sizeof(cmdsend));
		memcpy(ba.data(), &cmdsend, sizeof(cmdsend));
		if(m_tcpclient != NULL){
			m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
		}
	}
}

void IVFDActiveXN::SLOT_TestJsCmd()
{
	QString localIP = GetHostIpAddress();
	emit SIG_ToJs(localIP);
	ui.label_server->setText(localIP);

}

QString IVFDActiveXN::GetHostIpAddress()
{
	QString strIpAddress;  
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();  
	// 获取第一个本主机的IPv4地址  
	int nListSize = ipAddressesList.size();  
	for (int i = 0; i < nListSize; ++i)  
	{  

		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&  
			ipAddressesList.at(i).toIPv4Address()) {  
				strIpAddress = ipAddressesList.at(i).toString();  
				if(strIpAddress.startsWith("192.168")){
					break;
				}
		}  
	}  
	// 如果没有找到，则以本地IP地址为IP  
	if (strIpAddress.isEmpty())  
		strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();  
	return strIpAddress;  
}

void IVFDActiveXN::SLOT_SetServerIP(QString serverip)
{
	m_serverip = serverip;
	emit SIG_ToJs(m_serverip);
}

void IVFDActiveXN::SLOT_Regist(){
	
	if(m_flagonregister == 0)
	{
		if(ui.lineEdit_ClientIp->text().isEmpty()){
			QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("请输入本地ip地址"));
		}else{
			QString str = ui.lineEdit_ClientIp->text();
			QByteArray ba;
			int len;
			QMessageBox::about(NULL,QString::fromLocal8Bit("注册"),QString("serverip = ").append(m_serverip));

			CmdActiveX cmdsend;
			cmdsend.cmdtype = 0;//注册命令
			cmdsend.cmdval = 0;
			memset(cmdsend.cmdstr, 0 ,128);

			QByteArray strba = str.toLatin1();
			len = strba.length() < 128?strba.length():128;
			memcpy(cmdsend.cmdstr, strba.data(), len);
			ba.resize(sizeof(cmdsend));
			memcpy(ba.data(), &cmdsend, sizeof(cmdsend));

			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
				m_flagonregister = 1;
				ui.pushButton_regist->setText(QString::fromLocal8Bit("取消服务"));
			}
		}
	}else if(m_flagonregister == 1){
		QString str = ui.lineEdit_ClientIp->text();
		QByteArray ba;
		int len;
		CmdActiveX cmdsend;
		cmdsend.cmdtype = 1;//删除注册命令
		cmdsend.cmdval = 0;
		memset(cmdsend.cmdstr, 0 ,128);
		QByteArray strba = str.toLatin1();
		len = strba.length() < 128?strba.length():128;
		memcpy(cmdsend.cmdstr, strba.data(), len);
		ba.resize(sizeof(cmdsend));
		memcpy(ba.data(), &cmdsend, sizeof(cmdsend));

		if(m_tcpclient != NULL){
			m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);//58890为设备侦听客户端的端口
			m_flagonregister = 0;
			ui.pushButton_regist->setText(QString::fromLocal8Bit("注册服务"));
		}
	}
}

//向设备发第一次请求
void IVFDActiveXN::SLOT_FirstConnectAllRequest()
{
	GeneralMsg msgsend;
	msgsend.dest = 0;//0 for main
	msgsend.source = 4;//4 for activex
	msgsend.type = 0;
	msgsend.val = 1;
	msgsend.msgid = 1;

	QByteArray ba;
	ba.resize(sizeof(msgsend));
	memcpy(ba.data(), &msgsend, sizeof(msgsend));
	if(m_tcpclient != NULL){
		m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
	}
}


void IVFDActiveXN::SLOT_UpdateServerIP()
{
	m_tcpserver->m_hostAddr = ui.lineEdit_ServerIp->text();
}

void IVFDActiveXN::SLOT_InitReset()
{
	uitmp.lineEdit_ClientIP->setText("192.168.");
	uitmp.lineEdit_DevIP->setText("192.168.");
}

void IVFDActiveXN::SLOT_Init()
{	
	//this->setObjectName("mainWindow");
	QString devip =  uitmp.lineEdit_DevIP->text();
	QString clientip = uitmp.lineEdit_ClientIP->text();
	
	ui.setupUi(this);	
	this->setLayout(ui.horizontalLayout_all);

	m_commandwindow = NULL;
	m_calibratwindow = NULL;
	m_selfrebootset = NULL;
	m_diaryquerry = NULL;

	ui.widget_right->setLayout(ui.verticalLayout_rightwidget);
	ui.widget_bottom->setLayout(ui.horizontalLayout_bottom);
	ui.widget_equipbar->setLayout(ui.horizontalLayout_equipbar);
	ui.widget_control->setLayout(ui.horizontalLayout_control);
	ui.widget_config->setLayout(ui.horizontalLayout_config);

	ui.lineEdit_ServerIp->setText(devip);
	ui.lineEdit_ClientIp->setText(clientip);

	m_serverip = ui.lineEdit_ServerIp->text();
	m_process = new QProcess(this);

	m_timer = new QTimer;
	m_timer->setInterval(1000);
	m_timer->start(1000);

	uitmp.lineEdit_ClientIP->setVisible(false);
	uitmp.lineEdit_DevIP->setVisible(false);
	uitmp.pushButton_initconfirm->setVisible(false);
	uitmp.pushButton_initreset->setVisible(false);
	uitmp.label->setVisible(false);
	uitmp.label_2->setVisible(false);

	if(GetHostIpAddress().isEmpty()){
		ui.lineEdit_ClientIp->setText(QString("get local ip failed!"));		
	}else{
		m_clientip = GetHostIpAddress();
	}
	//myinisettings.beginGroup("remote");
	//myinisettings.setValue(ui.lineEdit_ServerIp->text(), QVariant(QDateTime::currentDateTime().toString()));
	//myinisettings.endGroup();

	m_flagonregister = 0;
	m_flagledstatus = 0;
	m_flagledalarm = 0;
	m_flagledpower = 0;
	m_flagrelayalarm = 0;
	m_flagrelayerror = 0;
	m_flagbuzzer = 0;
	m_flagsuvey = 0;
	m_flagfullscreen = 0;
	m_flaguserlevel = 0;

	m_threadpool = new QThreadPool;
	m_threadpool->setMaxThreadCount(3);

	rtspurl1 = QString("rtsp://192.168.120.166/h264Video0");
	rtspurl2 = QString("rtsp://192.168.120.166/h264Video1");

	ui.label_statuspic->setStyleSheet("QLabel{border-image:url(:/image/eyegrey.png)}");
	ui.label_hbnet->setWordWrap(true);
	ui.label_hbnet->setAlignment(Qt::AlignTop);

	m_chanelWidget1 = new ChanelWidget;
	//m_chanelWidget1->setObjectName("loadingWidget");
	m_chanelWidget2 = new ChanelWidget;
	//m_chanelWidget2->setObjectName("loadingWidget");

	m_chartstatus = new IVFDChart;
	m_chartstatus->setWindowTitle("cpu usage");

	m_currentVideoId = 0;

	QString ipaddr = ui.lineEdit_ServerIp->text();
	m_tcpserver = new IVFDTcpServer(this, ipaddr, 59000);//59000 是server的监听端口，ipaddr是监听指定设备的ip地址 
	m_tcpclient = new IVFDTcpClient(this, 58890, m_clientip);//服务器58890 是设备的监听端口

	m_offsetStep = 5;

	//ui.pushButton_play1->setStyleSheet("QPushButton{border-image:url(:/image/btn_playunchchecknor.png)}");
	//ui.pushButton_fullscreen->setStyleSheet("QPushButton{background-image:url(:/image/btn_fullscreendown.png)}");

	ui.pushButton_snap1->setDisabled(true);
	ui.pushButton_stop1->setDisabled(true);
	ui.pushButton_snap2->setDisabled(true);
	ui.pushButton_stop2->setDisabled(true);

	ui.label_green->setStyleSheet("QLabel{border-image:url(:/image/ledoff.png)}");
	ui.label_yellow->setStyleSheet("QLabel{border-image:url(:/image/ledoff.png)}");
	ui.label_red->setStyleSheet("QLabel{border-image:url(:/image/ledoff.png)}");
	ui.label_buzzer->setStyleSheet("QLabel{border-image:url(:/image/ringoff.png)}");
	ui.label_relayerror->setStyleSheet("QLabel{border-image:url(:/image/switch_on3.png)}");
	ui.label_relayalarm->setStyleSheet("QLabel{border-image:url(:/image/switch_off3.png)}");

	
	m_devmaintainlayout = new QVBoxLayout;
	m_devmaintainlayout->addWidget(ui.pushButton_resetsystem);
	m_devmaintainlayout->addWidget(ui.pushButton_resetapp);
	m_devmaintainlayout->addWidget(ui.pushButton_resetdip);
	m_devmaintainlayout->addWidget(ui.pushButton_resetftp);
	m_devmaintainlayout->addWidget(ui.pushButton_resethttp);
	m_devmaintainlayout->addWidget(ui.pushButton_resetrtsp);
	ui.groupBox_devmaintain->setLayout(m_devmaintainlayout);

	m_hardtestlayout = new QVBoxLayout;
	m_hardtestlayout->addWidget(ui.pushButton_alarm);
	m_hardtestlayout->addWidget(ui.pushButton_status);
	m_hardtestlayout->addWidget(ui.pushButton_power);
	m_hardtestlayout->addWidget(ui.pushButton_buzzer);
	m_hardtestlayout->addWidget(ui.pushButton_relayerror);
	m_hardtestlayout->addWidget(ui.pushButton_relayalarm);
	ui.groupBox_hardtest->setLayout(m_hardtestlayout);

	m_appupdatehlayout = new QHBoxLayout;
	m_appupdatevlayouleft = new QVBoxLayout;
	m_appupdatevlayoutright = new QVBoxLayout;
	m_updatefilegroup = new QButtonGroup;

	m_appupdatevlayouleft->addWidget(ui.radioButton_dip);
	m_appupdatevlayouleft->addWidget(ui.radioButton_web);
	m_appupdatevlayouleft->addWidget(ui.radioButton_ftp);
	m_appupdatevlayouleft->addWidget(ui.radioButton_rtsp);
	m_appupdatevlayouleft->addWidget(ui.radioButton_app);
	m_appupdatevlayouleft->addWidget(ui.radioButton_fire);

	m_appupdatevlayoutright->addWidget(ui.radioButton_smoke);
	m_appupdatevlayoutright->addWidget(ui.radioButton_authen);
	m_appupdatevlayoutright->addWidget(ui.radioButton_main);
	m_appupdatevlayoutright->addWidget(ui.pushButton_upload);
	m_appupdatevlayoutright->addWidget(ui.pushButton_backup);
	m_appupdatevlayoutright->addWidget(ui.pushButton_update);


	m_updatefilegroup->addButton(ui.radioButton_dip, 0);
	m_updatefilegroup->addButton(ui.radioButton_web, 1);
	m_updatefilegroup->addButton(ui.radioButton_ftp, 2);
	m_updatefilegroup->addButton(ui.radioButton_rtsp, 3);
	m_updatefilegroup->addButton(ui.radioButton_fire, 4);
	m_updatefilegroup->addButton(ui.radioButton_smoke, 5);
	m_updatefilegroup->addButton(ui.radioButton_app, 6);
	m_updatefilegroup->addButton(ui.radioButton_authen, 7);
	m_updatefilegroup->addButton(ui.radioButton_main, 8);
	ui.radioButton_dip->setChecked(true);

	m_appupdatehlayout->addLayout(m_appupdatevlayouleft);
	m_appupdatehlayout->addLayout(m_appupdatevlayoutright);

	ui.spinBox_camera->setRange(1,8);
	
	ui.groupBox_panel->setLayout(ui.verticalLayout_panel);
	ui.groupBox_appupdate->setLayout(m_appupdatehlayout);
	ui.groupBox_product->setLayout(ui.verticalLayout_product);
	ui.groupBox_camera->setLayout(ui.verticalLayout_camera);

	ui.lineEdit_ClientIp->installEventFilter(this);
	ui.lineEdit_product->installEventFilter(this);

	QObject::connect(ui.pushButton_play1, SIGNAL(clicked()), this, SLOT(SLOT_Play1()));
	QObject::connect(ui.pushButton_snap1, SIGNAL(clicked()), this, SLOT(SLOT_Snap1()));
	QObject::connect(ui.pushButton_stop1, SIGNAL(clicked()), this, SLOT(SLOT_StopPlay1()));
	QObject::connect(ui.pushButton_reconnect1, SIGNAL(clicked()), this, SLOT(SLOT_Reconnect1()));
	QObject::connect(this, SIGNAL(SIG_StartRegist()), this, SLOT(SLOT_Regist()));
	QObject::connect(ui.pushButton_regist, SIGNAL(clicked()), this, SLOT(SLOT_Regist()));
	QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), this, SLOT(SLOT_ClearAlarm()));

	QObject::connect(ui.pushButton_play2, SIGNAL(clicked()), this, SLOT(SLOT_Play2()));
	QObject::connect(ui.pushButton_snap2, SIGNAL(clicked()), this, SLOT(SLOT_Snap2()));
	QObject::connect(ui.pushButton_stop2, SIGNAL(clicked()), this, SLOT(SLOT_StopPlay2()));
	QObject::connect(ui.pushButton_reconnect2, SIGNAL(clicked()), this, SLOT(SLOT_Reconnect2()));

	QObject::connect(ui.checkBox_reverse, SIGNAL(stateChanged(int)), this, SIGNAL(SIG_Test(int)));
	QObject::connect(ui.pushButton_sendToJs, SIGNAL(clicked()), this, SLOT(SLOT_SendToJs()));
	QObject::connect(this, SIGNAL(SIG_ToJs(QString)), this, SLOT(SLOT_ToJs(QString)));

	QObject::connect(ui.pushButton_xd, SIGNAL(clicked()), this, SLOT(SLOT_OffsetXDec()));
	QObject::connect(ui.pushButton_yd, SIGNAL(clicked()), this, SLOT(SLOT_OffsetYDec()));
	QObject::connect(ui.pushButton_xi, SIGNAL(clicked()), this, SLOT(SLOT_OffsetXinc()));
	QObject::connect(ui.pushButton_yi, SIGNAL(clicked()), this, SLOT(SLOT_OffsetYinc()));

	QObject::connect(this, SIGNAL(SIG_UpdateServerIP()), this, SLOT(SLOT_UpdateServerIP()));
	QObject::connect(m_tcpserver, SIGNAL(SIG_UpdateServerIP()), this, SIGNAL(SIG_UpdateServerIP()), Qt::QueuedConnection);

	QObject::connect(ui.pushButton_switchdev, SIGNAL(clicked()), this, SLOT(SLOT_SwitchDev()));

	QObject::connect(ui.pushButton_statusclear, SIGNAL(clicked()), this, SLOT(SLOT_ClearStatus()));
	QObject::connect(ui.pushButton_eventclear, SIGNAL(clicked()), this, SLOT(SLOT_ClearEvent()));
	QObject::connect(ui.pushButton_posclear, SIGNAL(clicked()), this, SLOT(SLOT_ClearXY()));

	//cmd of pushbuttons
	QObject::connect(this, SIGNAL(SIG_PBCmdSel(int)), this, SLOT(SLOT_PBCmdSel(int)));
	QObject::connect(ui.pushButton_status, SIGNAL(clicked()), this, SLOT(SLOT_PBLedStatusClicked()));
	QObject::connect(ui.pushButton_alarm, SIGNAL(clicked()), this, SLOT(SLOT_PBLedAlarmClicked()));
	QObject::connect(ui.pushButton_power, SIGNAL(clicked()), this, SLOT(SLOT_PBLedPowerClicked()));
	QObject::connect(ui.pushButton_relayerror, SIGNAL(clicked()), this, SLOT(SLOT_PBRelayErrorClicked()));
	QObject::connect(ui.pushButton_relayalarm, SIGNAL(clicked()), this, SLOT(SLOT_PBRelayAlarmClicked()));
	QObject::connect(ui.pushButton_buzzer, SIGNAL(clicked()), this, SLOT(SLOT_PBBuzzerClicked()));
	QObject::connect(ui.pushButton_suvey, SIGNAL(clicked()), this, SLOT(SLOT_PBSuveyClicked()));
	
	QObject::connect(ui.pushButton_resetsystem, SIGNAL(clicked()), this, SLOT(SLOT_PBResetSysClicked()));
	QObject::connect(ui.pushButton_resetapp, SIGNAL(clicked()), this, SLOT(SLOT_PBResetAppClicked()));
	QObject::connect(ui.pushButton_resetdip, SIGNAL(clicked()), this, SLOT(SLOT_PBResetDipClicked()));
	QObject::connect(ui.pushButton_resetrtsp, SIGNAL(clicked()), this, SLOT(SLOT_PBResetRtspClicked()));
	QObject::connect(ui.pushButton_resetftp, SIGNAL(clicked()), this, SLOT(SLOT_PBResetFtpClicked()));
	QObject::connect(ui.pushButton_resethttp, SIGNAL(clicked()), this, SLOT(SLOT_PBResetWebClicked()));

	QObject::connect(ui.pushButton_upload, SIGNAL(clicked()), this, SLOT(SLOT_PBFileUploadClicked()));
	QObject::connect(ui.pushButton_backup, SIGNAL(clicked()), this, SLOT(SLOT_PBFileBackupClicked()));
	QObject::connect(ui.pushButton_update, SIGNAL(clicked()), this, SLOT(SLOT_PBFileUpdateClicked()));

	QObject::connect(ui.pushButton_writecam, SIGNAL(clicked()), this, SLOT(SLOT_PBWriteCamera()));
	QObject::connect(ui.pushButton_writetproduct, SIGNAL(clicked()), this, SLOT(SLOT_PBWriteProduct()));
	QObject::connect(ui.pushButton_readcam, SIGNAL(clicked()), this, SLOT(SLOT_PBReadCamera()));
	QObject::connect(ui.pushButton_readproduct, SIGNAL(clicked()), this, SLOT(SLOT_PBReadProduct()));

	QObject::connect(ui.pushButton_syctime, SIGNAL(clicked()), this, SLOT(SLOT_PBSyncTime()));

	QObject::connect(ui.comboBox_product, SIGNAL(currentIndexChanged(int)), this, SLOT(SLOT_DoComboProductSet(int)));
	QObject::connect(ui.comboBox_camera, SIGNAL(currentIndexChanged(int)), this, SLOT(SLOT_DoComboCamera(int)));

	QObject::connect(ui.pushButton_fullscreen, SIGNAL(clicked()), this, SLOT(SLOT_FullScreen()));

	QObject::connect(ui.pushButton_prompt, SIGNAL(clicked()), this, SLOT(SLOT_PromptLevel()));
	QObject::connect(ui.pushButton_downlevel, SIGNAL(clicked()), this, SLOT(SLOT_DownLevel()));
	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(SLOT_DisplayTime()));

	QObject::connect(m_updatefilegroup, SIGNAL(buttonClicked(int)), this, SLOT(SLOT_RadioButtonClicked(int)));

	QObject::connect(ui.pushButton_exitapp, SIGNAL(clicked()), this, SLOT(close()));

	QObject::connect(ui.pushButton_command, SIGNAL(clicked()), this, SLOT(SLOT_NewCommandWindow()));
	QObject::connect(ui.pushButton_selfbootset, SIGNAL(clicked()), this, SLOT(SLOT_SelfReboot()));
	QObject::connect(ui.pushButton_calibrate, SIGNAL(clicked()), this, SLOT(SLOT_NewCalibrate()));
	QObject::connect(ui.pushButton_diary, SIGNAL(clicked()), this, SLOT(SLOT_NewDiaryQuerry()));

	ui.lineEdit_product->setText("IVFD-FSBOTH");
	m_tabWidget = new QTabWidget(this);
	//m_tabWidget->setObjectName("loadingWidget");
	m_tabWidget->insertTab(0, m_chanelWidget1, tr("video1"));
	m_tabWidget->insertTab(1, m_chanelWidget2, tr("video2"));
	m_tabWidget->setCurrentIndex(m_currentVideoId);

	m_tabWidget->setGeometry(0, 0, 480, 320);
	m_tabWidget->setStyleSheet(QString::fromUtf8("border:2px solid green"));

	m_tabWidget->installEventFilter(this);

	ui.verticalLayout_status->addWidget(m_chartstatus);

	QString mainThread = QString::number((unsigned int )QThread::currentThreadId());
	ui.textBrowser_statusinfo->setText(mainThread);

	ui.horizontalLayout_video->addWidget(m_tabWidget);

	SetCurrentValid(m_currentVideoId);

	if(m_tabWidget != NULL)
	{
		QObject::connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(SLOT_CurrentVideoId(int)));

	}

	if(m_tcpserver != NULL)
	{
		QObject::connect(this, SIGNAL(SIG_StartTcpServerThread()), m_tcpserver, SIGNAL(SIG_StartTcpServerThread()));
		QObject::connect(m_tcpserver, SIGNAL(SIG_TellTop(int, QByteArray)), this, SIGNAL(SIG_GetAlgOutFromTcpServer(int, QByteArray)), Qt::QueuedConnection);
		//QObject::connect(ui.pushButton_selfbootset, SIGNAL(clicked()), m_tcpserver, SIGNAL(SIG_GetStatus())/*, Qt::QueuedConnection*/);
	}
	QObject::connect(this, SIGNAL(SIG_GetAlgOutFromTcpServer(int, QByteArray)), this, SLOT(SLOT_GetAlgOutFromTcpServer(int, QByteArray)));
	QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), this, SLOT(SLOT_GetAlgProcessMsg(int, QByteArray)));
	QObject::connect(this, SIGNAL(SIG_GetServerStatus()), this, SLOT(SLOT_GetServerStatus()));


	//connect the signal info alarm to m_channelwidget1
	if(m_chanelWidget1 != NULL)
	{
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget1, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget1, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget1, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget1, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget1, SIGNAL(SIG_UpdateOnWatch(int)));
	}

	if(m_chanelWidget2 != NULL)
	{
		QObject::connect(this, SIGNAL(SIG_GetAlgProcessMsg(int, QByteArray)), m_chanelWidget2, SIGNAL(SIG_DoAlgProcessMsg(int, QByteArray)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), m_chanelWidget2, SIGNAL(SIG_Test(int)));
		QObject::connect(this, SIGNAL(SIG_Test(int)), this, SLOT(SLOT_CamReverse(int)));
		QObject::connect(ui.pushButton_clearAlarm, SIGNAL(clicked()), m_chanelWidget2, SIGNAL(SIG_ClearAlarm()));
		QObject::connect(this, SIGNAL(SIG_OffsetXY(int, int)), m_chanelWidget2, SIGNAL(SIG_OffsetXY(int, int)));
		QObject::connect(this, SIGNAL(SIG_UpdateOnWatch(int)), m_chanelWidget2, SIGNAL(SIG_UpdateOnWatch(int)));
	}

	ui.pushButton_upload->setDisabled(true);
	ui.pushButton_update->setDisabled(true);
	ui.pushButton_backup->setDisabled(true);
	ui.pushButton_writecam->setDisabled(true);
	ui.pushButton_writetproduct->setDisabled(true);
	
	SLOT_FirstConnectAllRequest();//客户端上线，第一次主动连接设备，发送初始请求。
	emit SIG_StartRegist();
}

IVFDActiveXN::~IVFDActiveXN(){

	//delete m_devmaintainlayout;
	//delete m_hardtestlayout;
	//delete m_appupdatehlayout;
	//delete m_appupdatevlayouleft;
	//delete m_appupdatevlayoutright;
}

void IVFDActiveXN::SLOT_ClearAlarm()
{
	GeneralMsg msgsend;
	msgsend.source = 4;//0=main 1=http, 2=ftp, 3=dip 4=client,4 for activex
	msgsend.dest = 0;//0=main 1=http, 2=ftp, 3=dip 4=client activeX
	msgsend.type = 0;//0=cmd,1=status;
	msgsend.val = 1;
	msgsend.msgid = 2;//1:regist,2:clear alarm

	QByteArray ba;
	ba.resize(sizeof(msgsend));
	memcpy(ba.data(), &msgsend, sizeof(msgsend));
	if(m_tcpclient != NULL){
		m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
	}
	ui.label_status->setStyleSheet("QLabel{background:green}");
	ui.label_status->setAlignment(Qt::AlignCenter);
	ui.label_status->setText(QString::fromLocal8Bit("暂无火警 持续监控"));
}

void IVFDActiveXN::SLOT_SwitchDev()
{
	if(ui.lineEdit_ClientIp){
		QString str = ui.lineEdit_ClientIp->text();
		QByteArray ba;
		int len;
		CmdActiveX cmdsend;
		cmdsend.cmdtype = 1;//删除注册命令
		cmdsend.cmdval = 0;
		memset(cmdsend.cmdstr, 0 ,128);
		QByteArray strba = str.toLatin1();
		len = strba.length() < 128?strba.length():128;
		memcpy(cmdsend.cmdstr, strba.data(), len);
		ba.resize(sizeof(cmdsend));
		memcpy(ba.data(), &cmdsend, sizeof(cmdsend));

		if(m_tcpclient != NULL){
			m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);//58890为设备侦听客户端的端口
			m_flagonregister = 0;
			ui.pushButton_regist->setText(QString::fromLocal8Bit("注册服务"));
		}
	}
	QString progaram = QApplication::applicationFilePath();
	QStringList argument = QApplication::arguments();
	QString workingDirectory = QDir::currentPath();
	QProcess::startDetached(progaram, argument, workingDirectory);
	QApplication::exit();

}

void IVFDActiveXN::SLOT_PBLedStatusClicked(){
	emit SIG_PBCmdSel(0);
}
void IVFDActiveXN::SLOT_PBLedAlarmClicked(){
	emit SIG_PBCmdSel(1);
}
void IVFDActiveXN::SLOT_PBLedPowerClicked(){
	emit SIG_PBCmdSel(2);
}
void IVFDActiveXN::SLOT_PBBuzzerClicked(){
	emit SIG_PBCmdSel(3);
}

void IVFDActiveXN::SLOT_PBRelayErrorClicked(){
	emit SIG_PBCmdSel(4);
}

void IVFDActiveXN::SLOT_PBRelayAlarmClicked(){

	emit SIG_PBCmdSel(5);
}


void IVFDActiveXN::SLOT_PBSuveyClicked(){
	emit SIG_PBCmdSel(6);
}
void IVFDActiveXN::SLOT_PBResetDipClicked(){
	emit SIG_PBCmdSel(7);
}
void IVFDActiveXN::SLOT_PBResetWebClicked(){
	emit SIG_PBCmdSel(8);
}
void IVFDActiveXN::SLOT_PBResetFtpClicked(){
	emit SIG_PBCmdSel(9);
}

void IVFDActiveXN::SLOT_PBResetRtspClicked(){
	emit SIG_PBCmdSel(10);
	emit SIG_PBCmdSel(7);
}
void IVFDActiveXN::SLOT_PBResetAppClicked(){
	emit SIG_PBCmdSel(11);
}
void IVFDActiveXN::SLOT_PBResetSysClicked(){
	emit SIG_PBCmdSel(12);
}

void IVFDActiveXN::SLOT_PBFileUploadClicked(){
	m_process->setProgram("./Tftpd32Server/tftpd32.exe");
	qDebug() << "m_process state = " << m_process->state();

	if(m_process->state() == QProcess::Running){
		QMessageBox::warning(this,"running","ftpserver is running!");
		int id = m_updatefilegroup->checkedId();
		QString cmdstr;
		cmdstr = QString("control$%1@27").arg(id);
		if(m_tcpclient != NULL){
			m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
		}
	}else {
		QMessageBox::StandardButton button;
		button = QMessageBox::question(this, QString::fromLocal8Bit("ftp服务"), QString::fromLocal8Bit("确定启动？"), QMessageBox::Yes|QMessageBox::No);
		if(button == QMessageBox::No){
			return;
		}else{
			m_process->start();
			int id = m_updatefilegroup->checkedId();
			QString cmdstr;
			cmdstr = QString("control$%1@27").arg(id);
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
		}
	}
}
void IVFDActiveXN::SLOT_PBFileBackupClicked(){
	emit SIG_PBCmdSel(13);
}
void IVFDActiveXN::SLOT_PBFileUpdateClicked(){
	emit SIG_PBCmdSel(14);
}

void IVFDActiveXN::SLOT_PBWriteProduct()
{
	int id = 0;
	QString setvalstr = ui.lineEdit_product->text();
	int index = ui.comboBox_product->currentIndex();
	qDebug() << "index = " << index;
	switch(index)
	{
	case 0://type
		{
			QString regText("^IVFD-(FSBOTH|FONLY|SONLY)$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 11;//devclass
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入IVFD-FONLY/SONLY/FSBOTH"));
			}
			break;
		}
	case 1://class
		{
			QString regText("^IVFD2.[0-9]-[\\d]{4}$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 10;//devtype
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入IVFD-2.x-xxxx"));
			}
			break;
		}
	case 2://time
		{
			QString regText("^(([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]{1}|[0-9]{1}[1-9][0-9]{2}|[1-9][0-9]{3})-(((0[13578]|1[02])-(0[1-9]|[12][0-9]|3[01]))|((0[469]|11)-(0[1-9]|[12][0-9]|30))|(02-(0[1-9]|[1][0-9]|2[0-8]))))|((([0-9]{2})(0[48]|[2468][048]|[13579][26])|((0[48]|[2468][048]|[3579][26])00))-02-29)$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 9;//timelimit
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入日期xxxx-xx-xx"));
			}
			break;
		}
	case 3://func
		{
			QString regText("^[\\d]-[\\d]-[\\d]-[\\d]$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 12;//funcode
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入x-x-x-x"));
			}
			break;
		}
	case 4:
		{
			QString regText("^[A-Z]{4}-[A-Z]{5,13}$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 15;//devmore
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入PROD-XX(6-13)"));
			}
			break;
		}
	case 5:
		{
			QString regText("^[A-Z]{2}-[A-Z]-(C|I)-[\\d]{3}$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 13;//userid
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入XX-X-NNN"));
			}
			break;
		}
	case 6:
		{
			QString regText("^[a-z]{4,5}-[a-z]{5,10}$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 4;//username
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入X(4-5)-x(5-10)"));
			}

			break;
		}
	case 7:
		{
			QString regText("^USER-LEVEL[\\d]$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 14;//userlevel
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入USER-LEVEL(1|2|3)"));
			}
			break;
		}
	case 8:
		{
			QString regText("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 0;//ipaddress
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入正确的IP地址"));
			}
			break;
		}
	case 9:
		{
			QString regText("^(254|252|248|240|224|192|128|0)\\.0\\.0\\.0|255\\.(254|252|248|240|224|192|128|0)\\.0\\.0|255\\.255\\.(254|252|248|240|224|192|128|0)\\.0|255\\.255\\.255\\.(254|252|248|240|224|192|128|0)$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 3;//mask
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入正确的掩码"));
			}
			break;
		}
	case 10:
		{
			QString regText("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 1;//netgate
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入正确的IP地址"));
			}
			break;
		}
	case 11:
		{
			QString regText("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
			QString str = ui.lineEdit_product->text();
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 16;//client ftpserver
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入正确的IP地址"));
			}
			break;
		}
	}
	QString cmdstr = QString("setByteArray$%1@%2").arg(setvalstr).arg(id);
	if(m_tcpclient != NULL){
		m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
	}
}

void IVFDActiveXN::SLOT_PBWriteCamera()
{
	int id = 1;//set the detect camera
	int index = ui.comboBox_camera->currentIndex();
	int spinval = ui.spinBox_camera->value();
	qDebug() << "current args index is " << index << " cunrrent spinval is " << spinval;
	QString cmdstr;//for the command
	switch (index)
	{
	case 0://bright,just for camera0
		{
			id = 1;
			qDebug() << spinval << "id = " << id;
			cmdstr = QString("setBright$%1@%2").arg(QString::number(spinval)).arg(QString::number(id));
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 1://gain
		{
			cmdstr = QString("setGain$%1@%2").arg(QString::number(spinval)).arg(QString::number(id));
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 2://wb
		{
			cmdstr = QString("setBrightBal$%1@%2").arg(QString::number(spinval)).arg(QString::number(id));
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 3://contrast
		{
			cmdstr = QString("setCharoma$%1@%2").arg(QString::number(spinval)).arg(QString::number(id));
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 4://saturation
		{
			cmdstr = QString("setSaturation$%1@%2").arg(QString::number(spinval)).arg(QString::number(id));
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 5://explosure
		{
			cmdstr = QString("setExposure$%1@%2").arg(QString::number(spinval)).arg(QString::number(id));
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 6://sense
		{
			cmdstr = QString("setAlgPara$%1@%2").arg(QString::number(spinval)).arg(QString::number(12));
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 7://mindetect
		{
			cmdstr = QString("setAlgPara$%1@%2").arg(QString::number(spinval)).arg(QString::number(13));
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	}

}
void IVFDActiveXN::SLOT_PBReadCamera()
{
	int id = 1;//set the detect camera
	int index = ui.comboBox_camera->currentIndex();
	QString cmdstr;//for the command
	int sel = 0;
	switch(index){
	case 0:{//bright1
		sel = 1;
		break;
		   }
	case 1:{//gain1
		sel = 3;
		break;
		   }
	case 2:{//wb1
		sel = 5;
		break;
		   }
	case 3:{//contrast1
		sel = 7;
		break;
		   }
	case 4:{//saturation1
		sel = 9;
		break;
		   }
	case 5:{//exposure1
		sel = 11;
		break;
		   }
	case 6:{//sensefire
		sel = 12;
		break;
		   }
	case 7:{//mindetect
		sel =13;
		break;
		   }
	}
	cmdstr = QString("getsysinfo$%1@%2").arg(QString::number(id)).arg(QString::number(sel));
	if(m_tcpclient != NULL){
		m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
	}
}

void IVFDActiveXN::SLOT_PBReadProduct()
{
	int id = 2;//set the product area
	int index = ui.comboBox_product->currentIndex();
	QString cmdstr;//for the command
	qDebug() << "index = " << index;
	int sel = 0;
	switch(index){
	case 0:{//authendeviceclass
		sel = 16;
		break;
		   }
	case 1:{//autendevicename
		sel = 15;
		break;
		   }
	case 2:{//timelimit
		sel = 14;
		break;
		   }
	case 3:{//funccode
		sel = 17;
		break;
		   }
	case 4:{//more
		sel = 20;
		break;
		   }
	case 5:{//id
		sel = 18;
		break;
		   }
	case 6:{//name
		sel = 21;
		break;
		   }
	case 7:{//level
		sel = 19;
		break;
		   }
	case 8:{//ip
		sel = 22;
		break;
		   }
	case 9:{//mask
		sel = 23;
		break;
		   }
	case 10:{//netgate
		sel = 24;
		break;
		   }
	case 11:{//upload ftpservr
		sel = 25;
		break;
		   }
	}
	cmdstr = QString("getsysinfo$%1@%2").arg(QString::number(id)).arg(QString::number(sel));
	if(m_tcpclient != NULL){
		m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
	}

}


void IVFDActiveXN::SLOT_PBCmdSel(int sel){
	switch(sel){
	case 0:
		{	
			QString cmdstr;
			if(m_flagledstatus == 0){
				m_flagledstatus = 1;
				ui.pushButton_status->setText(QString::fromLocal8Bit("状态灯暗"));
				cmdstr = QString("setGpio$1@237");//237 for led status, yellow
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_yellow->setStyleSheet("QLabel{border-image:url(:/image/yellowled.png)}");
			}else {
				m_flagledstatus = 0;
				ui.pushButton_status->setText(QString::fromLocal8Bit("状态灯亮"));
				cmdstr = QString("setGpio$0@237");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_yellow->setStyleSheet("QLabel{border-image:url(:/image/ledoff.png)}");
			}
			break;
		}
	case 1:
		{
			QString cmdstr;
			if(m_flagledalarm == 0){
				m_flagledalarm = 1;
				ui.pushButton_alarm->setText(QString::fromLocal8Bit("报警灯暗"));
				cmdstr = QString("setGpio$1@239");//239 for led alarm ,red
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_red->setStyleSheet("QLabel{border-image:url(:/image/redled.png)}");
			}else {
				m_flagledalarm = 0;
				ui.pushButton_alarm->setText(QString::fromLocal8Bit("报警灯亮"));
				cmdstr = QString("setGpio$0@239");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_red->setStyleSheet("QLabel{border-image:url(:/image/ledoff.png)}");
			}
			break;
		}
	case 2:
		{
			QString cmdstr;
			if(m_flagledpower == 0){
				m_flagledpower = 1;
				ui.pushButton_power->setText(QString::fromLocal8Bit("电源灯暗"));
				cmdstr = QString("setGpio$1@240");//240 for led power ,green
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_green->setStyleSheet("QLabel{border-image:url(:/image/greenled.png)}");
			}else {
				m_flagledpower = 0;
				ui.pushButton_power->setText(QString::fromLocal8Bit("电源灯亮"));
				cmdstr = QString("setGpio$0@240");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_green->setStyleSheet("QLabel{border-image:url(:/image/ledoff.png)}");
			}
			break;
		}
	case 3:
		{
			QString cmdstr;
			if(m_flagbuzzer == 0){
				m_flagbuzzer = 1;
				ui.pushButton_buzzer->setText(QString::fromLocal8Bit("蜂鸣器关"));
				cmdstr = QString("setGpio$1@244");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_buzzer->setStyleSheet("QLabel{border-image:url(:/image/ringon.png)}");
			}else {
				m_flagbuzzer = 0;
				ui.pushButton_buzzer->setText(QString::fromLocal8Bit("蜂鸣器开"));
				cmdstr = QString("setGpio$0@244");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_buzzer->setStyleSheet("QLabel{border-image:url(:/image/ringoff.png)}");
			}
			break;
		}
	case 4:
		{
			QString cmdstr;
			if(m_flagrelayerror == 0){
				m_flagrelayerror = 1;
				ui.pushButton_relayerror->setText(QString::fromLocal8Bit("故障继电器闭"));
				cmdstr = QString("setGpio$1@241");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_relayerror->setStyleSheet("QLabel{border-image:url(:/image/switch_on3.png)}");
			}else {
				m_flagrelayerror = 0;
				ui.pushButton_relayerror->setText(QString::fromLocal8Bit("故障继电器开"));
				cmdstr = QString("setGpio$0@241");//relay1
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_relayerror->setStyleSheet("QLabel{border-image:url(:/image/switch_off3.png)}");
			}
			break;
		}
	case 5:
		{
			QString cmdstr;
			if(m_flagrelayalarm == 0){
				m_flagrelayalarm = 1;
				ui.pushButton_relayalarm->setText(QString::fromLocal8Bit("报警电器闭"));
				cmdstr = QString("setGpio$1@242");//relay2
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_relayalarm->setStyleSheet("QLabel{border-image:url(:/image/switch_on3.png)}");
			}else {
				m_flagrelayalarm = 0;
				ui.pushButton_relayalarm->setText(QString::fromLocal8Bit("报警继电器开"));
				cmdstr = QString("setGpio$0@242");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
				}
				ui.label_relayalarm->setStyleSheet("QLabel{border-image:url(:/image/switch_off3.png)}");
			}
			break;
		}
	case 6:
		{
			QString cmdstr;
			if(m_flagsuvey == 0){
				m_flagsuvey = 1;
				ui.pushButton_suvey->setText(QString::fromLocal8Bit("设备撤防"));
				cmdstr = QString("control$1@13");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
					ui.label_statuspic->setStyleSheet("QLabel{border-image:url(:/image/eyegreen.png)}");

				}
			}else {
				m_flagsuvey = 0;
				ui.pushButton_suvey->setText(QString::fromLocal8Bit("设备布防"));
				cmdstr = QString("control$0@13");
				if(m_tcpclient != NULL){
					m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
					ui.label_statuspic->setStyleSheet("QLabel{border-image:url(:/image/eyegrey.png)}");
				}
			}
			break;
		}

	case 7://reset dip
		{
			QString cmdstr;
			cmdstr = QString("control$0@17");
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 8://reset ftp
		{
			QString cmdstr;
			cmdstr = QString("control$0@18");
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 9://reset http
		{
			QString cmdstr;
			cmdstr = QString("control$0@19");
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 10://reset rtsp
		{
			QString cmdstr;
			cmdstr = QString("control$0@20");
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 11:
		{
			QString cmdstr;
			cmdstr = QString("control$0@21");
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 12:
		{
			QString cmdstr;
			cmdstr = QString("control$0@22");
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 13:
		{
			int id = m_updatefilegroup->checkedId();
			QString cmdstr;
			cmdstr = QString("control$%1@26").arg(id);//backup all app
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	case 14:
		{
			int id = m_updatefilegroup->checkedId();
			QString cmdstr;
			cmdstr = QString("control$%1@25").arg(id);//update all app
			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
			}
			break;
		}
	default:{

			}
	}

}


void IVFDActiveXN::SLOT_DoComboProductSet(int index){

	//qDebug() << index;
	ui.label_readproduct->clear();
	switch(index){
	case 0:
		{
			ui.lineEdit_product->setText("IVFD-FSBOTH");
			break;
		}
	case 1:
		{
			ui.lineEdit_product->setText("IVFD2.0-1000");
			break;
		}
	case 2:{
		ui.lineEdit_product->setText("2020-12-31");
			break;
		   }
	case 3:
		{
			ui.lineEdit_product->setText("0-0-0-0");
			break;
		}
	case 4:
		{
			ui.lineEdit_product->setText("PROD-CERTIFICATION");
			break;
		}
	case 5:
		{
			ui.lineEdit_product->setText("BD-P-I-002");
			break;
		}
	case 6:{
		ui.lineEdit_product->setText("pinghu-bandweaver");
		break;
		   }
	case 7:
		{
			ui.lineEdit_product->setText("USER-LEVEL2");
			break;
		}
	case 8:
		{
			ui.lineEdit_product->setText("192.168.120.166");
			break;
		}
	case 9:
		{
			ui.lineEdit_product->setText("255.255.255.0");
			break;
		}
	case 10:
		{
			ui.lineEdit_product->setText("192.168.120.1");
			break;
		}
	case 11:
		{
			qDebug() << index;
			ui.lineEdit_product->setText("192.168.120.229");
			break;
		}
	}
}


void IVFDActiveXN::SLOT_DoComboCamera(int index){
	ui.label_readcam->clear();
	switch(index)
	{
			ui.spinBox_camera->setSingleStep(1);
	case 0://bright
		{
			ui.spinBox_camera->setRange(1,8);
			ui.spinBox_camera->setValue(8);
			ui.spinBox_camera->setSingleStep(1);
			break;
		}
	case 1://gain
		{
			ui.spinBox_camera->setRange(1,8);
			ui.spinBox_camera->setValue(8);
			ui.spinBox_camera->setSingleStep(1);
			break;
		}
	case 2://wb
		{
			ui.spinBox_camera->setRange(1,6);
			ui.spinBox_camera->setValue(4);
			ui.spinBox_camera->setSingleStep(1);
			break;
		}
	case 3://contrast
		{
			ui.spinBox_camera->setRange(1,3);
			ui.spinBox_camera->setValue(4);
			ui.spinBox_camera->setSingleStep(1);
			break;
		}
	case 4://saturation
		{
			ui.spinBox_camera->setRange(1,8);
			ui.spinBox_camera->setValue(4);
			ui.spinBox_camera->setSingleStep(1);
			break;
		}
	case 5://explosure
		{
			ui.spinBox_camera->setRange(1,8);
			ui.spinBox_camera->setValue(1);
			ui.spinBox_camera->setSingleStep(1);
			break;
		}
	case 6://sense
		{
			ui.spinBox_camera->setRange(1,100);
			ui.spinBox_camera->setValue(60);
			ui.spinBox_camera->setSingleStep(5);
			break;
		}
	case 7://mindetect
		{
			ui.spinBox_camera->setRange(5,100);
			ui.spinBox_camera->setValue(30);
			ui.spinBox_camera->setSingleStep(5);
			break;
		}
	}
}

void IVFDActiveXN::SLOT_DisplayTime()
{
	QDateTime time = QDateTime::currentDateTime();
	ui.label_time->setText(time.toString());
	//m_chartstatus->addData(qrand()%100);
	//QDateTime utcTime = time.toUTC();
	//quint64 secs = time.toMSecsSinceEpoch();
	//quint64 ucDt = utcTime.toMSecsSinceEpoch();

	//QDateTime testtime = QDateTime::fromMSecsSinceEpoch(1521036578065);
	//QString testStr = testtime.toString("yyyy-MM-dd hh:mm:ss dddd");

	//QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
	//QString utcStr = utcTime.toString("yyyy-MM-dd hh:mm:ss dddd");
	//ui.label_time->setText(utcStr + testStr + QString::number(secs)/* + QString(" ") + QString::number(ucDt)*/);
}

void IVFDActiveXN::SLOT_PBSyncTime()
{
	QDateTime time = QDateTime::currentDateTime();
	QString datestr = time.date().toString("yyyy.MM.dd");
	QString timestr = time.time().toString("hh:mm:ss");
	QString datetimestr = datestr.append("-").append(timestr);
	QString cmdstr = QString("setByteArray$%1@%2").arg(datetimestr).arg(17);
	if(m_tcpclient != NULL){
		m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
	}
}

bool IVFDActiveXN::RegularFunc(QString strRegExp, QString strText)
{
	QRegularExpression regExp(strRegExp);
	QRegularExpressionMatch regExpMatch = regExp.match(strText);
	if(regExpMatch.hasMatch()){
		QString strTextCap = regExpMatch.captured(0);
		qDebug() << "matched!";
		return true;
	}else{
		qDebug() << "no matched! " << strRegExp << " " << strText;
		return false;
	}

}

bool IVFDActiveXN::event(QEvent *e)
{
	if(e->type() == QEvent::KeyPress){
		QKeyEvent *keyevent = static_cast<QKeyEvent *>(e);
		if(keyevent->key() == Qt::Key_Return){
			qDebug() << "you press return";
			return true;
		}
	}
	return QWidget::event(e);

}

bool IVFDActiveXN::eventFilter(QObject *obj, QEvent *ev){
	if(obj == ui.lineEdit_ClientIp){
		if(ev->type() == QEvent::KeyPress){
			QKeyEvent *keyevent = static_cast<QKeyEvent *>(ev);
			if(keyevent->key() == Qt::Key_Return){
				qDebug() << "the key event is " << keyevent->key();
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}	
	}else if(obj == ui.lineEdit_product){
		if(ev->type() == QEvent::KeyPress){
			QKeyEvent *keyevent = static_cast<QKeyEvent *>(ev);
			if(keyevent->key() == Qt::Key_Return){
				qDebug() << "the key event is " << keyevent->key();
				SLOT_RegularInput();
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}	
	}else if(obj == m_tabWidget){
		if(ev->type() == QEvent::KeyPress){
			QKeyEvent *keyevent = static_cast<QKeyEvent *>(ev);
			if(keyevent->key() == Qt::Key_Escape){
				qDebug() << "the key event is " << keyevent->key();
				SLOT_FullScreen();
				return true;
			}else{
				return false;
			}

		}else if(ev->type() == QEvent::MouseButtonDblClick){
			SLOT_FullScreen();
			return true;
		}else{
			return false;
		}	
	}else if (obj == uitmp.lineEdit_ClientIP){
		if(ev->type() == QEvent::KeyPress){
			QKeyEvent *keyevent = static_cast<QKeyEvent *>(ev);
			if(keyevent->key() == Qt::Key_Return){
				QString hostaddr = uitmp.lineEdit_DevIP->text();
				QString clientaddr = uitmp.lineEdit_ClientIP->text();
				QString regText("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
				bool retRegular1 = RegularFunc(regText, hostaddr);
				bool retRegular2 = RegularFunc(regText, clientaddr);
				if(retRegular1 == true && retRegular2 == true){
					SLOT_Init();
				}else{
					QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入正确的IP地址"));
				}
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}else if (obj == uitmp.lineEdit_DevIP){
		if(ev->type() == QEvent::KeyPress){
			QKeyEvent *keyevent = static_cast<QKeyEvent *>(ev);
			if(keyevent->key() == Qt::Key_Return){
				QString hostaddr = uitmp.lineEdit_DevIP->text();
				QString clientaddr = uitmp.lineEdit_ClientIP->text();
				QString regText("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
				bool retRegular1 = RegularFunc(regText, hostaddr);
				bool retRegular2 = RegularFunc(regText, clientaddr);
				if(retRegular1 == true && retRegular2 == true){
					SLOT_Init();
				}else{
					QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入正确的IP地址"));
				}
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}else{
		return QWidget::eventFilter(obj,ev);
	}
}

void IVFDActiveXN::SLOT_FullScreen()
{
	qDebug() << "Begin to full screen!";
	//this->setLayout(ui.horizontalLayout_video);
	//update();
	if(m_flagfullscreen == 0){
		m_tabWidget->setWindowFlags(Qt::Window);
		m_tabWidget->showFullScreen();
		m_flagfullscreen = 1;
	}else {
		m_tabWidget->setWindowFlags(Qt::SubWindow);
		m_tabWidget->showNormal();
		m_flagfullscreen = 0;
	}
}

void IVFDActiveXN::SLOT_RegularInput()
{
	int id = 0;
	QString str = ui.lineEdit_product->text();
	switch (ui.comboBox_product->currentIndex())
	{
	case 0:
		{
			QString regText("^IVFD-(FSBOTH|FONLY|SONLY)$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 11;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入IVFD-FONLY/SONLY/FSBOTH"));
			}
			break;
		}
	case 1:
		{
			QString regText("^IVFD2.[0-9]-[\\d]{4}$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 10;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入IVFD-2.x-xxxx"));
			}
			break;
		}
	case 2:
		{
			QString regText("^(([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]{1}|[0-9]{1}[1-9][0-9]{2}|[1-9][0-9]{3})-(((0[13578]|1[02])-(0[1-9]|[12][0-9]|3[01]))|((0[469]|11)-(0[1-9]|[12][0-9]|30))|(02-(0[1-9]|[1][0-9]|2[0-8]))))|((([0-9]{2})(0[48]|[2468][048]|[13579][26])|((0[48]|[2468][048]|[3579][26])00))-02-29)$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 9;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入日期xxxx-xx-xx"));
			}
			break;
		}
	case 3:
		{
			QString regText("^[\\d]-[\\d]-[\\d]-[\\d]$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 12;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入x-x-x-x"));
			}
			break;
		}
	case 4:
		{
			QString regText("^[A-Z]{4}-[A-Z]{5,13}$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 15;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入PROD-XX(6-13)"));
			}
			break;
		}
	case 5:
		{
			QString regText("^[A-Z]{2}-[A-Z]-(C|I)-[\\d]{3}$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 13;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入XX-X-NNN"));
			}
			break;
		}
	case 6:
		{
			QString regText("^[a-z]{4,5}-[a-z]{5,10}$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 4;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入X(4-5)-x(5-10)"));
			}
			break;
		}
	case 7:
		{
			QString regText("^USER-LEVEL[\\d]$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 14;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入USER-LEVEL(1|2|3)"));
			}
			break;
		}
	case 8:
		{
			QString regText("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 0;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入正确的IP地址"));
			}
			break;
		}
	case 9:
		{
			QString regText("^(254|252|248|240|224|192|128|0)\\.0\\.0\\.0|255\\.(254|252|248|240|224|192|128|0)\\.0\\.0|255\\.255\\.(254|252|248|240|224|192|128|0)\\.0|255\\.255\\.255\\.(254|252|248|240|224|192|128|0)$");	
			bool retRegular = RegularFunc(regText, str);
			if(retRegular == true){
				id = 3;
			}else{
				QMessageBox::warning(NULL,"error",QString::fromLocal8Bit("输入正确的掩码"));
			}
			break;
		}
	}
	QString cmdstr = QString("setByteArray$%1@%2").arg(str).arg(id);
	if(m_tcpclient != NULL){
		m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, cmdstr.toLatin1(), cmdstr.toLatin1());
	}
}

void IVFDActiveXN::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton button;
	button = QMessageBox::question(this, QString::fromLocal8Bit("退出设备？"), QString::fromLocal8Bit("确认退出设备"), QMessageBox::Yes|QMessageBox::No);
	if(button == QMessageBox::No){
		event->ignore();
	}else{
		if(ui.lineEdit_ClientIp){
			QString str = ui.lineEdit_ClientIp->text();
			QByteArray ba;
			int len;
			CmdActiveX cmdsend;
			cmdsend.cmdtype = 1;//删除注册命令
			cmdsend.cmdval = 0;
			memset(cmdsend.cmdstr, 0 ,128);
			QByteArray strba = str.toLatin1();
			len = strba.length() < 128?strba.length():128;
			memcpy(cmdsend.cmdstr, strba.data(), len);
			ba.resize(sizeof(cmdsend));
			memcpy(ba.data(), &cmdsend, sizeof(cmdsend));

			if(m_tcpclient != NULL){
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);//58890为设备侦听客户端的端口
				m_flagonregister = 0;
				ui.pushButton_regist->setText(QString::fromLocal8Bit("注册服务"));
			}
		}
		event->accept();
	}

}

void IVFDActiveXN::SLOT_CamReverse(int flag){
	qDebug() << "start to camreverse!" << flag;
	//tell the dip to reverse the cam capture
	GeneralMsg msgsend;
	msgsend.dest = 0;//0 for main
	msgsend.source = 4;//4 for activex
	msgsend.type = 5;//5 for reverses
	msgsend.val = 1;
	msgsend.msgid = 3;//3 FOR cam revert

	QByteArray ba;
	ba.resize(sizeof(msgsend));
	memcpy(ba.data(), &msgsend, sizeof(msgsend));
	if(m_tcpclient != NULL){
		m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
	}
}

void IVFDActiveXN::SLOT_RadioButtonClicked(int val)
{
	ui.textBrowser_eventinfo->append(QString::number(val));

}

void IVFDActiveXN::SLOT_PromptLevel()
{
	ui.pushButton_upload->setDisabled(false);
	ui.pushButton_update->setDisabled(false);
	ui.pushButton_backup->setDisabled(false);
	ui.pushButton_writecam->setDisabled(false);
	ui.pushButton_writetproduct->setDisabled(false);
	
}

void IVFDActiveXN::SLOT_DownLevel()
{
	ui.pushButton_upload->setDisabled(true);
	ui.pushButton_update->setDisabled(true);
	ui.pushButton_backup->setDisabled(true);
	ui.pushButton_writecam->setDisabled(true);
	ui.pushButton_writetproduct->setDisabled(true);
}

void IVFDActiveXN::SLOT_NewCommandWindow()
{
	m_commandwindow = new IVFDCommand(this);
	if(m_commandwindow != NULL){
		m_commandwindow->show();
		m_commandwindow->setModal(true);
		//QObject::connect(m_commandwindow->ui->okButton, SIGNAL(clicked()), this, SLOT(SLOT_OKButton()));
		int result = m_commandwindow->exec();
		if(result == QDialog::Accepted){
			QString str = m_commandwindow->ui->lineEdit->text();
			if(str.isEmpty()){
				QMessageBox::warning(this,"warning",QString::fromLocal8Bit("输入非法"));
				return;
			}
			QByteArray ba;
			int len;
			CmdActiveX cmdsend;
			cmdsend.cmdtype = 4;//cmdtype = 4:command to be excute in t3
			cmdsend.cmdval = 0;
			memset(cmdsend.cmdstr, 0 ,128);

			QByteArray strba = str.toLatin1();
			len = strba.length() < 128?strba.length():128;
			memcpy(cmdsend.cmdstr, strba.data(), len);
			ba.resize(sizeof(cmdsend));
			memcpy(ba.data(), &cmdsend, sizeof(cmdsend));
			if(m_tcpclient != NULL){
				qDebug() << "send the command:" << cmdsend.cmdstr;
				m_tcpclient->SIG_SendMsgToServer(m_serverip, 58890, ba, ba);
			}
		}else{
			qDebug() << "Cancle exit";
		}
	}


}

void IVFDActiveXN::SLOT_OKButton()
{
	if(m_commandwindow != NULL){

	}
}

void IVFDActiveXN::SLOT_SelfReboot()
{
	m_selfrebootset = new IVFDSelfRebootSet(this);
	qDebug() << "open selfreboot window";
	if(m_selfrebootset != NULL){
		m_selfrebootset->setAttribute(Qt::WA_TranslucentBackground, false);
		m_selfrebootset->show();
	}else{
		qDebug() << "create selfreboot window failed";
	}
}

void IVFDActiveXN::SLOT_NewDiaryQuerry()
{
	m_diaryquerry = new IVFDDiaryQuerry(this);
	qDebug() << "open diaryquerry window";
	if(m_diaryquerry != NULL){
		m_diaryquerry->setAttribute(Qt::WA_TranslucentBackground, false);
		m_diaryquerry->show();
	}else{
		qDebug() << "create diaryquerry window failed";
	}
}

void IVFDActiveXN::SLOT_NewCalibrate()
{
	m_calibratwindow = new IVFDCalibrate(this);
	qDebug() << "open calibration window";
	if(m_calibratwindow != NULL){
		QObject::connect(m_calibratwindow, SIGNAL(SIG_NotifyChannel()), this, SLOT(SLOT_NotifyChannel()));
		if(m_chanelWidget2 != NULL){
			qDebug() << "tell the channel to mark!";
			QObject::connect(m_calibratwindow, SIGNAL(SIG_NotifyChannel()), m_chanelWidget2, SIGNAL(SIG_NotifyChannel()));
		}

		m_calibratwindow->setAttribute(Qt::WA_TranslucentBackground, false);
		m_calibratwindow->show();
	}else{
		qDebug() << "create calibration window failed";
	}
}

void IVFDActiveXN::SLOT_NotifyChannel()
{
	qDebug() << "start to display calibrate mark!";
	if(m_chanelWidget2 == NULL){
		qDebug() << "no channel available!";
	}else{
		qDebug() << "channel available!";

	}

}

//QAXFACTORY_DEFAULT(IVFDActiveXN,					//widget class
//	   "{3A27CB1B-3434-4DD7-B81C-7967BD485CF2}",	//class ID
//	   "{65866944-9053-4235-9397-8EF2EA9A8229}",	//interface ID
//	   "{8F5F5DB5-2398-400C-9A40-BFF4249CCAEB}",	//event interface ID
//	   "{C42F12A1-3BAA-4C58-A914-10D3D4230FA3}",	//type library ID
//	   "{0C75BE21-DF0F-45B9-813F-C774DEEDF45B}")	//application ID
//
//
