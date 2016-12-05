#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include <ngl/Texture.h>


NGLScene::NGLScene()
{
  setTitle("Animated Billboard Textures");
  m_animate=true;
  m_time=0;

}



NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_cam.setShape(45.0f,static_cast<float>(width())/height(),0.05f,350.0f);
  m_win.width=static_cast<int>(_w*devicePixelRatio());
  m_win.height=static_cast<int>(_h*devicePixelRatio());
}
void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();


  // create a billboard shader use strings to avoid typos
  auto constexpr BillboardShader="Billboard";
  auto constexpr BillboardVert="BillboardVert";
  auto constexpr BillboardFrag="BillboardFrag";
  auto constexpr BillboardGeo="BillboardGeo";


  // we are creating a shader called Phong
  shader->createShaderProgram(BillboardShader);
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader(BillboardVert,ngl::ShaderType::VERTEX);
  shader->attachShader(BillboardFrag,ngl::ShaderType::FRAGMENT);
  shader->attachShader(BillboardGeo,ngl::ShaderType::GEOMETRY);

  // attach the source
  shader->loadShaderSource(BillboardVert,"shaders/BillboardVert.glsl");
  shader->loadShaderSource(BillboardFrag,"shaders/BillboardFrag.glsl");
  shader->loadShaderSource(BillboardGeo,"shaders/BillboardGeo.glsl");
  // compile the shaders
  shader->compileShader(BillboardVert);
  shader->compileShader(BillboardFrag);
  shader->compileShader(BillboardGeo);
  // add them to the program
  shader->attachShaderToProgram(BillboardShader,BillboardVert);
  shader->attachShaderToProgram(BillboardShader,BillboardFrag);
  shader->attachShaderToProgram(BillboardShader,BillboardGeo);

  // now we have associated this data we can link the shader
  shader->linkProgramObject(BillboardShader);
  // and make it active ready to load values
  (*shader)[BillboardShader]->use();
  shader->setUniform("time",2);

  // create a voa of points to draw
  m_vao.reset( ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS));
  m_vao->bind();

  ngl::Random *rng=ngl::Random::instance();

  VertexData p;
  std::vector <VertexData> points;
  rng->setSeed();

  for(int i=0; i<20000; ++i)
  {
   float radius=8+rng->randomPositiveNumber(1);
   float x=radius*cosf( ngl::radians(i));
   float z=radius*sinf( ngl::radians(i));

   p.p.m_x=x;
   p.p.m_y=-rng->randomPositiveNumber(1);
   p.p.m_z=z;

   p.p.m_w=static_cast<int>(rng->randomPositiveNumber(3));
   // time offset we have 10 frames for each texture
   p.offset=static_cast<int>(rng->randomPositiveNumber(10));
   points.push_back(p);
  }
  // use a lambda to depth sort via z
  std::sort(points.begin(),points.end(),[](VertexData _a, VertexData _b)
  {
    return _a.p.m_z<_b.p.m_z;
  });
  m_vao->setData(ngl::AbstractVAO::VertexData(points.size()*sizeof(VertexData),points[0].p.m_x));

  m_vao->setVertexAttributePointer(0,4,GL_FLOAT,sizeof(VertexData),0);
  m_vao->setVertexAttributePointer(1,1,GL_FLOAT,sizeof(VertexData),4);

  m_vao->setNumIndices(points.size());
  m_vao->unbind();

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0.0f,0.0f,10.0f);
  ngl::Vec3 to(0.0f,0.0f,-2.0f);
  ngl::Vec3 up(0.0f,1.0f,0.0f);
  // now load to our new camera
  m_cam.set(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setShape(45.0f,720.0f/576.0f,0.01f,35.0f);

  ngl::Texture t;

  t.loadImage("textures/map1.png");
  t.setMultiTexture(0);
  m_maps[0]=t.setTextureGL();
  glGenerateMipmap(GL_TEXTURE_2D);

  t.loadImage("textures/map2.png");
  t.setMultiTexture(1);
  m_maps[1]=t.setTextureGL();
  glGenerateMipmap(GL_TEXTURE_2D);

  t.loadImage("textures/map3.png");
  t.setMultiTexture(2);
  m_maps[2]=t.setTextureGL();
  glGenerateMipmap(GL_TEXTURE_2D);

  shader->setUniform("tex1",0);
  shader->setUniform("tex2",1);
  shader->setUniform("tex3",2);

  glDisable( GL_DEPTH_TEST );
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  startTimer(40);

}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MVP;
  MVP= m_cam.getVPMatrix();
  shader->setUniform("MVP",MVP);
}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  shader->setUniform("camerapos",m_cam.getEye().toVec3());
  shader->setUniform("VP",m_cam.getVPMatrix());
  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();

}


void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // turn on wirframe rendering
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  case Qt::Key_Space : m_animate ^=true; break;
  default : break;
  }
    update();
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::timerEvent(QTimerEvent *)
{
  if(m_animate)
  {
    ++m_time;
  }
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->setUniform("time",m_time);
  update();
}

