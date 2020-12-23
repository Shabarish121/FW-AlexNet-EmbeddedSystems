/*
 * inputData.c
 *
 *  Created on: 18-May-2020
 *      Author: Shabarish
 */


#include "header/inputData.h"

int main(){

	Image image;
	struct timeval tt1, tt2;
	double elapsedTime;
	double ****featureVector;
	double ****featureVectorBuff;
	double *featureVectorDense;
	double *featureVectorDenseBuff;
	double ****imageData=allocMemImage(200,3,227,227);
	int *label,*predLabel;
	int trueImageNo=0;
	double accuracy;
	label=malloc(200*sizeof(int));
	predLabel=malloc(200*sizeof(int));
	char dataPath[50];
	char dataPathFull[50];
	gettimeofday(&tt1, NULL);
	char* flag="file1";
	if(strcmp(flag,"file")==0){
		readImageFile(imageData,label);
	}else{
		for(int i=0;i<4;i++){
			switch(i)
			{
			case 0:strcpy(dataPath,"testImages/Maize/");
			break;
			case 1:strcpy(dataPath,"testImages/Roggen/");
			break;
			case 2:strcpy(dataPath,"testImages/Triticale/");
			break;
			case 3:strcpy(dataPath,"testImages/Wheat/");
			break;
			}
			DIR *folder;
			struct dirent *entry;
			folder = opendir(dataPath);
			if(folder == NULL)
			{
				perror("Unable to read directory");
				return(1);
			}
			while( (entry=readdir(folder)) )
			{
				if(strcmp(entry->d_name,".")!=0){
					if(strcmp(entry->d_name,"..")!=0){
						strcpy(dataPathFull,dataPath);
						strcat(dataPathFull,entry->d_name);
						Image_load(&image, dataPathFull);
						if(image.data == NULL){
							printf("%s",entry->d_name);
						}

						ON_ERROR_EXIT(image.data == NULL, "Error in loading the image");
						imageToArray(&image,imageData[imageNo]);
						label[imageNo]=i;
						imageNo++;
					}
				}
			}
			closedir(folder);
	}

	}



	initParams();
	double *****weightsConv = allocMemWeightsConv();
	double ***weightsDense = allocMemWeightsDense();
	double **biases = allocMemBiases();

	readWeightsConv(weightsConv);
	readWeightsDense(weightsDense);
	readBiases(biases);

	//gettimeofday(&tt1, NULL);
	for(int imageIndex = 0;imageIndex < imageNo;imageIndex++){
		featureVector=reallocMemFeatureVectorConv(1,0);
		Conv2dInput(imageData[imageIndex],weightsConv[0],featureVector);
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


		double flatten[outputShape*outputShape*convParams[4].filters];
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
		printf("output:\n");
		double max=0;
		for(int i=0;i<4;i++){
			if(featureVectorDense[i] > max){
				max = featureVectorDense[i];
				predLabel[imageIndex] = i;
			}
			printf("%f\t",featureVectorDense[i]);
		}
		if(label[imageIndex] == predLabel[imageIndex]){
			trueImageNo++;
		}
	}
	accuracy = (double)trueImageNo/imageNo*100;
	printf("Accuracy of the model = %0.3f%%\n",accuracy);
	gettimeofday(&tt2, NULL);
	elapsedTime = (tt2.tv_sec - tt1.tv_sec) * 1000000.0;      // sec to us
	elapsedTime += (tt2.tv_usec - tt1.tv_usec) ;
	printf("\nTime for inference T=%.0f uSec  => %f second \n\r ", elapsedTime, elapsedTime/1000000);
	return 0;

}



