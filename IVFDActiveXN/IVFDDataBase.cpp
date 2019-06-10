#include "IVFDDataBase.h"


IVFDDataBase::IVFDDataBase(QObject *parent) :QObject(parent)
{
	mpSqlDatabase = new QSqlDatabase;
}

IVFDDataBase::~IVFDDataBase()
{
	if (mpSqlDatabase != nullptr)
	{
		if (mpSqlDatabase->isOpen())
			mpSqlDatabase->close();
		delete mpSqlDatabase;
		mpSqlDatabase = nullptr;
	}
}

//建立一个数据库连接
bool IVFDDataBase::createConnection()
{
	//以后就可以用"sqlite1"与数据库进行连接了
	*mpSqlDatabase = QSqlDatabase::addDatabase(dbDriverName, dbConnectionName);//sqlite1 is the name of db for connection 
	mpSqlDatabase->setDatabaseName(".//IVFDRecordAlarmDb.db");//设置数据库名称为IVFDRecordAlarmDbS.db
	//db.setUserName("admin");
	//db.setPassword("admin");
	if( !mpSqlDatabase->open())
	{
		qDebug() << QString::fromLocal8Bit("无法建立数据库连接");
		qFatal("failed to connect.");
		return false;
	}
	qDebug() << QString::fromLocal8Bit("已经建立数据库连接");
	return true;
}

//创建数据库表
bool IVFDDataBase::createTable()
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //建立数据库连接
	QSqlQuery query(*mpSqlDatabase);
	QString create_sql = "create table if not exists alarminfo(id int primary key,attribute varchar(10),type varchar(10),kind varchar(10),nation int,carnumber int,elevaltor int,distance int,oil int,temperature int)";

	bool success = query.exec(create_sql);
	if(success)
	{
		qDebug() << QString::fromLocal8Bit("数据库表创建成功！\n");
		return true;
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("数据库表创建失败！\n");
		return false;
	}
}

//向数据库中插入记录
bool IVFDDataBase::insert()
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //建立数据库连接
	QSqlQuery query(*mpSqlDatabase);
	query.prepare("insert into alarminfo values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	long records = 2;
	for(int i=0; i<records; i++)
	{
		query.bindValue(0, i);
		query.bindValue(1, QString::fromLocal8Bit("测试"));
		query.bindValue(2, QString::fromLocal8Bit("设备"));
		query.bindValue(3, QString::fromLocal8Bit("调试"));
		query.bindValue(4, rand()%100);
		query.bindValue(5, rand()%10000);
		query.bindValue(6, rand()%300);
		query.bindValue(7, rand()%200000);
		query.bindValue(8, rand()%52);
		query.bindValue(9, rand()%100);

		bool success=query.exec();
		if(!success)
		{
			QSqlError lastError = query.lastError();
			qDebug() << lastError.driverText() << QString(QString::fromLocal8Bit("插入失败"));
			return false;
		}
	}
	return true;
}

//查询所有信息
bool IVFDDataBase::queryAll()
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //建立数据库连接
	QSqlQuery query(*mpSqlDatabase);
	query.exec("select * from alarminfo");
	QSqlRecord rec = query.record();
	qDebug() << QString::fromLocal8Bit("alarminfo表字段数：" ) << rec.count();

	while(query.next())
	{
		for(int index = 0; index < 10; index++)
			qDebug() << query.value(index) << " ";
		qDebug() << "\n";
	}
	return true;
}

//根据ID删除记录
bool IVFDDataBase::deleteById(int id)
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //建立数据库连接
	QSqlQuery query(*mpSqlDatabase);
	query.prepare(QString("delete from alarminfo where id = %1").arg(id));
	if(!query.exec())
	{
		qDebug() << "删除记录失败！";
		return false;
	}
	return true;
}

//根据ID更新记录
bool IVFDDataBase::updateById(int id)
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //建立数据库连接
	QSqlQuery query(*mpSqlDatabase);
	query.prepare(QString("update alarminfo set attribute=?,type=?,"
		"kind=?, nation=?,"
		"carnumber=?, elevaltor=?,"
		"distance=?, oil=?,"
		"temperature=? where id=%1").arg(id));

	//QTextCodec *codec = QTextCodec::codecForName("UTF-8");

	query.bindValue(0,"123");
	query.bindValue(1,"456");
	query.bindValue(2,"adb");
	query.bindValue(3,rand()%100);
	query.bindValue(4,rand()%10000);
	query.bindValue(5,rand()%300);
	query.bindValue(6,rand()%200000);
	query.bindValue(7,rand()%52);
	query.bindValue(8,rand()%100);

	bool success=query.exec();
	if(!success)
	{
		QSqlError lastError = query.lastError();
		qDebug() << lastError.driverText() << QString(QString::fromLocal8Bit("更新失败"));
	}
	return true;
}

//排序
bool IVFDDataBase::sortById()
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //建立数据库连接
	QSqlQuery query(*mpSqlDatabase);
	bool success=query.exec("select * from alarminfo order by id desc");
	if(success)
	{
		qDebug() << QString::fromLocal8Bit("排序成功");
		return true;
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("排序失败！");
		return false;
	}
}

bool IVFDDataBase::initDatabase()
{
	if (!createConnection())
	{
		return false;
	}
	QSqlQuery query(*mpSqlDatabase);
	//query.exec(UPDATE_STATUS_IN_USERINFORMATION_SQL);   // 查询表“ParseUrl”是否存在
	errorSQLOrder(query, "initiDatabase");

	if (!query.isActive())
	{
		mpSqlDatabase->close();
		return false;
	}
	mpSqlDatabase->close();
	return true;
}

void IVFDDataBase::close()
{
	if (mpSqlDatabase != nullptr && mpSqlDatabase->isOpen())
		mpSqlDatabase->close();
}


bool IVFDDataBase::isOpen()
{
	return (mpSqlDatabase->isOpen());
}


void IVFDDataBase::errorSQLOrder(QSqlQuery query, QString mark)
{
	// 如果SQL语句错误，就弹出警告信息
	if (!query.isActive())
	{
		QString str = query.lastError().text() + "\n" + mark;
		qDebug() << "Mysql qurey error: " << str;
		qDebug() << "query text:" << query.lastQuery();
	}
}


