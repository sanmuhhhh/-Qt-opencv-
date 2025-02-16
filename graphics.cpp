#include "Graphics.h"
#include "rgbSlider.h"
#include "hslslider.h"
#include "labslider.h"
#include "contrastbrightnessslider.h"
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaMenu.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"
#include "ElaGraphicsItem.h"
#include "ElaGraphicsView.h"
#include "ElaMessageBar.h"
#include "ElaPushButton.h"
/*--------------------构造函数初始化--------------------*/
Graphics::Graphics(QWidget* parent)
    : BasePage(parent)
{
    setWindowTitle("Graphics");
    createCustomWidget("图形视图\n按住Ctrl进行缩放，按住Shift进行连接,按住鼠标进行拖动\n新生成的图像默认在视图中央最上层");
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Graphics");
    scene = new ElaGraphicsScene(this);
    scene->setSceneRect(0, 0, 2000, 2000);
    view = new ElaGraphicsView(scene);
    view->setScene(scene);
    view->setFixedHeight(600);
    QHBoxLayout* viewLayout = new QHBoxLayout();
    viewLayout->setContentsMargins(0, 0, 12, 0);
    viewLayout->addWidget(view);
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addLayout(viewLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}
 /*--------------------析构函数-------------------*/
Graphics::~Graphics()
{
    delete scene;
}
/** 内容部件生产*/
void Graphics::createCustomWidget(QString desText)
{
QWidget* customWidget = new QWidget(this);
    ElaText* subTitleText = BasePage::setElaText("https://github.com/sanmuhhhh",this,13);
    ElaToolButton* documentationButton =BasePage::setElaTool("文件",35,ElaIconType::FileDoc,this,false);

    ElaMenu* documentationMenu = new ElaMenu(this);
    QAction* openAction = documentationMenu->addElaIconAction(ElaIconType::CardsBlank, "打开");//多线程不在此使用BasePage函数绑定槽函数
    QAction* outputAction = documentationMenu->addElaIconAction(ElaIconType::ArrowUpFromBracket, "导出");
    connect(outputAction,&QAction::triggered,this,&Graphics::OutputImage); //导出操作
    documentationButton->setMenu(documentationMenu);
/*--------------------打开线程开始--------------------*/
/** 打开图片线程操作*/
    connect(openAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg)"));

        if (!fileName.isEmpty()) {
            // 创建全新的线程和任务对象
            QThread* openthread = new QThread();
            mythread* opentask = new mythread();
            opentask->moveToThread(openthread);

            // 线程结束后自动清理
            connect(openthread, &QThread::finished, opentask, &QObject::deleteLater);
            connect(openthread, &QThread::finished, openthread, &QObject::deleteLater);
            connect(opentask, &mythread::taskfinished, openthread, &QThread::quit); //完成后释放线程
            connect(this, &Graphics::filename, opentask, &mythread::openImage_Task);
            connect(opentask, &mythread::loadImage, this, [=](const QImage &image) {
                BasePage::setImage2scene(image, scene);
                ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "Success", "打开成功.", 1000, nullptr);
            });
            openthread->start();
            emit filename(fileName);
        }
    });
/*--------------------打开线程结束--------------------*/
/** 基本算法*/
    ElaToolButton* sourceButton = BasePage::setElaTool("基本图像算法",35,ElaIconType::NfcSymbol,this,false);// 创建算法操作按钮
    ElaMenu* sourceMenu = new ElaMenu(this);
    QAction* GrayAction = sourceMenu->addElaIconAction(ElaIconType::Pipe, "灰度");
    QAction* BlackWhiteAction = sourceMenu->addElaIconAction(ElaIconType::BringForward, "黑白");
    QAction* InvertAction = sourceMenu->addElaIconAction(ElaIconType::BringFront, "黑白反向");
    QAction* IrgbAction = sourceMenu->addElaIconAction(ElaIconType::ArrowsRepeat, "反向");
    sourceButton->setMenu(sourceMenu);  // 将菜单设置为源码按钮的下拉菜单
    connect(GrayAction, &QAction::triggered, this, &Graphics::GrayAction);
    connect(BlackWhiteAction, &QAction::triggered, this, &Graphics::BWAction);
    connect(InvertAction,&QAction::triggered,this,&Graphics::IAction);
/** 色彩空间转换*/
    ElaToolButton* TransformColorSpaceButton = BasePage::setElaTool("色彩空间转换",35,ElaIconType::DiceD4,this,false);
    ElaMenu* TCBMenu = new ElaMenu(this);
    QAction* RGBTransform = TCBMenu->addElaIconAction(ElaIconType::R,"RGB调整");
    QAction* HSLTransform = TCBMenu->addElaIconAction(ElaIconType::H,"HSL调整");
    QAction* LabTransform = TCBMenu->addElaIconAction(ElaIconType::L,"Lab调整");
    QAction* ContrastTransform = TCBMenu->addElaIconAction(ElaIconType::ArrowRightArrowLeft,"对比度调整");
    TransformColorSpaceButton->setMenu(TCBMenu);
    connect(RGBTransform,&QAction::triggered,this,&Graphics::rGBTransform);
    connect(HSLTransform,&QAction::triggered,this,&Graphics::hSLTransform);
    connect(LabTransform,&QAction::triggered,this,&Graphics::LabTransform);
    connect(ContrastTransform,&QAction::triggered,this,&Graphics::contrastTransform);
/** 水下图像增强*/
    ElaToolButton* UnderWaterButton = BasePage::setElaTool("水下图像增强算法",35,ElaIconType::Droplet,this,false);
    ElaMenu* WaterMenu = new ElaMenu(this);
    QAction *CBCC = WaterMenu->addElaIconAction(ElaIconType::CircleC,"CBCC");
    QAction *BackGroundsplit = WaterMenu->addElaIconAction(ElaIconType::Split,"前后景分离");
    QAction *enhance = WaterMenu->addElaIconAction(ElaIconType::Merge,"主成分融合");
    QAction *total = WaterMenu->addElaIconAction(ElaIconType::RightLeftLarge,"基于主成分融合的增强算法");
    UnderWaterButton->setMenu(WaterMenu);

/** 删除与切换主题*/
    ElaPushButton* DeleteButton = new ElaPushButton("删除",this); //创建删除按钮
    DeleteButton->setFixedHeight(35);
    connect(DeleteButton,&ElaPushButton::clicked,this,&Graphics::DeleteItem);
    ElaToolButton* themeButton = new ElaToolButton(this); //创建主题切换按钮
    themeButton->setFixedSize(35, 35);
    themeButton->setIsTransparent(false);
    themeButton->setElaIcon(ElaIconType::MoonStars);
    connect(themeButton, &ElaToolButton::clicked, this, [=]() {
        // 主题切换按钮
        eTheme->setThemeMode(eTheme->getThemeMode() == ElaThemeType::Light ? ElaThemeType::Dark : ElaThemeType::Light);
    });
/** 布局配置*/
    QHBoxLayout* buttonLayout = new QHBoxLayout();// 创建按钮布局
    buttonLayout->addWidget(documentationButton);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(sourceButton);
    buttonLayout->addWidget(TransformColorSpaceButton);
    buttonLayout->addSpacing(15);
    buttonLayout->addWidget(UnderWaterButton);
    buttonLayout->addWidget(DeleteButton);
    buttonLayout->addSpacing(15);
    buttonLayout->addStretch();
    buttonLayout->addWidget(themeButton);
    buttonLayout->addSpacing(15);

    ElaText* descText =BasePage::setElaText(desText,this,13);

    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);// 创建顶部布局
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addLayout(buttonLayout);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);

    setCustomWidget(customWidget);
}
/** 导出*/
void Graphics::OutputImage()
{
    QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();//获取场景中item列表
    if (items.isEmpty()){ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);return;}
    if(items.length()>1){ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "只能导出单个项目.", 1000, nullptr);return;}
    BasePage::showConfirmDialog(
        "导出",
        "确定要导出选中的图像吗？",
        "选择路径",
        [=]() {
            auto item = items.first();
                QImage Image = item->getItemImage().copy();
                if (Image.isNull()) {
                    //qDebug() << "Failed to get image from item";
                    ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "获取图像项目失败!", 1000, nullptr);}
                QString fileName = QFileDialog::getSaveFileName(this, "导出图像", "", "图像文件 (*.png *.jpg *.bmp)");
                if (!fileName.isEmpty()) {
                    // 保存 QImage 到文件
                    if (!Image.save(fileName)) {
                        ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "导出图像项目失败!", 1000, nullptr);return;}
                }
                else{
                    ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "已取消", 1000, nullptr);return;
                }
            ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "Success", "执行成功.", 1000, nullptr);}
        ,this);
}
/** 灰度*/
void Graphics::GrayAction()
{
    QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();
    if (items.isEmpty()){ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);return;}
    BasePage::showConfirmDialog(
        "灰度算法",
        "确定要执行所选的算法吗？",
        "确认灰度化",
        [=]() {
            for (ElaGraphicsItem *item : items)
            {
                if(!item)continue;//防止空指针
                QImage Image = item->getItemImage().copy();
                if (Image.isNull()) {
                    //qDebug() << "Failed to get image from item";
                    ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "获取图像项目失败!", 1000, nullptr);
                    continue;
                }
                // 检查图像是否已经是灰度图像
                if (Image.format() == QImage::Format_Grayscale8 ||
                    Image.format() == QImage::Format_Grayscale16 ||
                    Image.format() == QImage::Format_Indexed8)  {
                    //qDebug() << "Image is already in grayscale.";
                    ElaMessageBar::error(ElaMessageBarType::PositionPolicy::BottomLeft, "Error", "该列表中存在已灰度化图像", 1000, nullptr);
                    continue;
                }
                cv::Mat mat;
                mat = QImage2cvMat(Image);
                cv::Mat grayMat;
                cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
                // 将灰度图像转换回 QImage
                QImage grayImage = cvmat2QImage(grayMat);
                grayImage = grayImage.copy();
                BasePage::setImage2scene(grayImage,scene);
            }
            ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "Success", "执行成功.", 1000, nullptr);}
        ,this//parent
        );
}
/** 黑白*/
void Graphics::BWAction()
{
    QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();//获取场景中item列表
    if (items.isEmpty()){ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);return;}
    BasePage::showConfirmDialog("黑白算法","确认要执行吗","确认黑白化",[=](){
        for (ElaGraphicsItem *item : items)
        {
            if(!item)continue;
            QImage Image = item->getItemImage();
            if (Image.isNull()) {
                ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "获取图像项目失败!", 1000, nullptr);
                continue;
            }
            cv::Mat grayMat;
            if (Image.format() == QImage::Format_Grayscale8 ||
                Image.format() == QImage::Format_Grayscale16 ||
                Image.format() == QImage::Format_Indexed8) {
                grayMat = QImage2cvMat(Image);//cv::Mat(Image.height(), Image.width(), CV_8U, Image.bits(), Image.bytesPerLine()).clone();
            }
            else{
                cv::Mat mat;
                mat = QImage2cvMat(Image);
                cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
            }
            cv::Mat binaryMat;
            double thresh = 127; // 阈值
            double maxval = 255;
            int type = cv::THRESH_BINARY; // 或cv::THRESH_BINARY_INV
            cv::threshold(grayMat, binaryMat, thresh, maxval, type);
            QImage binaryImage = cvmat2QImage(binaryMat);
            binaryImage = binaryImage.copy();
            BasePage::setImage2scene(binaryImage,scene);
        }
    },this);
}
/** 删除*/
void Graphics::DeleteItem()
{
    QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();//获取场景中item列表
    if (items.isEmpty()){ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);return;}
    BasePage::showConfirmDialog("删除选中项目","确认删除吗","确认",[=](){
    scene->removeSelectedItems();
        ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "Success", "删除成功!.", 1000, nullptr);
    },this);
}
/** 反向*/
void Graphics::IAction()
{   QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();//获取场景中item列表
    if (items.isEmpty()){ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);return;}
    BasePage::showConfirmDialog("反向算法","确认要执行吗","确认反向",[=](){
        for (ElaGraphicsItem *item : items)
        {
            if(!item)continue;
            QImage Image = item->getItemImage();
            if (Image.isNull()) {
                ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "获取图像项目失败!", 1000, nullptr);
                continue;
            }
            cv::Mat grayMat;
            if (Image.format() == QImage::Format_Grayscale8 ||
                Image.format() == QImage::Format_Grayscale16 ||
                Image.format() == QImage::Format_Indexed8) {
                grayMat = QImage2cvMat(Image);
            }
            else{
                cv::Mat mat;
                mat = QImage2cvMat(Image);
                cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
            }
            cv::Mat binaryMat;
            double thresh = 127; // 阈值
            double maxval = 255;
            int type = cv::THRESH_BINARY_INV;
            cv::threshold(grayMat, binaryMat, thresh, maxval, type);
            QImage binaryImage = cvmat2QImage(binaryMat);
            binaryImage = binaryImage.copy();
            BasePage::setImage2scene(binaryImage,scene);
        }
    },this);
}
/** 生成rgbPage*/
void Graphics::rGBTransform()
{
    QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();
    if (items.length()>1){
    ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "只能更改一个图片.", 1000, nullptr);
        return;
    }
    if(!items.isEmpty()){

        if(items.first()->getItemImage().format() == QImage::Format_Grayscale8){
            ElaMessageBar::error(ElaMessageBarType::PositionPolicy::TopLeft, "Error", "只能处理彩色图像!", 1000, nullptr);
            return;
        }
        else{
            rgbSlider* RgbPage = new rgbSlider(items.first(),scene);
            RgbPage->setFixedSize(1000, 500);
            RgbPage->moveToCenter();
            RgbPage->show();
        }
    }
    else{
        ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);}
}
/** 生成hslPage*/
void Graphics::hSLTransform()
{
    QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();
    if (items.length()>1)
    {
        ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "只能更改一个图片.", 1000, nullptr);
        return;
    }
    if(!items.isEmpty()){
        if(items.first()->getItemImage().format()==QImage::Format_Grayscale8){
            ElaMessageBar::error(ElaMessageBarType::PositionPolicy::TopLeft, "Error", "只能处理彩色图像!", 1000, nullptr);
            return;}
        else{hslSlider* HslPage = new hslSlider(items.first(),scene);
            HslPage->setFixedSize(1000, 500);
            HslPage->moveToCenter();
            HslPage->show();
        }
    }
    else{
        ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);
    }
}
/** 生成labPage*/
void Graphics::LabTransform()
{

    QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();
    if (items.length()>1)
    {
        ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "只能更改一个图片.", 1000, nullptr);
        return;
    }
    if(!items.isEmpty()){
        LabSlider* LabPage = new LabSlider(items.first(),scene);
        LabPage->setFixedSize(1000, 500);
        LabPage->moveToCenter();
        LabPage->show();
     }
    else{
        ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);
    }
}
/** 生成contrastBrightnessPage*/
void Graphics::contrastTransform()
{
    QList<ElaGraphicsItem*> items;
    items = scene->getSelectedElaItems();
    if (items.length()>1)
    {
        ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "只能更改一个图片.", 1000, nullptr);
        return;
    }
    if(!items.isEmpty()){
        ContrastBrightnessSlider* ContrastBrightnessPage = new ContrastBrightnessSlider(items.first(),scene);
        ContrastBrightnessPage->setFixedSize(1000, 500);
        ContrastBrightnessPage->moveToCenter();
        ContrastBrightnessPage->show();
    }
    else{
        ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::TopLeft, "Warning", "无选中项目.", 1000, nullptr);
    }
}
/** 图像格式转换*/
cv::Mat Graphics::QImage2cvMat(const QImage &image)
{
    cv::Mat mat;
    switch(image.format())
    {
    case QImage::Format_Grayscale8: // 灰度图，每个像素点1个字节（8位）
        // Mat构造：行数，列数，存储结构，数据，step每行多少字节
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_ARGB32: // uint32存储0xAARRGGBB，pc一般小端存储低位在前，所以字节顺序就成了BGRA
    case QImage::Format_RGB32: // Alpha为FF
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888: // RR,GG,BB字节顺序存储
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        // opencv需要转为BGR的字节顺序
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    case QImage::Format_RGBA64: // uint64存储，顺序和Format_ARGB32相反，RGBA
        mat = cv::Mat(image.height(), image.width(), CV_16UC4, (void*)image.constBits(), image.bytesPerLine());
        // opencv需要转为BGRA的字节顺序
        cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA);
        break;
    }
    return mat;
}
QImage Graphics::cvmat2QImage(const cv::Mat &mat)
{
    QImage image;
    switch(mat.type())
    {
    case CV_8UC1:
        // QImage构造：数据，宽度，高度，每行多少字节，存储结构
        image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        break;
    case CV_8UC3:
        image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        image = image.rgbSwapped(); // BRG转为RGB
        // Qt5.14增加了Format_BGR888
        // image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.cols * 3, QImage::Format_BGR888);
        break;
    case CV_8UC4:
        image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        break;
    case CV_16UC4:
        image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGBA64);
        image = image.rgbSwapped(); // BRG转为RGB
        break;
    }
    return image;
}
