#ifndef HSLSLIDER_H
#define HSLSLIDER_H

#include <ElaWidget.h>
#include <ElaImageCard.h>
#include "ElaGraphicsItem.h"
#include "ElaSlider.h"
#include "Graphics.h"
#include "mythread.h"
#include "QThread"

class hslSlider : public ElaWidget
{
    Q_OBJECT
public:
    explicit hslSlider(ElaGraphicsItem* selectedItem, ElaGraphicsScene* scene, QWidget* parent = nullptr);
    ~hslSlider();

    void setPreview();
    void setPreview(QImage image);
    ElaImageCard* pixCard = nullptr;

private slots:
    void PreTransform();
    void confirm();
    void onslider_changed();

private:
    ElaSlider* HSlider;
    ElaSlider* SSlider;
    ElaSlider* LSlider;
    ElaGraphicsItem* selectedItem_;
    ElaGraphicsScene* scene;
    mythread* pre_task;
    QThread* pre_thread;
    QTimer* debounceTimer;

signals:
    void triggerPreview(QImage, int, int, int);
};

#endif // HSLSLIDER_H
