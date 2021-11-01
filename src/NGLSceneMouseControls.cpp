#include "NGLScene.h"
#include <QMouseEvent>

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_win.rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_win.origX;
    m_win.origX = _event->x();
    m_win.origY = _event->y();
    m_yaw=diffx;
    update();

  }
    // right mouse translate code
  else if(m_win.translate && _event->buttons() == Qt::RightButton)
  {
    int diffY = static_cast<int>(_event->y() - m_win.origYPos);
    m_win.origXPos=_event->x();
    m_win.origYPos=_event->y();
    m_pitch=diffY;
    update();

   }
}



//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent( QMouseEvent* _event )
{
  // that method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if ( _event->button() == Qt::LeftButton )
  {
    m_win.origX  = _event->x();
    m_win.origY  = _event->y();
    m_win.rotate = true;
  }
  // right mouse translate mode
  else if ( _event->button() == Qt::RightButton )
  {
    m_win.origXPos  = _event->x();
    m_win.origYPos  = _event->y();
    m_win.translate = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent( QMouseEvent* _event )
{
  // that event is called when the mouse button is released
  // we then set Rotate to false
  if ( _event->button() == Qt::LeftButton )
  {
    m_win.rotate = false;
    m_yaw=0.0f;
  }
  // right mouse translate mode
  if ( _event->button() == Qt::RightButton )
  {
    m_win.translate = false;
    m_pitch=0.0f;

  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent( QWheelEvent* _event )
{

  // check the diff of the wheel position (0 means no change)
  if ( _event->angleDelta().x() > 0 )
  {
    m_modelPos.m_z += ZOOM;
  }
  else if ( _event->angleDelta().x() < 0 )
  {
    m_modelPos.m_z -= ZOOM;
  }
  m_eye=(-m_modelPos);
  update();
}
