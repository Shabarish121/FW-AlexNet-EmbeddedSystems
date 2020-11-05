#include<header/inputData.h>
void imageToArray(Image *image,double*** array){
	//storing image data from pointer to local variable
	int width = image->width;int height = image->height;int range = width*height*3;
	int i,j,k,l,m=0;
	static int fullRange = 0;
	for(k=2;k>=0;k--){
		l=0;
		for (i = 0;i < width;i++){
			for (j = 0;j < height;j++){
				array[m][i][j] = (double)*(image->data +k+l)/255;
				l=l+3;
				fullRange++;
				if (fullRange == range){
					break;
				}
			}
		}
		m++;
	}

}
