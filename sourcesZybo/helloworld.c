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
    /*double elapsedTime;
	double ****featureVector;
	double ****featureVectorBuff;
	double *featureVectorDense;
	double *featureVectorDenseBuff;*/
	double ****imageData=allocMemImage(40,3,227,227);
    //UINT frameGet[200*200];

	//int trueImageNo=0;
	//double accuracy;
	int *label=malloc(40*sizeof(int));
	//predLabel=malloc(200*sizeof(int));
	char dataPath[50];
	char dataPathFull[100];
	for(int i=0;i<4;i++){
		switch(i)
		{
		case 0:strcpy(dataPath,"0:/Maize/");
		strcpy(grain,"maize");
		break;
		case 1:strcpy(dataPath,"0:/Roggen/");
		strcpy(grain,"roggen");
		break;
		case 2:strcpy(dataPath,"0:/Triticale/");
		strcpy(grain,"triticale");
		break;
		case 3:strcpy(dataPath,"0:/Wheat/");
		strcpy(grain,"wheat");
		break;
		}

		for(int imagesPerGrain=1;imagesPerGrain<=10;imagesPerGrain++){
			//strcpy(dataPathFull,dataPath);
			sprintf(dataPathFull,"%s%s%d.bmp",dataPath,grain,imagesPerGrain);
			//strcat(dataPathFull,itoa(imagesPerGrain));
			//strcat(dataPathFull,".bmp");
			bmp_load(dataPathFull,imageData[imageNo]);
			label[imageNo]=i;
			imageNo++;
		}

	}
	free(imageData);
	initParams();
	printf("first data=%f",imageData[0][0][0][0]);
	double *****weightsConv = allocMemWeightsConv();
	//double ***weightsDense = allocMemWeightsDense();
	//double **biases = allocMemBiases();
	//readImageFile(imageData);
	printf("first data=%f",imageData[0][0][0][0]);
	readWeightsConv(weightsConv);
	//readWeightsDense(weightsDense);
	//readBiases(biases);

    cleanup_platform();
    return 0;
}
