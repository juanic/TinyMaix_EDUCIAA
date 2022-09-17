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


/* Header con los coeficientes del filtro a utilizar */

/*
#define ARM_MATH_CM4
#define __FPU_PRESENT 1
#include "arm_math.h"
#include "arm_const_structs.h"
*/


LPC_USART_T* UART = LPC_USART2;

#include "tinymaix.h"
#include "Syscalls.h"

//#define TEST_MNIST (1)
//#define TEST_CIFAR (1)
//#define TEST_VWW (1)
#define TEST_MBNET (1)

#if TEST_MNIST
#include "mnist_q_valid.h"
uint8_t pic[28*28]={
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,116,125,171,255,255,150, 93,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,253,253,253,218, 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,213,142,176,253,253,122,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 52,250,253,210, 32, 12,  0,  6,206,253,140,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 77,251,210, 25,  0,  0,  0,122,248,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 31, 18,  0,  0,  0,  0,209,253,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,117,247,253,198, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 76,247,253,231, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,253,253,144,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,176,246,253,159, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25,234,253,233, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,198,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0, 78,248,253,189, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 19,200,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,134,253,253,173, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 43, 20, 20, 20, 20,  5,  0,  5, 20, 20, 37,150,150,150,147, 10,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253,253,253,253,253,253,168,143,166,253,253,253,253,253,253,253,123,  0,
  0,  0,  0,  0,  0,  0,  0,  0,174,253,253,253,253,253,253,253,253,253,253,253,249,247,247,169,117,117, 57,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,118,123,123,123,166,253,253,253,155,123,123, 41,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
#define IMG_L 28
#define IMG_CH 1
#define CLASS_NUM (10)
char* labels[CLASS_NUM] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

#elif TEST_CIFAR
#include "cifar10_q.h"
#include "pic2.h"
#define IMG_L 32
#define IMG_CH 3
#define CLASS_NUM 10
char* labels[CLASS_NUM] = {"airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "hrose", "ship", "truck"};

#elif TEST_VWW
#include "vww96_q.h"
#include "pic_person1.h"
#define IMG_L 96
#define IMG_CH 3
#define CLASS_NUM 2
char* labels[CLASS_NUM] = {"no-person", "person"};

#elif TEST_MBNET
#define IMG_L 96
#define IMG_CH 3
#define CLASS_NUM (1000)
#include "label.h"
#if IMG_L == 128
	#include "model/mbnet/pic128.h"
	#include "model/mbnet/mbnet128_0.25_q.h"
#elif IMG_L==96
	#include "pic96.h"
	#include "mbnet96_0.25_q.h"
#else
  #error "err pic size"
#endif

extern const char* labels[1000];
#endif

static tm_err_t layer_cb(tm_mdl_t* mdl, tml_head_t* lh)
{   //dump middle result
    int h = lh->out_dims[1];
    int w = lh->out_dims[2];
    int ch= lh->out_dims[3];
    mtype_t* output = TML_GET_OUTPUT(mdl, lh);
    #if 0
    TM_PRINTF("Layer %d callback ========\n", mdl->layer_i);
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
    for(int i = 0; i < CLASS_NUM; i++){
        printf("%d: %.3f\r\n", i, data[i]);
        if(data[i] > maxp) {
            maxi = i;
            maxp = data[i];
        }
    }
    TM_PRINTF("### Predict output is: Class %d, Lable %s, prob %.3f\n", maxi, labels[maxi], maxp);
    return;
}

static uint8_t mdl_buf[MDL_BUF_LEN];


serial_config UART_USB;


int main(void)
{
	TM_DBGT_INIT();
	LedsInit();
	SystemCoreClockUpdate();
	/* Configuración de UART */
	UART_USB.baud_rate = 115200;
	UART_USB.port = SERIAL_PORT_PC;
	UART_USB.pSerial = NULL;
	UartInit(&UART_USB);


    TM_PRINTF("mnist demo\r\n");
    tm_mdl_t mdl;

    for (int i = 0; i < 28 * 28; i++) {
    			TM_PRINTF("%3d,", pic[i]);
    			if (i % 28 == 27)
    					TM_PRINTF("\r\n");
    	}

    	tm_mat_t in_uint8 = {3,IMG_L,IMG_L,IMG_CH, (mtype_t*)pic};
    	tm_mat_t in = {3,IMG_L,IMG_L,IMG_CH, NULL};
    	tm_mat_t outs[1];
    	tm_err_t res;
    	tm_stat((tm_mdlbin_t*) mdl_data);

    	res = tm_load(&mdl, mdl_data, mdl_buf, layer_cb, &in);
    	if (res != TM_OK) {
    			TM_PRINTF("tm model load err %d\n", res);
    			goto end;
    	}

    #if (TM_MDL_TYPE == TM_MDL_INT8) || (TM_MDL_TYPE == TM_MDL_INT16)
    	res = tm_preprocess(&mdl, TMPP_UINT2INT, &in_uint8, &in);
    #else
    	res = tm_preprocess(&mdl, TMPP_UINT2FP01, &in_uint8, &in);
    #endif
    	TM_DBGT_START();
    	res = tm_run(&mdl, &in, outs);
    	TM_DBGT("tm_run");
    	if (res == TM_OK)
    			parse_output(outs);
    	else
    			TM_PRINTF("tm run error: %d\n", res);
    	tm_unload(&mdl);
end:
  /* Infinite loop */
  while (1)
  {

  }

}
/*==================[end of file]============================================*/

/** @}*/
/** @}*/
/** @} doxygen end group definition */
