// HALCON/Qt pattern matching and measure example
//
// (c) 2004-2020 MVTec Software GmbH

// Purpose:
// This example application shows the use of pattern matching with shape
// models to locate an object.  Furthermore, it shows how to use the detected
// position and rotation of the object to construct search spaces for
// inspection tasks.  In this particular example, the print on an IC is used
// to find the IC.  From the found position and rotation, two measurement
// rectangles are constructed to measure the spacing between the leads of the
// IC.  Because of the lighting used in this example, the leads have the
// saturated gray value of 255 at several positions and rotations, which
// enlarges the apparent width of the leads, and hence seems to reduce the
// spacing between the leads, although the same board is used in all images.

// matching.cpp : Implementation of the class Matching
//

#include "matching.h"

#include <QGridLayout>

// Avoid compiler warnings with MSVC
#ifdef _WIN32
#  define sprintf sprintf_s
#endif

// Constructor: create GUI
Matching::Matching(QWidget *parent)
        : QWidget(parent), Timer(-1)
{
  // Pushbuttons
  CreateButton = new QPushButton(tr("Create Model"),this);
  connect(CreateButton,SIGNAL(clicked()),SLOT(Create()));

  StartButton  = new QPushButton(tr("Start"),this);
  StartButton->setEnabled(false);
  connect(StartButton,SIGNAL(clicked()),SLOT(Start()));

  StopButton  = new QPushButton(tr("Stop"),this);
  StopButton->setEnabled(false);
  connect(StopButton, SIGNAL(clicked()),SLOT(Stop()));

  // Labels
  QLabel *match_time    = new QLabel(tr("Matching:"),this);
  QLabel *match_time2   = new QLabel(tr("Time:"),this);
  MatchTimeLabel        = new QLabel("        ",this);
  QLabel *match_score   = new QLabel(tr("Score:  "),this);
  MatchScoreLabel       = new QLabel("        ",this);
  QLabel *meas_time     = new QLabel(tr("Measure:"),this);
  QLabel *meas_time2    = new QLabel(tr("Time:"),this);
  MeasTimeLabel         = new QLabel("        ",this);
  QLabel *num_leads     = new QLabel(tr("Number of leads:  "),this);
  NumLeadsLabel         = new QLabel("        ",this);
  QLabel *min_lead_dist = new QLabel(tr("Minimum Lead Distance:  "),this);
  MinLeadsDistLabel     = new QLabel("        ",this);
  // Label MVTec
  QLabel *MvtecLabel    = new QLabel(
      QLatin1String("\xa9 2004-2020 MVTec Software GmbH"),this);
  MvtecLabel->setFont(QFont(NULL,10,QFont::Bold ));
  // explication for HALCON widget
  QLabel *DispHintLabel = new QLabel(
      "Zoom: mouse wheel; Move: left mouse button; Reset: double click", this);

  // Layouts
  // Topmost VBoxLayout
  QVBoxLayout *TopBox = new QVBoxLayout(this);

  // MVTec label layout in TopBox
  QHBoxLayout *Mvtec  = new QHBoxLayout;
  Mvtec->addStretch(1);
  Mvtec->addWidget(MvtecLabel);
  Mvtec->addStretch(1);

  // TopVBox in TopBox
  QVBoxLayout *TopVBox = new QVBoxLayout;

  // HBoxDispAndButtons in TopVBox
  QHBoxLayout *HBoxDispAndButtons = new QHBoxLayout;

  // Disp: HALCON window widget in HBoxDispAndButtons
  Disp = new QHalconWindow(this);
  Disp->setMinimumSize(50,50);

  // One layout for HALCON widget and hint label
  QVBoxLayout *DispVBox = new QVBoxLayout;
  DispVBox->addWidget(Disp, 1);
  DispVBox->addSpacing(8);
  DispVBox->addWidget(DispHintLabel);

  // Buttons in HBoxDispAndButtons
  QVBoxLayout *Buttons = new QVBoxLayout;
  Buttons->addWidget(CreateButton);
  Buttons->addSpacing(8);
  Buttons->addWidget(StartButton);
  Buttons->addSpacing(8);
  Buttons->addWidget(StopButton);
  Buttons->addStretch(1);

  // HBoxDispAndButtons
  HBoxDispAndButtons->addSpacing(15);
  HBoxDispAndButtons->addLayout(DispVBox, 1);
  HBoxDispAndButtons->addSpacing(15);
  HBoxDispAndButtons->addLayout(Buttons);
  HBoxDispAndButtons->addSpacing(15);

  // HBoxLabels in TopVBox
  QHBoxLayout *HBoxLabels = new QHBoxLayout;
  // Labels in HBoxLabels
  QGridLayout *Labels = new QGridLayout();
  Labels->addWidget(match_time,0,0);
  Labels->addWidget(match_time2,0,1);
  Labels->addWidget(MatchTimeLabel,0,2);
  Labels->addWidget(match_score,0,3);
  Labels->addWidget(MatchScoreLabel,0,4);
  Labels->addWidget(meas_time,1,0);
  Labels->addWidget(meas_time2,1,1);
  Labels->addWidget(MeasTimeLabel,1,2);
  Labels->addWidget(num_leads,1,3);
  Labels->addWidget(NumLeadsLabel,1,4);
  Labels->addWidget(min_lead_dist,1,5);
  Labels->addWidget(MinLeadsDistLabel,1,6);

  // End Labels
  HBoxLabels->addSpacing(15);
  HBoxLabels->addLayout(Labels);
  HBoxLabels->addSpacing(130);
  // End HBoxLabels
  TopVBox->addLayout(HBoxDispAndButtons);
  TopVBox->addSpacing(15);
  TopVBox->addLayout(HBoxLabels);
  // End TopVBox
  TopBox->addSpacing(15);
  TopBox->addLayout(TopVBox);
  TopBox->addSpacing(15);
  TopBox->addLayout(Mvtec);
  TopBox->addSpacing(10);
  // End TopBox
}


