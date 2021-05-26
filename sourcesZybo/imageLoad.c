#include "inputData.h"
void bmp_load(char *bmp,float ***imageData)
{
    //short y,x;
   // short XImage,YImage;
    FIL fil;
   // u32 iPixelAddr = 0;
    FRESULT res;
    char TMPBUF[64];
    char line_buf[227*227*3];
    UINT br;
    u32 file_size = (227*227*3);

    res = f_open(&fil,bmp,FA_OPEN_EXISTING | FA_READ);
    if(res != FR_OK)
    {
        return;
    }

    res = f_read(&fil,&TMPBUF,54,&br);
   // xil_printf("brValue:%d",br);
    if(res != FR_OK)
    {
        return;
    }
   // XImage = (unsigned short int)TMPBUF[19]*256 + TMPBUF[18]; //What is calculated here is the Width and Height of the image
    //YImage = (unsigned short int)TMPBUF[23]*256 + TMPBUF[22];

  //  xil_printf("Begin%d ",TMPBUF[11]);xil_printf("%dEnd",TMPBUF[10]); //This shows how much data is invalid

   // xil_printf("%d",XImage);
    //xil_printf("%dEnd",YImage);
    //Before64After the data, the image data from the first1079Start,-54Because it has been read before54Data
         //here1078It is calculated directly by hand, not by program.
   res = f_read(&fil,&line_buf,file_size,&br);
   for(int channel=0;channel<3;channel++){
	   int channelInc=0;
	   for(int row=226;row>=0;row--){
		   for(int col=0;col<227;col++){
			   imageData[channel][row][col]=(float)(unsigned short int)line_buf[channel+channelInc]/255;
			   channelInc = channelInc+3;
		   }
		   channelInc = channelInc+3;
	   }
   }

   // xil_printf("BR%d",br);
   // int i;

    //f_read(&fil,frame,200*200,&br);
    //xil_printf("N%d K%d K%d,",br,frame[0],frame[200*200-1]);

    //xil_printf("Yes");
    f_close(&fil);
}
