#pragma execution_character_set("utf-8")
#include "LensCheckAlgorithm.h"
#include<QImage>
#include<QDebug>
#include "HalconAlgorithm.h"
#include "LensCheckAlgorithmPrivate.h"



LensCheckAlgorithm& LensCheckAlgorithm::globalInstance()
{
	static LensCheckAlgorithm algorithm;
	return algorithm;
}

LensCheckAlgorithm::LensCheckAlgorithm(QObject *parent /*= Q_NULLPTR*/)
	:QObject(parent),\
	data_(nullptr)
{
	data_ = new LensCheckAlgorithmPrivate(this);
}

LensCheckAlgorithm::~LensCheckAlgorithm()
{

}

void LensCheckAlgorithm::setRingImage(QSharedPointer<HalconCpp::HImage> image)
{
	data_->setRingImage(image);
}

void LensCheckAlgorithm::setBackImage(QSharedPointer<HalconCpp::HImage> image)
{
	data_->setBackImage(image);
}

QImage LensCheckAlgorithm::getRingImg()
{
	return data_->getRingImg();
}

QSharedPointer<HalconCpp::HImage> LensCheckAlgorithm::getHalconRingImg()
{
	return data_->getHalconRingImg();
}

QImage LensCheckAlgorithm::getBackImg()
{
	return data_->getBackImg();
}

QList<QString> LensCheckAlgorithm::ringCheck()
{
	return data_->ringCheck();
}

void LensCheckAlgorithm::setTestRingImage(const QString& filePath)
{
	data_->setTestRingImage(filePath);
}
