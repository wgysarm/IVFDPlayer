#pragma once
#ifndef _IVFDTOOLS_H_
#define _IVFDTOOLS_H_

#include <QDesktopWidget>
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QAbstractButton>
#include <QCoreApplication>
#include <QFileDialog>
#include <QTime>
#include <QProcess>
#include <QDir>
//#include <QMediaPlayer>
#include <QApplication>
#include <QStyleFactory>
#include <QInputDialog>
#include <QStyle>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class IVFDTool:public QObject
{
public:    

    //���ñ���ΪGB2312
    static void SetGB2312Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("GB2312");
        QTextCodec::setCodecForLocale(codec);
        //QTextCodec::setCodecForCStrings(codec);
        //QTextCodec::setCodecForTr(codec);
    }

    //���ñ���ΪUTF8
    static void SetUTF8Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        //QTextCodec::setCodecForCStrings(codec);
        //QTextCodec::setCodecForTr(codec);
    }

    //�ж��Ƿ���IP��ַ
    static bool IsIP(QString IP)
    {
        QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return RegExp.exactMatch(IP);
    }

    //��ʾ��Ϣ�򣬽�ȷ����ť
    static void ShowMessageBoxInfo(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font:12pt '����'");
        msg.setWindowTitle(QString::fromLocal8Bit("��ʾ"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Information);
        msg.addButton(QString::fromLocal8Bit("ȷ��"),QMessageBox::ActionRole);
        msg.exec();
    }

    static void ShowMessageBoxInfoX(QString info)
    {
        QMessageBox::information(0,QString::fromLocal8Bit("��ʾ"),info,QMessageBox::Ok);
    }

    //��ʾ����򣬽�ȷ����ť
    static void ShowMessageBoxError(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font:12pt '����'");
        msg.setWindowTitle(QString::fromLocal8Bit("����"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Critical);
        msg.addButton(QString::fromLocal8Bit("ȷ��"),QMessageBox::ActionRole);
        msg.exec();
    }

    static void ShowMessageBoxErrorX(QString info)
    {
        QMessageBox::critical(0,QString::fromLocal8Bit("����"),info,QMessageBox::Ok);
    }

    //��ʾѯ�ʿ�ȷ����ȡ����ť
    static int ShowMessageBoxQuesion(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font:12pt '����'");
        msg.setWindowTitle("ѯ��");
        msg.setText(info);
        msg.setIcon(QMessageBox::Question);
        msg.addButton(QString::fromLocal8Bit("ȷ��"),QMessageBox::ActionRole);
        msg.addButton(QString::fromLocal8Bit("ȡ��"),QMessageBox::RejectRole);

        return msg.exec();
    }

    static int ShowMessageBoxQuesionX(QString info)
    {
        return QMessageBox::question(0,QString::fromLocal8Bit("ѯ��"),info,QMessageBox::Yes|QMessageBox::No);
    }

    //��ʾ��׼�����
    static QString ShowInputBox(QWidget *frm,QString info)
    {
        bool ok;
        return QInputDialog::getText(frm,QString::fromLocal8Bit("��ʾ"),info,QLineEdit::Password,"",&ok);
    }

    //16�����ַ���ת�ֽ�����
    static QByteArray HexStrToByteArray(QString str)
    {
        QByteArray senddata;
        int hexdata,lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len/2);
        char lstr,hstr;
        for(int i=0; i<len; )
        {
            hstr=str[i].toLatin1();
            if(hstr == ' ')
            {
                i++;
                continue;
            }
            i++;
            if(i >= len)
                break;
            lstr = str[i].toLatin1();
            hexdata = ConvertHexChar(hstr);
            lowhexdata = ConvertHexChar(lstr);
            if((hexdata == 16) || (lowhexdata == 16))
                break;
            else
                hexdata = hexdata*16+lowhexdata;
            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }
        senddata.resize(hexdatalen);
        return senddata;
    }

    static char ConvertHexChar(char ch)
    {
        if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
    }

    //�ֽ�����ת16�����ַ���
    static QString ByteArrayToHexStr(QByteArray data)
    {
        QString temp="";
        QString hex=data.toHex();
        for (int i=0;i<hex.length();i=i+2)
        {
            temp+=hex.mid(i,2)+" ";
        }
        return temp.trimmed().toUpper();
    }

    //16�����ַ���ת10����
    static int StrHexToDecimal(QString strHex)
    {
        bool ok;
        return strHex.toInt(&ok,16);
    }

    //10�����ַ���ת10����
    static int StrDecimalToDecimal(QString strDecimal)
    {
        bool ok;
        return strDecimal.toInt(&ok,10);
    }

    //2�����ַ���ת10����
    static int StrBinToDecimal(QString strBin)
    {
        bool ok;
        return strBin.toInt(&ok,2);
    }

    //16�����ַ���ת2�����ַ���
    static QString StrHexToStrBin(QString strHex)
    {
        uchar decimal=StrHexToDecimal(strHex);
        QString bin=QString::number(decimal,2);

        uchar len=bin.length();
        if (len<8)
        {
            for (int i=0;i<8-len;i++)
            {
                bin="0"+bin;
            }
        }

        return bin;
    }

    //10����ת2�����ַ���һ���ֽ�
    static QString DecimalToStrBin1(int decimal)
    {
        QString bin=QString::number(decimal,2);

        uchar len=bin.length();
        if (len<=8)
        {
            for (int i=0;i<8-len;i++)
            {
                bin="0"+bin;
            }
        }

        return bin;
    }

    //10����ת2�����ַ��������ֽ�
    static QString DecimalToStrBin2(int decimal)
    {
        QString bin=QString::number(decimal,2);

        uchar len=bin.length();
        if (len<=16)
        {
            for (int i=0;i<16-len;i++)
            {
                bin="0"+bin;
            }
        }

        return bin;
    }

    //����У����
    static uchar GetCheckCode(uchar data[],uchar len)
    {
        uchar temp=0;

        for (uchar i=0;i<len;i++)
        {
            temp+=data[i];
        }

        return temp%256;

    }

    //�������charתΪ��ȷ��uchar
    static uchar GetUChar(char data)
    {
        uchar temp;
        if (data>127)
        {
            temp=data+256;
        }
        else
        {
            temp=data;
        }
        return temp;
    }

    //��ʱ
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    //��ȡ��ǰ·��
    static QString GetCurrentPath()
    {
        return QString(QCoreApplication::applicationDirPath()+"/");
    }

    //��������
    static void PlaySound(QString soundName)
    {
       // QMediaPlayer Qplay=new QMediaPlayer(soundName);
       // Qplay.play();
    }

    //����ϵͳ����ʱ��
    static void SetSystemDateTime(int year,int month,int day,int hour,int min,int sec)
    {
        QProcess p(0);

        p.start("cmd");
        p.waitForStarted();
        p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
        p.closeWriteChannel();
        p.waitForFinished(1000);
        p.close();

        p.start("cmd");
        p.waitForStarted();
        p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
        p.closeWriteChannel();
        p.waitForFinished(1000);
        p.close();
    }

    //������У�����ֻ�йرհ�ť�����ܵ�����С
    static void FormOnlyCloseInCenter(QWidget *frm)
    {
        //���ô������
        QDesktopWidget desktop;
        int screenX=desktop.availableGeometry().width();
        int screenY=desktop.availableGeometry().height()-40;
        int frmX=frm->width();
        int frmY=frm->height();
        QPoint movePoint(screenX/2-frmX/2,screenY/2-frmY/2);
        frm->move(movePoint);

        //���ô��岻�ܵ�����С
        frm->setFixedSize(frmX,frmY);

        //���ô���ֻ����С����ť
        frm->setWindowFlags(Qt::WindowCloseButtonHint);
    }

    //���������ʾ
    static void FormInCenter(QWidget *frm)
    {
        int screenX=qApp->desktop()->width();
        int screenY=qApp->desktop()->height()-60;
        int wndX=frm->width();
        int wndY=frm->height();
        QPoint movePoint((screenX-wndX)/2,(screenY-wndY)/2);
        frm->move(movePoint);
    }

    //����û����󻯰�ť
    static void FormNoMaxButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowMinimizeButtonHint);
    }

    //����ֻ�йرհ�ť
    static void FormOnlyCloseButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowCloseButtonHint);
    }

    //���岻�ܸı��С
    static void FormNotResize(QWidget *frm)
    {
        frm->setFixedSize(frm->width(),frm->height());
    }

    //��ȡ�����С
    static QSize GetDesktopSize()
    {
        QDesktopWidget desktop;
        return QSize(desktop.availableGeometry().width(),desktop.availableGeometry().height());
    }

    //��ȡѡ����ļ�
    static QString GetFileName(QString filter)
    {
        return QFileDialog::getOpenFileName(NULL,QString::fromLocal8Bit("ѡ���ļ�"),QCoreApplication::applicationDirPath(),filter);
    }

    //��ȡѡ����ļ�����
    static QStringList GetFileNames(QString filter)
    {
        return QFileDialog::getOpenFileNames(NULL,QString::fromLocal8Bit("ѡ���ļ�"),QCoreApplication::applicationDirPath(),filter);
    }

    //��ȡѡ���Ŀ¼
    static QString GetFolderName()
    {
        return QFileDialog::getExistingDirectory();;
    }

    //��ȡ�ļ���������չ��
    static QString GetFileNameWithExtension(QString strFilePath)
    {
        QFileInfo fileInfo(strFilePath);
        return fileInfo.fileName();
    }

    //��ȡѡ���ļ����е��ļ�
    static QStringList GetFolderFileNames(QStringList filter)
    {
        QStringList fileList;
        QString strFolder = QFileDialog::getExistingDirectory();
        if (!strFolder.length()==0)
        {
            QDir myFolder(strFolder);

            if (myFolder.exists())
            {
                fileList= myFolder.entryList(filter);
            }
        }
        return fileList;
    }

    //�ļ����Ƿ����
    static bool FolderIsExist(QString strFolder)
    {
        QDir tempFolder(strFolder);
        return tempFolder.exists();
    }

    //�ļ��Ƿ����
    static bool FileIsExist(QString strFile)
    {
        QFile tempFile(strFile);
        return tempFile.exists();
    }

    //�����ļ�
    static bool CopyFile(QString sourceFile, QString targetFile)
    {
        if (FileIsExist(targetFile))
        {
            int ret=QMessageBox::information(NULL,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("�ļ��Ѿ����ڣ��Ƿ��滻��"),QMessageBox::Yes | QMessageBox::No);
            if (ret!=QMessageBox::Yes)
            {
                return false;
            }
        }
        return QFile::copy(sourceFile,targetFile);
    }


	//�ж��Ƿ�����������ʽ
	static bool RegularFunc(QString strRegExp, QString strText)
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
};




#endif // _IVFDTOOLS_H_
