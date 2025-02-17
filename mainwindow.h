#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ElaWindow.h"
class home;
class screen;
class ElaContentDialog;
class About;
class Graphics;
class setting;
class leetcode;
class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initWindow();
    void initEdgeLayout();
    void initContent();

private:
    ElaContentDialog* _closeDialog{nullptr};
    home* _homePage{nullptr};
    About* _aboutPage{nullptr};
    Graphics* _graphicsPage{nullptr};
    setting* _settingPage{nullptr};
    leetcode* _algorithm{nullptr};
    class screen* _vedio{nullptr};
    QString _aboutKey{""};
    QString _settingKey{""};

};
#endif // MAINWINDOW_H
