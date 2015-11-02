#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include <QObject>
#include <QHash>
#include "basetool.h"
#include "basemanager.h"

class ScribbleArea;

class ToolManagerImpl;
class ToolManager : public BaseManager
{
    Q_OBJECT
public:
    explicit ToolManager( QObject* parent );
    ~ToolManager();

    bool init() override;

    BaseTool* currentTool();
    BaseTool* getTool( ToolType eToolType );
    void      setDefaultTool();
    void      setCurrentTool( ToolType eToolType );
    void      cleanupAllToolsData();

    void      tabletSwitchToEraser();
    void      tabletRestorePrevTool();

signals:
    void penWidthValueChange( float );
    void penFeatherValueChange( float );
    void penInvisiblityValueChange( int );
    void penPreserveAlphaValueChange( int );
    void penPressureValueChange( int );

    void toolChanged( ToolType );
    void displayToolOptions(QHash<ToolPropertyType, bool> options);
    void toolPropertyChanged();

public slots:
    void resetAllTools();

    void setWidth( float );
    void setFeather( float );
    void setInvisibility( int );
    void setPreserveAlpha( int );
    void setPressure( int );

private:
    ToolManagerImpl * m_impl;

};

#endif // TOOLMANAGER_H
