#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include "WindowParams.h"
#include <ngl/AbstractVAO.h>
#include <ngl/Vec4.h>
#include <ngl/Mat4.h>
#include <QOpenGLWindow>
#include <array>
#include <memory>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime the window is resized
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;


private:
    struct VertexData
    {
      ngl::Vec4 p;
      float offset;
    };
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the x rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    WinParams m_win;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    ngl::Vec3 m_eye;
    ngl::Vec3 m_look={0.0f,0.0f,-2.0f};
    ngl::Vec3 m_up={0.0f,1.0f,0.0f};
    ngl::Real m_yaw=0.0f;
    ngl::Real m_pitch=0.0f;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    /// VAO for points / billboards
    std::unique_ptr<ngl::AbstractVAO> m_vao;
    /// array of texture ID for images
    std::array<GLuint,3> m_maps;
    /// flag for animation
    bool m_animate=true;
    /// time for start frame
    int m_time=0;

     //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event ) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event ) override;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event) override;
    void timerEvent(QTimerEvent *) override;


};



#endif
