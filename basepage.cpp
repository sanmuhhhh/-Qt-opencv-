#include "basepage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"
#include "ElaSlider.h"
#include "ElaGraphicsItem.h"
#include "ElaGraphicsScene.h"
#include "ElaContentDialog.h"
#include "ElaMessageBar.h"
BasePage::BasePage(QWidget *parent)
  : ElaScrollPage(parent)
{
    connect(eTheme,&ElaTheme::themeModeChanged,this,[=](){
        if(!parent)
        {
            update();
        }
    });
}
BasePage::~BasePage()
{

}
/*--------------------静态函数实现--------------------*/
/** 文本配置 */
ElaText* BasePage::setElaText(const QString &text, QWidget *parent, int TextSize, bool isBold)
{
    ElaText* Text = new ElaText(parent);
    Text->setText(text);
    Text->setTextInteractionFlags(Qt::TextSelectableByMouse);
    if(isBold){
        Text->setFont(TextFont);
    }
    else{
        Text->setFont(TextNormalFont);
    }
    Text->setTextPixelSize(TextSize);
    Text->setWordWrap(false);
    return Text;
}
/** 工具栏配置 */
ElaToolButton *BasePage::setElaTool(const QString &textint, int ButtonHeight, ElaIconType::IconName icon, QWidget *parent, bool isTransparent)
{
    ElaToolButton * Tool = new ElaToolButton(parent);
    Tool->setFixedHeight(ButtonHeight);
    Tool->setIsTransparent(isTransparent);
    Tool->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Tool->setText(textint);
    Tool->setElaIcon(icon);
    return Tool;
}
/** 按钮配置*/
ElaPushButton *BasePage::setElaPushButton(const QString &text,QWidget *parent,int Width, int Height)
{
    ElaPushButton* btn = new ElaPushButton(parent);
    btn->setBorderRadius(8);
    btn->setFixedSize(Width,Height);
    btn->setText(text);
    return btn;
}
/** 横向布局配置*/
QHBoxLayout *BasePage::setElaHBoxLayout(ElaText *LeftText, ElaText *RightText, QWidget *parent)
{
    QHBoxLayout* Layout = new QHBoxLayout(parent);
    Layout->addWidget(LeftText);
    Layout->addStretch();
    Layout->addWidget(RightText);
    Layout->addSpacing(20);
    return Layout;
}
/** 滑块配置*/
ElaSlider *BasePage::setElaSlider(std::function<void ()> callback, int left, int right, int initpos, QWidget *parent)
{
    ElaSlider *slider = new ElaSlider(parent);
    slider->setRange(left,right);
    slider->setSliderPosition(initpos);
    if(callback){
        connect(slider,&ElaSlider::valueChanged,parent,[callback](){
            callback();
        });
    }
    return slider;
}
/** 场景项配置*/
void BasePage::setImage2scene(const QImage &image, ElaGraphicsScene *scene, int maxconnect)
{
    ElaGraphicsItem *item = new ElaGraphicsItem();
    item->setItemImage(image);
    item->setItemSelectedImage(item->getItemImage());
    item->setSelected(true);
    item->setWidth(image.width()/2);
    item->setHeight(image.height()/2);
    item->setMaxLinkPortCount(maxconnect);
    scene->addItem(item);
}

void BasePage::showConfirmDialog(const QString &title, const QString &message, const QString &confirmText, const std::function<void ()> &onConfirm, QWidget *parent)
{

    ElaContentDialog* dialog = new ElaContentDialog(parent);// 使用动态内存分配确保对话框在 lambda 中存活
    dialog->setLeftButtonText("取消");
    dialog->setMiddleButtonText("再想一下");
    dialog->setRightButtonText(confirmText);
    QWidget* content = new QWidget(dialog);
    QVBoxLayout* layout = new QVBoxLayout(content);
    ElaText* _title = new ElaText(dialog);
    _title->setText(title);
    _title->setTextStyle(ElaTextType::Title);
    ElaText* _message = new ElaText(dialog);
    _message->setTextStyle(ElaTextType::Body);
    _message->setText(message);
    layout->setContentsMargins(15, 25, 15, 10);
    layout->addWidget(_title);
    layout->addSpacing(2);
    layout->addWidget(_message);
    layout->addStretch();
    dialog->setCentralWidget(content);

    // 正确捕获方式
    connect(dialog, &ElaContentDialog::rightButtonClicked, parent, [=]() {
        onConfirm();
        dialog->deleteLater(); // 安全删除对话框
    });

    connect(dialog, &ElaContentDialog::leftButtonClicked, dialog, [=]() {
        dialog->deleteLater();
        ElaMessageBar::information(ElaMessageBarType::PositionPolicy::TopLeft, "Info", "操作已取消.", 1000, nullptr);
    });

    dialog->show(); // 改用非模态显示或保持 exec()

}

/*--------------------子类重写窗口--------------------*/
void BasePage::createCustomWidget(QString desText)
{
    /** 顶部部件*/
    QWidget* customWidget = new QWidget(this);
    ElaText* subTitleText=setElaText("https://github.com/sanmuhhhh",this,11);
    ElaToolButton* documentationButton=setElaTool("Documentation",35,ElaIconType::FileDoc,this);
    ElaMenu* documentationMenu = new ElaMenu(this);
    documentationMenu->addElaIconAction(ElaIconType::CardsBlank, "CardsBlank");
    documentationMenu->addElaIconAction(ElaIconType::EarthAmericas, "EarthAmericas");
    documentationButton->setMenu(documentationMenu);
    ElaToolButton* sourceButton =setElaTool("Source",35,ElaIconType::NfcSymbol,this,false);
    ElaMenu* sourceMenu = new ElaMenu(this);
    sourceMenu->addElaIconAction(ElaIconType::FireBurner, "在这里填写功能名");
    sourceMenu->addElaIconAction(ElaIconType::Galaxy, "在这里填写功能名");
    sourceButton->setMenu(sourceMenu);
    ElaToolButton* themeButton =setElaTool("",35,ElaIconType::MoonStars,this,false);
    connect(themeButton, &ElaToolButton::clicked, this, [=]() {
        eTheme->setThemeMode(eTheme->getThemeMode() == ElaThemeType::Light ? ElaThemeType::Dark : ElaThemeType::Light);
    });
    /** 布局实现*/
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(documentationButton);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(sourceButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(themeButton);
    buttonLayout->addSpacing(15);
    ElaText* descText =setElaText(desText,this,13,true);
    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addLayout(buttonLayout);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);
    setCustomWidget(customWidget);
}

