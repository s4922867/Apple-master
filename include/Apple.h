#ifndef APPLE_H__
#define APPLE_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Random.h>
#include <ngl/Transformation.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file Apple.h
/// @brief this class draws the different apples seen on screen and moves them
/// @author Miguel Goncalves
/// @version 1.0
/// @date 8/5/17
/// @class Apple
//----------------------------------------------------------------------------------------------------------------------

class Apple
{
  public :
    /// @param _pos the initial position
    /// @param _fname the name of mesh to load
    Apple(ngl::Vec3 _pos, std::string _fname);
    //----------------------------------------------------------------------------------------------------------------------
    ///@brief draw method
    void draw(const std::string &_shader, ngl::Camera *_cam );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief moves the apple's position
    //----------------------------------------------------------------------------------------------------------------------
    void move(float _x,	float _y	);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief rotates the apple on it's axis
    //----------------------------------------------------------------------------------------------------------------------
    void rotate(float _rot);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the position of the apple
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getPos ();
    //----------------------------------------------------------------------------------------------------------------------
  private :
    ngl::Random * rand = ngl::Random::instance();
    // position of the apple
    ngl::Vec3 m_pos;
    // the apple's mesh
    std::unique_ptr<ngl::Obj>m_mesh;
    // transform stack
    ngl::Transformation m_transform;
    // apple's rotation
    float m_rotation;
  };

#endif
