#include "Basket.h"
#include "Apple.h"
#include <ngl/ShaderLib.h>

const static float xLimit=20;
const static float zLimit=20;
//----------------------------------------------------------------------------------------------------------------------
Basket::Basket( ngl::Vec3 _pos, std::string _fname )
 {
   m_pos=_pos;
   m_mesh.reset( new ngl::Obj(_fname));
   m_mesh->createVAO();
 }
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void Basket::draw(const std::string &_shader,  ngl::Camera *_cam )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();

  m_transform.setPosition(m_pos);
  m_transform.setRotation(-100,0,0);
  m_transform.setScale(3,3,3);
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

//----------------------------------------------------------------------------------------------------------------------
void Basket::move( float _x,float _z )
{
  float currentX=m_pos.m_x;
  float currentZ=m_pos.m_z;
  m_pos.m_x+=_x;
  m_pos.m_z+=_z;
  if(m_pos.m_x<=-xLimit || m_pos.m_x>=xLimit)
  {
    m_pos.m_x=currentX;
  }


  if(m_pos.m_z<=-zLimit || m_pos.m_z>=zLimit)
  {
    m_pos.m_z=currentZ;
  }
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
ngl::Vec3 Basket::getPos ()
{
    return m_pos;
}
//----------------------------------------------------------------------------------------------------------------------


