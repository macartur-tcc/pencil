#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QTransform>
#include "basemanager.h"


class ViewManagerImpl;
class ViewManager : public BaseManager
{
    Q_OBJECT
public:
    explicit ViewManager(QObject *parent = 0);
    ~ViewManager();
    bool init() override;

    QTransform getView();
    void resetView();

    QPointF mapCanvasToScreen( QPointF p );
    QPointF mapScreenToCanvas( QPointF p );

    QRectF mapCanvasToScreen( const QRectF& rect );
    QRectF mapScreenToCanvas( const QRectF& rect );

    QPainterPath mapCanvasToScreen( const QPainterPath& path );
    QPainterPath mapScreenToCanvas( const QPainterPath& path );

    QPointF translation();
    void translate( float dx, float dy );
    void translate( QPointF offset );

    float rotation();
    void rotate( float degree );

    float scaling();
    void scale( float scaleValue );

    void flipHorizontal( bool b );
    void flipVertical( bool b );
    bool isFlipHorizontal();
    bool isFlipVertical();

    void setCanvasSize( QSize size );

private:
    ViewManagerImpl *m_impl;
};

#endif // VIEWMANAGER_H
