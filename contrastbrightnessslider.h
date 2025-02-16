#ifndef CONTRASTBRIGHTNESSSLIDER_H
#define CONTRASTBRIGHTNESSSLIDER_H

#include <ElaWidget.h>
#include <ElaImageCard.h>
#include "ElaGraphicsItem.h"
#include "ElaSlider.h"
#include "Graphics.h"
#include "mythread.h"
#include "QThread"

class ContrastBrightnessSlider : public ElaWidget
{
    Q_OBJECT
public:
    explicit ContrastBrightnessSlider(ElaGraphicsItem* selectedItem, ElaGraphicsScene* scene, QWidget* parent = nullptr);
    ~ContrastBrightnessSlider();

    void setPreview();
    void setPreview(QImage image);
    ElaImageCard* pixCard = nullptr;

private slots:
    void PreTransform();
    void confirm();
    void onslider_changed();

private:
    ElaSlider* brightnessSlider;  // 亮度滑块
    ElaSlider* contrastSlider;    // 对比度滑块
    ElaGraphicsItem* selectedItem_;
    ElaGraphicsScene* scene;
    mythread* pre_task;
    QThread* pre_thread;
    QTimer* debounceTimer;

signals:
    void triggerPreview(QImage, int, int);  // 参数顺序：亮度、对比度
};

#endif // CONTRASTBRIGHTNESSSLIDER_H
