#include <QMouseEvent>
#include <QGuiApplication>
#include <QApplication>

#include <iostream>
#include <iomanip>
#include <fstream>

#include "Basket.h"
#include <ngl/ShaderLib.h>
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <iostream>
#include <ngl/VAOPrimitives.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.cpp
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Miguel Goncalves
/// @version 1.0
/// @date 8/5/17
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
NGLScene::NGLScene()
{
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    setTitle("Apple Game");
    m_updateBasketTimer=startTimer(5);
    m_redrawTimer=startTimer(20);
    // now clear the key set
    m_keysPressed.clear();

    //the number of apples loaded in
    n_apples=1;

    //speed that the basket travesls at
    s_basketUpdate=0.6;

    //speed that the apples travel at
    s_appleUpdate=0.4;

    // the players score, this will get higher each time the player catches an apple
    score=0;

    //this is the players health, it decreases each time the player fails to catch an apple
    health=100;

    m_saved = false;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
NGLScene::~NGLScene()
{
    std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::resizeGL(int _w , int _h)
{
    m_cam2.setShape( 45.0f, static_cast<float>( _w ) / _h, 0.1f, 100.0f );
    m_cam.setShape( 45.0f, static_cast<float>( _w ) / _h, 0.1f, 100.0f );
    m_win.width  = static_cast<int>( _w * devicePixelRatio() );
    m_win.height = static_cast<int>( _h * devicePixelRatio() );
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::initializeGL()
{
    // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
    // be done once we have a valid GL context but before we call any GL commands. If we dont do
    // this everything will crash
    ngl::NGLInit::instance();
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);
    //gets basket mesh
    m_basket.reset(new Basket(ngl::Vec3(1,0,20), "models/basket.obj"));

    float x_coord;
    spacing=0;

    //gets multiple apples
    for (int i=0;i<8;i++)
    {
        spacing=spacing+10;
        //x_coord is set as a random number, its used as the x coordinate of the apples
        x_coord = rand->randomNumber(20);
        //creates the apples and spaces them out from each other
        m_apple[i].reset(new Apple(ngl::Vec3(x_coord,0,-50-spacing), "models/apple.obj"));
    }

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    //sets the shader
    shader->loadShader("TextureShader",
                       "shaders/TextureVertex.glsl",
                       "shaders/TextureFragment.glsl");
    shader->use("TextureShader");
    shader->setUniform("tex1",0);
    shader->setUniform("MVP",ngl::Mat4());
    shader->setUniform("lightPos",1.0f,1.0f,1.0f);
    shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);

    ngl::Vec3 from(0,50,40);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);

    //sets the coordinates of the two cameras
    m_cam.set(from,to,up);
    m_cam2.set(from,to,up);

    //initializes the text that is going to be on screen
    m_lineText.reset(new ngl::Text(QFont("Font", 20)));
    m_lineText->setScreenSize(width(), height());
    m_lineText->setColour(1, 0, 0);

    m_gameOverText.reset(new ngl::Text(QFont("Font", 50)));
    m_gameOverText->setScreenSize(width(), height());

    //starts the timer
    m_currenttime.start();

}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::paintGL()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_win.width,m_win.height);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);


    //checks if the game is over
    if (health>0)
    {
        //calls the function that will draw the apples
        for (int i=0;i<n_apples;i++)
        {

            m_apple[i]->draw("TextureShader",&m_cam);
        }
        //draws basket
        m_basket->draw("TextureShader",&m_cam);

        //draws the players score and health on screen
        m_lineText->renderText(0,0, "score:");
        m_lineText->renderText(80,0,  QString::number(score));
        m_lineText->renderText(0,40, "health:");
        m_lineText->renderText(100,40,  QString::number(health));


    }
    else
    {
        //if the game over state is true then it draws the game over screen and stops drawing everything else
        m_gameOverText->renderText((1024/2)-200,(720/2)-100, "GAME OVER");
        m_lineText->renderText((1024/2)-200,(720/2), " YOUR SCORE WAS");
        m_lineText->renderText((1024/2)+100,(720/2),  QString::number(score));
        m_lineText->renderText((1024/2)-200,(720/2)+50, " PRESS R TO RESTART");

          //reads the score files
          int readScore;
          std::string filePath = "resources/scores.txt";
          std::fstream scoreFile;
          std::ofstream outfile;
          scoreFile.open(filePath);

          //reads the files
          scoreFile>>readScore;

          //appends players name and score the container
          m_highScore.push_back(score);

          //writes the scores and names to a file
          if (score>readScore)
          {
              outfile.open(filePath);
              outfile<<score;
              outfile.close();
          }

          m_saved = true;

          m_lineText->renderText((1024/2)-200,(720/2)+100, " THE HIGH SCORE IS");
          m_lineText->renderText((1024/2)+100,(720/2)+100,  QString::number(readScore));
    }
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::timerEvent( QTimerEvent *_event)
{
    // the usual process is to check the event timerID and compare it to
    // any timers we have started with startTimer

    //moves the basket when the timer is called
    if (_event->timerId() == m_updateBasketTimer)
    {
        moveBasket();
    }
    if (_event->timerId() == m_redrawTimer)
    {
        update();
    }

    // the basket position will be stored here
    ngl::Vec3 basketpos;

    //the position of the apples will be stored here
    ngl::Vec3 applepos[10];

    bool collision;

    //checks if the set ammount of time is up.
    if(m_currenttime.elapsed() > 30)
    {

        //checks if the game over state isnt true
        if (health>0)
        {

            //restarts timer
            m_currenttime.restart();

            // stores the basket's position
            basketpos=m_basket->getPos();

            //stores the apples' positions
            for (int i=0;i<n_apples;i++)
            {
                applepos[i]=m_apple[i]->getPos();
            }


            for (int i=0;i<n_apples;i++)
            {
                //this will put the basket and every apple in a function that checks if there is a collision
                collision=checkColl(basketpos,applepos[i]);

                //checks if a collision has happened
                if (collision)
                {
                    //send the apple back to the top with a different random x position
                    float x_coord;
                    x_coord = rand->randomNumber(20);
                    m_apple[i]->move(x_coord,-75.0);
                    m_apple[i]->rotate(x_coord);

                    //adds one to the score and also regenerates a bit of health (only if its under the 100 limit)
                    score++;
                    if (health<100)
                    {
                        health++;
                    }

                    // each time a point is scored it calls a function to increase the difficulty
                    increaseDiff();
                }

                //checks if the player has failed to catch an apple
                if (basketpos.m_z<applepos[i].m_z-10)
                {
                    // send the apple back up with a different random x coordinate
                    float x_coord;
                    x_coord = rand->randomNumber(20);
                    m_apple[i]->move(x_coord,-90.0);
                    m_apple[i]->rotate(x_coord);

                    // takes some of the players health
                    health=health-10;
                }

            }
            //makes apples increment down the screen
            for (int i=0;i<n_apples;i++)
            {
                m_apple[i]->move(0.0,s_appleUpdate);
                m_apple[i]->rotate(s_appleUpdate*10);
            }
            update();
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::moveBasket()
{
    /// first we reset the movement values
    float xDirection=0.0;
    float zDirection=0.0;
    // now we loop for each of the pressed keys in the the set
    // and see which ones have been pressed. If they have been pressed
    // we set the movement value to be an incremental value
    foreach(Qt::Key key, m_keysPressed)
    {
        switch (key)
        {
        case Qt::Key_Left :  { xDirection=-s_basketUpdate; break;}
        case Qt::Key_Right : { xDirection=s_basketUpdate; break;}
        case Qt::Key_R : { restartGame(); break;}
        default : break;
        }
    }
    // if the set is non zero size we can update the basket movement
    // then tell openGL to re-draw
    if(m_keysPressed.size() !=0)
    {
        m_basket->move(xDirection,zDirection);
    }
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyPressEvent( QKeyEvent *_event)
{
    // add to our keypress set the values of any keys pressed
    m_keysPressed += (Qt::Key)_event->key();

    switch (_event->key())
    {
    case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
    }

}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// in this case this method is called from the MainWindow class and passed
// the key event from that, if we didn't need to process the events in main
// window we could just overload this as the keyPressEvent and only process here.
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyReleaseEvent( QKeyEvent *_event	)
{
    // remove from our key set any keys that have been released
    m_keysPressed -= (Qt::Key)_event->key();
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//changes the difficulty of the game by increasing the speed slightly each time time the player scores
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::increaseDiff()
{
    if (score<=2)
    {
        n_apples=1;
        s_appleUpdate=0.4;
    }
    else if (score<=5)
    {
        n_apples=2;
        s_appleUpdate=0.5;
    }
    else if  (score<10)
    {
        n_apples=3;
        s_appleUpdate=0.6;
    }
    else if  (score<20)
    {
        n_apples=4;
        s_appleUpdate=0.7;
    }
    else if  (score<40)
    {
        n_apples=5;
        s_appleUpdate=0.8;
    }
    else if  (score<60)
    {
        n_apples=6;
        s_appleUpdate=0.9;
    }
    else if (score<80)
    {
        n_apples=7;
        s_appleUpdate=1;
    }
    else if  (score<110)
    {
        n_apples=8;
        s_appleUpdate=1.2;
    }
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//checks if two objects are colliding, returns 1 if there is a collision
//----------------------------------------------------------------------------------------------------------------------
bool NGLScene::checkColl(ngl::Vec3 obj1Pos,ngl::Vec3 obj2Pos)
{
    bool collX;
    bool collZ;
    collX = obj1Pos.m_x + 4.5 >= obj2Pos.m_x - 1.5f && obj1Pos.m_x- 4.5 <= obj2Pos.m_x + 1.5;
    collZ = obj1Pos.m_z + 1.5 >= obj2Pos.m_z - 1.5f && obj1Pos.m_z- 1.5 <= obj2Pos.m_z + 1.5;
    return (collX&&collZ);
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::restartGame()
{
    health=100;
    score=0;
    n_apples=1;
    s_appleUpdate=0.4;
    s_basketUpdate=0.6;
    initializeGL();
}
//----------------------------------------------------------------------------------------------------------------------
