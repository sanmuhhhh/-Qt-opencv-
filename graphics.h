#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>
#include <QAction>
#include "mythread.h"
#include "BasePage.h"
#include "ElaGraphicsScene.h"
#include <opencv2/opencv.hpp>
#include <QThread>
/*--------------------前向声明--------------------*/
class ElaText;
class ElaToolButton;
class ElaMenu;
class ElaGraphicsScene;
class ElaGraphicsView;
class ElaGraphicsItem;

class Graphics : public BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit Graphics(QWidget* parent = nullptr);
    ~Graphics();
    static cv::Mat QImage2cvMat(const QImage &image);
    static QImage cvmat2QImage(const cv::Mat &mat);
protected:
    void createCustomWidget(QString desText) ;
private slots:
    void OutputImage();
    void GrayAction();
    void BWAction();
    void DeleteItem();
    void IAction();
    void rGBTransform();
    void hSLTransform();
    void LabTransform();
    void contrastTransform();
private:
    ElaGraphicsScene* scene;
    ElaGraphicsView* view;
    mythread* opentask;
    QThread* openthread;
signals:
    void filename(QString);
};

#endif // GRAPHICS_H
