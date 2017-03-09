/**********************************************************************************************************
AIOS(Advanced Input Output System) - An Embedded Real Time Operating System (RTOS)
Copyright (C) 2012~2017 SenseRate.Com All rights reserved.
http://www.aios.io -- Documentation, latest information, license and contact details.
http://www.SenseRate.com -- Commercial support, development, porting, licensing and training services.
--------------------------------------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met: 
1. Redistributions of source code must retain the above copyright notice, this list of 
conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice, this list 
of conditions and the following disclaimer in the documentation and/or other materials 
provided with the distribution. 
3. Neither the name of the copyright holder nor the names of its contributors may be used 
to endorse or promote products derived from this software without specific prior written 
permission. 
--------------------------------------------------------------------------------------------------------
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
--------------------------------------------------------------------------------------------------------
 Notice of Export Control Law 
--------------------------------------------------------------------------------------------------------
 SenseRate AIOS may be subject to applicable export control laws and regulations, which might 
 include those applicable to SenseRate AIOS of U.S. and the country in which you are located. 
 Import, export and usage of SenseRate AIOS in any manner by you shall be in compliance with such 
 applicable export control laws and regulations. 
***********************************************************************************************************/

#ifndef __FIT_CPU_H_
#define __FIT_CPU_H_

#include "OSType.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Scheduler utilities. */
#define FitSchedule()										\
{															\
	/* Set a PendSV to request a context switch. */			\
	FitNVIC_INT_CTRL_REG = FitNVIC_PENDSVSET_BIT;			\
	__DSB();												\
	__ISB();												\
}

#define FitNVIC_INT_CTRL_REG		( * ( ( volatile uOS32_t * ) 0xe000ed04 ) )
#define FitNVIC_PENDSVSET_BIT		( 1UL << 28UL )
#define FitScheduleFromISR( b ) 	if( b ) FitSchedule()
/*-----------------------------------------------------------*/

#include <intrinsics.h>
#define FIT_QUICK_GET_PRIORITY		1
#define FitGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31 - __CLZ( ( uxReadyPriorities ) ) )

/* Critical section management. */
extern void FitIntLock( void );
extern void FitIntUnlock( void );

#define FitDISABLE_INTERRUPTS()								\
{															\
	__set_BASEPRI( OSMAX_HWINT_PRI );						\
	__DSB();												\
	__ISB();												\
}

#define FitIntMask()							__get_BASEPRI(); FitDISABLE_INTERRUPTS()
#define FitIntUnmask( x )						__set_BASEPRI( x )

#define FitIntMaskFromISR()						__get_BASEPRI(); FitDISABLE_INTERRUPTS()
#define FitIntUnmaskFromISR( x )				__set_BASEPRI( x )

uOSStack_t *FitInitializeStack( uOSStack_t *pxTopOfStack, OSTaskFunction_t TaskFunction, void *pvParameters );
sOSBase_t FitStartScheduler( void );

void FitPendSVHandler( void );
void FitOSTickISR( void );
void FitSVCHandler( void );

#ifdef __cplusplus
}
#endif

#endif //__FIT_CPU_H_
