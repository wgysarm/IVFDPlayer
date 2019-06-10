#ifndef IVFDCOMMAND_H
#define IVFDCOMMAND_H

#include "ui_IVFDCommand.h"
#include <QDialog>

class IVFDCommand : public QDialog
{
	Q_OBJECT

public:
	IVFDCommand(QWidget *parent);
	~IVFDCommand();
	Ui_CommandDialog *ui;	
public slots:
	void SLOT_ClearInput();
private:

};

#endif // IVFDCOMMAND_H
