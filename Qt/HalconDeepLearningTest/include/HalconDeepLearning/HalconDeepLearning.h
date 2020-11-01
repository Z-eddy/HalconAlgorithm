#pragma once
#include<QObject>
#include<QSharedPointer>
#include<QImage>
#include "halcondeeplearning_global.h"

namespace HalconCpp { 
	class HImage;
	class HTuple;
}

QImage HALCONDEEPLEARNING_EXPORT hImage2Pixmap(QSharedPointer<HalconCpp::HImage> image);//转换HImage到QPixmap,通过复制的方式

class HALCONDEEPLEARNING_EXPORT HalconDeepLearning:public QObject
{//注意，只支持英伟达的GPU
	Q_OBJECT

public:
	HalconDeepLearning(QObject *parent = Q_NULLPTR);
	virtual ~HalconDeepLearning();
	virtual void initDlModel(QString modelPath="",int batchSize=0,int threadCount=0);//初始化模型
	void setModelPath(const QString&modelPath);//设置模型路径
	QString getModelPath();//获得模型路径
	void setBatchSize(int size);//设置批处理数
	int getBatchSize(int size);//获得批处理数
	void setUseCPU(bool use);//设置是否强制使用cpu进行解析
	bool isUseCPU();//是否强制使用cpu进行解析
	void setThreadCount(int count);//使用cpu时使用的线程数,默认使用gpu,没有则使用cpu
	int getThreadCount();//获得cpu使用的线程数

protected:

	HalconCpp::HTuple *hv_DLModelHandle_;//模型句柄
	HalconCpp::HTuple *hv_DLPreprocessParam_;//预处理信息
	HalconCpp::HTuple * hv_classNames_;//模型类名
	QString modelPath_;//模型的路径
	int batchSize_;//批处理数
	int threadCount_;//cpu使用线程数
	bool isUseCPU_;//是否强制使用CPU进行解析
};
