#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HalconAlgoTest.h"
#include "HalconAlgorithm.h"

class LensCheckAlgorithm;

class HalconAlgoTest : public QMainWindow
{
	Q_OBJECT

public:
	HalconAlgoTest(QWidget *parent = Q_NULLPTR);

private:
	Ui::HalconAlgoTestClass ui;
	LensCheckAlgorithm& algorithm_;
	HTuple winHandle;

private slots:
	void on_btnClicked_clicked();
};
