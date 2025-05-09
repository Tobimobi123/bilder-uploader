#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

void setup() {
    printf("EPD_1IN64G_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_1IN64G_Init();
    EPD_1IN64G_Clear(EPD_1IN64G_WHITE);
    DEV_Delay_ms(2000);

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_1IN64G_WIDTH % 4 == 0)? (EPD_1IN64G_WIDTH / 4 ): (EPD_1IN64G_WIDTH / 4 + 1)) * EPD_1IN64G_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while (1);
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_1IN64G_WIDTH, EPD_1IN64G_HEIGHT, 0, EPD_1IN64G_WHITE);
    Paint_SetScale(4);

#if 1   // show bmp
    printf("show BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_1in64g);
    EPD_1IN64G_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif        

#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_1IN64G_WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, EPD_1IN64G_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_1IN64G_YELLOW, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_1IN64G_BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_1IN64G_RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_1IN64G_RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_1IN64G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_1IN64G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_1IN64G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_1IN64G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_1IN64G_RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_1IN64G_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "Red, yellow, white and black", &Font16, EPD_1IN64G_RED, EPD_1IN64G_YELLOW);
    Paint_DrawString_EN(10, 35, "Four color e-Paper", &Font12, EPD_1IN64G_YELLOW, EPD_1IN64G_BLACK);
    Paint_DrawString_CN(10, 125, "微雪电子", &Font24CN, EPD_1IN64G_RED, EPD_1IN64G_WHITE);
    Paint_DrawNum(10, 50, 123456, &Font12, EPD_1IN64G_RED, EPD_1IN64G_WHITE);

    printf("EPD_Display\r\n");
    EPD_1IN64G_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_1IN64G_WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawRectangle(1, 1, 168, 55, EPD_1IN64G_RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(1, 112, 167, 167, EPD_1IN64G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(59, 1, 109, 167, EPD_1IN64G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    printf("EPD_Display\r\n");
    EPD_1IN64G_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_1IN64G_WHITE);

    int hNumber, hWidth, vNumber, vWidth;
    hNumber = 8;
	hWidth = EPD_1IN64G_HEIGHT/hNumber; // 168/16=21
    vNumber = 8;
	vWidth = EPD_1IN64G_WIDTH/vNumber; // 168/16=21

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
	for(int i=0; i<hNumber; i++) {  // horizontal
		Paint_DrawRectangle(1, 1+i*hWidth, EPD_1IN64G_WIDTH, hWidth*(1+i), EPD_1IN64G_BLACK + (i % 2), DOT_PIXEL_1X1, DRAW_FILL_FULL);
	}
	for(int i=0; i<vNumber; i++) {  // vertical
        if(i%2) {
            Paint_DrawRectangle(1+i*vWidth, 1, vWidth*(i+1), EPD_1IN64G_HEIGHT, EPD_1IN64G_YELLOW + (i/2%2), DOT_PIXEL_1X1, DRAW_FILL_FULL);
        }
	}

    printf("EPD_Display\r\n");
    EPD_1IN64G_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

    printf("Clear...\r\n");
    EPD_1IN64G_Clear(EPD_1IN64G_WHITE);

    printf("Goto Sleep...\r\n");
    EPD_1IN64G_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  //
}
