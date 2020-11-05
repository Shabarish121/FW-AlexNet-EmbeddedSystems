#include<sys/types.h> //file input
#include<sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include<header/inputData.h>

void readWeightsConv(double *****weights){
	for(int layerNo=0;layerNo<5;layerNo++){
		int kernel=convParams[layerNo].kernels;
		int prevFilter;
		char buffer1[65536] ;
		char *record2,*line2;
		int a=0,b=0,c=0,d=0;
		FILE *Weight_file;

		if(layerNo+1==1){
			prevFilter=3;
		}else if(layerNo+1>1&&layerNo+1<6){
			prevFilter=convParams[layerNo-1].filters;
		}
		switch(layerNo+1)
		{
		case 1:Weight_file = fopen("weightFiles/FirstLayer.csv","r");
		break;
		case 2:Weight_file = fopen("weightFiles/SecondLayer.csv","r");
		break;
		case 3:Weight_file = fopen("weightFiles/ThirdLayer.csv","r");
		break;
		case 4:Weight_file = fopen("weightFiles/FourthLayer.csv","r");
		break;
		case 5:Weight_file = fopen("weightFiles/FifthLayer.csv","r");
		break;
		}


		if(Weight_file == NULL)
		{
		   printf("\n file opening failed ");
		   //return -1 ;
		}
		while((line2=fgets(buffer1,sizeof(buffer1),Weight_file))!=NULL)
		{
			record2 = strtok(line2,",");
			while(record2 != NULL)
			{
				weights[layerNo][a][b][c][d++] = atof(record2) ;
				record2 = strtok(NULL,",");
			}
			d=0;
			c++;
			if(c==prevFilter){
				c=0;
				b++;
			}
			if(b==kernel){
				b=0;
				a++;
			}
		}
		fclose(Weight_file);
	}


}

void readWeightsDense(double ***weights){
	for(int layerNo=0;layerNo<3;layerNo++){
		char buffer1[65536] ;
		char *record2,*line2;
		int a=0,b=0;
		FILE *Weight_file;
		int units=denseParams[layerNo].units;
		switch(layerNo+1)
		{
		case 1:Weight_file = fopen("weightFiles/sixthLayer.csv","r");
		break;
		case 2:Weight_file = fopen("weightFiles/seventhLayer.csv","r");
		break;
		case 3:Weight_file = fopen("weightFiles/eighthLayer.csv","r");
		break;
		}
		if(Weight_file == NULL)
		{
		   printf("\n file opening failed ");
		   //return -1 ;
		}

		while((line2=fgets(buffer1,sizeof(buffer1),Weight_file))!=NULL)
		{
		  record2 = strtok(line2,",");
		  while(record2 != NULL)
		  {
			  weights[layerNo][a][b++]= atof(record2) ;
			  record2 = strtok(NULL,",");

		  }
		  if(b==units){
			  b=0;
			  a++;
		  }
		 }
		fclose(Weight_file);
	}
}



void readBiases(double **weightsAct){
	char buffer1[65536] ;
	char *record2,*line2;
	int a=0,b=0;
	FILE *Weight_file;
	Weight_file = fopen("weightFiles/biases.csv","r");
	if(Weight_file == NULL)
	{
	   printf("\n file opening failed ");
	   //return -1 ;
	}

	while((line2=fgets(buffer1,sizeof(buffer1),Weight_file))!=NULL)
	{
	  record2 = strtok(line2,",");
	  while(record2 != NULL)
	  {
		  weightsAct[a][b++]= atof(record2) ;
		  record2 = strtok(NULL,",");

	  }
	  b=0;
	  a++;
	}
	fclose(Weight_file);

}

