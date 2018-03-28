#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW

#include "QtOpenGl/qgl.h" //"qopenglwidget.h"

class MeGlWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();


};

#endif
