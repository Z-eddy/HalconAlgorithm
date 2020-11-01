#pragma once
#include<QMainWindow>
#include "HalconCpp.h"
#include "ui_HalconAlgoTest.h"

class LensCheckAlgorithm;

namespace Ui { class  HalconAlgoTestClass; }

class HalconAlgoTest:public QMainWindow
{
	Q_OBJECT

public:
	HalconAlgoTest(QWidget *parent = nullptr);
	~HalconAlgoTest();

private:
	Ui::HalconAlgoTestClass ui;

	LensCheckAlgorithm &algorithm_;
	HalconCpp::HTuple winHandle_;

public slots:
	void on_btnClicked_clicked();
};