#include "LoginWidget.h"

LoginWidget::LoginWidget(QWidget *parent)
	: QDialog(parent)
{
	qDebug() << QString(tr("good"));
	QPushButton *m_pushButtonConfirm = new QPushButton(tr("confirm1"), this);
	QPushButton *m_pushButtonClear = new QPushButton(tr("clear"), this);

	m_lineEditUserName = new QLineEdit;
	m_lineEditPassWord = new QLineEdit;

	QLabel *m_userNameLabel = new QLabel(tr("username:")) ;
	QLabel *m_userPassWordLabel = new QLabel(tr("password:"));

	QGridLayout *m_mainLayout = new QGridLayout(this);
	QGridLayout *m_layoutGrid = new QGridLayout(this);
	QHBoxLayout *m_layoutHBox1 = new QHBoxLayout(this);
	QHBoxLayout *m_layoutHBox0 = new QHBoxLayout(this);
	QVBoxLayout *m_layoutVBox = new QVBoxLayout(this);

	m_layoutGrid->setColumnStretch(0, 1);
	m_layoutGrid->setColumnStretch(1, 2);
	m_layoutGrid->setRowStretch(0, 1);
	m_layoutGrid->setRowStretch(1, 1);

	m_layoutGrid->addWidget(m_userNameLabel, 0, 0);
	m_layoutGrid->addWidget(m_lineEditUserName, 0, 1);

	m_layoutGrid->addWidget(m_userPassWordLabel, 1, 0);
	m_layoutGrid->addWidget(m_lineEditPassWord, 1, 1);
	

	m_layoutHBox1->addStretch(100);
	m_layoutHBox1->addWidget(m_pushButtonClear, 1);
	m_layoutHBox1->addWidget(m_pushButtonConfirm, 1);
	m_layoutHBox1->addStretch(100);

	m_layoutHBox0->addStretch(100);
	m_layoutHBox0->addLayout(m_layoutGrid, 1);
	m_layoutHBox0->addStretch(100);

	m_pushButtonClear->setMinimumWidth(100);
	m_pushButtonConfirm->setMinimumWidth(100);

	m_lineEditPassWord->setMinimumHeight(20);
	m_lineEditUserName->setMinimumHeight(20);
	m_lineEditPassWord->setMinimumWidth(150);
	m_lineEditUserName->setMinimumWidth(150);

	m_userNameLabel->setMinimumHeight(20);
	m_userNameLabel->setMinimumWidth(50);
	m_userPassWordLabel->setMinimumHeight(20);
	m_userPassWordLabel->setMinimumWidth(50);

	m_layoutVBox->addStretch(10);
	m_layoutVBox->addLayout(m_layoutHBox0);
	//m_layoutVBox->addStretch(1);
	m_layoutVBox->addLayout(m_layoutHBox1);
	m_layoutVBox->addStretch(10);

	m_lineEditPassWord->setEchoMode(QLineEdit::Password);
	m_lineEditUserName->setText("admin");
	m_lineEditPassWord->setText("123456");

	m_mainLayout->addLayout(m_layoutVBox, 1, 1);

	this->setLayout(m_mainLayout);


	QObject::connect(m_pushButtonConfirm, SIGNAL(clicked()), this, SLOT(SLOT_Confirm()));
	QObject::connect(m_pushButtonClear, SIGNAL(clicked()), this, SLOT(SLOT_Clear()));
	


}

LoginWidget::~LoginWidget()
{

}

void LoginWidget::SLOT_Clear(){
	m_lineEditPassWord->clear();

}

void LoginWidget::SLOT_Confirm(){
	accept();
}

void LoginWidget::accept(){
	if(m_lineEditUserName->text() == "admin" && m_lineEditPassWord->text() == "123456"){
		QDialog::accept();
	}else{
		IVFDTool::ShowMessageBoxInfo(QString::fromLocal8Bit("用户名或者密码错误！"));
		m_lineEditPassWord->clear();
		m_lineEditPassWord->setFocus();
	}

}

void LoginWidget::reject(){

}


void LoginWidget::closeEvent(QCloseEvent *event){
	qDebug() << "login close";
	reject();
	//event->accept();
}