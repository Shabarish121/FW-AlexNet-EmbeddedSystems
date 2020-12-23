#include"allocMem.h"
double ****allocMemImage(int noOfImages,int channel,int width,int height){
	int d,e,f;
	double ****array;
	array = malloc(noOfImages*sizeof(double *));
	for (d=0;d<noOfImages;d++){
		array[d]=malloc(channel*sizeof(double *));;
	}
	//array = malloc(channel*sizeof(int *));
	for (d=0;d<noOfImages;d++){
		for(e=0;e<channel;e++){
			array[d][e]=malloc(width*sizeof(double *));;
		}

	}
	for(d=0;d<noOfImages;d++){
		for(e=0;e<channel;e++){
			for(f=0;f<width;f++){
				array[d][e][f]=malloc(height*sizeof(double));
			}
		}
	}
	return array;

}

double *****allocMemWeightsConv(){
	int d,e,f;
	double *****array;
	int kernel,filter,prevFilter;
	array=malloc(5*sizeof(double *));
	for(int layerNo=0;layerNo<5;layerNo++){


		kernel=convParams[layerNo].kernels;
		filter=convParams[layerNo].filters;
		if(layerNo==0){
			prevFilter=3;
		}else if(layerNo>0&&layerNo<5){
			prevFilter=convParams[layerNo-1].filters;
		}
		array[layerNo]=malloc(kernel*sizeof(double *));

		for (d=0;d<kernel;d++){
			array[layerNo][d]=malloc(kernel*sizeof(double *));
		}


		for(d=0;d<kernel;d++){
			for(e=0;e<kernel;e++){
				array[layerNo][d][e]=malloc(prevFilter*sizeof(double *));
			}
		}

		for(d=0;d<kernel;d++){
			for(e=0;e<kernel;e++){
				for(f=0;f<prevFilter;f++){
					array[layerNo][d][e][f]=malloc(filter*sizeof(double));
				}
			}
		}
	}
	return array;

}
double ***allocMemWeightsDense(){
	int d;
	double ***array;
	int prevInput;
	array=malloc(3*sizeof(double *));
	for(int layerNo=0;layerNo<3;layerNo++){
		int units=denseParams[layerNo].units;
		if(layerNo==0){
			prevInput=1024;
		}else{
			prevInput=denseParams[layerNo-1].units;
		}
		array[layerNo]= malloc(prevInput*sizeof(double *));
		for (d=0;d<prevInput;d++){
			array[layerNo][d]=malloc(units*sizeof(double));
		}
	}

	return array;

}

double **allocMemBiases(){
	double **array;
	int i;
	array=malloc(8*sizeof(double *));
	for(i=0;i<5;i++){
		array[i] = malloc(convParams[i].filters*sizeof(double));
	}
	for(i=5;i<8;i++){
		array[i] = malloc(denseParams[i-5].units*sizeof(double));
	}
	return array;

}
double ****reallocMemFeatureVectorConv(int layerNo,bool pooling){

	int e,f;
	double ****array;
	int filter=convParams[layerNo-1].filters;
	if(layerNo==1){
		if(!pooling){
			outputShape = ceil(((227-convParams[0].kernels)/convParams[0].strides+1));
		}else{
			outputShape = ceil(((outputShape-poolParams[0].kernels)/poolParams[0].strides+1));
		}

	}else if(layerNo>1&&layerNo<6){
		if(!pooling){
			outputShape = ceil(((outputShape-convParams[layerNo-1].kernels)/convParams[layerNo-1].strides+1));
		}else{
			outputShape = ceil(((outputShape-poolParams[abs(layerNo-3)].kernels)/poolParams[abs(layerNo-3)].strides+1));
		}
	}
	array = malloc(1*sizeof(double *));
	array[0]=malloc(outputShape*sizeof(double *));

	for(e=0;e<outputShape;e++){
		array[0][e]=malloc(outputShape*sizeof(double *));
	}


	for(e=0;e<outputShape;e++){
		for(f=0;f<outputShape;f++){
			array[0][e][f]=malloc(filter*sizeof(double));
		}
	}
	return array;
}

double *reallocMemFeatureVectorDense(int layerNo){

	double *array;
	int units=denseParams[layerNo-1].units;
	outputShape=outputShape*outputShape*convParams[4].filters;
	array = malloc(units*sizeof(double));

	return array;
}

