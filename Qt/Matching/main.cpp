// HALCON/Qt pattern matching and measure example
//
// (c) 2004-2020 MVTec Software GmbH

// main.cpp : Initialization of the Qt application
//

#include "matching.h"

#include <QApplication>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

int main(int argc, char **argv)
{
#ifdef Q_WS_X11
  XInitThreads();
#endif
  QApplication application(argc,argv);

  try
  {
    Matching w;
    w.resize(QSize(700, 500));
    w.setWindowTitle("Matching and Measurement Demo");
    w.InitFg();
    w.show();
    return application.exec();
  }
  catch (HalconCpp::HException &exception)
  {
    fprintf(stderr, "Error #%u: %s\n",
            exception.ErrorCode(), exception.ErrorMessage().TextA());
    exit(-1);
  }

  return 0;
}
