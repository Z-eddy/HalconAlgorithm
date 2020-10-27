// HALCON/Qt pattern matching and measure example
//
// (c) 2004-2020 MVTec Software GmbH

// QHalconWindow.cpp : Implementation of the class QHalconWindow
//
#include <QMouseEvent>
#include <QWheelEvent>

#include "qhalconwindow.h"

#include <cmath>

Herror __stdcall ContentUpdateCallback(void *context)
{
  // this callback gets calls on flush_buffer
  // if auto flushing is enabled (default) it gets called after every
  // change to the graphics buffer
  QHalconWindow* hwindow = (QHalconWindow*)context;

  // schedule redraw in Qt thread
  hwindow->update();

  return H_MSG_OK;
}

QHalconWindow::QHalconWindow(QWidget *parent, long Width, long Height)
        : QWidget(parent), lastMousePos(-1, -1)
{
  show();
  resize(Width,Height);

  // initialize HALCON buffer window
  halconBuffer.reset(new HalconCpp::HWindow(0, 0, 100, 100, 0, "buffer", ""));
  // turn on graphics stack, so image and regions stay even after zooming or
  // resizing
  halconBuffer->SetWindowParam("graphics_stack", "true");
  // turn on explicit flushing
  halconBuffer->SetWindowParam("flush", "false");
  // register update callback
  halconBuffer->SetContentUpdateCallback((void*)&ContentUpdateCallback, this);
}

// Resize the HALCON window whenever the QHalconWindow widget is resized
void QHalconWindow::resizeEvent(QResizeEvent* event)
{
  Q_UNUSED(event);
  // Set the HALCON window to its new size.
  halconBuffer->SetWindowExtents(0,0,width(),height());
  // initiate redraw of buffer.
  // (this uses the graphics_stack to get the last image and objects)
  halconBuffer->FlushBuffer();
}



void QHalconWindow::paintEvent(QPaintEvent *event)
{
  using namespace HalconCpp;
  Q_UNUSED(event);

  HString type;
  Hlong   width, height;
  // get content of buffer
  HImage image = halconBuffer->DumpWindowImage();
  // transform buffer to format used in Qt
  HImage imageInterleaved = image.InterleaveChannels("argb", "match", 0);
  // get access to raw image data
  unsigned char* pointer = (unsigned char*)imageInterleaved.GetImagePointer1(&type, &width, &height);
  // create QImage from data
  QImage qimage(pointer, width/4, height, QImage::Format_RGB32);

  // draw image to widget
  QPainter painter(this);
  painter.drawImage(QPoint(0, 0), qimage);
}

void QHalconWindow::GetPartFloat(double *row1, double *col1, double *row2, double *col2)
{
  // to get float values from get_part, use HTuple parameters
  HalconCpp::HTuple trow1, tcol1, trow2, tcol2;
  halconBuffer->GetPart(&trow1, &tcol1, &trow2, &tcol2);
  *row1 = trow1.D();
  *col1 = tcol1.D();
  *row2 = trow2.D();
  *col2 = tcol2.D();
}

void QHalconWindow::SetPartFloat(double row1, double col1, double row2, double col2)
{
  // convert the double values to HTuple. Otherwise the int variant of SetPart is used
  // this enables smooth movement and zooming even when zoomed in
  halconBuffer->SetPart(HalconCpp::HTuple(row1), HalconCpp::HTuple(col1),
                        HalconCpp::HTuple(row2), HalconCpp::HTuple(col2));
}


void QHalconWindow::mouseMoveEvent(QMouseEvent *event)
{
  if ((event->buttons() == Qt::LeftButton) && lastMousePos.x() != -1)
  {
    QPoint delta = lastMousePos - event->globalPos();

    // scale delta to image zooming factor
    double scalex = (lastCol2 - lastCol1 + 1) / (double)width();
    double scaley = (lastRow2 - lastRow1 + 1) / (double)height();
    try
    {
      // set new visible part
      SetPartFloat(lastRow1 + (delta.y() * scaley),
                   lastCol1 + (delta.x() * scalex),
                   lastRow2 + (delta.y() * scaley),
                   lastCol2 + (delta.x() * scalex));
      // initiate redraw ()
      halconBuffer->FlushBuffer();
    }
    catch (HalconCpp::HOperatorException)
    {
      // this may happen, if the part image is moved outside the window
    }
  }
}

void QHalconWindow::mousePressEvent(QMouseEvent *event)
{
  // save last mouse position and image part
  GetPartFloat(&lastRow1, &lastCol1, &lastRow2, &lastCol2);
  lastMousePos = event->globalPos();
}

void QHalconWindow::mouseReleaseEvent(QMouseEvent *event)
{
  Q_UNUSED(event);
  // unset reference mouse position
  lastMousePos = QPoint(-1, -1);
}

void QHalconWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
  Q_UNUSED(event);
  if (event->buttons() == Qt::LeftButton)
  {
    // reset image part
    halconBuffer->SetPart(0, 0, -1, -1);
    halconBuffer->FlushBuffer();
  }
}

void QHalconWindow::wheelEvent(QWheelEvent *event)
{
  // event->delta() is a multiple of 120. For larger multiples, the user
  // rotated the wheel by multiple notches.
  int num_notch = std::abs(event->delta()) / 120;
  double factor = (event->delta() > 0) ? std::sqrt(2.0) : 1.0 / std::sqrt(2.0);
  while (num_notch > 1)
  {
    factor = factor * ((event->delta() > 0) ? std::sqrt(2.0) : 1.0 / std::sqrt(2.0));
    num_notch--;
  }

  // get zooming center
  double centerRow, centerCol;
  halconBuffer->ConvertCoordinatesWindowToImage(event->y(), event->x(), &centerRow, &centerCol);
  // get current image part
  double row1, col1, row2, col2;
  GetPartFloat(&row1, &col1, &row2, &col2);
  // zoom around center
  double left = centerRow - row1;
  double right = row2 - centerRow;
  double top = centerCol - col1;
  double buttom = col2 - centerCol;
  double newRow1 = centerRow - left * factor;
  double newRow2 = centerRow + right * factor;
  double newCol1 = centerCol - top * factor;
  double newCol2 = centerCol + buttom * factor;
  try
  {
    SetPartFloat(newRow1, newCol1, newRow2, newCol2);
    halconBuffer->FlushBuffer();
  }
  catch (HalconCpp::HOperatorException)
  {
    // this may happen, if the part is much too small or too big
  }
}
