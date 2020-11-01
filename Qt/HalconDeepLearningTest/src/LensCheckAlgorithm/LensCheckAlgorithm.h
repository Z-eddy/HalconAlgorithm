#pragma once

#include<QObject>
#include <QPixmap>
#include<QMap>
#include<QSharedPointer>
#include "lenscheckalgorithm_global.h"

namespace HalconCpp { 
	class HImage;
	class HTuple;
}

class  LensCheckAlgorithmPrivate;

class LENSCHECKALGORITHM_EXPORT LensCheckAlgorithm:public QObject
{
	Q_OBJECT

public:
	static LensCheckAlgorithm& globalInstance();//全局唯一
	~LensCheckAlgorithm();
	void setRingImage(QSharedPointer<HalconCpp::HImage> image);//设置环光图片
	void setBackImage(QSharedPointer<HalconCpp::HImage> image);//设置背光图片
	QImage getRingImg();//获得环光图片
	QSharedPointer<HalconCpp::HImage> getHalconRingImg();//获得环光图片
	QImage getBackImg();//获得背光图片
	QList<QString> ringCheck();//环光检测<镜片类型>
	//测试
	void setTestRingImage(const QString& filePath);//设置环光图片路径,读取图片到RingImage
private:
	LensCheckAlgorithm(QObject *parent = Q_NULLPTR);

	LensCheckAlgorithmPrivate *data_;
};