// The destructor is called when the user closes the application by clicking
// on the close button in the window manager decoration.
Matching::~Matching(void)
{
  using namespace HalconCpp;

  // Close all allocated HALCON resources.
  CloseFramegrabber(FGHandle);
  if (Timer != -1)
  {
    killTimer(Timer);
    Timer = -1;
  }
}


// Open framegrabber and grab initial image
void Matching::InitFg(void)
{
  using namespace HalconCpp;

  // Open framegrabber and grab initial image
  OpenFramegrabber("File",1,1,0,0,0,0,"default",-1,"default",-1,"default",
                    "board/board.seq","default",-1,1,&FGHandle);
  GrabImage(&Image,FGHandle);
  Image.GetImageSize(&ImageWidth, &ImageHeight);
  Disp->GetHalconBuffer()->SetPart(0, 0, ImageHeight-1, ImageWidth-1);
  Disp->GetHalconBuffer()->SetLineWidth(3);
  Disp->GetHalconBuffer()->DispObj(Image);
  Disp->GetHalconBuffer()->FlushBuffer();
}


// Create shape model
void Matching::Create(void)
{
  using namespace HalconCpp;

  HalconCpp::HTuple Area;
  HalconCpp::HTuple Row1, Column1, Row2, Column2;
  HalconCpp::HTuple RectPhi;

  HalconCpp::HObject Rectangle0, Rectangle1, Rectangle2;
  HImage   ImageReduced, ShapeModelImage;

  // Prevent the model from being generated twice by setting the create button
  // to insensitive.
  CreateButton->setEnabled(false);
  CreateButton->repaint();
  setCursor(Qt::WaitCursor);
  // Generate the model from a rectangle with the following four coordinates:
  Row1 = 188;
  Column1 = 182;
  Row2 = 298;
  Column2 = 412;

  GenRectangle1(&Rectangle0,Row1,Column1,Row2,Column2);
  AreaCenter(Rectangle0,&Area,&ModelRow,&ModelColumn);

  // Compute the coordinates of the measurement rectangles relative to the
  // center of the model.
  Rect1Row = ModelRow-102;
  Rect1Col = ModelColumn+5;
  Rect2Row = ModelRow+107;
  Rect2Col = ModelColumn+5;
  RectPhi = 0;
  RectLength1 = 170;
  RectLength2 = 5;
  // Generate the two measurement rectangles for visualization purposes.
  GenRectangle2(&Rectangle1,Rect1Row,Rect1Col,RectPhi,RectLength1,
                 RectLength2);
  GenRectangle2(&Rectangle2,Rect2Row,Rect2Col,RectPhi,RectLength1,
                 RectLength2);

  GenRectangle1(&Rectangle0,Row1,Column1,Row2,Column2);
  AreaCenter(Rectangle0,&Area,&ModelRow,&ModelColumn);

  ReduceDomain(Image,Rectangle0,&ImageReduced);
  // Create an iconic representation of the model.  This region will be
  // transformed by the measured position of the model for visualization
  // purposes later on.
  InspectShapeModel(ImageReduced,&ShapeModelImage,&ShapeModelRegion,1,30);
  // Create the model.
  ShapeModel.reset(new HShapeModel(ImageReduced,4,0,2*PI,PI/180,"none","use_polarity",30,10));
  // Display the model and measurement rectangles.
  Disp->GetHalconBuffer()->SetColor("green");
  Disp->GetHalconBuffer()->DispObj(ShapeModelRegion);
  Disp->GetHalconBuffer()->SetColor("blue");
  Disp->GetHalconBuffer()->SetDraw("margin");
  Disp->GetHalconBuffer()->DispObj(Rectangle1);
  Disp->GetHalconBuffer()->DispObj(Rectangle2);
  Disp->GetHalconBuffer()->SetDraw("fill");
  Disp->GetHalconBuffer()->FlushBuffer();
  // Allow the user to start the matching.
  StartButton->setEnabled(true);
  setCursor(Qt::ArrowCursor);
}


