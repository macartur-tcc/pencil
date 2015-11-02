
#include "pentool.h"
#include "penciltool.h"
#include "brushtool.h"
#include "buckettool.h"
#include "erasertool.h"
#include "eyedroppertool.h"
#include "handtool.h"
#include "movetool.h"
#include "polylinetool.h"
#include "selecttool.h"
#include "smudgetool.h"
#include "toolmanager.h"
#include "editor.h"
#include "pencilsettings.h"

class ToolManagerImpl
{
public:
    ToolManagerImpl(BaseTool* currentTool=nullptr,
                    ToolType eTabletBackupTool= INVALID_TOOL,
                    bool isSwitchedToEraser = false);

    BaseTool* m_pCurrentTool;
    ToolType  m_eTabletBackupTool;
    bool isSwitchedToEraser;
    QHash<ToolType, BaseTool*> m_toolSetHash;
};

ToolManagerImpl::ToolManagerImpl(BaseTool* currentTool,
                                 ToolType eTabletBackupTool,
                                 bool isSwitchedToEraser)
  : m_pCurrentTool(currentTool), m_eTabletBackupTool(eTabletBackupTool),
    isSwitchedToEraser(isSwitchedToEraser)
{
}

ToolManager::ToolManager(QObject* parent )
    : BaseManager( parent ), m_impl(new ToolManagerImpl())
{
}
ToolManager::~ToolManager()
{
  delete m_impl;
}


bool ToolManager::init()
{
    this->m_impl->isSwitchedToEraser = false;

    this->m_impl->m_toolSetHash.insert( PEN, new PenTool );
    this->m_impl->m_toolSetHash.insert( PENCIL, new PencilTool );
    this->m_impl->m_toolSetHash.insert( BRUSH, new BrushTool );
    this->m_impl->m_toolSetHash.insert( ERASER, new EraserTool );
    this->m_impl->m_toolSetHash.insert( BUCKET, new BucketTool );
    this->m_impl->m_toolSetHash.insert( EYEDROPPER, new EyedropperTool );
    this->m_impl->m_toolSetHash.insert( HAND, new HandTool );
    this->m_impl->m_toolSetHash.insert( MOVE, new MoveTool );
    this->m_impl->m_toolSetHash.insert( POLYLINE, new PolylineTool );
    this->m_impl->m_toolSetHash.insert( SELECT, new SelectTool );
    this->m_impl->m_toolSetHash.insert( SMUDGE, new SmudgeTool );

    foreach( BaseTool* pTool, this->m_impl->m_toolSetHash.values() )
    {
        pTool->initialize( editor(), editor()->getScribbleArea() );
    }

    return true;
}

BaseTool* ToolManager::currentTool(){
 return this->m_impl->m_pCurrentTool;
}

BaseTool* ToolManager::getTool(ToolType eToolType)
{
    return this->m_impl->m_toolSetHash[ eToolType ];
}

void ToolManager::setDefaultTool()
{
    // Set default tool
    // (called by the main window init)
    ToolType defaultToolType = PENCIL;

    setCurrentTool(defaultToolType);
    this->m_impl->m_eTabletBackupTool = defaultToolType;
}

void ToolManager::setCurrentTool( ToolType eToolType )
{
    this->m_impl->m_pCurrentTool = getTool( eToolType );

    setWidth( this->m_impl->m_pCurrentTool->properties.width );
    setFeather( this->m_impl->m_pCurrentTool->properties.feather );
    setPressure( this->m_impl->m_pCurrentTool->properties.pressure );
    setPreserveAlpha( this->m_impl->m_pCurrentTool->properties.preserveAlpha );
    setInvisibility( this->m_impl->m_pCurrentTool->properties.invisibility ); // by definition the pencil is invisible in vector mode


    emit toolChanged( eToolType );
    emit displayToolOptions(this->m_impl->m_pCurrentTool->m_enabledProperties);
}

void ToolManager::cleanupAllToolsData()
{
    foreach ( BaseTool* pTool, this->m_impl->m_toolSetHash.values() )
    {
        pTool->clear();
    }
}

void ToolManager::resetAllTools()
{
    // Reset can be useful to solve some pencil settings problems.
    // Betatesters should be recommended to reset before sending tool related issues.
    // This can prevent from users to stop working on their project.
    getTool( PEN )->properties.width = 1.5; // not supposed to use feather
    getTool( POLYLINE )->properties.width = 1.5; // PEN dependent
    getTool( PENCIL )->properties.width = 1.0;
    getTool( PENCIL )->properties.feather = -1.0; // locks feather usage (can be changed)
    getTool( ERASER )->properties.width = 25.0;
    getTool( ERASER )->properties.feather = 50.0;
    getTool( BRUSH )->properties.width = 15.0;
    getTool( BRUSH )->properties.feather = 200.0;
    getTool( SMUDGE )->properties.width = 25.0;
    getTool( SMUDGE )->properties.feather = 200.0;

    pencilSettings()->setValue( SETTING_TOOL_CURSOR, true );
    // todo: add all the default settings

    qDebug( "tools restored to default settings" );
}

void ToolManager::setWidth( float newWidth )
{
    currentTool()->setWidth(newWidth);
    emit penWidthValueChange( newWidth );
    emit toolPropertyChanged();
}

void ToolManager::setFeather( float newFeather )
{
    currentTool()->setFeather(newFeather);
    emit penFeatherValueChange( newFeather );
    emit toolPropertyChanged();
}

void ToolManager::setInvisibility( int isInvisible  )
{
    currentTool()->setInvisibility(isInvisible);
    emit penInvisiblityValueChange( isInvisible );
    emit toolPropertyChanged();
}

void ToolManager::setPreserveAlpha( int isPreserveAlpha )
{
    currentTool()->setPreserveAlpha(isPreserveAlpha);
    emit penPreserveAlphaValueChange( isPreserveAlpha );
    emit toolPropertyChanged();
}

void ToolManager::setPressure( int isPressureOn )
{
    currentTool()->setPressure(isPressureOn);
    emit penPressureValueChange( isPressureOn );
    emit toolPropertyChanged();
}

void ToolManager::tabletSwitchToEraser()
{
    if (!this->m_impl->isSwitchedToEraser)
    {
        this->m_impl->isSwitchedToEraser = true;

        this->m_impl->m_eTabletBackupTool = this->m_impl->m_pCurrentTool->type();
        setCurrentTool( ERASER );
    }
}

void ToolManager::tabletRestorePrevTool()
{
    if ( this->m_impl->isSwitchedToEraser )
    {
        this->m_impl->isSwitchedToEraser = false;
        if ( this->m_impl->m_eTabletBackupTool == INVALID_TOOL )
        {
            this->m_impl->m_eTabletBackupTool = PENCIL;
        }
        setCurrentTool( this->m_impl->m_eTabletBackupTool );
    }
}
