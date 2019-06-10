#include "IVFDCalibrate.h"

IVFDCalibrate::IVFDCalibrate(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	m_argcnt = 0;
	m_video_calib = (SMTK_CHANNEL_VIDEO_CALIB *)malloc(sizeof(SMTK_CHANNEL_VIDEO_CALIB));
	m_video_clbr_para = (VFD_SENCE_CLBR *)malloc(6*sizeof(VFD_SENCE_CLBR));

	m_video_calib->calib_flag = 0;

	VFD_SENCE_CLBR clbtmp[6] = {{1,0,8000,220,430},{1,0,16000,294,392},{1,0,20000,310,382},{1,0,24000,320,376},{1,800,24000,350,376},{1,1200,24000,368,376}};

	memcpy((m_video_calib->video_clbr), clbtmp, 6*sizeof(VFD_SENCE_CLBR));

	VFD_PARA_SENCE paratmp = {518067.4,3229.8,118,866.7,176.5,848.9};
	memcpy(&m_video_calib->video_result, &paratmp, sizeof(VFD_PARA_SENCE));
	VFD_REGULATE regulatetmp = {2, -2, 10, 2};
	memcpy(&m_video_calib->video_regulate, &regulatetmp, sizeof(VFD_REGULATE));

	//*m_video_calib = {{518067.4,3229.8,118,866.7,176.5,848.9},//k,c,b,w,v,lamt
	//{2,-2,10,2}, //fire_bd_regulate,fire_bd_regu_high,fire_bd_regu_level,fire_bd_regumode
	//{{1,0,8000,220,430},{1,0,16000,294,392},{1,0,20000,310,382},{1,0,24000,320,376},{1,800,24000,350,376},{1,1200,24000,368,376}},//{enableflag,real_x,real_y,image_x,image_y}
	//0};



	SMTK_CHANNEL_VIDEO_CALIB 	video_calib_default = {{518067.4,3229.8,118,866.7,176.5,848.9},//k,c,b,w,v,lamt
	{2,-2,10,2}, //fire_bd_regulate,fire_bd_regu_high,fire_bd_regu_level,fire_bd_regumode
	{{1,0,8000,220,430},{1,0,16000,294,392},{1,0,20000,310,382},{1,0,24000,320,376},{1,800,24000,350,376},{1,1200,24000,368,376}},//{enableflag,real_x,real_y,image_x,image_y}
	0};

	if(m_video_clbr_para){
		for(int i = 0; i < 6; i++){
			m_video_clbr_para->enableflag = 1;
			m_video_clbr_para->image_x = 0;
			m_video_clbr_para->image_y = 0;
			m_video_clbr_para->real_x = 0;
			m_video_clbr_para->real_y = 0;
			m_video_clbr_para++;
		}
	}


	ui.pushButton_inputXY->setDisabled(true);
	ui.pushButton_calibrate->setDisabled(true);

	QObject::connect(ui.pushButton_calculate, SIGNAL(clicked()), this, SLOT(SLOT_CalculateRealXY()));
	QObject::connect(ui.pushButton_currentclbarg, SIGNAL(clicked()), this, SLOT(SLOT_GetCurrentArgs()));
	QObject::connect(ui.pushButton_inputXY, SIGNAL(clicked()), this, SLOT(SLOT_Input()));
	QObject::connect(ui.pushButton_manulinput, SIGNAL(clicked()), this, SLOT(SLOT_InputManual()));
	QObject::connect(ui.pushButton_saveclbargs, SIGNAL(clicked()), this, SLOT(SLOT_SaveCurrentArgs()));
	QObject::connect(ui.pushButton_startclb, SIGNAL(clicked()), this, SLOT(SLOT_StartCalibrate()));
	QObject::connect(ui.pushButton_restore, SIGNAL(clicked()), this, SLOT(SLOT_RestoreDefaultArgs()));
	QObject::connect(ui.pushButton_calibrate, SIGNAL(clicked()), this, SLOT(SLOT_Calibrate()));
	QObject::connect(ui.pushButton_readInputXY, SIGNAL(clicked()), this, SLOT(SLOT_ReadInputXY()));
	QObject::connect(this, SIGNAL(SIG_SetPushButton(int, bool)), this, SLOT(SLOT_SetPushButton(int, bool)));
}

IVFDCalibrate::~IVFDCalibrate()
{

}


