
#include <QTimer>
#include "editor.h"
#include "layermanager.h"
#include "playbackmanager.h"

class PlaybackManagerImpl
{
public:
    PlaybackManagerImpl(QTimer* QTimer = nullptr,
                        bool isLooping = false,
                        bool isPlaying = false,
                        bool isRangedPlayBack = false,
                        bool isPlayingSound = false,
                        int startFrame = 1,
                        int endFrame = 60,
                        int markInFrame = 1,
                        int markOutFrame = 10,
                        int fps = 12
                        );
    bool m_isLooping;
    bool m_isPlaying;
    bool m_isPlaySound;
    int mStartFrame;
    int mEndFrame;

    bool m_isRangedPlayback;
    int m_markInFrame;
    int m_markOutFrame;

    int m_fps;

    QTimer* m_pTimer;
};

PlaybackManagerImpl::PlaybackManagerImpl(QTimer* QTimer, bool isLooping,
bool isPlaying, bool isRangedPlayBack, bool isPlayingSound,
int startFrame, int endFrame, int markInFrame, int markOutFrame,
int fps)
  : m_isLooping(isLooping),m_isPlaying(isPlaying),
    m_isPlaySound(isPlayingSound),mStartFrame(startFrame),mEndFrame(endFrame),
    m_isRangedPlayback(isRangedPlayBack),
    m_markInFrame(markInFrame), m_markOutFrame(markOutFrame), m_fps(fps),
    m_pTimer(QTimer)
{
}



PlaybackManager::PlaybackManager( QObject* parent ) : BaseManager( parent )
{
}

bool PlaybackManager::init()
{
    this->m_impl = new PlaybackManagerImpl(new QTimer(this));
    connect( this->m_impl->m_pTimer, &QTimer::timeout, this, &PlaybackManager::timerTick );
    return true;
}

bool PlaybackManager::isPlaying() {
 return this->m_impl->m_isPlaying;
}

bool PlaybackManager::isLooping() {
 return this->m_impl->m_isLooping;
}


void PlaybackManager::play()
{
    int projectLength = editor()->layers()->projectLength();

    this->m_impl->mStartFrame = ( this->m_impl->m_isRangedPlayback ) ? this->m_impl->m_markInFrame : 1;
    this->m_impl->mEndFrame = ( this->m_impl->m_isRangedPlayback ) ? this->m_impl->m_markOutFrame : projectLength;

    if ( editor()->currentFrame() >= this->m_impl->mEndFrame )
    {
        editor()->scrubTo( this->m_impl->mStartFrame );
    }

    this->m_impl->m_pTimer->setInterval( 1000.0f / this->m_impl->m_fps );
    this->m_impl->m_pTimer->start();
}

void PlaybackManager::stop()
{
    this->m_impl->m_pTimer->stop();
}

void PlaybackManager::setFps( int fps )
{
    if ( this->m_impl->m_fps != fps )
    {
        this->m_impl->m_fps = fps;
        emit fpsChanged( this->m_impl->m_fps );
    }
}

int  PlaybackManager::fps()
{
 return this->m_impl->m_fps;
}

int PlaybackManager::startFrame()
{
 return this->m_impl->mStartFrame;
}

int PlaybackManager::endFrame()
{
 return this->m_impl->mEndFrame;
}

void PlaybackManager::timerTick()
{
    if ( editor()->currentFrame() > this->m_impl->mEndFrame )
    {
        if ( this->m_impl->m_isLooping )
        {
            editor()->scrubTo( this->m_impl->mStartFrame );
        }
        else
        {
            stop();
        }
        return;
    }

    editor()->scrubTo( editor()->currentFrame() + 1 );

    // TODO: play sound if any
}

void PlaybackManager::setLooping( bool isLoop )
{
    if ( this->m_impl->m_isLooping != isLoop )
    {
        this->m_impl->m_isLooping = isLoop;
        emit loopStateChanged( this->m_impl->m_isLooping );
    }
}

void PlaybackManager::enableRangedPlayback( bool b )
{
    if ( this->m_impl->m_isRangedPlayback != b )
    {
        this->m_impl->m_isRangedPlayback = b;
        emit rangedPlaybackStateChanged( this->m_impl->m_isRangedPlayback );
    }
}


void PlaybackManager::setRangedStartFrame( int frame )
{
 this->m_impl->m_markInFrame = frame;
}

void PlaybackManager::setRangedEndFrame( int frame )
{
 this->m_impl->m_markOutFrame = frame;
}

void PlaybackManager::enbaleSound( bool b ) {
 this->m_impl->m_isPlaySound = b;
}
