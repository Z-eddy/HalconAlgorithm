#include "HalconAlgoTest.h"
#include<QDebug>
#include<QStringList>
#include<QTime>
#include<QImage>
#include<QPixmap>
#include "HalconDeepLearning.h"
#include "LensCheckAlgorithm.h"

HalconAlgoTest::HalconAlgoTest(QWidget *parent)
	: QMainWindow(parent),\
	algorithm_(LensCheckAlgorithm::globalInstance())
{
	ui.setupUi(this);
	auto id{ static_cast<Hlong>(ui.halconWindow->winId()) };
	auto w{ ui.halconWindow->width() };
	auto h{ ui.halconWindow->height() };
	OpenWindow(0, 0, w, h, id, "visible", "", &winHandle);
	HDevWindowStack::Push(winHandle);
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
	//»ñµÃhalconÍ¼Ïñ
	auto rawImg{ algorithm_.getHalconRingImg() };
	QSharedPointer<HImage> img{new HImage};
	Rgb1ToGray(*rawImg, img.get());
	//ÏÔÊ¾halconÍ¼Ïñ
	HTuple w, h;
	GetImageSize(*img, &w, &h);
	SetPart(winHandle, 0, 0, h, w);
	DispObj(*img,HDevWindowStack::GetActive());
	//ÏÔÊ¾QPixmapÍ¼Ïñ
	auto pixmap{ QPixmap::fromImage(QImage{}) };
	ui.labImage->setPixmap(pixmap);

	QTime time;
	time.start();
	QStringList lacks{ algorithm_.ringCheck() };
	auto val{ time.elapsed() };
	ui.text->appendPlainText(lacks.join(',') + QString{"   %1"}.arg(val));
	qDebug() << lacks;
}