void IVFDCalibrate::SLOT_CalculateRealXY(){
	qDebug() << "start to calculate Real XY!";
	QString stringImageXY = ui.lineEdit_imagecalXY->text();
	QStringList imageXYList = stringImageXY.split(",");

	short realx, realy;
	_VideoRealXYcount(&m_video_calib->video_result, imageXYList.at(0).toInt(), imageXYList.at(1).toInt(), &realx, &realy);

	ui.lineEdit_realcalXY->setText(QString::number(realx).append(",").append(QString::number(realy)));
}

void IVFDCalibrate::SLOT_GetCurrentArgs(){
	qDebug() << "start to get args !";
	QFile initfile("sysinfo.ini");
	if (!initfile.exists()){
		qDebug() << "no init file exist!";
		return;
	}

	QSettings argset("sysinfo.ini", QSettings::IniFormat);
	argset.beginGroup("cameraclb");
	QVariant val;
	float k = argset.value("k").toFloat();
	float b = argset.value("b").toFloat();
	float c = argset.value("c").toFloat();
	float v = argset.value("v").toFloat();
	float w = argset.value("w").toFloat();
	float lamt = argset.value("lamt").toFloat();
	ui.lineEdit_k->setText(QString::number(double(k)));
	ui.lineEdit_c->setText(QString::number(double(c)));
	ui.lineEdit_b->setText(QString::number(double(b)));
	ui.lineEdit_v->setText(QString::number(double(v)));
	ui.lineEdit_w->setText(QString::number(double(w)));
	ui.lineEdit_lamt->setText(QString::number(double(lamt)));
	argset.endGroup();

}


void IVFDCalibrate::SLOT_InputManual(){
	qDebug() << "start to input by Mannual!";
	m_video_calib->video_result.k = ui.lineEdit_k->text().toFloat();
	m_video_calib->video_result.b = ui.lineEdit_b->text().toFloat();
	m_video_calib->video_result.c = ui.lineEdit_c->text().toFloat();
	m_video_calib->video_result.w = ui.lineEdit_w->text().toFloat();
	m_video_calib->video_result.v = ui.lineEdit_v->text().toFloat();
	m_video_calib->video_result.lamt = ui.lineEdit_lamt->text().toFloat();
}

void IVFDCalibrate::SLOT_Input(){
	qDebug() << "start to Input args!";

	QString label1 = QString(tr("please input imagexy "));
	QString label2 = QString(tr("please input realxy "));
	QString regText1 = QString("^([1-9])(\\d{0,3})(\\,)([1-9])(\\d{0,3})$");
	QString regText2 = QString("^([0-9])(\\d{0,5})(\\,)([0-9])(\\d{0,5})$");
	QString imageXYstring = ui.lineEdit_imageXY->text();
	QString realXYstring = ui.lineEdit_realXY->text();
	if(imageXYstring == "" || (!IVFDTool::RegularFunc(regText1, imageXYstring)) || realXYstring == "" || (!IVFDTool::RegularFunc(regText2, realXYstring))){ 
		IVFDTool::ShowMessageBoxErrorX(tr("input error!"));
	}else{
		QStringList imageXYList = imageXYstring.split(",");
		QStringList realXYList = realXYstring.split(",");
		m_video_calib->video_clbr[m_argcnt].image_x = imageXYList[0].toInt();
		m_video_calib->video_clbr[m_argcnt].image_y = imageXYList[1].toInt();
		qDebug() << imageXYstring << imageXYList[1].toInt();

		m_video_calib->video_clbr[m_argcnt].real_x = realXYList[0].toInt();
		m_video_calib->video_clbr[m_argcnt].real_y = realXYList[1].toInt();

		m_argcnt++;
		ui.label_imgxyhint->setText(label1.append(QString::number(m_argcnt)));
		ui.label_realxyhint->setText(label2.append(QString::number(m_argcnt)));
		ui.textBrowser_clbinput->append(imageXYstring.append(" ").append(realXYstring));
	}


	if(m_argcnt == 6){
		emit SIG_SetPushButton(1, false);
		emit SIG_SetPushButton(2, true);
		for(int i = 0; i < 6; i++){
			ui.textBrowser_clbresult->append(QString("the calibrate data input is %1,%2,%3,%4").\
				arg(QString::number(m_video_calib->video_clbr[i].image_x)).\
				arg(QString::number(m_video_calib->video_clbr[i].image_y)).\
				arg(QString::number(m_video_calib->video_clbr[i].real_x)).\
				arg(QString::number(m_video_calib->video_clbr[i].real_y)));	
		}
	}

}


