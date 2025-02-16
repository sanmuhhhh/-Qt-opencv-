#include "About.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>

#include "ElaImageCard.h"
#include "ElaText.h"
#include "basepage.h"
About::About(QWidget* parent)
    : ElaWidget(parent)
{
/*--------------------窗口默认配置--------------------*/
    setWindowTitle("");
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    ElaImageCard* pixCard = new ElaImageCard(this);//图像卡片
    pixCard->setFixedSize(80, 80);
    pixCard->setIsPreserveAspectCrop(false);
    pixCard->setCardImage(QImage(":/include/Image/Amiya.jpg"));
    pixCard->backgroundRole();
    QVBoxLayout* pixCardLayout = new QVBoxLayout();
    pixCardLayout->addWidget(pixCard);
    pixCardLayout->addStretch();
/** 文本元素 */
    ElaText* versionText = BasePage::setElaText("Sanmu_Graphics1.0/2024",this,18,true);
    ElaText* licenseText = BasePage::setElaText("MIT、GPL授权协议",this,13);
    ElaText* supportText = BasePage::setElaText("Windows支持版本: QT5.12以上\nLinux支持版本: Qt5.14以上\nIos支持版本:未知", this,13);
    ElaText* contactText = BasePage::setElaText("作者: Sanmu\n qq: 3501099157n", this,13);
    ElaText* helperText = BasePage::setElaText("用于毕业设计\n第三方库OpenCV、ElaFluentUi\n需要提供技术支持请联系作者", this,13);
    ElaText* copyrightText =BasePage::setElaText("版权所有 © 2025 @Sanmu", this,14,true);

/** 布局部分 */
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->addWidget(versionText);
    textLayout->addSpacing(20);
    textLayout->addWidget(licenseText);
    textLayout->addWidget(supportText);
    textLayout->addWidget(contactText);
    textLayout->addWidget(helperText);
    textLayout->addWidget(copyrightText);
    textLayout->addStretch();
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->addLayout(pixCardLayout);
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);// 创建主布局
    mainLayout->setContentsMargins(0, 25, 0, 0);
    mainLayout->addLayout(contentLayout);
}

About::~About()
{
}
