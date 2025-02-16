#include "rgbSlider.h"

/*-------------------------------- 基础库包含 --------------------------------*/
#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "ElaImageCard.h"
#include "ElaText.h"
#include "ElaMessageBar.h"
#include "ElaPushButton.h"
rgbSlider::rgbSlider(ElaGraphicsItem* selectedItem,ElaGraphicsScene* scene,QWidget* parent)
    : ElaWidget(parent), selectedItem_(selectedItem),scene(scene)
{

    setWindowTitle("");
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);  // 仅显示关闭按钮

    pixCard = new ElaImageCard(this);
    pixCard->setIsPreserveAspectCrop(false);
    pixCard->backgroundRole();

    /*-------------------------------- 控件生成 --------------------------------*/

    // 创建三组颜色标签布局
    QHBoxLayout* RedLayout = BasePage::setElaHBoxLayout(BasePage::setElaText("青色",this,18),BasePage::setElaText("红色",this,18),nullptr);
    QHBoxLayout* GreenLayout = BasePage::setElaHBoxLayout(BasePage::setElaText("洋红",this,18),BasePage::setElaText("绿色",this,18),nullptr);
    QHBoxLayout* BlueLayout = BasePage::setElaHBoxLayout(BasePage::setElaText("黄色",this,18),BasePage::setElaText("蓝色",this,18),nullptr);

    QVBoxLayout* pixCardLayout = new QVBoxLayout();
    ElaText * pretext = BasePage::setElaText("预览",this,18,true);
    pixCardLayout->addWidget(pretext);
    pixCardLayout->addWidget(pixCard);
    pixCardLayout->addStretch();
/** 滑块*/
    RSlider = BasePage::setElaSlider([this](){this->onslider_changed();},-100,100,0,this);
    GSlider = BasePage::setElaSlider([this](){this->onslider_changed();},-100,100,0,this);
    BSlider = BasePage::setElaSlider([this](){this->onslider_changed();},-100,100,0,this);
/** 按钮*/
    ElaPushButton* ConfirmButton = BasePage::setElaPushButton("确认",this);
    ElaPushButton* CancelButton  = BasePage::setElaPushButton("取消",this);

    /*-------------------------------- 布局配置 --------------------------------*/
    QHBoxLayout* ButtonLayout = new QHBoxLayout();
    ButtonLayout->addWidget(ConfirmButton);
    ButtonLayout->addSpacing(120);
    ButtonLayout->addWidget(CancelButton);
    ButtonLayout->addSpacing(20);

    connect(CancelButton,&ElaPushButton::clicked,this,&rgbSlider::close);
    connect(ConfirmButton,&ElaPushButton::clicked,this,&rgbSlider::confirm);

    QVBoxLayout* SlideLayout = new QVBoxLayout();
    /** lambda实现布局组装函数
        * 参数顺序：标签布局，对应滑块
        * addStretch()添加弹性分隔空间 */
    auto initSliderLayout = [SlideLayout](QHBoxLayout* layout,ElaSlider* slider){
        SlideLayout->addLayout(layout);
        SlideLayout->addWidget(slider);
        SlideLayout->addStretch();
    };


    initSliderLayout(RedLayout,RSlider);   // 组装三通道布局
    initSliderLayout(GreenLayout,GSlider);
    initSliderLayout(BlueLayout,BSlider);

    // 添加底部按钮布局
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
/** 处理线程与定时器*/
    // 使用定时器合并连续请求
    debounceTimer = new QTimer(this);
    debounceTimer->setSingleShot(true);
    connect(debounceTimer, &QTimer::timeout, this, &rgbSlider::PreTransform);
    pre_task = new mythread();
    pre_thread = new QThread(this);
    pre_task->moveToThread(pre_thread);
    connect(pre_thread,&QThread::finished,pre_task,&QObject::deleteLater);
    connect(pre_task,&mythread::loadImage,[=](const QImage& image){
        setPreview(image);
    });
    connect(this, &rgbSlider::triggerPreview, pre_task, &mythread::rgb_preview);
}

/*-------------------------------- 析构函数 --------------------------------*/
rgbSlider::~rgbSlider()
{
    delete RSlider;
    delete GSlider;
    delete BSlider;
    delete pixCard;
    if(pre_thread && pre_thread->isRunning()){
        pre_thread->quit();
        pre_thread->wait();
    }
    delete pre_thread;
    delete debounceTimer;
}

/** 预览*/
void rgbSlider::setPreview()
{

    QImage image =  selectedItem_->getItemImage();
    int width = image.width(),height=image.height();
    while(width>800||height>400){
        width = width/2;
        height = height/2;
    }
    pixCard->setFixedSize(width,height);
    pixCard->setCardImage(image);
    pixCard->update();
}
/** 带参预览*/
void rgbSlider::setPreview(QImage image)
{
    QImage Pimage = image.copy();
    int width = image.width(),height=image.height();
    while(width>800||height>400){
        width = width/2;
        height = height/2;
    }
    pixCard->setFixedSize(width,height);
    pixCard->setCardImage(Pimage);
    pixCard->update();
}
/** 色彩平衡算法与预览图更新*/
void rgbSlider::PreTransform()
{
    int r = RSlider->sliderPosition();
    int g = GSlider->sliderPosition();
    int b = BSlider->sliderPosition();
    QImage image = selectedItem_->getItemImage().copy();
    pre_thread->start();
    emit triggerPreview(image,r,g,b);
}
/** 确认处理*/
void rgbSlider::confirm()
{
    QImage image = pixCard->getCardImage().copy();
    BasePage::setImage2scene(image,scene);
    ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "Success", "执行成功.", 1000, nullptr);
    this->hide();
    this->close();
}
/** 定时任务*/
void rgbSlider::onslider_changed()
{
    debounceTimer->start(4); // 4ms内仅触发最后一次
}


