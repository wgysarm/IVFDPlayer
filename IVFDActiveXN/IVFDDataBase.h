
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

const QString dbDriverName = "QSQLITE";             // 数据库驱动名： SQLite- QSQLITE， MySQL- QMYSQL ……
const QString dbConnectionName = "ivfdalarm";      // 数据库连接名
const QString dbName = "IVFDRecordAlarmDb.db";      // 数据库名称
const QString PDRIParseUrlTabName = "ParseUrl";     // PDRI解析URL表名
const QString dbUserName = "admin";                   // 数据库用户名
const QString dbPWD = "admin";                 // 数据库用户密码

class IVFDDataBase : public QObject
{
	Q_OBJECT

	QString m_datafile;
public:
	explicit IVFDDataBase(QObject *parent = nullptr);
	~IVFDDataBase();
	// 创建连接
	bool createConnection();
	// 初始化数据库
	bool initDatabase();
	// 打开数据库
	bool isOpen();
	// 关闭数据库
	void close();

	bool createTable();       //创建数据库表
	bool insert();            //插入数据
	bool queryAll();          //查询所有信息
	bool updateById(int id);  //更新
	bool deleteById(int id);  //删除
	bool sortById();          //排序

private:
	//检查SQL语句的正确性
	void errorSQLOrder(QSqlQuery query, QString mark);
private:
	QSqlDatabase *mpSqlDatabase;
};

