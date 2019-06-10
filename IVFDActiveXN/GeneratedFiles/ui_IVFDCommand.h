/********************************************************************************
** Form generated from reading UI file 'IVFDCommand.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVFDCOMMAND_H
#define UI_IVFDCOMMAND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CommandDialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *clearButton;
    QPushButton *cancelButton;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QDialog *CommandDialog)
    {
        if (CommandDialog->objectName().isEmpty())
            CommandDialog->setObjectName(QStringLiteral("CommandDialog"));
        CommandDialog->resize(400, 300);
        layoutWidget = new QWidget(CommandDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 250, 351, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));

        hboxLayout->addWidget(okButton);

        clearButton = new QPushButton(layoutWidget);
        clearButton->setObjectName(QStringLiteral("clearButton"));
        clearButton->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));

        hboxLayout->addWidget(clearButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));

        hboxLayout->addWidget(cancelButton);

        lineEdit = new QLineEdit(CommandDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(30, 120, 341, 31));
        lineEdit->setStyleSheet(QStringLiteral("background-color: rgb(0, 170, 255);"));
        label = new QLabel(CommandDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 60, 101, 16));
        label->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 0);"));

        retranslateUi(CommandDialog);
        QObject::connect(okButton, SIGNAL(clicked()), CommandDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), CommandDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CommandDialog);
    } // setupUi

    void retranslateUi(QDialog *CommandDialog)
    {
        CommandDialog->setWindowTitle(QApplication::translate("CommandDialog", "\345\221\275\344\273\244\350\241\214\347\252\227\345\217\243", 0));
        okButton->setText(QApplication::translate("CommandDialog", "\346\211\247\350\241\214", 0));
        clearButton->setText(QApplication::translate("CommandDialog", "\346\270\205\351\231\244", 0));
        cancelButton->setText(QApplication::translate("CommandDialog", "\345\217\226\346\266\210", 0));
        label->setText(QApplication::translate("CommandDialog", "\350\257\267\350\276\223\345\205\245\345\221\275\344\273\244\350\241\214\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class CommandDialog: public Ui_CommandDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVFDCOMMAND_H
