/********************************************************************************
** Form generated from reading UI file 'IVFDActiveXInit.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVFDACTIVEXINIT_H
#define UI_IVFDACTIVEXINIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IVFDActiveXinitClass
{
public:
    QPushButton *pushButton_initconfirm;
    QPushButton *pushButton_initreset;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_DevIP;
    QLineEdit *lineEdit_ClientIP;

    void setupUi(QWidget *IVFDActiveXinitClass)
    {
        if (IVFDActiveXinitClass->objectName().isEmpty())
            IVFDActiveXinitClass->setObjectName(QStringLiteral("IVFDActiveXinitClass"));
        IVFDActiveXinitClass->resize(1157, 839);
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/fire.png"), QSize(), QIcon::Normal, QIcon::Off);
        IVFDActiveXinitClass->setWindowIcon(icon);
        IVFDActiveXinitClass->setStyleSheet(QStringLiteral(""));
        pushButton_initconfirm = new QPushButton(IVFDActiveXinitClass);
        pushButton_initconfirm->setObjectName(QStringLiteral("pushButton_initconfirm"));
        pushButton_initconfirm->setGeometry(QRect(490, 160, 75, 23));
        pushButton_initreset = new QPushButton(IVFDActiveXinitClass);
        pushButton_initreset->setObjectName(QStringLiteral("pushButton_initreset"));
        pushButton_initreset->setGeometry(QRect(590, 160, 75, 23));
        label = new QLabel(IVFDActiveXinitClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(450, 70, 100, 25));
        label->setMinimumSize(QSize(100, 25));
        label->setMaximumSize(QSize(100, 25));
        label->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_2 = new QLabel(IVFDActiveXinitClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(450, 110, 100, 25));
        label_2->setMinimumSize(QSize(100, 25));
        label_2->setMaximumSize(QSize(100, 25));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lineEdit_DevIP = new QLineEdit(IVFDActiveXinitClass);
        lineEdit_DevIP->setObjectName(QStringLiteral("lineEdit_DevIP"));
        lineEdit_DevIP->setGeometry(QRect(560, 70, 160, 25));
        lineEdit_DevIP->setMinimumSize(QSize(160, 25));
        lineEdit_DevIP->setMaximumSize(QSize(160, 25));
        lineEdit_DevIP->setStyleSheet(QStringLiteral(""));
        lineEdit_ClientIP = new QLineEdit(IVFDActiveXinitClass);
        lineEdit_ClientIP->setObjectName(QStringLiteral("lineEdit_ClientIP"));
        lineEdit_ClientIP->setGeometry(QRect(560, 110, 160, 25));
        lineEdit_ClientIP->setMinimumSize(QSize(160, 25));
        lineEdit_ClientIP->setMaximumSize(QSize(160, 25));
        lineEdit_ClientIP->setStyleSheet(QStringLiteral(""));

        retranslateUi(IVFDActiveXinitClass);

        QMetaObject::connectSlotsByName(IVFDActiveXinitClass);
    } // setupUi

    void retranslateUi(QWidget *IVFDActiveXinitClass)
    {
        IVFDActiveXinitClass->setWindowTitle(QApplication::translate("IVFDActiveXinitClass", "\350\257\267\350\276\223\345\205\245ip\345\234\260\345\235\200", 0));
        pushButton_initconfirm->setText(QApplication::translate("IVFDActiveXinitClass", "\347\241\256\350\256\244", 0));
        pushButton_initreset->setText(QApplication::translate("IVFDActiveXinitClass", "\351\207\215\347\275\256", 0));
        label->setText(QApplication::translate("IVFDActiveXinitClass", "\350\257\267\350\276\223\345\205\245\350\256\276\345\244\207\345\234\260\345\235\200", 0));
        label_2->setText(QApplication::translate("IVFDActiveXinitClass", "\350\257\267\350\276\223\345\205\245\345\256\242\346\210\267\347\253\257\345\234\260\345\235\200", 0));
        lineEdit_DevIP->setText(QApplication::translate("IVFDActiveXinitClass", "192.168.1.166", 0));
        lineEdit_ClientIP->setText(QApplication::translate("IVFDActiveXinitClass", "192.168.1.6", 0));
    } // retranslateUi

};

namespace Ui {
    class IVFDActiveXinitClass: public Ui_IVFDActiveXinitClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVFDACTIVEXINIT_H
