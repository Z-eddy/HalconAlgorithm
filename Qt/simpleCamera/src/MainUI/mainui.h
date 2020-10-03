#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainUI;
}
QT_END_NAMESPACE

namespace HalconCpp {
class HTuple;
}

class MainUI : public QMainWindow {
  Q_OBJECT

 public:
  MainUI(QWidget *parent = nullptr);
  ~MainUI();

 private slots:
  void on_btnStart_clicked();

 private:
  Ui::MainUI *ui;
  HalconCpp::HTuple *halConWindowHandle_;
};
#endif  // MAINUI_H
