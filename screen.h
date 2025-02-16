#ifndef SCREEN_H
#define SCREEN_H
#include <QWidget>

#include "basepage.h"
#ifdef Q_OS_WIN
class ElaDxgiScreen;
class ElaComboBox;
class screen:public BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit screen(QWidget* parent = nullptr);
    ~screen();
private:
    ElaDxgiScreen* _dxgiScreen{nullptr};
    ElaComboBox* _dxComboBox{nullptr};
    ElaComboBox* _outputComboBox{nullptr};
};
#endif
#endif // SCREEN_H
