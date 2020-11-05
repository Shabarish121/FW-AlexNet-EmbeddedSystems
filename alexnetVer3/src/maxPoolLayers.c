#include "header/inputData.h"

void MaxPooling(double ****ptr,double ****featureVector,int layerNo){

	double max=0;
	int moveX=0,moveY=0;
	int filter=convParams[layerNo-1].filters;
	int kernel=poolParams[layerNo-1].kernels;
	int stride=poolParams[layerNo-1].strides;
	if(layerNo==3){
		filter=convParams[4].filters;
	}
	for(int m=0;m<filter;m++){
		for(int i =0;i<outputShape;i++){
			for(int j=0;j<outputShape;j++){
				for(int k=0;k<kernel;k++){
					for(int l=0;l<poolParams[layerNo-1].kernels;l++){
						if (ptr[0][k+moveY][l+moveX][m]>max){
							max=ptr[0][k+moveY][l+moveX][m];
						}
					}

				}
				featureVector[0][i][j][m]=max;
				max=0;
				if (j != outputShape-1){
					moveX = moveX + stride;}
				else {moveX = 0	;}


			}
			if (i != outputShape-1){
				moveY = moveY + stride;}
			else {moveY = 0	;}
		}
	}
}

