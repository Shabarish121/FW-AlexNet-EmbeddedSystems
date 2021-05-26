/*
 * inputData.h
 *
 *  Created on: 25-Sep-2020
 *      Author: Shabarish
 */

#ifndef INPUTDATA_H_
#define INPUTDATA_H_

#include <stdio.h>
#include "utils.h"
#include<sys/types.h>
#include<sys/stat.h>
#include <sys/time.h>
#include <math.h>
#include "allocMem.h"
#include <stdlib.h>
#include "ff.h"
#include "xil_printf.h"
struct Conv{
	int filters;
	int kernels;
	int strides;
}convParams[5];

struct Pooling{
	int kernels;
	int strides;
}poolParams[3];

struct Dense{
	int units;
}denseParams[3];
int imageNo;
int imagesPerClass;

void Conv2dInput(float*** imageData,float ****weights,float ****featureVector);
void Conv2d(float ****ptr,float ****weights,float ****featureVector,int layerNo);
void dense(float flatten[],float **weights,float *featureVector,int layerNo);
void MaxPooling(float ****ptr,float ****featureVector,int layerNo);
void ActivationReluConv(float ****ptr,int layerNo,float *weights);
void ActivationReluDense(float *ptr,int layerNo,float *weights);
void ActivationSoftmax(float *ptr,float *weights);
void initParams();
void readWeightsConv(float *****weights);
void readWeightsDense(float ***weights);
void readBiases(float **weightsAct);
void bmp_load(char *bmp,float ***imageData);

int outputShape;

//extern const Conv2 conv2[];
#endif /* INPUTDATA_H_ */
