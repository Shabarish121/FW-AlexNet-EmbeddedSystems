/*
 * inputData.h
 *
 *  Created on: 25-Sep-2020
 *      Author: Shabarish
 */

#ifndef INPUTDATA_H_
#define INPUTDATA_H_

#include "header/Image.h"
#include "utils.h"
#include<sys/types.h>
#include<sys/stat.h>
#include <sys/time.h>
#include <math.h>
#include "allocMem.h"
#include <dirent.h> //reading directory

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

void Conv2dInput(double*** imageData,double ****weights,double ****featureVector);
void Conv2d(double ****ptr,double ****weights,double ****featureVector,int layerNo);
void dense(double flatten[],double **weights,double *featureVector,int layerNo);
void MaxPooling(double ****ptr,double ****featureVector,int layerNo);
void ActivationReluConv(double ****ptr,int layerNo,double *weights);
void ActivationReluDense(double *ptr,int layerNo,double *weights);
void ActivationSoftmax(double *ptr,double *weights);
void imageToArray(Image *image,double ***array);
void initParams();
void readWeightsConv(double *****weights);
void readWeightsDense(double ***weights);
void readBiases(double **weightsAct);

int outputShape;

//extern const Conv2 conv2[];
#endif /* INPUTDATA_H_ */
