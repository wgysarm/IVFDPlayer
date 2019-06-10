#ifndef IVFDDIARYQUERRY_H
#define IVFDDIARYQUERRY_H

#include <QDialog>
#include "ui_IVFDDiaryQuerry.h"

class IVFDDiaryQuerry : public QDialog
{
	Q_OBJECT

public:
	IVFDDiaryQuerry(QWidget *parent = 0);
	~IVFDDiaryQuerry();

private:
	Ui::IVFDDiaryQuerry ui;
};

#endif // IVFDDIARYQUERRY_H
