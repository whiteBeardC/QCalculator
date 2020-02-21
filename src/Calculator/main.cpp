#include "MainWindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setStyle(QStyleFactory::create("Default"));
	MainWindow w;
	w.show();

	return app.exec();
}