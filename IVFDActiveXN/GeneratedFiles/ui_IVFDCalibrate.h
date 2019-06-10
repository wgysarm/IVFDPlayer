/********************************************************************************
** Form generated from reading UI file 'IVFDCalibrate.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVFDCALIBRATE_H
#define UI_IVFDCALIBRATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_IVFDCalibrate
{
public:
    QGroupBox *groupBox_input;
    QTextBrowser *textBrowser_clbinput;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_8;
    QLineEdit *lineEdit_realXY;
    QLineEdit *lineEdit_imageXY;
    QLabel *label_imgxyhint;
    QPushButton *pushButton_startclb;
    QLabel *label_realxyhint;
    QPushButton *pushButton_inputXY;
    QPushButton *pushButton_calibrate;
    QPushButton *pushButton_readInputXY;
    QGroupBox *groupBox_result;
    QPushButton *pushButton_saveclbargs;
    QPushButton *pushButton_restore;
    QPushButton *pushButton_currentclbarg;
    QTextBrowser *textBrowser_clbresult;
    QGroupBox *groupBox_3;
    QLabel *label_2;
    QLineEdit *lineEdit_imagecalXY;
    QPushButton *pushButton_calculate;
    QLabel *label;
    QLineEdit *lineEdit_realcalXY;
    QLabel *label_3;
    QLineEdit *lineEdit_realcaldiffXY;
    QGroupBox *groupBox_manual;
    QLineEdit *lineEdit_lamt;
    QLabel *label_14;
    QLabel *label_16;
    QLabel *label_11;
    QLineEdit *lineEdit_w;
    QLineEdit *lineEdit_b;
    QLineEdit *lineEdit_c;
    QLineEdit *lineEdit_v;
    QLineEdit *lineEdit_k;
    QLabel *label_13;
    QLabel *label_15;
    QPushButton *pushButton_manulinput;
    QLabel *label_12;

    void setupUi(QDialog *IVFDCalibrate)
    {
        if (IVFDCalibrate->objectName().isEmpty())
            IVFDCalibrate->setObjectName(QStringLiteral("IVFDCalibrate"));
        IVFDCalibrate->resize(800, 600);
        groupBox_input = new QGroupBox(IVFDCalibrate);
        groupBox_input->setObjectName(QStringLiteral("groupBox_input"));
        groupBox_input->setGeometry(QRect(10, 10, 781, 231));
        groupBox_input->setStyleSheet(QStringLiteral("background-color: rgb(59, 147, 103);"));
        textBrowser_clbinput = new QTextBrowser(groupBox_input);
        textBrowser_clbinput->setObjectName(QStringLiteral("textBrowser_clbinput"));
        textBrowser_clbinput->setGeometry(QRect(370, 30, 371, 151));
        textBrowser_clbinput->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_5 = new QLabel(groupBox_input);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(220, 30, 131, 16));
        label_5->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_4 = new QLabel(groupBox_input);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 30, 141, 16));
        label_4->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_8 = new QLabel(groupBox_input);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(180, 100, 54, 12));
        lineEdit_realXY = new QLineEdit(groupBox_input);
        lineEdit_realXY->setObjectName(QStringLiteral("lineEdit_realXY"));
        lineEdit_realXY->setGeometry(QRect(220, 100, 131, 20));
        lineEdit_realXY->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lineEdit_imageXY = new QLineEdit(groupBox_input);
        lineEdit_imageXY->setObjectName(QStringLiteral("lineEdit_imageXY"));
        lineEdit_imageXY->setGeometry(QRect(30, 100, 141, 20));
        lineEdit_imageXY->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_imgxyhint = new QLabel(groupBox_input);
        label_imgxyhint->setObjectName(QStringLiteral("label_imgxyhint"));
        label_imgxyhint->setGeometry(QRect(30, 70, 141, 16));
        label_imgxyhint->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        pushButton_startclb = new QPushButton(groupBox_input);
        pushButton_startclb->setObjectName(QStringLiteral("pushButton_startclb"));
        pushButton_startclb->setGeometry(QRect(30, 160, 75, 23));
        pushButton_startclb->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        label_realxyhint = new QLabel(groupBox_input);
        label_realxyhint->setObjectName(QStringLiteral("label_realxyhint"));
        label_realxyhint->setGeometry(QRect(220, 70, 131, 16));
        label_realxyhint->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        pushButton_inputXY = new QPushButton(groupBox_input);
        pushButton_inputXY->setObjectName(QStringLiteral("pushButton_inputXY"));
        pushButton_inputXY->setGeometry(QRect(110, 160, 75, 23));
        pushButton_inputXY->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        pushButton_calibrate = new QPushButton(groupBox_input);
        pushButton_calibrate->setObjectName(QStringLiteral("pushButton_calibrate"));
        pushButton_calibrate->setGeometry(QRect(270, 160, 75, 23));
        pushButton_calibrate->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        pushButton_readInputXY = new QPushButton(groupBox_input);
        pushButton_readInputXY->setObjectName(QStringLiteral("pushButton_readInputXY"));
        pushButton_readInputXY->setGeometry(QRect(190, 160, 75, 23));
        pushButton_readInputXY->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        groupBox_result = new QGroupBox(IVFDCalibrate);
        groupBox_result->setObjectName(QStringLiteral("groupBox_result"));
        groupBox_result->setGeometry(QRect(10, 260, 781, 111));
        groupBox_result->setStyleSheet(QStringLiteral("background-color: rgb(59, 147, 103);"));
        pushButton_saveclbargs = new QPushButton(groupBox_result);
        pushButton_saveclbargs->setObjectName(QStringLiteral("pushButton_saveclbargs"));
        pushButton_saveclbargs->setGeometry(QRect(150, 40, 75, 23));
        pushButton_saveclbargs->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        pushButton_restore = new QPushButton(groupBox_result);
        pushButton_restore->setObjectName(QStringLiteral("pushButton_restore"));
        pushButton_restore->setGeometry(QRect(270, 40, 75, 23));
        pushButton_restore->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        pushButton_currentclbarg = new QPushButton(groupBox_result);
        pushButton_currentclbarg->setObjectName(QStringLiteral("pushButton_currentclbarg"));
        pushButton_currentclbarg->setGeometry(QRect(30, 40, 75, 23));
        pushButton_currentclbarg->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        textBrowser_clbresult = new QTextBrowser(groupBox_result);
        textBrowser_clbresult->setObjectName(QStringLiteral("textBrowser_clbresult"));
        textBrowser_clbresult->setGeometry(QRect(370, 20, 371, 61));
        textBrowser_clbresult->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        groupBox_3 = new QGroupBox(IVFDCalibrate);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 520, 781, 61));
        groupBox_3->setStyleSheet(QStringLiteral("background-color: rgb(59, 147, 103);"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(230, 20, 61, 16));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        lineEdit_imagecalXY = new QLineEdit(groupBox_3);
        lineEdit_imagecalXY->setObjectName(QStringLiteral("lineEdit_imagecalXY"));
        lineEdit_imagecalXY->setGeometry(QRect(90, 20, 113, 20));
        lineEdit_imagecalXY->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        pushButton_calculate = new QPushButton(groupBox_3);
        pushButton_calculate->setObjectName(QStringLiteral("pushButton_calculate"));
        pushButton_calculate->setGeometry(QRect(670, 20, 75, 23));
        pushButton_calculate->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 61, 16));
        label->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        lineEdit_realcalXY = new QLineEdit(groupBox_3);
        lineEdit_realcalXY->setObjectName(QStringLiteral("lineEdit_realcalXY"));
        lineEdit_realcalXY->setGeometry(QRect(300, 20, 113, 20));
        lineEdit_realcalXY->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(440, 20, 61, 16));
        label_3->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        lineEdit_realcaldiffXY = new QLineEdit(groupBox_3);
        lineEdit_realcaldiffXY->setObjectName(QStringLiteral("lineEdit_realcaldiffXY"));
        lineEdit_realcaldiffXY->setGeometry(QRect(510, 20, 113, 20));
        lineEdit_realcaldiffXY->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        groupBox_manual = new QGroupBox(IVFDCalibrate);
        groupBox_manual->setObjectName(QStringLiteral("groupBox_manual"));
        groupBox_manual->setGeometry(QRect(10, 390, 781, 111));
        groupBox_manual->setStyleSheet(QStringLiteral("background-color: rgb(59, 147, 103);"));
        lineEdit_lamt = new QLineEdit(groupBox_manual);
        lineEdit_lamt->setObjectName(QStringLiteral("lineEdit_lamt"));
        lineEdit_lamt->setGeometry(QRect(600, 40, 51, 20));
        lineEdit_lamt->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_14 = new QLabel(groupBox_manual);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(360, 40, 21, 16));
        label_14->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_16 = new QLabel(groupBox_manual);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(550, 40, 41, 16));
        label_16->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_11 = new QLabel(groupBox_manual);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(30, 40, 21, 16));
        label_11->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        lineEdit_w = new QLineEdit(groupBox_manual);
        lineEdit_w->setObjectName(QStringLiteral("lineEdit_w"));
        lineEdit_w->setGeometry(QRect(390, 40, 61, 20));
        lineEdit_w->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lineEdit_b = new QLineEdit(groupBox_manual);
        lineEdit_b->setObjectName(QStringLiteral("lineEdit_b"));
        lineEdit_b->setGeometry(QRect(280, 40, 71, 20));
        lineEdit_b->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lineEdit_c = new QLineEdit(groupBox_manual);
        lineEdit_c->setObjectName(QStringLiteral("lineEdit_c"));
        lineEdit_c->setGeometry(QRect(180, 40, 61, 20));
        lineEdit_c->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lineEdit_v = new QLineEdit(groupBox_manual);
        lineEdit_v->setObjectName(QStringLiteral("lineEdit_v"));
        lineEdit_v->setGeometry(QRect(490, 40, 51, 20));
        lineEdit_v->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lineEdit_k = new QLineEdit(groupBox_manual);
        lineEdit_k->setObjectName(QStringLiteral("lineEdit_k"));
        lineEdit_k->setGeometry(QRect(60, 40, 81, 20));
        lineEdit_k->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_13 = new QLabel(groupBox_manual);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(150, 40, 21, 16));
        label_13->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_15 = new QLabel(groupBox_manual);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(250, 40, 21, 16));
        label_15->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        pushButton_manulinput = new QPushButton(groupBox_manual);
        pushButton_manulinput->setObjectName(QStringLiteral("pushButton_manulinput"));
        pushButton_manulinput->setGeometry(QRect(670, 40, 75, 23));
        pushButton_manulinput->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        label_12 = new QLabel(groupBox_manual);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(460, 40, 21, 16));
        label_12->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));

        retranslateUi(IVFDCalibrate);

        QMetaObject::connectSlotsByName(IVFDCalibrate);
    } // setupUi

    void retranslateUi(QDialog *IVFDCalibrate)
    {
        IVFDCalibrate->setWindowTitle(QApplication::translate("IVFDCalibrate", "\347\233\270\346\234\272\346\240\207\345\256\232\347\252\227\345\217\243", 0));
        groupBox_input->setTitle(QApplication::translate("IVFDCalibrate", "\346\240\207\345\256\232\346\225\260\346\215\256\350\276\223\345\205\245\357\274\232", 0));
        label_5->setText(QApplication::translate("IVFDCalibrate", "\345\256\236\351\231\205\345\217\202\350\200\203\345\235\220\346\240\207\347\263\273", 0));
        label_4->setText(QApplication::translate("IVFDCalibrate", "\345\203\217\347\264\240\345\235\220\346\240\207\347\263\273(\346\234\200\345\244\2471080P)", 0));
        label_8->setText(QApplication::translate("IVFDCalibrate", "---->", 0));
        label_imgxyhint->setText(QApplication::translate("IVFDCalibrate", "\350\257\267\350\276\223\345\205\245\345\203\217\347\264\240\345\235\220\346\240\207\345\200\274", 0));
        pushButton_startclb->setText(QApplication::translate("IVFDCalibrate", "\345\274\200\345\247\213\346\240\207\345\256\232", 0));
        label_realxyhint->setText(QApplication::translate("IVFDCalibrate", "\350\257\267\350\276\223\345\205\245\345\256\236\351\231\205\345\235\220\346\240\207\345\200\274", 0));
        pushButton_inputXY->setText(QApplication::translate("IVFDCalibrate", "\346\211\213\345\212\250\350\276\223\345\205\245", 0));
        pushButton_calibrate->setText(QApplication::translate("IVFDCalibrate", "\346\240\207\345\256\232\350\256\241\347\256\227", 0));
        pushButton_readInputXY->setText(QApplication::translate("IVFDCalibrate", "\350\257\273\345\205\245\345\217\202\346\225\260", 0));
        groupBox_result->setTitle(QApplication::translate("IVFDCalibrate", "\346\240\207\345\256\232\345\217\202\346\225\260\350\256\241\347\256\227\347\273\223\346\236\234\357\274\232", 0));
        pushButton_saveclbargs->setText(QApplication::translate("IVFDCalibrate", "\344\277\235\345\255\230", 0));
        pushButton_restore->setText(QApplication::translate("IVFDCalibrate", "\346\201\242\345\244\215\351\273\230\350\256\244", 0));
        pushButton_currentclbarg->setText(QApplication::translate("IVFDCalibrate", "\345\275\223\345\211\215\345\217\202\346\225\260", 0));
        groupBox_3->setTitle(QApplication::translate("IVFDCalibrate", "\351\252\214\350\257\201\346\240\207\345\256\232\347\273\223\346\236\234\357\274\232", 0));
        label_2->setText(QApplication::translate("IVFDCalibrate", "\345\256\236\351\231\205\345\235\220\346\240\207\357\274\232", 0));
        pushButton_calculate->setText(QApplication::translate("IVFDCalibrate", "\350\256\241\347\256\227", 0));
        label->setText(QApplication::translate("IVFDCalibrate", "\345\203\217\347\264\240\345\235\220\346\240\207\357\274\232", 0));
        label_3->setText(QApplication::translate("IVFDCalibrate", "\346\240\207\345\256\232\350\257\257\345\267\256\357\274\232", 0));
        groupBox_manual->setTitle(QApplication::translate("IVFDCalibrate", "\346\211\213\345\212\250\350\256\276\347\275\256\357\274\232", 0));
        label_14->setText(QApplication::translate("IVFDCalibrate", "w =", 0));
        label_16->setText(QApplication::translate("IVFDCalibrate", "lamt =", 0));
        label_11->setText(QApplication::translate("IVFDCalibrate", "k =", 0));
        label_13->setText(QApplication::translate("IVFDCalibrate", "c =", 0));
        label_15->setText(QApplication::translate("IVFDCalibrate", "b =", 0));
        pushButton_manulinput->setText(QApplication::translate("IVFDCalibrate", "\346\211\213\345\212\250\350\276\223\345\205\245", 0));
        label_12->setText(QApplication::translate("IVFDCalibrate", "v =", 0));
    } // retranslateUi

};

namespace Ui {
    class IVFDCalibrate: public Ui_IVFDCalibrate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVFDCALIBRATE_H
