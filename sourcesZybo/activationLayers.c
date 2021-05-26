#include "inputData.h"

void ActivationReluConv(float ****ptr,int layerNo,float *weights){
	for (int m = 0;m < convParams[layerNo-1].filters ;m++){		//loop for filters
		for (int k = 0;k < outputShape; k++){	//loop for Y-featuremap
			for (int l=0; l < outputShape; l++){	//loop for X-featuremap

					if (ptr[0][k][l][m] <0){
						ptr[0][k][l][m]=0;

						}
					else {
						ptr[0][k][l][m]=ptr[0][k][l][m]+weights[m];
					}

			}



		}
	}
}


void ActivationReluDense(float *ptr,int layerNo,float *weights){
	for(int i=0;i<denseParams[layerNo-1].units;i++){
		if(ptr[i]<0){
			ptr[i]=0;
		}
		else{
			ptr[i]=ptr[i]+weights[i];
		}
	}
}




void ActivationSoftmax(float *ptr,float *weights){
	float max=0;
	float sum=0;
	//function to get maximum value in array
	for(int i=0;i<denseParams[2].units;i++){
		if(ptr[i]>max){
			max=ptr[i];
		}
	}
	for(int i=0;i<denseParams[2].units;i++){
		ptr[i]=exp(ptr[i]-max);
		sum=ptr[i]+sum;
	}
	for(int i=0;i<denseParams[2].units;i++){
		ptr[i]=ptr[i]/sum;
		ptr[i]=ptr[i]+weights[i];
	}
}
