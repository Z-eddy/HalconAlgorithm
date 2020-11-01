#pragma once

#include <QObject>
#include<QList>
#include "HalconAlgorithm.h"
#include "HalconDeepLearning/HalconDeepLearning.h"

class LensCheckAlgorithmPrivate:public HalconDeepLearning
{
	Q_OBJECT

public:
	LensCheckAlgorithmPrivate(QObject *parent = Q_NULLPTR);
	~LensCheckAlgorithmPrivate();
	void setRingImage(QSharedPointer<HalconCpp::HImage> image);//设置环光图片
	void setBackImage(QSharedPointer<HalconCpp::HImage> image);//设置背光图片
	QImage getRingImg();//获得环光图片
	QSharedPointer<HalconCpp::HImage> getHalconRingImg();//获得环光图片
	QImage getBackImg();//获得背光图片
	QList<QString> ringCheck();//环光检测<镜片类型>
	//测试
	void setTestRingImage(const QString& filePath);//设置环光图片路径,读取图片到RingImage
private:
	QSharedPointer<HalconCpp::HImage> ringImage_;//环光图片
	QSharedPointer<HalconCpp::HImage> backImage_;//背光图片
};