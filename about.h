#ifndef ABOUT_H
#define ABOUT_H

#include <ElaWidget.h>

class About : public ElaWidget
{
    Q_OBJECT
public:
    explicit About(QWidget* parent = nullptr);
    ~About();
};

#endif // ABOUT_H
