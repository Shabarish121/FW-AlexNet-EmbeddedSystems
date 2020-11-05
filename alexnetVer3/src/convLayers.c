#include<header/inputData.h>

void Conv2dInput(double*** imageData,double ****weights,double ****featureVector){
	int i,j,k,l,m=0,p;
	int moveX = 0;
	int moveY = 0;
	//=========================
	    //calculating feature map by moving kernel window on the image and writing feature map to text file
	for (m = 0;m < convParams[0].filters ;m++){		//loop for filters
		for (k = 0;k < outputShape; k++){	//loop for Y-featuremap
			for (l=0; l < outputShape; l++){	//loop for X-featuremap

				double featureMapValue = 0;
				for(p=0;p<3;p++){
					for (i = 0; i < convParams[0].kernels; i++){		//loop for Y-kernel
						for (j = 0; j < convParams[0].kernels; j++){	//loop for X-kernel

							featureMapValue = weights[i][j][p][m] * imageData[p][i+moveY][j+moveX] + featureMapValue;
						}
					}
				}
				featureVector[0][k][l][m] = featureMapValue;
				if (l != outputShape-1){
					moveX = moveX + convParams[0].strides;}
				else {moveX = 0	;}

			}

			if (k != outputShape-1){
				moveY = moveY + convParams[0].strides;}
					else {moveY = 0	;}

		}

		}

}

void Conv2d(double ****ptr,double ****weights,double ****featureVector,int layerNo){

	int i,j,k,l,m,p;

	int moveX = 0;
	int moveY = 0;
		    //calculating feature map by moving kernel window on the image and writing feature map to text file
	//double featureVector[1][outputShape][outputShape][convParams[1].filters] ;

	for (m = 0;m < convParams[layerNo-1].filters ;m++){		//loop for filters
		for (k = 0;k < outputShape; k++){	//loop for Y-featuremap
			for (l=0; l < outputShape; l++){	//loop for X-featuremap

				double featureMapValue = 0;
				for(p=0;p<convParams[layerNo-2].filters;p++){		//loop for filters in previous input
					for (i = 0; i < convParams[layerNo-1].kernels; i++){		//loop for Y-kernel
						for (j = 0; j < convParams[layerNo-1].kernels; j++){	//loop for X-kernel
							featureMapValue = weights[i][j][p][m] * ptr[0][i+moveY][j+moveX][p] + featureMapValue;
						}
					}
				}
				featureVector[0][k][l][m] = featureMapValue;
				if (l != outputShape-1){
					moveX = moveX + convParams[layerNo-1].strides;}
				else {moveX = 0	;}

			}

			if (k != outputShape-1){
				moveY = moveY + convParams[layerNo-1].strides;}
					else {moveY = 0	;}

		}

		}

}

