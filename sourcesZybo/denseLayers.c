#include "inputData.h"
void dense(float flatten[],float **weights,float *featureVector,int layerNo){

	//calculating feature vector
	int units=denseParams[layerNo-1].units;
	if(layerNo!=1){
		outputShape=denseParams[layerNo-2].units;
	}
	float value=0;
	for(int j=0;j<units;j++){
		for(int i=0;i<outputShape;i++){
			value = flatten[i]*weights[i][j]+value;
		}
		featureVector[j]=value;
		value=0;

	}
}

