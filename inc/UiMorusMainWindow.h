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
#include <QtWidgets/QComboBox>
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
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

// Forward declaration of QComboBox - for IDE syntax error processing
class QComboBox;

class Ui_MorusMainWindow
{
public:
    QWidget *centralWidget;
    QLabel *local_node_properties_label;
    QFrame *local_node_properties_frame;
    QLabel *local_node_id_label;
    QSpinBox *localNodeIDSpinBox;
    QLabel *can_iface_name_label;
    QPlainTextEdit *canIfaceNamePlainTextEdit;
    QPushButton *startLocalNodeButton;
    QLabel *online_nodes_label;
    QFrame *can_node_monitor_frame;
    QTreeWidget *canNodeMonitor;
    QLabel *node_controls_label;
    QFrame *node_controls_frame;
    QPushButton *updateFirmwareButton;
    QPushButton *restartHardwareButton;
    QFrame *configuration_parameters_frame;
    QPushButton *fetchParamButton;
    QPushButton *storeParamButton;
    QPushButton *eraseParamButton;
    QTreeWidget *parameterTreeWidget;
    QPushButton *updateButton;
    QLabel *group_label_4;
    QComboBox *parameterGroupBox;
    QPushButton *calibrateButton;
    QPushButton *loadParametersButton;
    QLabel *InfoLabel_2;
    QLabel *configuration_parameters_label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *morus_main_window)
    {
        if (morus_main_window->objectName().isEmpty())
            morus_main_window->setObjectName(QStringLiteral("morus_main_window"));
        morus_main_window->resize(1092, 553);
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
        localNodeIDSpinBox = new QSpinBox(local_node_properties_frame);
        localNodeIDSpinBox->setObjectName(QStringLiteral("localNodeIDSpinBox"));
        localNodeIDSpinBox->setGeometry(QRect(120, 10, 51, 31));
        QPalette palette2;
        QBrush brush8(QColor(240, 119, 70, 0));
        brush8.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush8);
        palette2.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
        localNodeIDSpinBox->setPalette(palette2);
        localNodeIDSpinBox->setCursor(QCursor(Qt::ArrowCursor));
        localNodeIDSpinBox->setContextMenuPolicy(Qt::DefaultContextMenu);
        localNodeIDSpinBox->setMaximum(255);
        can_iface_name_label = new QLabel(local_node_properties_frame);
        can_iface_name_label->setObjectName(QStringLiteral("can_iface_name_label"));
        can_iface_name_label->setGeometry(QRect(280, 10, 111, 31));
        can_iface_name_label->setFont(font2);
        canIfaceNamePlainTextEdit = new QPlainTextEdit(local_node_properties_frame);
        canIfaceNamePlainTextEdit->setObjectName(QStringLiteral("canIfaceNamePlainTextEdit"));
        canIfaceNamePlainTextEdit->setGeometry(QRect(400, 10, 71, 31));
        QFont font3;
        font3.setFamily(QStringLiteral("Ubuntu"));
        canIfaceNamePlainTextEdit->setFont(font3);
        canIfaceNamePlainTextEdit->setMouseTracking(false);
        canIfaceNamePlainTextEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        canIfaceNamePlainTextEdit->setFrameShape(QFrame::StyledPanel);
        canIfaceNamePlainTextEdit->setFrameShadow(QFrame::Sunken);
        canIfaceNamePlainTextEdit->setReadOnly(false);
        canIfaceNamePlainTextEdit->setBackgroundVisible(false);
        startLocalNodeButton = new QPushButton(local_node_properties_frame);
        startLocalNodeButton->setObjectName(QStringLiteral("startLocalNodeButton"));
        startLocalNodeButton->setGeometry(QRect(210, 40, 81, 21));
        online_nodes_label = new QLabel(centralWidget);
        online_nodes_label->setObjectName(QStringLiteral("online_nodes_label"));
        online_nodes_label->setGeometry(QRect(20, 110, 461, 20));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        online_nodes_label->setPalette(palette3);
        online_nodes_label->setFont(font2);
        can_node_monitor_frame = new QFrame(centralWidget);
        can_node_monitor_frame->setObjectName(QStringLiteral("can_node_monitor_frame"));
        can_node_monitor_frame->setGeometry(QRect(20, 130, 501, 201));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush5);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush6);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush3);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        can_node_monitor_frame->setPalette(palette4);
        can_node_monitor_frame->setFont(font1);
        can_node_monitor_frame->setAutoFillBackground(true);
        can_node_monitor_frame->setFrameShape(QFrame::StyledPanel);
        can_node_monitor_frame->setFrameShadow(QFrame::Sunken);
        can_node_monitor_frame->setLineWidth(1);
        canNodeMonitor = new QTreeWidget(can_node_monitor_frame);
        QBrush brush9(QColor(77, 126, 56, 255));
        brush9.setStyle(Qt::SolidPattern);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setTextAlignment(5, Qt::AlignCenter);
        __qtreewidgetitem->setForeground(5, brush9);
        __qtreewidgetitem->setTextAlignment(4, Qt::AlignCenter);
        __qtreewidgetitem->setForeground(4, brush9);
        __qtreewidgetitem->setTextAlignment(3, Qt::AlignCenter);
        __qtreewidgetitem->setForeground(3, brush9);
        __qtreewidgetitem->setTextAlignment(2, Qt::AlignCenter);
        __qtreewidgetitem->setForeground(2, brush9);
        __qtreewidgetitem->setTextAlignment(1, Qt::AlignCenter);
        __qtreewidgetitem->setForeground(1, brush9);
        __qtreewidgetitem->setTextAlignment(0, Qt::AlignCenter);
        __qtreewidgetitem->setForeground(0, brush9);
        canNodeMonitor->setHeaderItem(__qtreewidgetitem);
        canNodeMonitor->setObjectName(QStringLiteral("canNodeMonitor"));
        canNodeMonitor->setGeometry(QRect(10, 10, 481, 181));
        QFont font4;
        font4.setPointSize(9);
        font4.setKerning(true);
        font4.setStyleStrategy(QFont::PreferDefault);
        canNodeMonitor->setFont(font4);
        canNodeMonitor->setMouseTracking(false);
        canNodeMonitor->setFrameShape(QFrame::StyledPanel);
        canNodeMonitor->setFrameShadow(QFrame::Sunken);
        canNodeMonitor->setLineWidth(1);
        canNodeMonitor->setMidLineWidth(1);
        canNodeMonitor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        canNodeMonitor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        canNodeMonitor->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        canNodeMonitor->setEditTriggers(QAbstractItemView::SelectedClicked);
        canNodeMonitor->setDragEnabled(false);
        canNodeMonitor->setSelectionMode(QAbstractItemView::SingleSelection);
        canNodeMonitor->setIndentation(0);
        canNodeMonitor->header()->setVisible(true);
        canNodeMonitor->header()->setCascadingSectionResizes(false);
        canNodeMonitor->header()->setDefaultSectionSize(50);
        canNodeMonitor->header()->setHighlightSections(false);
        canNodeMonitor->header()->setStretchLastSection(true);
        node_controls_label = new QLabel(centralWidget);
        node_controls_label->setObjectName(QStringLiteral("node_controls_label"));
        node_controls_label->setGeometry(QRect(20, 340, 461, 17));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        node_controls_label->setPalette(palette5);
        node_controls_label->setFont(font);
        node_controls_frame = new QFrame(centralWidget);
        node_controls_frame->setObjectName(QStringLiteral("node_controls_frame"));
        node_controls_frame->setGeometry(QRect(20, 360, 501, 41));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Light, brush3);
        palette6.setBrush(QPalette::Active, QPalette::Midlight, brush4);
        palette6.setBrush(QPalette::Active, QPalette::Dark, brush5);
        palette6.setBrush(QPalette::Active, QPalette::Mid, brush6);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush);
        palette6.setBrush(QPalette::Active, QPalette::BrightText, brush3);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Light, brush3);
        palette6.setBrush(QPalette::Inactive, QPalette::Midlight, brush4);
        palette6.setBrush(QPalette::Inactive, QPalette::Dark, brush5);
        palette6.setBrush(QPalette::Inactive, QPalette::Mid, brush6);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::BrightText, brush3);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Light, brush3);
        palette6.setBrush(QPalette::Disabled, QPalette::Midlight, brush4);
        palette6.setBrush(QPalette::Disabled, QPalette::Dark, brush5);
        palette6.setBrush(QPalette::Disabled, QPalette::Mid, brush6);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette6.setBrush(QPalette::Disabled, QPalette::BrightText, brush3);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush5);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        node_controls_frame->setPalette(palette6);
        node_controls_frame->setFont(font1);
        node_controls_frame->setAutoFillBackground(true);
        node_controls_frame->setFrameShape(QFrame::StyledPanel);
        node_controls_frame->setFrameShadow(QFrame::Sunken);
        node_controls_frame->setLineWidth(1);
        updateFirmwareButton = new QPushButton(node_controls_frame);
        updateFirmwareButton->setObjectName(QStringLiteral("updateFirmwareButton"));
        updateFirmwareButton->setGeometry(QRect(280, 10, 211, 21));
        QFont font5;
        font5.setPointSize(10);
        updateFirmwareButton->setFont(font5);
        restartHardwareButton = new QPushButton(node_controls_frame);
        restartHardwareButton->setObjectName(QStringLiteral("restartHardwareButton"));
        restartHardwareButton->setGeometry(QRect(10, 10, 211, 21));
        restartHardwareButton->setFont(font5);
        configuration_parameters_frame = new QFrame(centralWidget);
        configuration_parameters_frame->setObjectName(QStringLiteral("configuration_parameters_frame"));
        configuration_parameters_frame->setGeometry(QRect(540, 70, 501, 401));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Light, brush3);
        palette7.setBrush(QPalette::Active, QPalette::Midlight, brush4);
        palette7.setBrush(QPalette::Active, QPalette::Dark, brush5);
        palette7.setBrush(QPalette::Active, QPalette::Mid, brush6);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush);
        palette7.setBrush(QPalette::Active, QPalette::BrightText, brush3);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Light, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::Midlight, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::Dark, brush5);
        palette7.setBrush(QPalette::Inactive, QPalette::Mid, brush6);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::BrightText, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Light, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::Midlight, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Dark, brush5);
        palette7.setBrush(QPalette::Disabled, QPalette::Mid, brush6);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette7.setBrush(QPalette::Disabled, QPalette::BrightText, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush5);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        configuration_parameters_frame->setPalette(palette7);
        configuration_parameters_frame->setFont(font1);
        configuration_parameters_frame->setAutoFillBackground(true);
        configuration_parameters_frame->setFrameShape(QFrame::StyledPanel);
        configuration_parameters_frame->setFrameShadow(QFrame::Sunken);
        configuration_parameters_frame->setLineWidth(1);
        fetchParamButton = new QPushButton(configuration_parameters_frame);
        fetchParamButton->setObjectName(QStringLiteral("fetchParamButton"));
        fetchParamButton->setGeometry(QRect(10, 10, 101, 31));
        fetchParamButton->setFont(font5);
        storeParamButton = new QPushButton(configuration_parameters_frame);
        storeParamButton->setObjectName(QStringLiteral("storeParamButton"));
        storeParamButton->setGeometry(QRect(280, 10, 101, 31));
        storeParamButton->setFont(font5);
        eraseParamButton = new QPushButton(configuration_parameters_frame);
        eraseParamButton->setObjectName(QStringLiteral("eraseParamButton"));
        eraseParamButton->setGeometry(QRect(390, 10, 101, 31));
        eraseParamButton->setFont(font5);
        parameterTreeWidget = new QTreeWidget(configuration_parameters_frame);
        parameterTreeWidget->setObjectName(QStringLiteral("parameterTreeWidget"));
        parameterTreeWidget->setGeometry(QRect(10, 130, 481, 261));
        QFont font6;
        font6.setPointSize(9);
        parameterTreeWidget->setFont(font6);
        parameterTreeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        parameterTreeWidget->header()->setDefaultSectionSize(50);
        updateButton = new QPushButton(configuration_parameters_frame);
        updateButton->setObjectName(QStringLiteral("updateButton"));
        updateButton->setGeometry(QRect(120, 10, 101, 31));
        updateButton->setFont(font5);
        group_label_4 = new QLabel(configuration_parameters_frame);
        group_label_4->setObjectName(QStringLiteral("group_label_4"));
        group_label_4->setGeometry(QRect(10, 90, 51, 27));
        group_label_4->setFont(font5);
        parameterGroupBox = new QComboBox(configuration_parameters_frame);
        parameterGroupBox->setObjectName(QStringLiteral("parameterGroupBox"));
        parameterGroupBox->setGeometry(QRect(60, 90, 251, 27));
        parameterGroupBox->setFont(font5);
        calibrateButton = new QPushButton(configuration_parameters_frame);
        calibrateButton->setObjectName(QStringLiteral("calibrateButton"));
        calibrateButton->setGeometry(QRect(280, 40, 211, 41));
        calibrateButton->setFont(font5);
        loadParametersButton = new QPushButton(configuration_parameters_frame);
        loadParametersButton->setObjectName(QStringLiteral("loadParametersButton"));
        loadParametersButton->setGeometry(QRect(10, 40, 211, 41));
        loadParametersButton->setFont(font5);
        InfoLabel_2 = new QLabel(centralWidget);
        InfoLabel_2->setObjectName(QStringLiteral("InfoLabel_2"));
        InfoLabel_2->setGeometry(QRect(540, 397, 491, 20));
        InfoLabel_2->setFont(font5);
        configuration_parameters_label = new QLabel(centralWidget);
        configuration_parameters_label->setObjectName(QStringLiteral("configuration_parameters_label"));
        configuration_parameters_label->setGeometry(QRect(540, 40, 461, 17));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        configuration_parameters_label->setPalette(palette8);
        configuration_parameters_label->setFont(font);
        morus_main_window->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(morus_main_window);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1092, 25));
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
        startLocalNodeButton->setText(QApplication::translate("morus_main_window", "Start", 0));
        online_nodes_label->setText(QApplication::translate("morus_main_window", "Online nodes", 0));
        QTreeWidgetItem *___qtreewidgetitem = canNodeMonitor->headerItem();
        ___qtreewidgetitem->setText(5, QApplication::translate("morus_main_window", "VSSC", 0));
        ___qtreewidgetitem->setText(4, QApplication::translate("morus_main_window", "Uptime", 0));
        ___qtreewidgetitem->setText(3, QApplication::translate("morus_main_window", "Health", 0));
        ___qtreewidgetitem->setText(2, QApplication::translate("morus_main_window", "Mode", 0));
        ___qtreewidgetitem->setText(1, QApplication::translate("morus_main_window", "Name", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("morus_main_window", "NID", 0));
        node_controls_label->setText(QApplication::translate("morus_main_window", "Node  controls", 0));
        updateFirmwareButton->setText(QApplication::translate("morus_main_window", "Update firmware", 0));
        restartHardwareButton->setText(QApplication::translate("morus_main_window", "Restart", 0));
        fetchParamButton->setText(QApplication::translate("morus_main_window", "Fetch All", 0));
        storeParamButton->setText(QApplication::translate("morus_main_window", "Store All", 0));
        eraseParamButton->setText(QApplication::translate("morus_main_window", "Set to default", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = parameterTreeWidget->headerItem();
        ___qtreewidgetitem1->setText(6, QApplication::translate("morus_main_window", "Max", 0));
        ___qtreewidgetitem1->setText(5, QApplication::translate("morus_main_window", "Min", 0));
        ___qtreewidgetitem1->setText(4, QApplication::translate("morus_main_window", "Default", 0));
        ___qtreewidgetitem1->setText(3, QApplication::translate("morus_main_window", "Unit", 0));
        ___qtreewidgetitem1->setText(2, QApplication::translate("morus_main_window", "Value", 0));
        ___qtreewidgetitem1->setText(1, QApplication::translate("morus_main_window", "Type", 0));
        ___qtreewidgetitem1->setText(0, QApplication::translate("morus_main_window", "Name", 0));
        updateButton->setText(QApplication::translate("morus_main_window", "Update All", 0));
        group_label_4->setText(QApplication::translate("morus_main_window", "Group:", 0));
        calibrateButton->setText(QApplication::translate("morus_main_window", "Calibrate sensors", 0));
        loadParametersButton->setText(QApplication::translate("morus_main_window", "Load Parameters", 0));
        InfoLabel_2->setText(QString());
        configuration_parameters_label->setText(QApplication::translate("morus_main_window", "Configuration parameters", 0));
    } // retranslateUi

};

namespace Ui {
    class MorusMainWindow: public Ui_MorusMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MORUS_MAIN_WINDOW_H
