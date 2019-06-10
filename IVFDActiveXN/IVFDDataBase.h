
#pragma once

#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QString>
#include <QFile>

const QString dbDriverName = "QSQLITE";             // ���ݿ��������� SQLite- QSQLITE�� MySQL- QMYSQL ����
const QString dbConnectionName = "ivfdalarm";      // ���ݿ�������
const QString dbName = "IVFDRecordAlarmDb.db";      // ���ݿ�����
const QString PDRIParseUrlTabName = "ParseUrl";     // PDRI����URL����
const QString dbUserName = "admin";                   // ���ݿ��û���
const QString dbPWD = "admin";                 // ���ݿ��û�����

class IVFDDataBase : public QObject
{
	Q_OBJECT

	QString m_datafile;
public:
	explicit IVFDDataBase(QObject *parent = nullptr);
	~IVFDDataBase();
	// ��������
	bool createConnection();
	// ��ʼ�����ݿ�
	bool initDatabase();
	// �����ݿ�
	bool isOpen();
	// �ر����ݿ�
	void close();

	bool createTable();       //�������ݿ��
	bool insert();            //��������
	bool queryAll();          //��ѯ������Ϣ
	bool updateById(int id);  //����
	bool deleteById(int id);  //ɾ��
	bool sortById();          //����

private:
	//���SQL������ȷ��
	void errorSQLOrder(QSqlQuery query, QString mark);
private:
	QSqlDatabase *mpSqlDatabase;
};

