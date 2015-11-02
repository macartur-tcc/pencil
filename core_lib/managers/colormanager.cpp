
#include "object.h"
#include "editor.h"
#include "colormanager.h"

class ColorManagerImpl
{
public:
    ColorManagerImpl();
    void set_CurrentColorIndex(int index);
    int get_CurrentColorIndex();
private:
    int mCurrentColorIndex;
};

ColorManagerImpl::ColorManagerImpl()
  : mCurrentColorIndex(0)
{
}

void ColorManagerImpl::set_CurrentColorIndex(int index)
{
  this->mCurrentColorIndex = index;
}
int ColorManagerImpl::get_CurrentColorIndex()
{
  return this->mCurrentColorIndex;
}




ColorManager::ColorManager( QObject* parent )
  : BaseManager( parent ), m_impl(new ColorManagerImpl())
{
}

ColorManager::~ColorManager()
{
  delete m_impl;
}

bool ColorManager::init()
{
    return true;
}

QColor ColorManager::frontColor()
{
    return editor()->object()->getColour( m_impl->get_CurrentColorIndex() ).colour;
}

void ColorManager::setColorNumber( int n )
{
    Q_ASSERT( n >= 0 );

    if ( m_impl->get_CurrentColorIndex() != n )
    {
        m_impl->set_CurrentColorIndex(n);

        QColor currentColor = editor()->object()->getColour(m_impl->get_CurrentColorIndex()).colour;
        emit colorNumberChanged(m_impl->get_CurrentColorIndex());
	    	emit colorChanged(currentColor);
    }
}

void ColorManager::setColor(const QColor& newColor)
{
    QColor currentColor = editor()->object()->getColour(m_impl->get_CurrentColorIndex()).colour;
    if (currentColor != newColor)
    {
        editor()->object()->setColour( m_impl->get_CurrentColorIndex(), newColor );
        emit colorChanged(newColor);

		qDebug("Pick Color(R=%d, G=%d, B=%d)", newColor.red(), newColor.green(), newColor.blue());
    }
}

int ColorManager::frontColorNumber()
{
    return m_impl->get_CurrentColorIndex();
}
