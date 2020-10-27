// HALCON/Qt pattern matching and measure example
//
// (c) 2004-2020 MVTec Software GmbH

// QHalconWindow.h : Class used for opening HALCON windows in Qt
//
#ifndef QHALCONWINDOW_H
#define QHALCONWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QScopedPointer>

#ifndef __APPLE__
#  include "HalconCpp.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif

class QHalconWindow: public QWidget
{
  Q_OBJECT

  public:
    QHalconWindow(QWidget *parent=0, long Width=0, long Height=0);

    HalconCpp::HWindow* GetHalconBuffer(void) {return halconBuffer.data();}

  protected:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

  private:
    void GetPartFloat(double *row1, double *col1, double *row2, double *col2);
    void SetPartFloat(double row1, double col1, double row2, double col2);
    QScopedPointer<HalconCpp::HWindow> halconBuffer;
    QPoint lastMousePos;
    double lastRow1, lastCol1, lastRow2, lastCol2;
};

#endif // !QHALCONWINDOW_H
