#ifndef IVFDCALIBRATE_H
#define IVFDCALIBRATE_H

#include <QDialog>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QSettings>
#include <QFile>
#include <QVariant>

#include "ui_IVFDCalibrate.h"
#include "IVFDDrivers.h"
#include "IVFDTools.h"

class IVFDCalibrate : public QDialog
{
	Q_OBJECT

public:
	IVFDCalibrate(QWidget *parent = 0);
	~IVFDCalibrate();

	int m_argcnt;
	SMTK_CHANNEL_VIDEO_CALIB *m_video_calib;
	VFD_SENCE_CLBR *m_video_clbr_para;

private:
	Ui::IVFDCalibrate ui;


signals:
	void SIG_SetPushButton(int id, bool flag);
	void SIG_NotifyChannel();//notify the channel begin to calibrate 

public slots:
	void SLOT_Input();
	void SLOT_StartCalibrate();
	void SLOT_GetCurrentArgs();
	void SLOT_RestoreDefaultArgs();
	void SLOT_SaveCurrentArgs();
	void SLOT_InputManual();
	void SLOT_CalculateRealXY();
	void SLOT_Calibrate();
	void SLOT_ReadInputXY();
	void SLOT_SetPushButton(int id, bool flag);
};

#endif // IVFDCALIBRATE_H