// Grab next image and do matching
void Matching::StartMatching(void)
{
  using namespace HalconCpp;
  double   S1, S2;
  HTuple   Rect1RowCheck, Rect1ColCheck, Rect2RowCheck, Rect2ColCheck;
  HTuple   MeasureHandle1, MeasureHandle2, NumLeads;
  HTuple   RowCheck, ColumnCheck, AngleCheck, Score, HomMat2D, MinDistance;
  HTuple   RowEdgeFirst1, ColumnEdgeFirst1, AmplitudeFirst1;
  HTuple   RowEdgeSecond1, ColumnEdgeSecond1, AmplitudeSecond1;
  HTuple   IntraDistance1, InterDistance1;
  HTuple   RowEdgeFirst2, ColumnEdgeFirst2, AmplitudeFirst2;
  HTuple   RowEdgeSecond2, ColumnEdgeSecond2, AmplitudeSecond2;
  HTuple   IntraDistance2, InterDistance2;
  HObject  ShapeModelTrans;
  HObject  Rectangle1, Rectangle2;
  HImage   Image;
  char     buf[MAX_STRING];
  QString  string;

  GrabImage(&Image,FGHandle);
  // Note that all display operations are called with the buffer window.
  // This buffer window will be copied to the visible window at the end of
  // this function.  This leads to flicker-free display of the results.
  Disp->GetHalconBuffer()->DispObj(Image);
  // Find the IC in the current image.
  S1 = HSystem::CountSeconds();
  ShapeModel->FindShapeModel(Image, 0, 2*PI, 0.7, 1, 0.5, "least_squares", 4,
                             0.7, &RowCheck,&ColumnCheck,&AngleCheck,&Score);
  S2 = HSystem::CountSeconds();
  // Update the matching time label with the actual time.
  sprintf(buf,"%5.2f",(S2-S1)*1000);
  string = buf;
  MatchTimeLabel->setText(string);
  if (Score.Length() == 1)
  {
    // Update the matching score label with the measured score.
    sprintf(buf,"%7.5f",(double)Score[0]);
    string = buf;
    MatchScoreLabel->setText(string);
    // Rotate the model for visualization purposes.
    VectorAngleToRigid(ModelRow,ModelColumn,0,RowCheck,ColumnCheck,AngleCheck,
                       &HomMat2D);
    AffineTransRegion(ShapeModelRegion,&ShapeModelTrans,HomMat2D,"false");
    Disp->GetHalconBuffer()->SetColor("green");
    Disp->GetHalconBuffer()->DispObj(ShapeModelTrans);
    // Compute the parameters of the measurement rectangles.
    AffineTransPixel(HomMat2D,Rect1Row,Rect1Col,&Rect1RowCheck,
                       &Rect1ColCheck);
    AffineTransPixel(HomMat2D,Rect2Row,Rect2Col,&Rect2RowCheck,
                       &Rect2ColCheck);
    // For visualization purposes, generate the two rectangles as regions and
    // display them.

    GenRectangle2(&Rectangle1,Rect1RowCheck,Rect1ColCheck,AngleCheck,
                   RectLength1,RectLength2);
    GenRectangle2(&Rectangle2,Rect2RowCheck,Rect2ColCheck,AngleCheck,
                   RectLength1,RectLength2);
    Disp->GetHalconBuffer()->SetColor("blue");
    Disp->GetHalconBuffer()->SetDraw("margin");
    Disp->GetHalconBuffer()->DispObj(Rectangle1);
    Disp->GetHalconBuffer()->DispObj(Rectangle2);
    Disp->GetHalconBuffer()->SetDraw("fill");
    // Do the actual measurements.
    S1 = HSystem::CountSeconds();
    GenMeasureRectangle2(Rect1RowCheck,Rect1ColCheck,AngleCheck,
                           RectLength1,RectLength2,ImageWidth,ImageHeight,"bilinear",
                           &MeasureHandle1);
    GenMeasureRectangle2(Rect2RowCheck,Rect2ColCheck,AngleCheck,
                           RectLength1,RectLength2,ImageWidth,ImageHeight,"bilinear",
                           &MeasureHandle2);
    MeasurePairs(Image,MeasureHandle1,2,90,"positive","all",
                  &RowEdgeFirst1,&ColumnEdgeFirst1,&AmplitudeFirst1,
                  &RowEdgeSecond1,&ColumnEdgeSecond1,&AmplitudeSecond1,
                  &IntraDistance1,&InterDistance1);
    MeasurePairs(Image,MeasureHandle2,2,90,"positive","all",
                  &RowEdgeFirst2,&ColumnEdgeFirst2,&AmplitudeFirst2,
                  &RowEdgeSecond2,&ColumnEdgeSecond2,&AmplitudeSecond2,
                  &IntraDistance2,&InterDistance2);
    CloseMeasure(MeasureHandle1);
    CloseMeasure(MeasureHandle2);

    S2 = HSystem::CountSeconds();
     // Display the measurement results.
    Disp->GetHalconBuffer()->SetColor("red");
    Disp->GetHalconBuffer()->DispLine(
              RowEdgeFirst1-RectLength2*AngleCheck.TupleCos(),
              ColumnEdgeFirst1-RectLength2*AngleCheck.TupleSin(),
              RowEdgeFirst1+RectLength2*AngleCheck.TupleCos(),
              ColumnEdgeFirst1+RectLength2*AngleCheck.TupleSin());
    Disp->GetHalconBuffer()->DispLine(
              RowEdgeSecond1-RectLength2*AngleCheck.TupleCos(),
              ColumnEdgeSecond1-RectLength2*AngleCheck.TupleSin(),
              RowEdgeSecond1+RectLength2*AngleCheck.TupleCos(),
              ColumnEdgeSecond1+RectLength2*AngleCheck.TupleSin());
    Disp->GetHalconBuffer()->DispLine(
              RowEdgeFirst2-RectLength2*AngleCheck.TupleCos(),
              ColumnEdgeFirst2-RectLength2*AngleCheck.TupleSin(),
              RowEdgeFirst2+RectLength2*AngleCheck.TupleCos(),
              ColumnEdgeFirst2+RectLength2*AngleCheck.TupleSin());
    Disp->GetHalconBuffer()->DispLine(
              RowEdgeSecond2-RectLength2*AngleCheck.TupleCos(),
              ColumnEdgeSecond2-RectLength2*AngleCheck.TupleSin(),
              RowEdgeSecond2+RectLength2*AngleCheck.TupleCos(),
              ColumnEdgeSecond2+RectLength2*AngleCheck.TupleSin());

    Disp->GetHalconBuffer()->SetDraw("fill");
    // Update the measurement time label with the actual time.
    sprintf(buf,"%5.2f",(S2-S1)*1000);
    string = buf;
    MeasTimeLabel->setText(string);
    // Update the lead number label with the measured number of leads.
    NumLeads = IntraDistance1.Length()+IntraDistance2.Length();

    sprintf(buf,"%2ld",(long)((Hlong)NumLeads[0]));
    string = buf;

    NumLeadsLabel->setText(string);
    // Update the lead distance label with the measured minimum distance.
    MinDistance = (InterDistance1.TupleConcat(InterDistance2)).TupleMin();
    sprintf(buf,"%6.3f",(double)MinDistance[0]);
    string = buf;
    MinLeadsDistLabel->setText(string);

  }
  Disp->GetHalconBuffer()->FlushBuffer();
}


// This function is called continuously after Timer is started in ::Start()
void Matching::timerEvent(QTimerEvent*)
{
  StartMatching();
}


// Start continuous matching
void Matching::Start(void)
{
  StartButton->setEnabled(false);
  StopButton->setEnabled(true);
  // Start Timer -> ::timerEvent() is called continuously
  Timer = startTimer(20);
}


// Stop continuous matching
void Matching::Stop(void)
{
  StartButton->setEnabled(true);
  StopButton->setEnabled(false);
  // Kill Timer
  if (Timer != -1)
  {
    killTimer(Timer);
    Timer = -1;
  }
}
