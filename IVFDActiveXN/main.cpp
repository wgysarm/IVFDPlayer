#include "IVFDActiveXN.h"
#include "IVFDDataBase.h"
#include "LoginWidget.h"
#include "IVFDTools.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDialog>
#include <QTranslator>
#include <QObject>

void createFile(QString filePath,QString fileName)
{
	QDir tempDir;
	//��ʱ�������ǰ·��
	QString currentDir = tempDir.currentPath();
	//���filePath·�������ڣ�������
	if(!tempDir.exists(filePath))
	{
		qDebug()<<"current path no exist"<<endl;
		tempDir.mkpath(filePath);
	}
	QFile *tempFile = new QFile;
	//�������ִ��·�����õ�filePath��
	tempDir.setCurrent(filePath);
	qDebug()<<tempDir.currentPath();
	//���filePath·�����Ƿ�����ļ�fileName,���ֹͣ������
	if(tempFile->exists(fileName))
	{
		qDebug()<<"file exist";
		return ;
	}
	//��ʱ��·����û��fileName�ļ���ʹ����������ڵ�ǰ·���´����ļ�
	tempFile->setFileName(fileName);
	if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
	{
		qDebug()<<"open failed";
	}
	tempFile->close();
	//������ǰ·������Ϊԭ����·��
	tempDir.setCurrent(currentDir);
	qDebug()<<tempDir.currentPath();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationVersion("v1.0");
	a.setApplicationName("IVFDPlayer");
	a.setWindowIcon(QIcon(":/image/fire.png"));
	QFile file(":/style/ivfdstyle.qss");
	if(file.open(QFile::ReadOnly)){
		QString style = QString::fromLatin1(file.readAll());
		a.setStyleSheet(style);
		file.close();
	}

	QTranslator translator;
	translator.load("./ivfdactivexn_zh");
	a.installTranslator(&translator);
	qDebug() << QString::fromLocal8Bit("��");

	createFile("./","sysinfo.ini");
	IVFDActiveXN w;
	//w.show();
	LoginWidget wlogin;
	if(wlogin.exec() == QDialog::Accepted){
		qDebug() << "username match password!";
		w.show();
	}else{
		return 0;
	}
	return a.exec();
}
