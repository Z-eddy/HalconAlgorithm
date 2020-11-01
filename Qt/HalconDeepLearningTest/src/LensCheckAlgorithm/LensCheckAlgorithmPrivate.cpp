#include "LensCheckAlgorithmPrivate.h"

LensCheckAlgorithmPrivate::LensCheckAlgorithmPrivate(QObject *parent /*= Q_NULLPTR*/)
	:HalconDeepLearning(parent)
{
	setModelPath("./data/classifier_Lens.hdl");
	setBatchSize(4);
	setThreadCount(4);
	//setUseCPU(true);
	initDlModel();
}

LensCheckAlgorithmPrivate::~LensCheckAlgorithmPrivate()
{
}

void LensCheckAlgorithmPrivate::setRingImage(QSharedPointer<HalconCpp::HImage> image)
{
	ringImage_ = image;
}

void LensCheckAlgorithmPrivate::setBackImage(QSharedPointer<HalconCpp::HImage> image)
{
	backImage_ = image;
}

QImage LensCheckAlgorithmPrivate::getRingImg()
{
	if (ringImage_->IsInitialized()) {
		return hImage2Pixmap(ringImage_);
	}
	else
	{
		return {};
	}
}

QSharedPointer<HalconCpp::HImage> LensCheckAlgorithmPrivate::getHalconRingImg()
{
	return ringImage_;
}

QImage LensCheckAlgorithmPrivate::getBackImg()
{
	if (ringImage_->IsInitialized()) {
		return hImage2Pixmap(backImage_);
	}
	else
	{
		return {};
	}
}

QList<QString> LensCheckAlgorithmPrivate::ringCheck()
{
	HTuple hv_lacks;//4个镜片类型集
	lensImgCheck(*ringImage_, *hv_DLModelHandle_, *hv_DLPreprocessParam_, &hv_lacks);
	auto count{hv_lacks.Length() };
	//qDebug() << count;
	QList<QString>fourLensLacks;
	for (int i{ 0 }; i != count; ++i) {
		fourLensLacks.push_back(QString{hv_lacks[i].S() });
	}
	return fourLensLacks;
}

void LensCheckAlgorithmPrivate::setTestRingImage(const QString& filePath)
{
	QSharedPointer<HImage>ringImg{ new HImage() };
	ReadImage(ringImg.get(), &(filePath.toStdString()[0]));
	ringImage_.swap(ringImg);
}
