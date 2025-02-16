#ifndef SETTING_H
#define SETTING_H

#include "BasePage.h"
class ElaRadioButton;
class ElaToggleSwitch;
class ElaComboBox;
class setting : public BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit setting(QWidget* parent = nullptr);
    ~setting();

private:
    ElaComboBox* _themeComboBox{nullptr};
    ElaToggleSwitch* _micaSwitchButton{nullptr};
    ElaToggleSwitch* _logSwitchButton{nullptr};
    ElaRadioButton* _minimumButton{nullptr};
    ElaRadioButton* _compactButton{nullptr};
    ElaRadioButton* _maximumButton{nullptr};
    ElaRadioButton* _autoButton{nullptr};
};

#endif // SETTING_H
