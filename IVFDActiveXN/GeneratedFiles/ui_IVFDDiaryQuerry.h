/********************************************************************************
** Form generated from reading UI file 'IVFDDiaryQuerry.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVFDDIARYQUERRY_H
#define UI_IVFDDIARYQUERRY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_IVFDDiaryQuerry
{
public:

    void setupUi(QDialog *IVFDDiaryQuerry)
    {
        if (IVFDDiaryQuerry->objectName().isEmpty())
            IVFDDiaryQuerry->setObjectName(QStringLiteral("IVFDDiaryQuerry"));
        IVFDDiaryQuerry->resize(400, 300);

        retranslateUi(IVFDDiaryQuerry);

        QMetaObject::connectSlotsByName(IVFDDiaryQuerry);
    } // setupUi

    void retranslateUi(QDialog *IVFDDiaryQuerry)
    {
        IVFDDiaryQuerry->setWindowTitle(QApplication::translate("IVFDDiaryQuerry", "\346\227\245\345\277\227\346\237\245\350\257\242\347\252\227\345\217\243", 0));
    } // retranslateUi

};

namespace Ui {
    class IVFDDiaryQuerry: public Ui_IVFDDiaryQuerry {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVFDDIARYQUERRY_H
