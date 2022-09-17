/* Copyright 2022 Sipeed Technology Co., Ltd. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "tm_port.h"

//RV32 P-extend acceleration
//https://occ.t-head.cn/community/download?id=3916180248689188864
//https://occ.t-head.cn/vendor/cpu/index?id=3900588052540035072&key=download#sticky   //pdf
//https://github.com/T-head-Semi
//We use T-head Xuantie E907 as example, it have its own instructions!!!
#define ENABLE_THEAD_EXT 1

#if TM_MDL_TYPE == TM_MDL_INT8
#if ENABLE_THEAD_EXT
TM_INLINE void tm_dot_prod(mtype_t* sptr, mtype_t* kptr,uint32_t size, sumtype_t* result)
{
    int32_t sum;
    asm  volatile(
    "mv     t6,%[size]          \n"
    "mv     t0,%[sptr]          \n"       
    "mv     t1,%[kptr]          \n"       
    "andi	a7,t6,15	    	\n"
    "srli	t6,t6,0x4	    	\n"
    "li	%[sum],0				\n"
    "beqz	t6,LOOPEND%=\n"
    "LOOP%=:\n"
    "lwia	t2,(t0),4,0		\n"
    "lwia	t3,(t1),4,0		\n"
    "lwia	t4,(t0),4,0		\n"
    "lwia	t5,(t1),4,0		\n"
    "smaqa	%[sum],t2,t3		\n"
    "lwia	t2,(t0),4,0		\n"
    "lwia	t3,(t1),4,0		\n"
    "smaqa	%[sum],t4,t5		\n"
    "lwia	t4,(t0),4,0		\n"
    "lwia	t5,(t1),4,0		\n"
    "smaqa	%[sum],t2,t3		\n"
    "addi	t6,t6,-1		\n"
    "smaqa	%[sum],t4,t5		\n"
    "bnez	t6,LOOP%=		\n"
    "LOOPEND%=:"
    "beqz	a7,OUT%=	\n"
    "REST%=:\n"
    "lbia	t2,(t0),1,0		\n"
    "lbia	t3,(t1),1,0		\n"
    "addi	a7,a7,-1		\n"
    "smaqa	%[sum],t2,t3		\n"
    "bnez	a7,REST%=	\n"
    "OUT%=:\n"
    :[sum]"=r"(sum)
    :[sptr]"r"(sptr),[kptr]"r"(kptr),[size]"r"(size)
    :"t0", "t1", "t2", "t3", "t4", "t5", "t6", "a7"
    );
    //t0,t1: sptr, kptr
    //t2,t3: *sptr0, *kptr0
    //t4,t5: *sptr1, *kptr1
    //t6:    size -> size>>4
    //a7:    size%15
    *result = sum;
    return;
}

#else
#error "standard RV32P is not implement yet!"
#endif

#else
#error "RV32P opt for FP32 in not implement yet!"
#endif