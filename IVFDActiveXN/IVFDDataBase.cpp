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

//����һ�����ݿ�����
bool IVFDDataBase::createConnection()
{
	//�Ժ�Ϳ�����"sqlite1"�����ݿ����������
	*mpSqlDatabase = QSqlDatabase::addDatabase(dbDriverName, dbConnectionName);//sqlite1 is the name of db for connection 
	mpSqlDatabase->setDatabaseName(".//IVFDRecordAlarmDb.db");//�������ݿ�����ΪIVFDRecordAlarmDbS.db
	//db.setUserName("admin");
	//db.setPassword("admin");
	if( !mpSqlDatabase->open())
	{
		qDebug() << QString::fromLocal8Bit("�޷��������ݿ�����");
		qFatal("failed to connect.");
		return false;
	}
	qDebug() << QString::fromLocal8Bit("�Ѿ��������ݿ�����");
	return true;
}

//�������ݿ��
bool IVFDDataBase::createTable()
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //�������ݿ�����
	QSqlQuery query(*mpSqlDatabase);
	QString create_sql = "create table if not exists alarminfo(id int primary key,attribute varchar(10),type varchar(10),kind varchar(10),nation int,carnumber int,elevaltor int,distance int,oil int,temperature int)";

	bool success = query.exec(create_sql);
	if(success)
	{
		qDebug() << QString::fromLocal8Bit("���ݿ�����ɹ���\n");
		return true;
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("���ݿ����ʧ�ܣ�\n");
		return false;
	}
}

//�����ݿ��в����¼
bool IVFDDataBase::insert()
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //�������ݿ�����
	QSqlQuery query(*mpSqlDatabase);
	query.prepare("insert into alarminfo values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	long records = 2;
	for(int i=0; i<records; i++)
	{
		query.bindValue(0, i);
		query.bindValue(1, QString::fromLocal8Bit("����"));
		query.bindValue(2, QString::fromLocal8Bit("�豸"));
		query.bindValue(3, QString::fromLocal8Bit("����"));
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
			qDebug() << lastError.driverText() << QString(QString::fromLocal8Bit("����ʧ��"));
			return false;
		}
	}
	return true;
}

//��ѯ������Ϣ
bool IVFDDataBase::queryAll()
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //�������ݿ�����
	QSqlQuery query(*mpSqlDatabase);
	query.exec("select * from alarminfo");
	QSqlRecord rec = query.record();
	qDebug() << QString::fromLocal8Bit("alarminfo���ֶ�����" ) << rec.count();

	while(query.next())
	{
		for(int index = 0; index < 10; index++)
			qDebug() << query.value(index) << " ";
		qDebug() << "\n";
	}
	return true;
}

//����IDɾ����¼
bool IVFDDataBase::deleteById(int id)
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //�������ݿ�����
	QSqlQuery query(*mpSqlDatabase);
	query.prepare(QString("delete from alarminfo where id = %1").arg(id));
	if(!query.exec())
	{
		qDebug() << "ɾ����¼ʧ�ܣ�";
		return false;
	}
	return true;
}

//����ID���¼�¼
bool IVFDDataBase::updateById(int id)
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //�������ݿ�����
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
		qDebug() << lastError.driverText() << QString(QString::fromLocal8Bit("����ʧ��"));
	}
	return true;
}

//����
bool IVFDDataBase::sortById()
{
	*mpSqlDatabase = QSqlDatabase::database(dbConnectionName); //�������ݿ�����
	QSqlQuery query(*mpSqlDatabase);
	bool success=query.exec("select * from alarminfo order by id desc");
	if(success)
	{
		qDebug() << QString::fromLocal8Bit("����ɹ�");
		return true;
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("����ʧ�ܣ�");
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
	//query.exec(UPDATE_STATUS_IN_USERINFORMATION_SQL);   // ��ѯ��ParseUrl���Ƿ����
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
	// ���SQL�����󣬾͵���������Ϣ
	if (!query.isActive())
	{
		QString str = query.lastError().text() + "\n" + mark;
		qDebug() << "Mysql qurey error: " << str;
		qDebug() << "query text:" << query.lastQuery();
	}
}


