#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QObject>
#include <opencv2/opencv.hpp>
class mythread :public QObject
{
    Q_OBJECT
public:
   explicit mythread(QObject* parent = nullptr);
    ~mythread();
public slots:
   void openImage_Task(const QString &fileName);
   void rgb_preview(const QImage& image, int r, int g, int b);
   void hsl_preview(const QImage& image,int h,int s,int l);
   void lab_preview(const QImage& image,int l,int a,int b);
   void contrastBrightness_preview(const QImage& image, int brightness, int contrast);
private:
    cv::Mat createHueLUT(int adjust);// 创建色相调整查找表
signals:
    void loadImage(const QImage& image);
    void taskfinished();
};

#endif // MYTHREAD_H
