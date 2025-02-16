#include "labslider.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>
#include <QTimer>
#include "ElaImageCard.h"
#include "ElaText.h"
#include "ElaMessageBar.h"
#include "ElaPushButton.h"

LabSlider::LabSlider(ElaGraphicsItem* selectedItem, ElaGraphicsScene* scene, QWidget* parent)
    : ElaWidget(parent), selectedItem_(selectedItem), scene(scene)
{
    setWindowTitle("");
    setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    pixCard = new ElaImageCard(this);
    pixCard->setIsPreserveAspectCrop(false);
    pixCard->backgroundRole();

    // 创建Lab通道标签布局
    QHBoxLayout* LLayout = BasePage::setElaHBoxLayout(
        BasePage::setElaText("明度", this, 18), BasePage::setElaText("", this, 18), nullptr);
    QHBoxLayout* aLayout = BasePage::setElaHBoxLayout(
        BasePage::setElaText("A轴(绿-红)", this, 18), BasePage::setElaText("", this, 18), nullptr);
    QHBoxLayout* bLayout = BasePage::setElaHBoxLayout(
        BasePage::setElaText("B轴(蓝-黄)", this, 18), BasePage::setElaText("", this, 18), nullptr);

    QVBoxLayout* pixCardLayout = new QVBoxLayout();
    ElaText* pretext = BasePage::setElaText("预览", this, 18, true);
    pixCardLayout->addWidget(pretext);
    pixCardLayout->addWidget(pixCard);
    pixCardLayout->addStretch();

    // 滑块初始化（L:0-200对应常规L*范围，a/b:-100-100）
    LSlider = BasePage::setElaSlider([this](){this->onslider_changed();}, -100, 100, 0, this);
    aSlider = BasePage::setElaSlider([this](){this->onslider_changed();}, -100, 100, 0, this);
    bSlider = BasePage::setElaSlider([this](){this->onslider_changed();}, -100, 100, 0, this);

    // 按钮初始化
    ElaPushButton* ConfirmButton = BasePage::setElaPushButton("确认", this);
    ElaPushButton* CancelButton = BasePage::setElaPushButton("取消", this);

    QHBoxLayout* ButtonLayout = new QHBoxLayout();
    ButtonLayout->addWidget(ConfirmButton);
    ButtonLayout->addSpacing(120);
    ButtonLayout->addWidget(CancelButton);
    ButtonLayout->addSpacing(20);

    connect(CancelButton, &ElaPushButton::clicked, this, &LabSlider::close);
    connect(ConfirmButton, &ElaPushButton::clicked, this, &LabSlider::confirm);

    QVBoxLayout* SlideLayout = new QVBoxLayout();
    auto initSliderLayout = [SlideLayout](QHBoxLayout* layout, ElaSlider* slider) {
        SlideLayout->addLayout(layout);
        SlideLayout->addWidget(slider);
        SlideLayout->addStretch();
    };

    initSliderLayout(LLayout, LSlider);
    initSliderLayout(aLayout, aSlider);
    initSliderLayout(bLayout, bSlider);

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
    connect(debounceTimer, &QTimer::timeout, this, &LabSlider::PreTransform);

    pre_task = new mythread();
    pre_thread = new QThread(this);
    pre_task->moveToThread(pre_thread);
    connect(pre_thread, &QThread::finished, pre_task, &QObject::deleteLater);
    connect(pre_task, &mythread::loadImage, [=](const QImage& image) {
        setPreview(image);
    });
    connect(this, &LabSlider::triggerPreview, pre_task, &mythread::lab_preview);
}

LabSlider::~LabSlider()
{
    delete LSlider;
    delete aSlider;
    delete bSlider;
    delete pixCard;
    if (pre_thread && pre_thread->isRunning()) {
        pre_thread->quit();
        pre_thread->wait();
    }
    delete pre_thread;
    delete debounceTimer;
}

void LabSlider::setPreview()
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

void LabSlider::setPreview(QImage image)
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

void LabSlider::PreTransform()
{
    int l = LSlider->sliderPosition();
    int a = aSlider->sliderPosition();
    int b = bSlider->sliderPosition();
    QImage image = selectedItem_->getItemImage().copy();
    pre_thread->start();
    emit triggerPreview(image, l, a, b);
}

void LabSlider::confirm()
{
    QImage image = pixCard->getCardImage().copy();
    BasePage::setImage2scene(image, scene);
    ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "Success", "执行成功.", 1000, nullptr);
    this->hide();
    this->close();
}

void LabSlider::onslider_changed()
{
    debounceTimer->start(4);
}
