#ifndef RGBSLIDER_H
#define RGBSLIDER_H

#include <ElaWidget.h>
#include <ElaImageCard.h>
#include "ElaGraphicsItem.h"
#include "ElaSlider.h"
#include "Graphics.h"
#include "mythread.h"
#include "QThread"
class rgbSlider : public ElaWidget
{
    Q_OBJECT
public:
    /** 显式调用*/
    explicit rgbSlider(ElaGraphicsItem* selectedItem,ElaGraphicsScene* scene,QWidget* parent = nullptr);//传入图像项目,场景
    ~rgbSlider();
    void setPreview();
    void setPreview(QImage image);
    ElaImageCard* pixCard = nullptr;
/*--------------------槽函数配置--------------------*/
private slots:
    void PreTransform();//预览
    void confirm();//确认
    void onslider_changed();
private:
    ElaSlider* RSlider;
    ElaSlider* GSlider;
    ElaSlider* BSlider;
    ElaGraphicsItem* selectedItem_;
    ElaGraphicsScene* scene;
    mythread* pre_task;//子任务
    QThread* pre_thread;//子线程
    QTimer* debounceTimer;//定时器
signals:
    void triggerPreview(QImage, int, int, int);//传向子线程的信号
};

#endif // RGBSLIDER_H
