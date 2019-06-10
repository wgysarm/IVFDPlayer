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
	//临时保存程序当前路径
	QString currentDir = tempDir.currentPath();
	//如果filePath路径不存在，创建它
	if(!tempDir.exists(filePath))
	{
		qDebug()<<"current path no exist"<<endl;
		tempDir.mkpath(filePath);
	}
	QFile *tempFile = new QFile;
	//将程序的执行路径设置到filePath下
	tempDir.setCurrent(filePath);
	qDebug()<<tempDir.currentPath();
	//检查filePath路径下是否存在文件fileName,如果停止操作。
	if(tempFile->exists(fileName))
	{
		qDebug()<<"file exist";
		return ;
	}
	//此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
	tempFile->setFileName(fileName);
	if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
	{
		qDebug()<<"open failed";
	}
	tempFile->close();
	//将程序当前路径设置为原来的路径
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
	qDebug() << QString::fromLocal8Bit("好");

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
