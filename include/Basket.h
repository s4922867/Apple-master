#ifndef BASKET_H__
#define BASKET_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file Basket.h
/// @brief this class draws the basket and moves it
/// @author Miguel Goncalves
/// @version 1.0
/// @date 8/5/17
/// @class Basket
//----------------------------------------------------------------------------------------------------------------------

class Basket
{
  public :
    /// @brief ctor
    /// @param _pos the initial position
    /// @param _fname the name of mesh to load
    Basket(ngl::Vec3 _pos, std::string _fname);
    //----------------------------------------------------------------------------------------------------------------------
    ///@brief draw method
    //----------------------------------------------------------------------------------------------------------------------
    void draw(const std::string &_shader, ngl::Camera *_cam );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief moves the position of the basket
    //----------------------------------------------------------------------------------------------------------------------
    void move(float _x,	float _y	);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief gets the basket's position
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getPos ();
    //----------------------------------------------------------------------------------------------------------------------
  private :

    // the position of the basket
    ngl::Vec3 m_pos;
    // the mesh of the basket
    std::unique_ptr<ngl::Obj>m_mesh;
    // transform stack
    ngl::Transformation m_transform;
  };

#endif
