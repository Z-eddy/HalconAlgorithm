// HALCON/Qt pattern matching and measure example
//
// (c) 2004-2020 MVTec Software GmbH

// matching.h : Definition of the class Matching
//

#if QT_VERSION >= 0x050000
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/qpushbutton.h>
#else
    #include <qlabel.h>
    #include <qpushbutton.h>
#endif

#include <QScopedPointer>

#include "qhalconwindow.h"

class Matching: public QWidget
{
  Q_OBJECT

  public:
    Matching(QWidget *parent=0);
    virtual ~Matching(void);
    void InitFg(void);

  protected slots:
    void Create(void);
    void Start(void);
    void Stop(void);

  protected:
    void timerEvent(QTimerEvent*);
    void StartMatching(void);

  private:
    // HALCON variables
    Hlong ImageWidth, ImageHeight;
    HalconCpp::HTuple FGHandle;
    HalconCpp::HImage Image;
    QScopedPointer<HalconCpp::HShapeModel> ShapeModel;
    HalconCpp::HTuple ModelRow, ModelColumn;
    HalconCpp::HTuple Rect1Row, Rect1Col, Rect2Row, Rect2Col;
    HalconCpp::HTuple RectLength1, RectLength2;
    HalconCpp::HObject ShapeModelRegion;

    // GUI elements
    QLabel *MatchTimeLabel, *MatchScoreLabel, *MeasTimeLabel;
    QLabel *NumLeadsLabel, *MinLeadsDistLabel;
    QPushButton *CreateButton, *StartButton, *StopButton;
    QHalconWindow *Disp;

    // Timer
    long Timer;
};
