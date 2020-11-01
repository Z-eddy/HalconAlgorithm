#include "HalconDeepLearning.h"
#include<QDebug>
#include<cstring>
#include "HalconCpp.h"

using namespace HalconCpp;
extern void check_dl_devices(HTuple *hv_PossibleRuntimes);
extern void create_dl_preprocess_param_from_model (HTuple hv_DLModelHandle, HTuple hv_NormalizationType, 
    HTuple hv_DomainHandling, HTuple hv_SetBackgroundID, HTuple hv_ClassIDsBackground, 
    HTuple hv_GenParam, HTuple *hv_DLPreprocessParam);

QImage hImage2Pixmap(QSharedPointer<HalconCpp::HImage> halconImage)
{
	if (halconImage.isNull() || !halconImage->IsInitialized()) {
		return {};
	}
	Hlong width;
	Hlong height;
	halconImage->GetImageSize(&width, &height);

	HTuple channels = halconImage->CountChannels();
	HTuple type = halconImage->GetImageType();

	if (strcmp(type[0].S(), "byte")) // 如果不是 byte 类型，则失败
	{
		return {};
	}

	QImage::Format format;
	switch (channels[0].I())
	{
	case 1:
		format = QImage::Format_Grayscale8;
		break;
	case 3:
		format = QImage::Format_RGB32;
		break;
	default:
		return {};
	}

	QImage image;
	if (image.width() != width || image.height() != height || image.format() != format)
	{
		image = QImage(static_cast<int>(width),
			static_cast<int>(height),
			format);
	}
	HString Type;
	if (channels[0].I() == 1)
	{
		unsigned char * pSrc = reinterpret_cast<unsigned char *>(halconImage->GetImagePointer1(&Type, &width, &height));
		memcpy(image.bits(), pSrc, static_cast<size_t>(width) * static_cast<size_t>(height));
	}
	else if (channels[0].I() == 3)
	{
		uchar *R, *G, *B;
		halconImage->GetImagePointer3(reinterpret_cast<void **>(&R),
			reinterpret_cast<void **>(&G),
			reinterpret_cast<void **>(&B), &Type, &width, &height);

		for (int row = 0; row < height; row++)
		{
			QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(row));
			for (int col = 0; col < width; col++)
			{
				line[col] = qRgb(*R++, *G++, *B++);
			}
		}
	}
	return image;
}


HalconDeepLearning::HalconDeepLearning(QObject *parent /*= Q_NULLPTR*/)
	:hv_DLModelHandle_(new HTuple),\
	hv_DLPreprocessParam_(new HTuple),\
	hv_classNames_(new HTuple),\
	batchSize_(1),\
	threadCount_(4),\
	isUseCPU_(false)
{
}

HalconDeepLearning::~HalconDeepLearning()
{
	delete hv_DLModelHandle_;
	delete hv_DLPreprocessParam_;
	delete hv_classNames_;
}

void HalconDeepLearning::setModelPath(const QString&modelPath)
{
	modelPath_ = modelPath;
}

QString HalconDeepLearning::getModelPath()
{
	return modelPath_;
}

void HalconDeepLearning::setBatchSize(int size)
{
	batchSize_ = size;
}

int HalconDeepLearning::getBatchSize(int size)
{
	return batchSize_;
}

void HalconDeepLearning::setUseCPU(bool use)
{
	isUseCPU_ = use;
}

bool HalconDeepLearning::isUseCPU()
{
	return isUseCPU_;
}

void HalconDeepLearning::setThreadCount(int count)
{
	threadCount_ = count;
}

int HalconDeepLearning::getThreadCount()
{
	return threadCount_;
}

void HalconDeepLearning::initDlModel(QString modelPath,int batchSize,int threadCount)
{
	if (!modelPath.isEmpty()) {
		setModelPath(modelPath);
	}
	if (batchSize) {
		setBatchSize(batchSize);
	}
	if (threadCount) {
		setThreadCount(threadCount);
	}
	try
	{
		HTuple  hv_modelFile{ &(modelPath_.toStdString()[0]) };//模型路径
		//加载训练好的网络Model
		ReadDlModel(hv_modelFile, hv_DLModelHandle_);
		//获得模型类名
		GetDlModelParam(*hv_DLModelHandle_, "class_names", hv_classNames_);
		//检查可以用的硬件(cpu/gpu)
		HTuple hv_PossibleRuntimes;
		check_dl_devices(&hv_PossibleRuntimes);
		//有gpu则设置为gpu训练
		HTuple hv_Runtime;//运行时使用gpu/cpu
		if (0 != (hv_PossibleRuntimes.TupleRegexpTest("gpu")) && !isUseCPU_)
		{
			hv_Runtime = "gpu";
		}
		else if (0 != (hv_PossibleRuntimes.TupleRegexpTest("cpu")))
		{
			hv_Runtime = "cpu";
			//设定cpu线程数
			SetSystem("thread_num", threadCount_);//使用4个线程
		}
		else
		{
			throw HException("No supported device found to continue this example.");
		}
		SetDlModelParam(*hv_DLModelHandle_, "runtime", hv_Runtime);
		//每4个镜片一起测试
		SetDlModelParam(*hv_DLModelHandle_, "batch_size", batchSize_);
		//创建预处理的信息字典
		create_dl_preprocess_param_from_model(*hv_DLModelHandle_, "none", "full_domain",
			HTuple(), HTuple(), HTuple(), hv_DLPreprocessParam_);
	}
	catch(HException &e){
		qDebug()<<e.ErrorMessage().ToLocal8bit();
	}
}
