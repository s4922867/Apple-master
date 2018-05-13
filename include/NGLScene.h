#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Vec3.h>

#include <ngl/Random.h>
#include "Basket.h"
#include <ngl/Light.h>
#include "Apple.h"
#include <ngl/Transformation.h>
#include <ngl/Colour.h>
#include <ngl/Obj.h>
#include <memory>
#include <QOpenGLWindow>
#include <QSet>
#include <QTime>
#include "WindowParams.h"
#include <ngl/Camera.h>
#include <ngl/Text.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Miguel Goncalves
/// @version 1.0
/// @date 8/5/17
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
    //-------------------------------------#include <ngl/Random.h>---------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize the window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief depending on the players input it makes the basket move right or left
    //----------------------------------------------------------------------------------------------------------------------
    void moveBasket();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief makes the game harder based on the score by increasing the number of apples and making them fall at a faster rate
    //----------------------------------------------------------------------------------------------------------------------
    void increaseDiff();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief gets 2 objects positions and returns 1 if they collide
    //----------------------------------------------------------------------------------------------------------------------
    bool checkColl(ngl::Vec3 obj1Pos, ngl::Vec3 obj2Pos) ;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this function resets all the values and restarts the game
    //----------------------------------------------------------------------------------------------------------------------
    void restartGame();
    //----------------------------------------------------------------------------------------------------------------------
private:

    int m_redrawTimer;
    int m_updateBasketTimer;
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
    void timerEvent( QTimerEvent *_event);
    QSet<Qt::Key> m_keysPressed;
    void keyReleaseEvent(QKeyEvent *_event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event) override;
    /// @brief windows parameters for mouse control etc.
    WinParams m_win;

    ///two ngl cameras used to draw the basket and apples
    ngl::Camera m_cam;
    ngl::Camera m_cam2;

    /// position for our model
    ngl::Vec3 m_modelPos;

    ///basket pointer
    std::unique_ptr<Basket> m_basket;
    ///pointer to the apples
    std::unique_ptr<Apple> m_apple[10];

    ngl::Transformation m_transform;
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    ngl::Colour m_colour;
    QTime m_currenttime;
    GLuint m_texID;
    ngl::Random * rand = ngl::Random::instance();
    ngl::Vec3 m_lightPosition;

    /// increment values for the apples and basket
    float s_basketUpdate;
    float s_appleUpdate;

    ///text pointer
    std::unique_ptr<ngl::Text> m_lineText;
    std::unique_ptr<ngl::Text> m_gameOverText;

    /// pointer to the high score
    std::vector<int> m_highScore;

    int score;
    int health;
    int n_apples;
    float spacing;
    bool m_saved;

};



#endif
