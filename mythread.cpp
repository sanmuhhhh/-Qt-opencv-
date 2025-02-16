#include "mythread.h"
#include <QFileDialog>
#include "graphics.h"
mythread::mythread(QObject *parent):QObject(parent)
{
}

mythread::~mythread()
{

}

void mythread::openImage_Task(const QString &fileName) {
    QImage image(fileName);
    if (!image.isNull()) {
        emit loadImage(image);  // 发射信号通知主线程
    }
    emit taskfinished();
}
void mythread::rgb_preview(const QImage &image,int r,int g,int b)
{

    int offset = std::min({r, g, b}); //设置偏移量
    cv::Mat inmat;
    inmat = Graphics::QImage2cvMat(image);
    // cv::Vec3b pixel = inmat.at<cv::Vec3b>(0, 0);
    // qDebug()<<"inmat"<<pixel[2]  <<pixel[1]  <<pixel[0]  ;
    std::vector<cv::Mat> channels; // 分割通道
    cv::split(inmat, channels);
    cv::Mat b_mat = channels[0];
    cv::Mat g_mat = channels[1];
    cv::Mat r_mat = channels[2];
    r -= offset;
    g -= offset;
    b -= offset;
    float rk = 255.0f / (255 - r);
    float gk = 255.0f / (255 - g);
    float bk = 255.0f / (255 - b);
    // qDebug()<< "r:"<<r;
    auto adjust_channels = [](float k, cv::Mat &mat) {
        mat.convertTo(mat, CV_32F);
        mat = mat * k;
        cv::threshold(mat, mat, 255, 255, cv::THRESH_TRUNC);
        mat.convertTo(mat, CV_8U);
    };

    adjust_channels(bk, b_mat);
    adjust_channels(gk, g_mat);
    adjust_channels(rk, r_mat);

    // 合并通道
    std::vector<cv::Mat> merge_channels = {b_mat, g_mat, r_mat};
    cv::Mat outmat;
    cv::merge(merge_channels, outmat);

    QImage out_image = Graphics::cvmat2QImage(outmat);
    emit loadImage(out_image);
}

void mythread::hsl_preview(const QImage &image, int h, int s, int l)
{
    cv::Mat src = Graphics::QImage2cvMat(image);
    cv::cvtColor(src,src,cv::COLOR_BGR2HLS);
    std::vector<cv::Mat> channels;
    cv::split(src,channels);
    cv::Mat lut = createHueLUT(h);
    cv::LUT(channels[0], lut, channels[0]);

    auto affine_channel =[](int k,cv::Mat channel){
        float rake = k/100.0f;
        channel.convertTo(channel,CV_32F);
        channel = channel*(1+rake);
        channel.convertTo(channel,CV_8U);
        return channel;
    };
    channels[1] = affine_channel(l,channels[1]);
    channels[2] = affine_channel(s,channels[2]);

    cv::Mat outmat;
    cv::merge(channels, outmat);
    cv::cvtColor(outmat,outmat,cv::COLOR_HLS2BGR);
    QImage outimage = Graphics::cvmat2QImage(outmat);
    emit loadImage(outimage);
}

void mythread::lab_preview(const QImage &image, int l, int a, int b)
{
    cv::Mat lab_image = Graphics::QImage2cvMat(image);
    cv::cvtColor(lab_image, lab_image, cv::COLOR_BGR2Lab);

    // 生成查找表（每次参数变化时重新生成）
    cv::Mat lut(1, 256, CV_8UC3); // 三通道LUT
    const float Lk = l / 150.0f + 1;
    const float ak = 255.0f / (255 - a);
    const float bk = 255.0f / (255 - b);

    // 并行生成LUT（OpenCV的并行框架）
    cv::parallel_for_(cv::Range(0, 256), [&](const cv::Range& range){
        for(int i = range.start; i < range.end; ++i) {
            int L = cv::saturate_cast<uchar>(i * Lk);
            int a_val = cv::saturate_cast<uchar>(i  * ak);
            int b_val = cv::saturate_cast<uchar>(i * bk);
            lut.at<cv::Vec3b>(0,i) = cv::Vec3b(L, a_val, b_val);
        }
    });

    // 应用LUT
    cv::Mat adjusted_lab;
    cv::LUT(lab_image, lut, adjusted_lab);

    cv::Mat bgr_image;
    cv::cvtColor(adjusted_lab, bgr_image, cv::COLOR_Lab2BGR);
    emit loadImage(Graphics::cvmat2QImage(bgr_image));
}
/**
 * 亮度对比度调整算法
 * 公式：
 *   y = [x - 127.5 * (1 - B)] * k + 127.5 * (1 + B)
 * 其中：
 *   x 输入像素值 (0-255)
 *   y 输出像素值
 *   B 亮度参数，范围[-1, 1]
 *   k 对比度系数，计算方式：
 *     k = tan( (45 + 44 * c) * M_PI / 180.0 )
 *     c 对比度参数，范围[-1, 1]
 */
void mythread::contrastBrightness_preview(const QImage &image, int brightness, int contrast)
{
    brightness = qBound(-100, brightness, 100);
    contrast = qBound(-100, contrast, 100);
    const double B = brightness / 100.0;  // [-1, 1]
    const double c = contrast / 100.0;    // [-1, 1]
    const double radians = (45.0 + 44.0 * c) * M_PI / 180.0;
    const double k = tan(radians);
    cv::Mat lookupTable(1, 256, CV_8UC1);// 创建查找表
    uchar* p = lookupTable.data;
    const double term1 = 127.5 * (1 - B);// 预计算公共项
    const double term2 = 127.5 * (1 + B);

    // 填充查找表
    for (int i = 0; i < 256; ++i) {
        double y = (i - term1) * k + term2;
        p[i] = cv::saturate_cast<uchar>(y);
    }

    cv::Mat srcMat = Graphics::QImage2cvMat(image);
    cv::Mat dstMat;
    // 分通道处理
    if (srcMat.channels() == 3) {
        std::vector<cv::Mat> channels;
        cv::split(srcMat, channels);

        for (auto& channel : channels) {
            cv::LUT(channel, lookupTable, channel);
        }

        cv::merge(channels, dstMat);
    } else {
        cv::LUT(srcMat, lookupTable, dstMat);
    }
    emit loadImage(Graphics::cvmat2QImage(dstMat));
}

cv::Mat mythread::createHueLUT(int adjust)
{
    cv::Mat lut(1, 256, CV_8U);
    for(int i = 0; i < 256; ++i) {
        float val = i + adjust; // 应用调整值

        // 手动实现循环逻辑
        val = val - 180 * std::floor(val / 180.0f);
        val = std::fmax(0.0f, std::fmin(val, 180.0f));

        lut.at<uchar>(i) = static_cast<uchar>(val);
    }
    return lut;
}

