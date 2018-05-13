#include "Apple.h"
#include "NGLScene.h"
#include <ngl/ShaderLib.h>

const static float s_xLimit=20;
const static float s_zLimit=250;
//----------------------------------------------------------------------------------------------------------------------
Apple::Apple( ngl::Vec3 _pos, std::string _fname )
 {
   m_pos=_pos;
   m_mesh.reset( new ngl::Obj(_fname,"textures/t.png"));
   m_mesh->createVAO();
   m_rotation=0;
 }
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void Apple::draw(const std::string &_shader,  ngl::Camera *_cam )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();
  m_transform.setScale(3,3,3);
  m_transform.setPosition(m_pos);
  m_transform.setRotation(-90,m_rotation,0);
  ngl::Mat4 MVP= _cam->getVPMatrix()*m_transform.getMatrix();
  ngl::Mat4 MV= _cam->getViewMatrix()*m_transform.getMatrix();
  ngl::Mat3 N= MV.inverse();
  shader->setUniform("MVP",MVP);
  shader->setUniform("MV",MV);
  shader->setUniform("N",N);

  m_mesh->draw();
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void Apple::move( float _x,float _z )
{


  float currentX=m_pos.m_x;
  float currentZ=m_pos.m_z;
  m_pos.m_x+=_x;
  m_pos.m_z+=_z;
  if(m_pos.m_x<=-s_xLimit || m_pos.m_x>=s_xLimit)
  {
    m_pos.m_x=currentX;
  }


  if(m_pos.m_z<=-s_zLimit || m_pos.m_z>=s_zLimit)
  {
    m_pos.m_z=currentZ;
  }
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//makes the apple rotate, only here for visual impact
//----------------------------------------------------------------------------------------------------------------------
void Apple::rotate(float _rot )
{
  m_rotation+=_rot;
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//function that gets the coordinates of the apple
//----------------------------------------------------------------------------------------------------------------------
ngl::Vec3 Apple::getPos ()
{
    return m_pos;
}
//----------------------------------------------------------------------------------------------------------------------


