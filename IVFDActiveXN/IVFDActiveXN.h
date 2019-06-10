#ifndef IVFDACTIVEXN_H
#define IVFDACTIVEXN_H

#include <QtWidgets/QWidget>
#include <ActiveQt/QAxBindable>
#include <QTabWidget>
#include <QByteArray>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QTextCodec>
#include <QTranslator>
#include <QDialog>
#include <QThreadPool>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDir>
#include <QTimer>
#include <QProcess>
#include <QButtonGroup>
#include <QDateTime>
#include <QFile>
#include <QSettings>
#include <QVariant>
#include <QRegExp>
#include <QRegExpValidator>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QEvent>
#include <QKeyEvent>
#include <QProcess>

#include "ui_IVFDActiveXN.h"
#include "ChanelWidget.h"
#include "IVFDTcpServer.h"
#include "IVFDTcpClient.h"
#include "IVFDChart.h"
#include "ui_IVFDActiveXInit.h"
#include "IVFDCommand.h"
#include "IVFDCalibrate.h"
#include "IVFDDiaryQuerry.h"
#include "IVFDSelfRebootSet.h"

#define DUALDISPLAY 1

//#pragma execution_character_set("uft-8")

typedef struct CmdARG{
	int cmdtype;
	int argnum;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
}CmdARG;

typedef struct CmdActiveX{
	int cmdtype;
	int cmdval;
	char cmdstr[128];
}CmdActiveX;

typedef struct GeneralMsg{
	int source;//0=main,1=http, 2=ftp, 3=dip 4=client
	int dest;//0=main,1=http,2=ftp,3=dip 4=client
	int type;//0=cmd,1=status;
	int msgid;//defined by user;
	int val;//defined by user;
}GeneralMsg;

class IVFDActiveXN : public QWidget
{
	Q_OBJECT

public:
	IVFDActiveXN(QWidget *parent = 0);
	~IVFDActiveXN();
	QProcess *m_process;
	QThreadPool *m_threadpool;
	ChanelWidget *m_chanelWidget1;
	IVFDChart *m_chartstatus;
	//IVFDActiveXIint *m_initWidget;
	QVBoxLayout *m_devmaintainlayout;
	QVBoxLayout *m_hardtestlayout;

	QHBoxLayout *m_appupdatehlayout;
	QVBoxLayout *m_appupdatevlayouleft;
	QVBoxLayout *m_appupdatevlayoutright;
	QButtonGroup *m_updatefilegroup;

	QTimer *m_timer;

	quint16 listenport1;
	QString rtspurl1;
	QString m_serverip;
	QString m_clientip;

	int m_flagonregister;
	int m_flagledstatus;
	int m_flagledalarm;
	int m_flagledpower;
	int m_flagbuzzer;
	int m_flagrelayerror;
	int m_flagrelayalarm;
	int m_flagsuvey;
	int m_flagfullscreen;
	int m_flaguserlevel;

#ifdef DUALDISPLAY
	ChanelWidget *m_chanelWidget2;
	quint16 listenport2;
	QString rtspurl2;
#endif
	QTabWidget *m_tabWidget;
	int m_currentVideoId;

	IVFDTcpServer *m_tcpserver;
	IVFDTcpClient *m_tcpclient;
	int m_offsetStep;
	//ALARMDATAINFO *m_alarmData;

private:
	Ui::IVFDActiveXNClass ui;
	Ui::IVFDActiveXinitClass uitmp;
	IVFDCommand *m_commandwindow;
	IVFDCalibrate *m_calibratwindow;
	IVFDDiaryQuerry *m_diaryquerry;
	IVFDSelfRebootSet *m_selfrebootset;

	void InitThis();
	void SetCurrentValid(int index);
	QString GetHostIpAddress();
	bool event(QEvent *e);
	bool RegularFunc(QString strRegExp, QString strText);

	void closeEvent(QCloseEvent *event);
protected:
	bool eventFilter(QObject *obj, QEvent *ev);
signals:
	void SIG_GetAlgOutFromTcpServer(int, QByteArray);
	void SIG_GetAlgProcessMsg(int, QByteArray);
	void SIG_GetServerStatus();
	void SIG_StartTcpServerThread();
	void SIG_Test(int);
	void SIG_ToJs(QString);
	void SIG_OffsetXY(int type, int val);
	void SIG_UpdateServerIP();
	void SIG_UpdateOnWatch(int);
	void SIG_StartRegist();
	void SIG_PBCmdSel(int);
	void SIG_Snap(int);

public slots:
	void SLOT_CurrentVideoId(int id);
	void SLOT_Play1();
	void SLOT_Reconnect1();
	void SLOT_StopPlay1();
	void SLOT_Snap1();
	void SLOT_ToJs(QString);
	void SLOT_SetServerIP(QString);
	void SLOT_ClearAlarm();

#ifdef DUALDISPLAY
	void SLOT_Play2();
	void SLOT_Reconnect2();
	void SLOT_StopPlay2();
	void SLOT_Snap2();
#endif
	void SLOT_GetAlgOutFromTcpServer(int, QByteArray);
	void SLOT_GetAlgProcessMsg(int, QByteArray);
	void SLOT_SendToJs();
	void SLOT_GetServerStatus();
	void SLOT_TestJsCmd();
	void SLOT_ClearStatus();
	void SLOT_ClearEvent();
	void SLOT_ClearXY();

	void SLOT_OffsetXinc();
	void SLOT_OffsetYinc();
	void SLOT_OffsetXDec();
	void SLOT_OffsetYDec();
	void SLOT_Regist();
	void SLOT_UpdateServerIP();
	void SLOT_ReconnectAll();
	void SLOT_FirstConnectAllRequest();

	void SLOT_Init();
	void SLOT_InitReset();
	void SLOT_DisplayTime();

	void SLOT_SwitchDev();
	
	void SLOT_PBLedStatusClicked();
	void SLOT_PBLedAlarmClicked();
	void SLOT_PBLedPowerClicked();
	void SLOT_PBBuzzerClicked();
	void SLOT_PBRelayErrorClicked();
	void SLOT_PBRelayAlarmClicked();
	void SLOT_PBSuveyClicked();
	void SLOT_PBResetDipClicked();
	void SLOT_PBResetFtpClicked();
	void SLOT_PBResetWebClicked();
	void SLOT_PBResetRtspClicked();
	void SLOT_PBResetAppClicked();
	void SLOT_PBResetSysClicked();
	void SLOT_PBFileUploadClicked();
	void SLOT_PBFileBackupClicked();
	void SLOT_PBFileUpdateClicked();

	void SLOT_PBWriteProduct();
	void SLOT_PBWriteCamera();
	void SLOT_PBReadCamera();
	void SLOT_PBReadProduct();

	void SLOT_PBCmdSel(int);

	void SLOT_PBSyncTime();

	void SLOT_DoComboProductSet(int);
	void SLOT_DoComboCamera(int);

	void SLOT_FullScreen();
	void SLOT_RegularInput();

	void SLOT_RadioButtonClicked(int);
	void SLOT_PromptLevel();
	void SLOT_DownLevel();

	void SLOT_NewCommandWindow();
	void SLOT_OKButton();
	void SLOT_SelfReboot();
	void SLOT_NewCalibrate();
	void SLOT_NewDiaryQuerry();

	void SLOT_CamReverse(int);
	void SLOT_NotifyChannel();
};

#endif // IVFDACTIVEXN_H
