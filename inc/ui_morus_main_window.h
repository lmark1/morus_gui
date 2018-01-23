/********************************************************************************
** Form generated from reading UI file 'morus_main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MORUS_MAIN_WINDOW_H
#define UI_MORUS_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_morus_main_window
{
public:
    QWidget *centralWidget;
    QLabel *local_node_properties_label;
    QFrame *local_node_properties_frame;
    QLabel *local_node_id_label;
    QSpinBox *local_node_id_spinBox;
    QLabel *can_iface_name_label;
    QPlainTextEdit *can_iface_name_plainTextEdit;
    QPushButton *start_local_node_button;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *morus_main_window)
    {
        if (morus_main_window->objectName().isEmpty())
            morus_main_window->setObjectName(QStringLiteral("morus_main_window"));
        morus_main_window->resize(557, 202);
        centralWidget = new QWidget(morus_main_window);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        local_node_properties_label = new QLabel(centralWidget);
        local_node_properties_label->setObjectName(QStringLiteral("local_node_properties_label"));
        local_node_properties_label->setGeometry(QRect(10, 20, 461, 17));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(159, 158, 158, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        local_node_properties_label->setPalette(palette);
        QFont font;
        font.setFamily(QStringLiteral("Ubuntu"));
        font.setPointSize(11);
        local_node_properties_label->setFont(font);
        local_node_properties_frame = new QFrame(centralWidget);
        local_node_properties_frame->setObjectName(QStringLiteral("local_node_properties_frame"));
        local_node_properties_frame->setGeometry(QRect(10, 40, 491, 61));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush2(QColor(225, 225, 225, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush2);
        QBrush brush3(QColor(255, 255, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush3);
        QBrush brush4(QColor(240, 240, 240, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush4);
        QBrush brush5(QColor(112, 112, 112, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush5);
        QBrush brush6(QColor(150, 150, 150, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush3);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        local_node_properties_frame->setPalette(palette1);
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        local_node_properties_frame->setFont(font1);
        local_node_properties_frame->setAutoFillBackground(true);
        local_node_properties_frame->setFrameShape(QFrame::StyledPanel);
        local_node_properties_frame->setFrameShadow(QFrame::Sunken);
        local_node_properties_frame->setLineWidth(1);
        local_node_id_label = new QLabel(local_node_properties_frame);
        local_node_id_label->setObjectName(QStringLiteral("local_node_id_label"));
        local_node_id_label->setGeometry(QRect(20, 10, 101, 31));
        QFont font2;
        font2.setPointSize(11);
        local_node_id_label->setFont(font2);
        local_node_id_spinBox = new QSpinBox(local_node_properties_frame);
        local_node_id_spinBox->setObjectName(QStringLiteral("local_node_id_spinBox"));
        local_node_id_spinBox->setGeometry(QRect(120, 10, 51, 31));
        QPalette palette2;
        QBrush brush8(QColor(240, 119, 70, 0));
        brush8.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush8);
        palette2.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
        local_node_id_spinBox->setPalette(palette2);
        local_node_id_spinBox->setCursor(QCursor(Qt::ArrowCursor));
        local_node_id_spinBox->setContextMenuPolicy(Qt::DefaultContextMenu);
        local_node_id_spinBox->setMaximum(255);
        can_iface_name_label = new QLabel(local_node_properties_frame);
        can_iface_name_label->setObjectName(QStringLiteral("can_iface_name_label"));
        can_iface_name_label->setGeometry(QRect(280, 10, 111, 31));
        can_iface_name_label->setFont(font2);
        can_iface_name_plainTextEdit = new QPlainTextEdit(local_node_properties_frame);
        can_iface_name_plainTextEdit->setObjectName(QStringLiteral("can_iface_name_plainTextEdit"));
        can_iface_name_plainTextEdit->setGeometry(QRect(400, 10, 71, 31));
        QFont font3;
        font3.setFamily(QStringLiteral("Ubuntu"));
        can_iface_name_plainTextEdit->setFont(font3);
        can_iface_name_plainTextEdit->setMouseTracking(false);
        can_iface_name_plainTextEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        can_iface_name_plainTextEdit->setFrameShape(QFrame::StyledPanel);
        can_iface_name_plainTextEdit->setFrameShadow(QFrame::Sunken);
        can_iface_name_plainTextEdit->setReadOnly(false);
        can_iface_name_plainTextEdit->setBackgroundVisible(false);
        start_local_node_button = new QPushButton(local_node_properties_frame);
        start_local_node_button->setObjectName(QStringLiteral("start_local_node_button"));
        start_local_node_button->setGeometry(QRect(210, 40, 81, 21));
        morus_main_window->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(morus_main_window);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 557, 25));
        morus_main_window->setMenuBar(menuBar);
        mainToolBar = new QToolBar(morus_main_window);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        morus_main_window->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(morus_main_window);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        morus_main_window->setStatusBar(statusBar);

        retranslateUi(morus_main_window);

        QMetaObject::connectSlotsByName(morus_main_window);
    } // setupUi

    void retranslateUi(QMainWindow *morus_main_window)
    {
        morus_main_window->setWindowTitle(QApplication::translate("morus_main_window", "morus_main_window", 0));
        local_node_properties_label->setText(QApplication::translate("morus_main_window", "Local node properties", 0));
        local_node_id_label->setText(QApplication::translate("morus_main_window", "Local node ID:", 0));
        can_iface_name_label->setText(QApplication::translate("morus_main_window", "CAN iface name:", 0));
        start_local_node_button->setText(QApplication::translate("morus_main_window", "Start", 0));
    } // retranslateUi

};

namespace Ui {
    class morus_main_window: public Ui_morus_main_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MORUS_MAIN_WINDOW_H
