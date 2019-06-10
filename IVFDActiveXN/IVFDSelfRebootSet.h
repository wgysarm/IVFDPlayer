#ifndef IVFDSELFREBOOTSET_H
#define IVFDSELFREBOOTSET_H

#include <QDialog>
#include "ui_IVFDSelfRebootSet.h"

class IVFDSelfRebootSet : public QDialog
{
	Q_OBJECT

public:
	IVFDSelfRebootSet(QWidget *parent = 0);
	~IVFDSelfRebootSet();

private:
	Ui::IVFDSelfRebootSet ui;
};

#endif // IVFDSELFREBOOTSET_H
