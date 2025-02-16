#ifndef HOME_H
#define HOME_H

#include "basepage.h"
class ElaMenu;
class ElaPromotionView;
class home : public BasePage
{
    Q_OBJECT;
public:
    Q_INVOKABLE explicit home(QWidget *parent = nullptr);
    ~home();


protected:virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    ElaMenu * _homeMenu{nullptr};
    ElaPromotionView* _promotionView{nullptr};
};

#endif // HOME_H
