#include "hslSlider.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>
#include <QTimer>
#include "ElaImageCard.h"
#include "ElaText.h"
#include "ElaMessageBar.h"
#include "ElaPushButton.h"

hslSlider::hslSlider(ElaGraphicsItem* selectedItem, ElaGraphicsScene* scene, QWidget* parent)
    : ElaWidget(parent), selectedItem_(selectedItem), scene(scene)
{
    setWindowTitle("");
    setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    pixCard = new ElaImageCard(this);
    pixCard->setIsPreserveAspectCrop(false);
    pixCard->backgroundRole();

    // 创建HSL标签布局
    QHBoxLayout* HueLayout = BasePage::setElaHBoxLayout(
        BasePage::setElaText("色相", this, 18), BasePage::setElaText("", this, 18), nullptr);
    QHBoxLayout* SatLayout = BasePage::setElaHBoxLayout(
        BasePage::setElaText("饱和度", this, 18), BasePage::setElaText("", this, 18), nullptr);
    QHBoxLayout* LightLayout = BasePage::setElaHBoxLayout(
        BasePage::setElaText("亮度", this, 18), BasePage::setElaText("", this, 18), nullptr);

    QVBoxLayout* pixCardLayout = new QVBoxLayout();
    ElaText* pretext = BasePage::setElaText("预览", this, 18, true);
    pixCardLayout->addWidget(pretext);
    pixCardLayout->addWidget(pixCard);
    pixCardLayout->addStretch();

    // 滑块初始化
    HSlider = BasePage::setElaSlider([this](){this->onslider_changed();}, -90, 90, 0, this);
    SSlider = BasePage::setElaSlider([this](){this->onslider_changed();}, -100, 100, 0, this);
    LSlider = BasePage::setElaSlider([this](){this->onslider_changed();}, -100, 100, 0, this);

    // 按钮初始化
    ElaPushButton* ConfirmButton = BasePage::setElaPushButton("确认", this);
    ElaPushButton* CancelButton = BasePage::setElaPushButton("取消", this);

    QHBoxLayout* ButtonLayout = new QHBoxLayout();
    ButtonLayout->addWidget(ConfirmButton);
    ButtonLayout->addSpacing(120);
    ButtonLayout->addWidget(CancelButton);
    ButtonLayout->addSpacing(20);

    connect(CancelButton, &ElaPushButton::clicked, this, &hslSlider::close);
    connect(ConfirmButton, &ElaPushButton::clicked, this, &hslSlider::confirm);

    QVBoxLayout* SlideLayout = new QVBoxLayout();
    auto initSliderLayout = [SlideLayout](QHBoxLayout* layout, ElaSlider* slider) {
        SlideLayout->addLayout(layout);
        SlideLayout->addWidget(slider);
        SlideLayout->addStretch();
    };

    initSliderLayout(HueLayout, HSlider);
    initSliderLayout(SatLayout, SSlider);
    initSliderLayout(LightLayout, LSlider);

    SlideLayout->addSpacing(30);
    SlideLayout->addLayout(ButtonLayout);
    SlideLayout->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->addLayout(pixCardLayout);
    contentLayout->addSpacing(80);
    contentLayout->addLayout(SlideLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 25, 0, 0);
    mainLayout->addLayout(contentLayout);
    setPreview();

    debounceTimer = new QTimer(this);
    debounceTimer->setSingleShot(true);
    connect(debounceTimer, &QTimer::timeout, this, &hslSlider::PreTransform);

    pre_task = new mythread();
    pre_thread = new QThread(this);
    pre_task->moveToThread(pre_thread);
    connect(pre_thread, &QThread::finished, pre_task, &QObject::deleteLater);
    connect(pre_task, &mythread::loadImage, [=](const QImage& image) {
        setPreview(image);
    });
    connect(this, &hslSlider::triggerPreview, pre_task, &mythread::hsl_preview);
}

hslSlider::~hslSlider()
{
    delete HSlider;
    delete SSlider;
    delete LSlider;
    delete pixCard;
    if (pre_thread && pre_thread->isRunning()) {
        pre_thread->quit();
        pre_thread->wait();
    }
    delete pre_thread;
    delete debounceTimer;
}

void hslSlider::setPreview()
{
    QImage image = selectedItem_->getItemImage();
    int width = image.width(), height = image.height();
    while (width > 800 || height > 400) {
        width = width / 2;
        height = height / 2;
    }
    pixCard->setFixedSize(width, height);
    pixCard->setCardImage(image);
    pixCard->update();
}

void hslSlider::setPreview(QImage image)
{
    QImage Pimage = image.copy();
    int width = image.width(), height = image.height();
    while (width > 800 || height > 400) {
        width = width / 2;
        height = height / 2;
    }
    pixCard->setFixedSize(width, height);
    pixCard->setCardImage(Pimage);
    pixCard->update();
}

void hslSlider::PreTransform()
{
    int h = HSlider->sliderPosition();
    int s = SSlider->sliderPosition();
    int l = LSlider->sliderPosition();
    QImage image = selectedItem_->getItemImage().copy();
    pre_thread->start();
    emit triggerPreview(image, h, s, l);
}

void hslSlider::confirm()
{
    QImage image = pixCard->getCardImage().copy();
    BasePage::setImage2scene(image, scene);
    ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "Success", "执行成功.", 1000, nullptr);
    this->hide();
    this->close();
}

void hslSlider::onslider_changed()
{
    debounceTimer->start(4);
}
