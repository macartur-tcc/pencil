#ifndef PLAYBACKMANAGER_H
#define PLAYBACKMANAGER_H

#include "basemanager.h"

class QTimer;

class PlaybackManagerImpl;
class PlaybackManager : public BaseManager
{
    Q_OBJECT
public:
    explicit PlaybackManager(QObject* parent);

    bool init() override;

    bool isPlaying();
    bool isLooping();

    void play();
    void stop();

    int  fps();
    int startFrame();
    int endFrame();

    void setFps( int fps );
    void setLooping( bool isLoop );
    void enableRangedPlayback( bool b );
    void setRangedStartFrame( int frame );
    void setRangedEndFrame( int frame );
    void enbaleSound( bool b );

Q_SIGNALS:
    void fpsChanged( int fps );
    void loopStateChanged( bool b );
    void rangedPlaybackStateChanged( bool b );

private:
    void timerTick();
    PlaybackManagerImpl * m_impl;
};

#endif // PLAYBACKMANAGER_H
