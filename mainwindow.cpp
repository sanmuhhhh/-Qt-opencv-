#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QStatusBar>
#include <QVBoxLayout>

#include "ElaContentDialog.h"
#include "ElaDockWidget.h"
#include "ElaMenuBar.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"

#include "graphics.h"
#include "home.h"
#include "about.h"
#include "screen.h"
#include "setting.h"

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
{
    initWindow();
    initEdgeLayout();
    initContent();
    // 关闭事件
    _closeDialog = new ElaContentDialog(this);
    _closeDialog->setLeftButtonText("取消");
    _closeDialog->setMiddleButtonText("最小化");
    _closeDialog->setRightButtonText("退出");
    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(_closeDialog, &ElaContentDialog::middleButtonClicked, this, [=]() {
        _closeDialog->close();
        showMinimized();
    });
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        _closeDialog->exec();
    });
    moveToCenter();

}

MainWindow::~MainWindow() {}


void MainWindow::initWindow()
{

    setWindowIcon(QIcon(":/include/Image/Amiya.jpg"));
    resize(1200,740);
    setUserInfoCardVisible(false);
    setWindowTitle("Sammu_Graphics");

}

void MainWindow::initEdgeLayout()
{
    //主选项
    ElaMenuBar* menuBar = new ElaMenuBar(this);
    menuBar->setFixedHeight(30);
    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();
 
    //工具栏
    ElaToolBar* toolBar = new ElaToolBar("工具栏", this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setToolBarSpacing(3);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setIconSize(QSize(25, 25));

    ElaToolButton* toolButton1 =
    BasePage::setElaTool("",30,ElaIconType::ChartUser,this);
    toolBar->addWidget(toolButton1);
    toolBar->addSeparator();
    this->addToolBar(Qt::TopToolBarArea, toolBar);

    //停靠窗口
    ElaDockWidget* logDockWidget = new ElaDockWidget("日志信息", this);
    //logDockWidget->setWidget(new T_LogWidget(this));
    logDockWidget->setWindowIcon(QIcon(""));
    this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
    resizeDocks({logDockWidget}, {200}, Qt::Horizontal);

    ElaDockWidget* updateDockWidget = new ElaDockWidget("项目内容", this);
    //updateDockWidget->setWidget(new T_UpdateWidget(this));
    updateDockWidget->setWindowIcon(QIcon(""));
    this->addDockWidget(Qt::RightDockWidgetArea, updateDockWidget);
    resizeDocks({updateDockWidget}, {200}, Qt::Horizontal);


}



void MainWindow::initContent()
{
    _homePage = new home(this);
    _graphicsPage = new Graphics(this);
    _settingPage = new setting(this);
    _algorithm = new leetcode(this);
    _vedio = new  class screen(this);
    QString testKey_1;
    QString testKey_2;

    addPageNode("HOME",_homePage,ElaIconType::House);
    addPageNode("Graphics", _graphicsPage, 1, ElaIconType::Paintbrush);
    addPageNode("Algorithm", _algorithm, ElaIconType::PenRuler);
    addPageNode("Vedio",_vedio,ElaIconType::Camera);
    addFooterNode("About", nullptr, _aboutKey, 0, ElaIconType::User);
    _aboutPage = new About();
    _aboutPage->hide();
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        if (_aboutKey == nodeKey)
        {
            _aboutPage->setFixedSize(400, 400);
            _aboutPage->moveToCenter();
            _aboutPage->show();
        }
    });
    addFooterNode("Setting", _settingPage, _settingKey, 0, ElaIconType::GearComplex);

}

























