#include "HalconAlgoTest.h"
#include<QDebug>
#include<QStringList>
#include<QTime>
#include "LensCheckAlgorithm.h"
using namespace HalconCpp;

HalconAlgoTest::HalconAlgoTest(QWidget *parent)
	: QMainWindow(parent),\
	algorithm_(LensCheckAlgorithm::globalInstance())
{
	ui.setupUi(this);
	auto id{ static_cast<Hlong>(ui.halconWindow->winId()) };
	auto w{ ui.halconWindow->width() };
	auto h{ ui.halconWindow->height() };
	OpenWindow(0, 0, w, h, id, "visible", "", &winHandle_);
	HDevWindowStack::Push(winHandle_);
}

HalconAlgoTest::~HalconAlgoTest()
{

}

void HalconAlgoTest::on_btnClicked_clicked()
{
	static int i{ 0 };
	auto path{ ui.editFilePath->text() };
	if (i == 58) {
		i = 0;
		ui.text->appendPlainText("***finish***");
	}
	path += QString("%1.jpg").arg(i++);
	algorithm_.setTestRingImage(path);
	auto img{ algorithm_.getHalconRingImg() };
	HTuple w, h;
	GetImageSize(*img, &w, &h);
	SetPart(winHandle_, 0, 0, h, w);
	DispObj(*img,HDevWindowStack::GetActive());


	QTime time;
	time.start();
	QStringList lacks{ algorithm_.ringCheck() };
	auto val{ time.elapsed() };
	ui.text->appendPlainText(lacks.join(',') + QString{"   %1"}.arg(val));
	qDebug() << lacks;
}
