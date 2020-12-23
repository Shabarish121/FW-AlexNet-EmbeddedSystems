#include "inputData.h"
void initParams(){
	int filtersConv[5]={96,256,384,384,256};
	int kernelsConv[5]={11,5,3,3,3};
	int stridesConv[5]={4,1,1,1,1};
	int kernelsPooling[3]={3,3,3};
	int stridesPooling[3]={2,2,2};
	int unitsDense[3]={512,64,4};
	for(int i=0;i<5;i++){
		convParams[i].filters=filtersConv[i];
		convParams[i].kernels=kernelsConv[i];
		convParams[i].strides=stridesConv[i];
	}
	for(int i=0;i<3;i++){
		poolParams[i].kernels=kernelsPooling[i];
		poolParams[i].strides=stridesPooling[i];
	}
	for(int i=0;i<3;i++){
		denseParams[i].units=unitsDense[i];
	}
}

