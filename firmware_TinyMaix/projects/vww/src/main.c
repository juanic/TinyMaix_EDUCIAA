/** \addtogroup Template Aplicación
 ** @{ */
/** \addtogroup Template Aplicación
 ** @{ */

/*==================[inclusions]=============================================*/

#include "systemclock.h"

#include "printf.h"

#include "uart.h"


#include "fpu_init.h"
#include "printf.h"
#include "ili9341.h"
#include "fonts.h"
#include "gpio.h"
#include "spi.h"
#include "led.h"

#define ARM_MATH_CM4
#define __FPU_PRESENT 1
#include "arm_math.h"
#include "arm_const_structs.h"

LPC_USART_T* UART = LPC_USART2;
//#include "stdio.h"
#include "tinymaix.h"
#include "Syscalls.h"

#define MARGIN 20			/*!< Distance in pixels from left border to start writing */
#define OLED	1
#include "vww96_q.h"
#include "pic_person1.h"
#include "pic_person_oled.h"

#define IMG_L 96
#define IMG_CH 3
#define CLASS_NUM 2
char* labels[CLASS_NUM] = {"no-person", "person"};


static uint32_t _t0,_t1;
static tm_err_t layer_cb(tm_mdl_t* mdl, tml_head_t* lh)
{   //dump middle result
    int h = lh->out_dims[1];
    int w = lh->out_dims[2];
    int ch= lh->out_dims[3];
    mtype_t* output = TML_GET_OUTPUT(mdl, lh);
    //TM_PRINTF("Layer %d callback ========\n", mdl->layer_i);
    #if 0
    for(int y=0; y<h; y++){
        TM_PRINTF("[");
        for(int x=0; x<w; x++){
            TM_PRINTF("[");
            for(int c=0; c<ch; c++){
            #if TM_MDL_TYPE == TM_MDL_FP32
                TM_PRINTF("%.3f,", output[(y*w+x)*ch+c]);
            #else
                TM_PRINTF("%.4f,", TML_DEQUANT(lh,output[(y*w+x)*ch+c]));
            #endif
            }
            TM_PRINTF("],");
        }
        TM_PRINTF("],\n");
    }
    TM_PRINTF("\n");
    #endif
    _t1 = TM_GET_US();
    //TM_PRINTF("===L%d use %.3f ms\n", mdl->layer_i, (float)(_t1-_t0)/1000.0);
    _t0 = TM_GET_US();

    return TM_OK;
}

static void parse_output(tm_mat_t* outs)
{
    tm_mat_t out = outs[0];
    float* data  = out.dataf;
    float maxp = 0;
    int maxi = -1;
    for(int i=0; i<CLASS_NUM; i++){
    	printf("%d: %.3f\r\n", i, data[i]);
        if(data[i] > maxp) {
            maxi = i;
            maxp = data[i];
        }
    }
    TM_PRINTF("### Predict output is: Class %d (%s), Prob %.3f\r\n", maxi, labels[maxi], maxp);
#if OLED
    ILI9341DrawString(MARGIN, 190,"### Predict output is:", &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawString(MARGIN, 210,"Class", &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawInt(MARGIN + 7*6, 210, maxi , 4, &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawString(MARGIN, 230,"Prob:", &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawInt(MARGIN + 7*6, 230, maxp*100 , 2, &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawString(MARGIN + 7*8, 230,"%", &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawString(MARGIN, 250, labels[maxi], &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
#endif
    return;
}


serial_config UART_USB;
int main(int argc, char** argv)
{   TM_DBGT_INIT();
	SystemCoreClockUpdate();
	LedsInit();

	UART_USB.baud_rate = 115200;
	UART_USB.port = SERIAL_PORT_PC;
	UART_USB.pSerial = NULL;
	UartInit(&UART_USB);
#if OLED
	ILI9341Init(SPI_1, GPIO_1, GPIO_5, GPIO_3);
	ILI9341Rotate(ILI9341_Portrait_2);
	ILI9341DrawString(ILI9341_WIDTH/2-11*5, 20,"vww demo", &font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	ILI9341DrawPicture(ILI9341_WIDTH/2 - (96/2), 90, 96, 96, pic_oled);
#endif
	TM_PRINTF("vww demo\r\n");
    tm_mdl_t mdl;

    tm_mat_t in_uint8 = {3,IMG_L,IMG_L,3, (mtype_t*)pic};
    tm_mat_t in = {3,IMG_L,IMG_L,3, NULL};
    tm_mat_t outs[1];
    tm_err_t res;
    tm_stat((tm_mdlbin_t*)mdl_data);

    res = tm_load(&mdl, mdl_data, NULL, layer_cb, &in);
    if(res != TM_OK) {
        TM_PRINTF("tm model load err %d\n", res);
        return -1;
    }
#if (TM_MDL_TYPE == TM_MDL_INT8)||(TM_MDL_TYPE == TM_MDL_INT16)
    res = tm_preprocess(&mdl, TMPP_UINT2INT, &in_uint8, &in);
#else
    res = tm_preprocess(&mdl, TMPP_UINT2FP01, &in_uint8, &in);
#endif
    TM_DBGT_START();//_t0=clock();
    res = tm_run(&mdl, &in, outs);
    TM_DBGT("tm_run");
    if(res==TM_OK) parse_output(outs);
    else TM_PRINTF("tm run error: %d\n", res);
    tm_unload(&mdl);
    return 0;
}

/*==================[end of file]============================================*/

/** @}*/
/** @}*/
/** @} doxygen end group definition */
