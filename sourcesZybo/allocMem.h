#include <stdlib.h>
#include "inputData.h"
#ifndef HEADER_ALLOCMEM_H_
#define HEADER_ALLOCMEM_H_
float ****allocMemImage(int noOfImages,int channel,int width,int height);
float *****allocMemWeightsConv();
float ***allocMemWeightsDense();
float **allocMemBiases();
float ****reallocMemFeatureVectorConv(int layerNo,bool pooling);
float *reallocMemFeatureVectorDense(int layerNo);

#endif
