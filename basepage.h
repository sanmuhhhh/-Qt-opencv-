#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <ElaScrollPage.h>
#include <ElaIcon.h>
#include <ElaMenu.h>
#include <ElaPushButton.h>
class QVBoxLayout;
class ElaToolButton;
class ElaIcon;
class Elamenu;
class ElaPushButton;
class ElaSlider;
class ElaGraphicsScene;
/*--------------------全局字体配置--------------------*/
static const QFont TextFont = [](){
    QFont font;
    font.setWeight(QFont::Bold);  // 设置字体为粗体
    return font;
}();
static const QFont TextNormalFont = []() {
    QFont font;
    font.setWeight(QFont::Normal);  // 设置字体为正常权重
    return font;
}();
class BasePage: public ElaScrollPage
{
    Q_OBJECT
public:
/*--------------------全局静态函数--------------------*/
    explicit BasePage(QWidget *parent = nullptr);
    ~BasePage();
    static ElaText* setElaText(const QString &text,QWidget *parent =nullptr,int TextSize = 20,bool isBold = false); //文本配置
    static ElaToolButton* setElaTool(const QString &textint,int ButtonHeight,ElaIconType::IconName icon,QWidget *parent = nullptr,bool isTransparent = true);//工具栏配置
    static ElaPushButton* setElaPushButton(const QString &text,QWidget *parent = nullptr,int Width = 120,int Height = 50);//按钮配置
    static QHBoxLayout* setElaHBoxLayout(ElaText* LeftText,ElaText *RightText,QWidget *parent =nullptr);                //横向布局
    static ElaSlider* setElaSlider(std::function<void()> callback = nullptr,int left = -100,int right = 100,int initpos = 0,QWidget* parent = nullptr);
    static void setImage2scene(const QImage &image,ElaGraphicsScene* scene,int maxconnect = 10);//图像加入场景
    static void showConfirmDialog(
        const QString& title,
        const QString& message,
        const QString& confirmText,
        const std::function<void()>& onConfirm,
        QWidget *parent = nullptr);
protected:
    void createCustomWidget(QString desText);
private:

};

#endif // BASEPAGE_H
