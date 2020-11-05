/*
 * allocMem.h
 *
 *  Created on: 08-Oct-2020
 *      Author: Shabarish
 */
#include "header/inputData.h"
#ifndef HEADER_ALLOCMEM_H_
#define HEADER_ALLOCMEM_H_

double ****allocMemImage(int noOfImages,int channel,int width,int height);
double *****allocMemWeightsConv();
double ***allocMemWeightsDense();
double **allocMemBiases();
double ****reallocMemFeatureVectorConv(int layerNo,bool pooling);
double *reallocMemFeatureVectorDense(int layerNo);


#endif /* HEADER_ALLOCMEM_H_ */
