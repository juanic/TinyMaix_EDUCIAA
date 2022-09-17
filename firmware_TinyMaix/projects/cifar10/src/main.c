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
#include "gpio.h"

#include "ili9341.h"
#include "fonts.h"
#include "gpio.h"
#include "spi.h"
#include "led.h"




LPC_USART_T* UART = LPC_USART2;


#include "tinymaix.h"
#include "Syscalls.h"
#if TM_MDL_TYPE == TM_MDL_INT8
#include "../../tools/tmdl/cifar10_q.h"
#elif TM_MDL_TYPE == TM_MDL_FP32
#include "../../tools/tmdl/cifar10_f.h"
#else
#error "fill your modl path here"
#endif

#define IMG_L 32
#define IMG_CH 3
#define CLASS_NUM 10
#define OLED	1
#define MARGIN 20			/*!< Distance in pixels from left border to start writing */

//Note: this model trained as BGR input
//#define BIRD 1
#define DOG 1
#if BIRD
	#include "pic2.h"
	#include "pic_2.h"
#endif
#if DOG
	#include "pic5.h"
	#include "pic_5.h"
#endif
//#include "pic/pic3.h"
//#include "pic/pic4.h"
//#include "pic/pic5.h"
//#include "pic/pic7.h"
//#include "pic/pic8.h"


char* labels[CLASS_NUM] = {\
    "airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "hrose", "ship", "truck"};

static tm_err_t layer_cb(tm_mdl_t* mdl, tml_head_t* lh)
{   //dump middle result
    int h = lh->out_dims[1];
    int w = lh->out_dims[2];
    int ch= lh->out_dims[3];
    mtype_t* output = TML_GET_OUTPUT(mdl, lh);
    return TM_OK;
    TM_PRINTF("Layer %d callback ========\n", mdl->layer_i);
    #if 1
    for(int y=0; y<h; y++){
        TM_PRINTF("[");
        for(int x=0; x<w; x++){
            TM_PRINTF("[");
            for(int c=0; c<ch; c++){
            #if TM_MDL_TYPE == TM_MDL_FP32
                TM_PRINTF("%.3f,", output[(y*w+x)*ch+c]);
            #else
                TM_PRINTF("%.3f,", TML_DEQUANT(lh,output[(y*w+x)*ch+c]));
            #endif
            }
            TM_PRINTF("],");
        }
        TM_PRINTF("],\n");
    }
    TM_PRINTF("\n");
    #endif
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
    TM_PRINTF("### Predict output is: Class %d, %s, prob %.3f\n", maxi, labels[maxi], maxp);
	#if OLED
    ILI9341DrawString(MARGIN, 190,"### Predict output is:", &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawString(MARGIN, 210,"Class:", &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawInt(MARGIN + 7*6, 210, maxi , 1, &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawString(MARGIN + 7*12, 210, labels[maxi], &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawString(MARGIN, 230,"Prob:", &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawInt(MARGIN + 7*6, 230, maxp*100 , 2, &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
    ILI9341DrawString(MARGIN + 7*8, 230,"%", &font_7x10, ILI9341_BLACK, ILI9341_WHITE);
	#endif
    return;
}

serial_config UART_USB;
int main(int argc, char** argv)
{   TM_DBGT_INIT();
    SystemCoreClockUpdate();
	LedsInit();
	#if OLED
	ILI9341Init(SPI_1, GPIO_1, GPIO_5, GPIO_3);
	ILI9341Rotate(ILI9341_Portrait_2);
	ILI9341DrawString(ILI9341_WIDTH/2-11*7, 20,"cifar10 demo", &font_11x18, ILI9341_BLACK, ILI9341_WHITE);
	ILI9341DrawPicture(ILI9341_WIDTH/2 - (32/2), 90, 32, 32, pic_oled);
	#endif
	GPIOInit(GPIO_T_FIL3, GPIO_OUTPUT);
	GPIOOff(GPIO_T_FIL3);
	UART_USB.baud_rate = 115200;
	UART_USB.port = SERIAL_PORT_PC;
	UART_USB.pSerial = NULL;
	UartInit(&UART_USB);

    TM_PRINTF("cifar10 demo\r\n");
    tm_mdl_t mdl;

    tm_mat_t in_uint8 = {3,IMG_L,IMG_L,IMG_CH, (mtype_t*)pic};
        tm_mat_t in = {3,IMG_L,IMG_L,IMG_CH, NULL};
        tm_mat_t outs[1];
        tm_err_t res;
        tm_stat((tm_mdlbin_t*)mdl_data);

        res = tm_load(&mdl, mdl_data, NULL, layer_cb, &in);
        if(res != TM_OK) {
            TM_PRINTF("tm model load err %d\n", res);
            return -1;
        }

    #if (TM_MDL_TYPE == TM_MDL_INT8) || (TM_MDL_TYPE == TM_MDL_INT16)
        res = tm_preprocess(&mdl, TMPP_UINT2INT, &in_uint8, &in);
    #else
        res = tm_preprocess(&mdl, TMPP_UINT2FP01, &in_uint8, &in);
    #endif
        TM_DBGT_START();
        GPIOOn(GPIO_T_FIL3);
        res = tm_run(&mdl, &in, outs);
        GPIOOff(GPIO_T_FIL3);
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
