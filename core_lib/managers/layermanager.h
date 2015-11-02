#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H

#include "basemanager.h"

class Layer;
class LayerManagerImpl;

class LayerManager : public BaseManager
{
    Q_OBJECT

public:
    LayerManager( QObject* pParant );
    ~LayerManager();
    bool init() override;

    // Layer Management
    Layer* currentLayer();
    Layer* currentLayer( int offset );
    int    currentLayerIndex();
    void   setCurrentLayer( int );
    int    count();

    bool   deleteCurrentLayer();

    void gotoNextLayer();
    void gotoPreviouslayer();

    // KeyFrame Management
    int  LastFrameAtFrame( int frameIndex );
    int  firstKeyFrameIndex();
    int  lastKeyFrameIndex();

    void gotoLastKeyFrame();
    void gotoFirstKeyFrame();

    int  projectLength();

Q_SIGNALS:
    void currentLayerChanged( int n );
    void layerCountChanged( int count );

private:
    LayerManagerImpl * m_impl;
};

#endif
