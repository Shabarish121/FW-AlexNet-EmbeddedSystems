/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * alexNet.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */


#include "platform.h"
#include "inputData.h"
#include "xtime_l.h"

static FATFS fatfs;
void mount_sd_card(){

	TCHAR *Path = "0:/";

	FRESULT Res;

	Res = f_mount(&fatfs, Path, 1);

	if (Res != FR_OK) {
		printf("Not OK \n");
	}
	if(Res == FR_OK){
		printf("SD Card mounted successfully \n");
	}


}
int main()
{
	char grain[10];
    init_platform();
    mount_sd_card();
    XTime tStart,tEnd;
    //float elapsedTime;
	float ****featureVector;
	float ****featureVectorBuff;
	float *featureVectorDense;
	float *featureVectorDenseBuff;
	float ****imageData=allocMemImage(1,3,227,227);
   	int trueImageNo=0;
	float accuracy;
	int *label=malloc(100*sizeof(int));
	int *predLabel=malloc(100*sizeof(int));
	char dataPath[50];
	char dataPathFull[100];
	double tLap=0;
	initParams();
	//printf("first data=%f",imageData[0][0][0][0]);
	float *****weightsConv = allocMemWeightsConv();
	float ***weightsDense = allocMemWeightsDense();
	float **biases = allocMemBiases();

	readWeightsConv(weightsConv);
	readWeightsDense(weightsDense);
	readBiases(biases);
	printf("   Test image\t   Actual Label\t  Predicted Label\n");
	for(int labelIndex=0;labelIndex<4;labelIndex++){
		switch(labelIndex)
		{
		case 0:strcpy(dataPath,"0:/Maize/");
		strcpy(grain,"maize");
		break;
		case 1:strcpy(dataPath,"0:/Roggen/");
		strcpy(grain,"roggen");
		break;
		case 2:strcpy(dataPath,"0:/Trit/");
		strcpy(grain,"trit");
		break;
		case 3:strcpy(dataPath,"0:/Wheat/");
		strcpy(grain,"wheat");
		break;
		}

		for(int imagesPerGrain=1;imagesPerGrain<=imagesPerClass;imagesPerGrain++){
			//strcpy(dataPathFull,dataPath);
			sprintf(dataPathFull,"%s%s%d.bmp",dataPath,grain,imagesPerGrain);
			//strcat(dataPathFull,itoa(imagesPerGrain));
			//strcat(dataPathFull,".bmp");
			bmp_load(dataPathFull,imageData[0]);
			label[imageNo]=labelIndex;

			XTime_GetTime(&tStart);

			featureVector=reallocMemFeatureVectorConv(1,0);
			Conv2dInput(imageData[0],weightsConv[0],featureVector);
			ActivationReluConv(featureVector,1,biases[0]);
			featureVectorBuff=reallocMemFeatureVectorConv(1,1);
			MaxPooling(featureVector,featureVectorBuff,1);

			featureVector=reallocMemFeatureVectorConv(2,0);
			Conv2d(featureVectorBuff,weightsConv[1],featureVector,2);
			ActivationReluConv(featureVector,2,biases[1]);
			featureVectorBuff=reallocMemFeatureVectorConv(2,1);
			MaxPooling(featureVector,featureVectorBuff,2);

			featureVector=reallocMemFeatureVectorConv(3,0);
			Conv2d(featureVectorBuff,weightsConv[2],featureVector,3);
			ActivationReluConv(featureVector,3,biases[2]);

			featureVectorBuff=reallocMemFeatureVectorConv(4,0);
			Conv2d(featureVector,weightsConv[3],featureVectorBuff,4);
			ActivationReluConv(featureVectorBuff,4,biases[3]);

			featureVector=reallocMemFeatureVectorConv(5,0);
			Conv2d(featureVectorBuff,weightsConv[4],featureVector,5);
			ActivationReluConv(featureVector,5,biases[4]);
			featureVectorBuff=reallocMemFeatureVectorConv(5,1);
			MaxPooling(featureVector,featureVectorBuff,3);


			float flatten[outputShape*outputShape*convParams[4].filters];
			int flattenIndex=0;
			for(int i=0;i<outputShape;i++){
				for(int j=0;j<outputShape;j++){
					for(int k=0;k<convParams[4].filters;k++){
						flatten[flattenIndex]=featureVectorBuff[0][i][j][k];
						flattenIndex++;

					}

				}
			}


			featureVectorDense=reallocMemFeatureVectorDense(1);
			dense(flatten,weightsDense[0],featureVectorDense,1);
			ActivationReluDense(featureVectorDense,1,biases[5]);

			featureVectorDenseBuff=reallocMemFeatureVectorDense(2);
			dense(featureVectorDense,weightsDense[1],featureVectorDenseBuff,2);
			ActivationReluDense(featureVectorDenseBuff,2,biases[6]);

			featureVectorDense=reallocMemFeatureVectorDense(3);
			dense(featureVectorDenseBuff,weightsDense[2],featureVectorDense,3);

			ActivationSoftmax(featureVectorDense,biases[7]);
			//printf("output:\n");
			double max=0;
			for(int i=0;i<4;i++){
				if(featureVectorDense[i] > max){
					max = featureVectorDense[i];
					predLabel[imageNo] = i;
				}
				//printf("%f\t",featureVectorDense[i]);

			}

			if(label[imageNo] == predLabel[imageNo]){
				trueImageNo++;
			}
			imageNo++;
			free(featureVector);
			free(featureVectorBuff);
			free(featureVectorDense);
			free(featureVectorDenseBuff);
			printf("\t%d\t",imageNo  );
			printf("\t%d\t",label[imageNo-1]  );
			printf("\t%d\n",predLabel[imageNo-1] );
			XTime_GetTime(&tEnd);
			tLap = tLap+ 1.0*(tEnd-tStart);
		}
	}

	accuracy = (double)trueImageNo/imageNo*100;
	printf("Accuracy of the model = %0.3f%%\n",accuracy);
	tLap = tLap/COUNTS_PER_SECOND;

	printf("Output took %.2f s.\n",tLap);
    cleanup_platform();
    return 0;
}
