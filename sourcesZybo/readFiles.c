#include "inputData.h"

void readWeightsConv(double *****weights){

	for(int layerNo=0;layerNo<5;layerNo++){
		int kernel=convParams[layerNo].kernels;
		int filter=convParams[layerNo].filters;
		int prevFilter;
		int a=0,b=0,c=0,d=0;
		FIL filePointer;
		char * temp,*line,*value;
		if(layerNo+1==1){
			prevFilter=3;
		}else if(layerNo+1>1&&layerNo+1<6){
			prevFilter=convParams[layerNo-1].filters;
		}
		u32 file_size = (kernel*kernel * filter * prevFilter);
		UINT NumBytesRead;
		char DestinationAddress[kernel*kernel * filter * prevFilter];
		FRESULT result;
		switch(layerNo+1)
		{
		case 1:result = f_open( & filePointer, "0:/First.csv", FA_READ);
		break;
		case 2:result = f_open( & filePointer, "0:/Second.csv", FA_READ);
		break;
		case 3:result = f_open( & filePointer, "0:/Third.csv", FA_READ);
		break;
		case 4:result = f_open( & filePointer, "0:/Fourth.csv", FA_READ);
		break;
		case 5:result = f_open( & filePointer, "0:/Fifth.csv", FA_READ);
		break;
		}


		if(result != FR_OK)
		{
		   printf("\n file opening failed ");
		   //return -1 ;
		}
		FRESULT read_result = f_read( & filePointer, (void * ) DestinationAddress, file_size, & NumBytesRead);
		  if (read_result == FR_OK) {
		    line = strtok_r(DestinationAddress, "\n", & temp);
		    while (line != NULL)
		    {
		    	value = strtok_r(line, ",", & line);

				while(value != NULL)
				{
					value = strtok_r(NULL, ",", & line);
					weights[layerNo][a][b][c][d++] = atof(value) ;
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
				line = strtok_r(NULL, "\n", & temp);
		    	}

		  }
	}


}

void readWeightsDense(double ***weights){
	for(int layerNo=0;layerNo<3;layerNo++){
		char buffer1[65536] ;
		char *record2,*line2;
		int a=0,b=0,prevInput;
		FIL filePointer;
		int units=denseParams[layerNo].units;
		if(layerNo+1==1){
			prevInput=1024;
		}
		else{
			prevInput=denseParams[layerNo-1].units;
		}
		u32 file_size = (prevInput*units);
		UINT NumBytesRead;
		char DestinationAddress[prevInput*units];
		char * temp,*line,*value;
		FRESULT result;
		switch(layerNo+1)
		{
		case 1:result = f_open( & filePointer, "0:/sixth.csv", FA_READ);
		break;
		case 2:result = f_open( & filePointer, "0:/seventh.csv", FA_READ);
		break;
		case 3:result = f_open( & filePointer, "0:/eighth.csv", FA_READ);
		break;
		}
		if(result != FR_OK)
		{
		   printf("\n file opening failed ");
		   //return -1 ;
		}
		FRESULT read_result = f_read( & filePointer, (void * ) DestinationAddress, file_size, & NumBytesRead);

		if (read_result == FR_OK) {
			line = strtok_r(DestinationAddress, "\n", & temp);
		    while (line != NULL)
		    {
		    	value = strtok_r(line, ",", & line);
		    	while(value != NULL)
		    	{
		    		value = strtok_r(NULL, ",", & line);
		    		weights[layerNo][a][b++]= atof(value) ;
		    	}
		    	if(b==units){
		    		b=0;
		    		a++;
		    	}
		    	line = strtok_r(NULL, "\n", & temp);
		    }
		}
	}
}



void readBiases(double **weightsAct){
	char buffer1[65536] ;
	char *record2,*line2;
	int a=0,b=0;
	FRESULT result;
	FIL filePointer;
	u32 file_size = (convParams[0].filters+convParams[1].filters+convParams[2].filters+convParams[3].filters+convParams[4].filters+denseParams[0].units+denseParams[1].units+denseParams[2].units);
	UINT NumBytesRead;
	char DestinationAddress[convParams[0].filters+convParams[1].filters+convParams[2].filters+convParams[3].filters+convParams[4].filters+denseParams[0].units+denseParams[1].units+denseParams[2].units];
	char * temp,*line,*value;
	result = f_open( & filePointer, "0:/biases.csv", FA_READ);
	if(result != FR_OK)
	{
	   printf("\n file opening failed ");
	   //return -1 ;
	}
	FRESULT read_result = f_read( & filePointer, (void * ) DestinationAddress, file_size, & NumBytesRead);

	if (read_result == FR_OK) {
		line = strtok_r(DestinationAddress, "\n", & temp);
	    while (line != NULL)
	    {
	    	value = strtok_r(line, ",", & line);
	    	while(record2 != NULL)
	    	{
	    		weightsAct[a][b++]= atof(record2) ;
	    		record2 = strtok(NULL,",");

	    	}
	    	b=0;
	    	a++;
	    	line = strtok_r(NULL, "\n", & temp);
	    }
	}

}

