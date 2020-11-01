#include <QApplication>
#include"HalconAlgoTest.h"

int main(int argc,char**argv)
{
	QApplication a(argc,argv);

	HalconAlgoTest w;
	w.show();

	return a.exec();
}