#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QColor>
#include "basemanager.h"


class ColorManagerImpl;
class ColorManager : public BaseManager
{
    Q_OBJECT
public:
    ColorManager( QObject* parent );
    ~ColorManager();
    bool init() override;

    QColor frontColor();
    int frontColorNumber();
    void setColor(const QColor& color);
    void setColorNumber( int n );

Q_SIGNALS:
    void colorChanged(QColor);
    void colorNumberChanged(int);

private:
    ColorManagerImpl *m_impl;
};

#endif // COLORMANAGER_H
