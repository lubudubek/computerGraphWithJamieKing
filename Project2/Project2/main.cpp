//#include "QtGuiApplication2.h"
//#include "stdafx.h"
#include <QtWidgets/QApplication>

#include "MeGlWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QtGuiApplication2 w;
	//w.show();

	MeGlWindow meWindow;
	meWindow.show();

	return a.exec();
}
