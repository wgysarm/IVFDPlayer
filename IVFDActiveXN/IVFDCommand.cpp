#include "IVFDCommand.h"

IVFDCommand::IVFDCommand(QWidget *parent)
	: QDialog(parent),ui(new Ui::CommandDialog)
{
	ui->setupUi(this);
	QObject::connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(SLOT_ClearInput()));

}

IVFDCommand::~IVFDCommand()
{

}

void IVFDCommand::SLOT_ClearInput()
{
	ui->lineEdit->clear();

}
