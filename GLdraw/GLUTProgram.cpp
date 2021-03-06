#include "GLUTProgram.h"

#if HAVE_GLUT

#include "GL.h"
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif // Apple

#include <assert.h>
#include <stdio.h>

GLUTProgramBase::GLUTProgramBase(int w,int h)
:main_window(0),width(w),height(h),fullscreen_mode(false)
{}

GLUTProgramBase* GLUTProgramBase::current_program=NULL;
void GLUTProgramBase::DisplayFunc() { current_program->Handle_Display(); }
void GLUTProgramBase::ReshapeFunc(int w,int h) { current_program->Handle_Reshape(w,h); }
void GLUTProgramBase::KeyboardFunc(unsigned char c,int x,int y) {
  current_program->Handle_Keypress(c,x,y);
  if(GLUT_API_VERSION < 4)
    current_program->Handle_KeypressUp(c,x,y);
}
void GLUTProgramBase::KeyboardUpFunc(unsigned char c,int x,int y) { current_program->Handle_KeypressUp(c,x,y); }
void GLUTProgramBase::SpecialFunc(int key,int x,int y) {
  current_program->Handle_Special(key,x,y);
  if(GLUT_API_VERSION < 4)
    current_program->Handle_SpecialUp(key,x,y);
}
void GLUTProgramBase::SpecialUpFunc(int key,int x,int y) { current_program->Handle_SpecialUp(key,x,y); }
void GLUTProgramBase::MouseFunc(int button,int state,int x,int y) { current_program->Handle_Click(button,state,x,y); }
void GLUTProgramBase::MotionFunc(int x,int y) { current_program->Handle_Drag(x,y); }
void GLUTProgramBase::PassiveMotionFunc(int x,int y) { current_program->Handle_Motion(x,y); }
void GLUTProgramBase::IdleFunc() { current_program->Handle_Idle(); }

int GLUTProgramBase::Run(const char *window_title,unsigned int mode)
{
	current_program=this;
	int argc=1;char *argv[1];argv[0]="Program";
	glutInit(&argc,(char**)argv);
	if(mode == 0) mode=GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH;
	glutInitDisplayMode(mode);
	glutInitWindowSize(width,height);
	main_window = glutCreateWindow(window_title);
	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutIdleFunc(IdleFunc);
#if GLUT_API_VERSION >= 4
	glutKeyboardUpFunc(KeyboardUpFunc);
	glutSpecialUpFunc(SpecialUpFunc);
#endif //GLUT_API_VERSION

	if(!Initialize()) return -1;

	glutMainLoop();
	return 0;
}

bool GLUTProgramBase::Initialize()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);

	return true;
}

void GLUTProgramBase::Refresh()
{
	glutPostRedisplay();
}

void GLUTProgramBase::SetFullscreen(bool fullscreen_on)
{
	if(fullscreen_mode != fullscreen_on) {
		fullscreen_mode=fullscreen_on;
		if(fullscreen_mode) { glutFullScreen(); saved_width=width; saved_height=height; }
		else glutReshapeWindow(saved_width,saved_height);
	}
}

void GLUTProgramBase::Handle_Idle()
{
  SleepIdleCallback();
}

typedef void (*VOIDFUNC)();
VOIDFUNC oldIdleFunc=NULL;
void enable_idle_func(int)
{
  glutIdleFunc(oldIdleFunc);
}

void GLUTProgramBase::SleepIdleCallback(unsigned int time)
{
  if(time==0) glutIdleFunc(IdleFunc);
  else {
    glutIdleFunc(NULL);
    oldIdleFunc=GLUTProgramBase::IdleFunc;
    glutTimerFunc(time,enable_idle_func,0);
  }
}

#else //HAVE_GLUT=0

#include <iostream>
using namespace std;

GLUTProgramBase::GLUTProgramBase(int w,int h)
:main_window(0),width(w),height(h),fullscreen_mode(false)
{
}

void GLUTProgramBase::DisplayFunc() {  }
void GLUTProgramBase::ReshapeFunc(int w,int h) {  }
void GLUTProgramBase::KeyboardFunc(unsigned char c,int x,int h) {  }
void GLUTProgramBase::SpecialFunc(int key,int x,int h) {  }
void GLUTProgramBase::MouseFunc(int button,int state,int x,int y) {  }
void GLUTProgramBase::MotionFunc(int x,int y) { }
void GLUTProgramBase::PassiveMotionFunc(int x,int y) {  }
void GLUTProgramBase::IdleFunc() { }

int GLUTProgramBase::Run(const char *window_title,unsigned int mode)
{
  cerr<<"Warning, GLUT not defined"<<endl;
  return -1;
}

bool GLUTProgramBase::Initialize()
{
  cerr<<"Warning, GLUT not defined"<<endl;
	return false;
}

void GLUTProgramBase::Refresh()
{
  cerr<<"Warning, GLUT not defined"<<endl;
}

void GLUTProgramBase::SetFullscreen(bool fullscreen_on)
{
  cerr<<"Warning, GLUT not defined"<<endl;
}

void GLUTProgramBase::Handle_Idle()
{
  cerr<<"Warning, GLUT not defined"<<endl;
}

void GLUTProgramBase::SleepIdleCallback(unsigned int time)
{
  cerr<<"Warning, GLUT not defined"<<endl;
}

#endif
