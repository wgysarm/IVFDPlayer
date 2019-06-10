/********************************************************************************
** Form generated from reading UI file 'IVFDSelfRebootSet.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVFDSELFREBOOTSET_H
#define UI_IVFDSELFREBOOTSET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_IVFDSelfRebootSet
{
public:

    void setupUi(QDialog *IVFDSelfRebootSet)
    {
        if (IVFDSelfRebootSet->objectName().isEmpty())
            IVFDSelfRebootSet->setObjectName(QStringLiteral("IVFDSelfRebootSet"));
        IVFDSelfRebootSet->resize(400, 300);

        retranslateUi(IVFDSelfRebootSet);

        QMetaObject::connectSlotsByName(IVFDSelfRebootSet);
    } // setupUi

    void retranslateUi(QDialog *IVFDSelfRebootSet)
    {
        IVFDSelfRebootSet->setWindowTitle(QApplication::translate("IVFDSelfRebootSet", "\350\207\252\345\220\257\346\227\266\351\227\264\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class IVFDSelfRebootSet: public Ui_IVFDSelfRebootSet {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVFDSELFREBOOTSET_H
