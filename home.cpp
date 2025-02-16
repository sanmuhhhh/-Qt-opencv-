#include "home.h"

#include <QDebug>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

#include "ElaImageCard.h"
#include "ElaMenu.h"
#include "ElaMessageBar.h"
#include "ElaNavigationRouter.h"
#include "ElaScrollArea.h"
#include "ElaText.h"
#include "ElaPromotionCard.h"
#include "ElaPromotionView.h"
#include "ElaAcrylicUrlCard.h"
#include "ElaToolTip.h"
home::home(QWidget *parent)
    :BasePage(parent)
{
    setWindowTitle("Home");
    setTitleVisible(false);
    setContentsMargins(2,2,0,0);
/*--------------------界面内容配置--------------------*/
    ElaText * titleText = setElaText("Sammu_Graphics",this,35);
    titleText->setTextStyle(ElaTextType::Title);
    titleText->setStyleSheet("color:rgba(255,255,255,1);");
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setContentsMargins(30, 60, 0, 0);
    titleLayout->addWidget(titleText);

    ElaImageCard* backgroundCard = new ElaImageCard(this);//卡片背景
    backgroundCard->setBorderRadius(15);
    backgroundCard->setFixedHeight(400);
    backgroundCard->setMaximumAspectRatio(1.7);
    backgroundCard->setCardImage(QImage(":/include/Image/scene.png"));
/** url*/
    ElaAcrylicUrlCard* urlCard = new ElaAcrylicUrlCard(this);
    urlCard->setCardPixmapSize(QSize(62,62));
    urlCard->setTitlePixelSize(17);
    urlCard->setTitleSpacing(25);
    urlCard->setSubTitleSpacing(13);
    urlCard->setUrl("https://github.com/sanmuhhhh");
    urlCard->setCardPixmap(QPixmap(":/include/Image/icons8-github-50.png"));
    urlCard->setTitle("Sanmu Github");
    urlCard->setSubTitle("Graphics");
    urlCard->setBorderRadius(1);
    //urlCard->setBrushAlpha(4);
    ElaToolTip* urlCard1ToolTip = new ElaToolTip(urlCard);
    urlCard1ToolTip->setToolTip("https://github.com/sanmuhhhh");

    ElaAcrylicUrlCard* urlCard1 = new ElaAcrylicUrlCard(this);
    urlCard1->setCardPixmapSize(QSize(62,62));
    urlCard1->setTitlePixelSize(17);
    urlCard1->setTitleSpacing(25);
    urlCard1->setSubTitleSpacing(13);
    urlCard1->setUrl("https://space.bilibili.com/172342555?spm_id_from=333.1007.0.0");
    urlCard1->setCardPixmap(QPixmap(":/include/Image/icons8-胆汁-50.png"));
    urlCard1->setTitle("Sanmu Bilibili");
    urlCard1->setSubTitle("Graphics");
    //urlCard1->setBrushAlpha(4);
    urlCard1->setBorderRadius(1);
    ElaToolTip* urlCard1ToolTip1 = new ElaToolTip(urlCard1);
    urlCard1ToolTip1->setToolTip("https://bilibili.com");
    QHBoxLayout* urlCardLayout = new QHBoxLayout();
    urlCardLayout->setSpacing(15);
    urlCardLayout->setContentsMargins(30, 0, 0, 6);
    urlCardLayout->addWidget(urlCard);
    urlCardLayout->addWidget(urlCard1);
    urlCardLayout->addStretch();

/** 卡片内容配置*/

    QColor back2Color(248,170,220,255);
    _promotionView = new ElaPromotionView(this);
    ElaPromotionCard *card1 = new ElaPromotionCard(this);
    card1->setFixedSize(600,300);
    card1->setCardPixmap(QPixmap(":/include/Image/moring.png"));
    card1->setCardTitle("Sparrowfart");
    card1->setSubTitle("风把我不想知道的事情告诉我");
    card1->setSubTitleColor(back2Color);
    ElaPromotionCard *card2 = new ElaPromotionCard(this);
    card2->setFixedSize(600,300);
    card2->setCardPixmap(QPixmap(":/include/Image/afternoon.png"));
    card2->setCardTitle("Nightfall");
    card2->setSubTitle("河把我不想留下的故事也带走");
    ElaPromotionCard *card3 = new ElaPromotionCard(this);
    card3->setFixedSize(600,300);
    card3->setCardPixmap(QPixmap(":/include/Image/city.jpg"));
    card3->setCardTitle("City");
    card3->setSubTitle("我摘下我的翅膀");
    ElaPromotionCard *card4 = new ElaPromotionCard(this);
    card4->setFixedSize(600,300);
    card4->setCardPixmap(QPixmap(":/include/Image/plain.png"));
    card4->setCardTitle("Plain");
    card4->setSubTitle("它变成白鸟");
    _promotionView->appendPromotionCard(card1);
    _promotionView->appendPromotionCard(card2);
    _promotionView->appendPromotionCard(card3);
    _promotionView->appendPromotionCard(card4);
    ElaScrollArea *cardScrollArea = new ElaScrollArea(this);//滚动区域
    cardScrollArea->setWidgetResizable(true);
    cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setIsGrabGesture(true, 0);
    cardScrollArea->setIsOverShoot(Qt::Horizontal, true);

    QWidget* cardScrollAreaWidget = new QWidget(this);//内容部件
    cardScrollAreaWidget->setStyleSheet("background-color:transparent;");
    cardScrollArea->setWidget(cardScrollAreaWidget);
 /** 布局配置 */

    QVBoxLayout* cardScrollAreaWidgetLayout = new QVBoxLayout(cardScrollAreaWidget);
    cardScrollAreaWidgetLayout->setContentsMargins(0, 0, 0, 0);
    cardScrollAreaWidgetLayout->addStretch();
    //cardScrollAreaWidgetLayout->addLayout(urlCardLayout);


    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundCard);
    backgroundLayout->setContentsMargins(0, 0, 0, 0);
    backgroundLayout->addLayout(titleLayout);
    backgroundLayout->addWidget(cardScrollArea);
    _homeMenu = new ElaMenu(this);// 右键菜单
    ElaMenu* checkMenu = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu->addAction("查看1");
    checkMenu->addAction("查看2");
    checkMenu->addAction("查看3");
    checkMenu->addAction("查看4");

    ElaMenu* checkMenu1 = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu1->addAction("查看1");
    checkMenu1->addAction("查看2");
    checkMenu1->addAction("查看3");
    checkMenu1->addAction("查看4");

    ElaMenu* checkMenu2 = checkMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu2->addAction("查看1");
    checkMenu2->addAction("查看2");
    checkMenu2->addAction("查看3");
    checkMenu2->addAction("查看4");

    // QKeySequence key = QKeySequence(Qt::CTRL | Qt::Key_S);

    _homeMenu->addSeparator();
    _homeMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Save);
    _homeMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    QAction* action = _homeMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    connect(action, &QAction::triggered, this, [=]() {
        ElaNavigationRouter::getInstance()->navigationRouteBack();
    });

    _homeMenu->addElaIconAction(ElaIconType::Copy, "复制");
    _homeMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Home");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(backgroundCard);
    centerVLayout->addWidget(_promotionView);
    centerVLayout->addStretch();
    centerVLayout->addLayout(urlCardLayout);
    addCentralWidget(centralWidget);
    ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success", "初始化成功!", 2000);
    qDebug() << "初始化成功";   // 初始化提示

}

home::~home()
{

}
/** 鼠标事件*/
void home::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::RightButton:
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        _homeMenu->popup(event->globalPosition().toPoint());
#else
        _homeMenu->popup(event->globalPos());
#endif
        break;
    }
    default:
    {
        break;
    }
    }
    ElaScrollPage::mouseReleaseEvent(event);
}
