#pragma once
#  include "HalconCpp.h"
#  include "HDevThread.h"
using namespace HalconCpp;


extern void lensImgCheck(HObject ho_Image, HTuple hv_DLModelHandle, HTuple hv_DLPreprocessParam,
	HTuple *hv_lacks);