#include "object.h"
#include "editor.h"
#include "layerimage.h"
#include "layermanager.h"

class LayerManagerImpl
{
public:
    LayerManagerImpl();
    void set_CurrentLayerIndex(int index);
    int get_CurrentLayerIndex();
private:
    int mCurrentLayerIndex = 0; // the current layer to be edited/displayed
};

LayerManagerImpl::LayerManagerImpl()
  : mCurrentLayerIndex(0)
{
}
void LayerManagerImpl::set_CurrentLayerIndex(int index)
{
  this->mCurrentLayerIndex = index;
}

int LayerManagerImpl::get_CurrentLayerIndex()
{
  return this->mCurrentLayerIndex;
}

LayerManager::LayerManager( QObject* pParent )
    : BaseManager( pParent ), m_impl(new LayerManagerImpl())
{
}

LayerManager::~LayerManager()
{
}

bool LayerManager::init()
{
    return true;
}

// Layer management
Layer* LayerManager::currentLayer()
{
    return currentLayer( 0 );
}

Layer* LayerManager::currentLayer( int incr )
{
    Q_ASSERT( editor()->object() != NULL );

    return editor()->object()->getLayer( this->m_impl->get_CurrentLayerIndex() + incr );
}

int LayerManager::currentLayerIndex()
{
    return this->m_impl->get_CurrentLayerIndex();
}

void LayerManager::setCurrentLayer( int layerIndex )
{
	if ( this->m_impl->get_CurrentLayerIndex() != layerIndex )
	{
    this->m_impl->set_CurrentLayerIndex( layerIndex );
		Q_EMIT currentLayerChanged( this->m_impl->get_CurrentLayerIndex() );
	}
}

void LayerManager::gotoNextLayer()
{
    if ( this->m_impl->get_CurrentLayerIndex() < editor()->object()->getLayerCount() - 1 )
    {
        this->m_impl->set_CurrentLayerIndex( this->m_impl->get_CurrentLayerIndex() + 1);
    		emit currentLayerChanged( this->m_impl->get_CurrentLayerIndex() );
    }
}

void LayerManager::gotoPreviouslayer()
{
    if ( this->m_impl->get_CurrentLayerIndex() > 0 )
    {
        this->m_impl->set_CurrentLayerIndex( this->m_impl->get_CurrentLayerIndex() - 1);
		    emit currentLayerChanged( this->m_impl->get_CurrentLayerIndex() );
    }
}

int LayerManager::LastFrameAtFrame( int frameIndex )
{
    Object* pObj = editor()->object();
    for ( int i = frameIndex; i >= 0; i -= 1 )
    {
        for ( int layerIndex = 0; layerIndex < pObj->getLayerCount(); ++layerIndex )
        {
            auto pLayer = pObj->getLayer( layerIndex );
            if ( pLayer->keyExists( i ) )
            {
                return i;
            }
        }
    }
    return -1;
}

int LayerManager::firstKeyFrameIndex()
{
    int minPosition = INT_MAX;

    Object* pObj = editor()->object();
    for ( int i = 0; i < pObj->getLayerCount(); ++i )
    {
        Layer* pLayer = pObj->getLayer( i );

        int position = pLayer->firstKeyFramePosition();
        if ( position < minPosition )
        {
            minPosition = position;
        }
    }
    return minPosition;
}

int LayerManager::lastKeyFrameIndex()
{
    int maxPosition = 0;

    for ( int i = 0; i < editor()->object()->getLayerCount(); ++i )
    {
        Layer* pLayer = editor()->object()->getLayer( i );

        int position = pLayer->getMaxKeyFramePosition();
        if ( position > maxPosition )
        {
            maxPosition = position;
        }
    }
    return maxPosition;
}

int LayerManager::count()
{
    return editor()->object()->getLayerCount();
}

bool LayerManager::deleteCurrentLayer()
{
    if ( currentLayer()->type() == Layer::CAMERA )
    {
        return false;
    }

    editor()->object()->deleteLayer( currentLayerIndex() );

    if ( currentLayerIndex() == editor()->object()->getLayerCount() )
    {
        setCurrentLayer( currentLayerIndex() - 1 );
    }
    emit editor()->updateAllFrames();
    emit layerCountChanged( count() );

    return true;
}

int LayerManager::projectLength()
{
    int maxFrame = -1;

    Object* pObject = editor()->object();
    for ( int i = 0; i < pObject->getLayerCount(); i++ )
    {
        int frame = pObject->getLayer( i )->getMaxKeyFramePosition();
        if ( frame > maxFrame )
        {
            maxFrame = frame;
        }
    }
    return maxFrame;
}

void LayerManager::gotoLastKeyFrame()
{
    int nFrame = lastKeyFrameIndex();
    editor()->scrubTo( nFrame );
}

void LayerManager::gotoFirstKeyFrame()
{
    int nFrame = firstKeyFrameIndex();
    editor()->scrubTo( nFrame );
}
