/********************************************************************************
** Form generated from reading UI file 'canupdaterwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANUPDATERWINDOW_H
#define UI_CANUPDATERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_CanUpdaterWindow
{
public:
    QPushButton *closeButton;
    QLabel *select_file_label;
    QPushButton *browseButton;
    QTextBrowser *firmwareImageText;
    QPushButton *uploadButton;

    void setupUi(QDialog *CanUpdaterWindow)
    {
        if (CanUpdaterWindow->objectName().isEmpty())
            CanUpdaterWindow->setObjectName(QStringLiteral("CanUpdaterWindow"));
        CanUpdaterWindow->resize(406, 111);
        closeButton = new QPushButton(CanUpdaterWindow);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        closeButton->setGeometry(QRect(70, 80, 99, 21));
        QFont font;
        font.setPointSize(10);
        closeButton->setFont(font);
        select_file_label = new QLabel(CanUpdaterWindow);
        select_file_label->setObjectName(QStringLiteral("select_file_label"));
        select_file_label->setGeometry(QRect(10, 10, 141, 17));
        select_file_label->setFont(font);
        browseButton = new QPushButton(CanUpdaterWindow);
        browseButton->setObjectName(QStringLiteral("browseButton"));
        browseButton->setGeometry(QRect(290, 40, 99, 26));
        browseButton->setFont(font);
        firmwareImageText = new QTextBrowser(CanUpdaterWindow);
        firmwareImageText->setObjectName(QStringLiteral("firmwareImageText"));
        firmwareImageText->setGeometry(QRect(20, 40, 256, 26));
        firmwareImageText->setMaximumSize(QSize(256, 26));
        QFont font1;
        font1.setPointSize(9);
        firmwareImageText->setFont(font1);
        firmwareImageText->setInputMethodHints(Qt::ImhMultiLine);
        firmwareImageText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        firmwareImageText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        firmwareImageText->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        firmwareImageText->setLineWrapMode(QTextEdit::WidgetWidth);
        uploadButton = new QPushButton(CanUpdaterWindow);
        uploadButton->setObjectName(QStringLiteral("uploadButton"));
        uploadButton->setGeometry(QRect(220, 80, 99, 21));
        uploadButton->setFont(font);

        retranslateUi(CanUpdaterWindow);

        QMetaObject::connectSlotsByName(CanUpdaterWindow);
    } // setupUi

    void retranslateUi(QDialog *CanUpdaterWindow)
    {
        CanUpdaterWindow->setWindowTitle(QApplication::translate("CanUpdaterWindow", "Dialog", 0));
        closeButton->setText(QApplication::translate("CanUpdaterWindow", "Close", 0));
        select_file_label->setText(QApplication::translate("CanUpdaterWindow", "Select firmware image:", 0));
        browseButton->setText(QApplication::translate("CanUpdaterWindow", "Browse", 0));
        firmwareImageText->setHtml(QApplication::translate("CanUpdaterWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
        uploadButton->setText(QApplication::translate("CanUpdaterWindow", "Upload", 0));
    } // retranslateUi

};

namespace Ui {
    class CanUpdaterWindow: public Ui_CanUpdaterWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANUPDATERWINDOW_H
