#ifndef LABSLIDER_H
#define LABSLIDER_H

#include <ElaWidget.h>
#include <ElaImageCard.h>
#include "ElaGraphicsItem.h"
#include "ElaSlider.h"
#include "Graphics.h"
#include "mythread.h"
#include "QThread"

class LabSlider : public ElaWidget
{
    Q_OBJECT
public:
    explicit LabSlider(ElaGraphicsItem* selectedItem, ElaGraphicsScene* scene, QWidget* parent = nullptr);
    ~LabSlider();

    void setPreview();
    void setPreview(QImage image);
    ElaImageCard* pixCard = nullptr;

private slots:
    void PreTransform();
    void confirm();
    void onslider_changed();

private:
    ElaSlider* LSlider;
    ElaSlider* aSlider;
    ElaSlider* bSlider;
    ElaGraphicsItem* selectedItem_;
    ElaGraphicsScene* scene;
    mythread* pre_task;
    QThread* pre_thread;
    QTimer* debounceTimer;

signals:
    void triggerPreview(QImage, int, int, int);
};

#endif // LABSLIDER_H
