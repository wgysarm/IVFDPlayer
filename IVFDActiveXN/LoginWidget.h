#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <QCloseEvent>
#include "IVFDTools.h"

class LoginWidget : public QDialog
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = 0);
	~LoginWidget();

	void closeEvent(QCloseEvent *event);

private:
	QLineEdit *m_lineEditUserName;
	QLineEdit *m_lineEditPassWord;

public slots:
	void SLOT_Confirm();
	void SLOT_Clear();

	void accept();
	void reject();
};

#endif // LOGINWIDGET_H