void IVFDCalibrate::SLOT_RestoreDefaultArgs(){
	qDebug() << "start to restore args default!";
	QString result = QString("k,b,c,w,v,lamt! default !");

	ui.textBrowser_clbresult->append(result);
}

void IVFDCalibrate::SLOT_SaveCurrentArgs(){
	qDebug() << "save the result of args!";
	IVFDTool::ShowMessageBoxInfo(QString(tr("current args has been saved!")));
}


void IVFDCalibrate::SLOT_StartCalibrate(){
	qDebug() << "start to calibrate!";
	m_argcnt = 0;
	QString label1 = QString(tr("please input imagexy "));
	QString label2 = QString(tr("please input realxy "));
	ui.pushButton_inputXY->setEnabled(true);
	ui.pushButton_calibrate->setEnabled(false);

	ui.label_imgxyhint->setText(label1);
	ui.label_realxyhint->setText(label2);

	ui.textBrowser_clbinput->clear();
	ui.textBrowser_clbresult->clear();
	ui.lineEdit_imageXY->clear();
	ui.lineEdit_realXY->clear();
	emit SIG_NotifyChannel();
}

void IVFDCalibrate::SLOT_SetPushButton(int id, bool flag){
	switch(id){
	case 1:{
		ui.pushButton_inputXY->setEnabled(flag);
		break;
		   }
	case 2:{
		ui.pushButton_calibrate->setEnabled(flag);
		break;
		   }
	}
}

void IVFDCalibrate::SLOT_Calibrate(){
	qDebug() << "Now calibrate!";
	QString result = QString("k,b,c,w,v,lamt!");

	VFD_SENCE_CLBR	video_clbr_para_tmp[6]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	//vfd_sence_count(video_clbr_para_tmp,&(m_video_calib->video_result));//此处是在channel初始化时赋值的。//此处求得k,c,v,b,m,lamt，通过标定的imgxy,和realxy，来求。
	vfd_sence_count(m_video_calib->video_clbr, &(m_video_calib->video_result));
	result = QString("%1 is k=%2, b=%3, c=%4, w=%5, v=%6, lamt=%7").arg(result)\
													.arg(m_video_calib->video_result.k)\
													.arg(m_video_calib->video_result.b)\
													.arg(m_video_calib->video_result.c)\
													.arg(m_video_calib->video_result.w)\
													.arg(m_video_calib->video_result.v)\
													.arg(m_video_calib->video_result.lamt);

	ui.textBrowser_clbresult->append(result);

}

void IVFDCalibrate::SLOT_ReadInputXY(){
	qDebug() << "start to read the input xy from sysinfo.ini";
	QFile initfile("sysinfo.ini");
	if(!initfile.exists()){
		qDebug() << "no init file exists!";
		return;
	}

	QSettings argset("sysinfo.ini", QSettings::IniFormat);
	argset.beginGroup("cameraclb");
	foreach(QString key, argset.allKeys()){
		qDebug() << key << " = " << argset.value(key);
	}

	for(int i = 0; i < 6; i++){
		QString keyheadimg("img");
		QString keyheadreal("real");
		QString keyimg = keyheadimg.append(QString::number(i+1));
		QString keyreal = keyheadreal.append(QString::number(i+1));
		m_video_calib->video_clbr[i+1].image_x = argset.value(keyimg).toStringList()[0].toInt();
		m_video_calib->video_clbr[i+1].image_y = argset.value(keyimg).toStringList()[1].toInt();

		m_video_calib->video_clbr[i+1].real_x = argset.value(keyreal).toStringList()[0].toInt();
		m_video_calib->video_clbr[i+1].real_y = argset.value(keyreal).toStringList()[1].toInt();
		//qDebug() << argset.value(keyimg).toString() << "   " << keyreal << argset.value(keyimg).toString() << "   " << argset.value(keyreal).toString();

		ui.textBrowser_clbinput->append(argset.value(keyimg).toStringList()[0].append("   ").append(argset.value(keyimg).toStringList()[1]));
		ui.textBrowser_clbinput->append(argset.value(keyreal).toStringList()[0].append("   ").append(argset.value(keyreal).toStringList()[1]));
	}
	argset.endGroup();

}

