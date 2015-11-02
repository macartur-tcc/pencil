#include "viewmanager.h"
#include <utility>
#include "object.h"



class ViewManagerImpl
{
public:
    ViewManagerImpl(float rotate = 0.f, float scale = 1.f ,bool isFlipHorizontal = false ,bool isFlipVertical = false, QSize canvas_size = {1, 1});
    QTransform createViewTransform();
    float mRotate;
    float mScale;

    bool mIsFlipHorizontal;
    bool mIsFlipVertical;

    QSize mCanvasSize;

    QTransform mView;
    QPointF mTranslate;
};

ViewManagerImpl::ViewManagerImpl(float rotate, float scale,bool isFlipHorizontal,bool isFlipVertical, QSize canvas_size)
  : mRotate(rotate), mScale(scale), mIsFlipHorizontal(isFlipHorizontal),
    mIsFlipVertical(isFlipVertical), mCanvasSize(canvas_size)
{
}

QTransform ViewManagerImpl::createViewTransform()
{
    QTransform t;
    t.translate( this->mCanvasSize.width() / 2.f , this->mCanvasSize.height() / 2.f );
    t.translate( this->mTranslate.x(), this->mTranslate.y() );
    t.scale( this->mScale, this->mScale );
    t.rotate( this->mRotate );

    return std::move( t );
}

ViewManager::ViewManager(QObject *parent)
  :BaseManager(parent), m_impl(new ViewManagerImpl())
{
}

ViewManager::~ViewManager()
{
  delete m_impl;
}

bool ViewManager::init()
{
    return true;
}

QPointF ViewManager::mapCanvasToScreen( QPointF p )
{
    return this->m_impl->mView.map( p );
}

QPointF ViewManager::mapScreenToCanvas(QPointF p)
{
    return this->m_impl->mView.inverted().map( p );
}

QPainterPath ViewManager::mapCanvasToScreen( const QPainterPath& path )
{
    return this->m_impl->mView.map( path );
}

QRectF ViewManager::mapCanvasToScreen( const QRectF& rect )
{
    return std::move( this->m_impl->mView.mapRect( rect ) );
}

QRectF ViewManager::mapScreenToCanvas( const QRectF& rect )
{
    return std::move( this->m_impl->mView.inverted().mapRect( rect ) );
}

QPainterPath ViewManager::mapScreenToCanvas( const QPainterPath& path )
{
    return this->m_impl->mView.inverted().map( path );
}

QTransform ViewManager::getView()
{
    return this->m_impl->mView;
}


void ViewManager::translate(float dx, float dy)
{
    this->m_impl->mTranslate += QPointF( dx, dy );
    this->m_impl->mView = this->m_impl->createViewTransform();
}

void ViewManager::translate(QPointF offset)
{
    translate( offset.x(), offset.y() );
}


QPointF ViewManager::translation()
{
 return this->m_impl->mTranslate;
}

float ViewManager::rotation()
{
 return this->m_impl->mRotate;
}

void ViewManager::rotate(float degree)
{
    this->m_impl->mRotate += degree;
    this->m_impl->mView = this->m_impl->createViewTransform();
}

float ViewManager::scaling()
{
 return this->m_impl->mScale;
}

void ViewManager::scale(float scaleValue)
{
    this->m_impl->mScale *= scaleValue;
    this->m_impl->mView = this->m_impl->createViewTransform();
}

void ViewManager::flipHorizontal( bool b )
{
 this->m_impl->mIsFlipHorizontal = b;
}

void ViewManager::flipVertical( bool b )
{
 this->m_impl->mIsFlipVertical = b;
}

bool ViewManager::isFlipHorizontal()
{
 return this->m_impl->mIsFlipHorizontal;
}

bool ViewManager::isFlipVertical()
{
 return this->m_impl->mIsFlipVertical;
}

void ViewManager::setCanvasSize(QSize size)
{
    this->m_impl->mCanvasSize = size;
    this->m_impl->mView = this->m_impl->createViewTransform();
}

void ViewManager::resetView()
{
    this->m_impl->mView = QTransform();
}
