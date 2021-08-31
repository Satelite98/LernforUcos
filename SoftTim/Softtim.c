/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
CPU_TS  ts_start;//时间戳
CPU_TS  ts_end;

static OS_TCB  AppTaskStartTCB;
static OS_TCB  AppTaskSoftTimTCB;

static CPU_STK AppTaskStartStk[ APP_TASK_START_STK_SIZE];
static CPU_STK AppTaskSoftTimStk[ APP_TASK_SOFTTIM_STK_SIZE];

void AppTaskStart(void * p_arg);
void AppTaskSoftTim(void * p_arg);
void SoftTimDone (void* p_arg);

int main(void)
{
  OS_ERR err;
	OSInit(&err);
	
	OSTaskCreate ((OS_TCB        *) &AppTaskStartTCB,
							(CPU_CHAR      *)"AppTaskStart",
							(OS_TASK_PTR)    AppTaskStart,
							(void          * )0,
							(OS_PRIO)        APP_TASK_START_PRIO,
							(	CPU_STK       *) &AppTaskStartStk[0],
							(CPU_STK_SIZE)   APP_TASK_START_STK_SIZE/10,
							(CPU_STK_SIZE)   APP_TASK_START_STK_SIZE,
							(OS_MSG_QTY)     5,
							(OS_TICK)        0,
							(void          *)0,
							(OS_OPT)         (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
							(OS_ERR        *) &err);
	
	OSStart(&err);
}


/*
*硬件初始化 CPU 时基初始化 内存初始化 创建整个工程任务以及要用到的消息队列等模块
*/
void AppTaskStart(void * p_arg)
{
 CPU_INT32U cpu_clk_freq;
 CPU_INT32U  cnts;
 OS_ERR err;
 (void) p_arg;
	
	BSP_Init();
	CPU_Init();
	cpu_clk_freq = BSP_CPU_ClkFreq();
	cnts = cpu_clk_freq/(CPU_INT32U)OSCfg_TickRate_Hz;
	OS_CPU_SysTickInit(cnts);
	
	Mem_Init();
	
#if OS_CFG_STAT_TASK_EN > 0u                                    //如果使能（默认使能）了统计任务
    OSStatTaskCPUUsageInit(&err);                               //计算没有应用任务（只有空闲任务）运行时 CPU 的（最大）
#endif  
	   CPU_IntDisMeasMaxCurReset();
   	OSTaskCreate ((OS_TCB        *) &AppTaskSoftTimTCB,
									(CPU_CHAR      *)"AppTaskSoftTim",
									(OS_TASK_PTR)    AppTaskSoftTim,
									(void          * )0,
									(OS_PRIO)        APP_TASK_SOFTTIM_PRIO,
								  (	CPU_STK       *) &AppTaskSoftTimStk[0],
									(CPU_STK_SIZE)   APP_TASK_SOFTTIM_STK_SIZE/10,
									(CPU_STK_SIZE)   APP_TASK_SOFTTIM_STK_SIZE,
									(OS_MSG_QTY)     5,
									(OS_TICK)        0,
									(void          *)0,
									(OS_OPT)         (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
									(OS_ERR        *) &err);
		OSTaskDel( &AppTaskStartTCB, &err);
		
}


void AppTaskSoftTim(void * p_arg)
{
  OS_ERR err;
	OS_TMR my_tmr;//申明软件定时器
	
	(void) p_arg;
	
	/*创建定时器*/
  OSTmrCreate ((OS_TMR               * )&my_tmr,          //定时器控制块指针
							 (CPU_CHAR             *)"my_tmr",         //命名定时器，有助于调试
							 (OS_TICK)               10,            //初始定时节拍数
							 (OS_TICK)               10,         //周期定时重载节拍数
							 (OS_OPT)                OS_OPT_TMR_PERIODIC,            //选项
							 (OS_TMR_CALLBACK_PTR)   SoftTimDone,     //定时到期时的回调函数
							 (void                 *)0, //传给回调函数的参数
							 (OS_ERR               *)&err);          //返回错误类型

	OSTmrStart((OS_TMR               * )&my_tmr,&err);
	/*启动定时器*/
	ts_start = OS_TS_GET();
				
	while(DEF_TRUE)
	{
	
	  OSTimeDly (500,                        //延时的时钟节拍数
							 OS_OPT_TIME_DLY,                        //选项
          		(OS_ERR   *)&err);                    //返回错误类型
	}
}

void SoftTimDone (void* p_arg)
{
 CPU_INT32U cpu_clk_freq;
 CPU_SR_ALLOC();//记录中断值
 cpu_clk_freq = BSP_CPU_ClkFreq();//获取CPU时钟
 macLED1_TOGGLE();
 ts_end = OS_TS_GET() - ts_start; //计算时间差值
	OS_CRITICAL_ENTER();//临界段
	printf("CLK has gone");
	printf ("定时时间1s ,通过时间戳计时的时间为 %d us",ts_end/(cpu_clk_freq/1000000));
  OS_CRITICAL_EXIT();
	ts_start = OS_TS_GET();

}
///*
//*********************************************************************************************************
//*                                            LOCAL DEFINES
//*********************************************************************************************************
//*/

//CPU_TS             ts_start;       //时间戳变量
//CPU_TS             ts_end; 



///*
//*********************************************************************************************************
//*                                                 TCB
//*********************************************************************************************************
//*/

//static  OS_TCB   AppTaskStartTCB;                                //任务控制块

//static  OS_TCB   AppTaskTmrTCB;


///*
//*********************************************************************************************************
//*                                                STACKS
//*********************************************************************************************************
//*/

//static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];       //任务堆栈

//static  CPU_STK  AppTaskTmrStk [ APP_TASK_TMR_STK_SIZE ];


///*
//*********************************************************************************************************
//*                                         FUNCTION PROTOTYPES
//*********************************************************************************************************
//*/

//static  void  AppTaskStart  (void *p_arg);                       //任务函数声明

//static  void  AppTaskTmr  ( void * p_arg );


///*
//*********************************************************************************************************
//*                                                main()
//*
//* Description : This is the standard entry point for C code.  It is assumed that your code will call
//*               main() once you have performed all necessary initialization.
//*
//* Arguments   : none
//*
//* Returns     : none
//*********************************************************************************************************
//*/

//int  main (void)
//{
//    OS_ERR  err;


//    OSInit(&err);                                                           //初始化 uC/OS-III

//	  /* 创建起始任务 */
//    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //任务控制块地址
//                 (CPU_CHAR   *)"App Task Start",                            //任务名称
//                 (OS_TASK_PTR ) AppTaskStart,                               //任务函数
//                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
//                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //任务的优先级
//                 (CPU_STK    *)&AppTaskStartStk[0],                         //任务堆栈的基地址
//                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
//                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
//                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
//                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
//                 (void       *) 0,                                          //任务扩展（0表不扩展）
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
//                 (OS_ERR     *)&err);                                       //返回错误类型

//    OSStart(&err);                                                          //启动多任务管理（交由uC/OS-III控制）
//		
//		
//}


///*
//*********************************************************************************************************
//*                                          STARTUP TASK
//*
//* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
//*               initialize the ticker only once multitasking has started.
//*
//* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
//*
//* Returns     : none
//*
//* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
//*                  used.  The compiler should not generate any code for this statement.
//*********************************************************************************************************
//*/

//static  void  AppTaskStart (void *p_arg)
//{
//    CPU_INT32U  cpu_clk_freq;
//    CPU_INT32U  cnts;
//    OS_ERR      err;


//   (void)p_arg;

//    BSP_Init();                                                 //板级初始化
//    CPU_Init();                                                 //初始化 CPU 组件（时间戳、关中断时间测量和主机名）

//    cpu_clk_freq = BSP_CPU_ClkFreq();                           //获取 CPU 内核时钟频率（SysTick 工作时钟）
//    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        //根据用户设定的时钟节拍频率计算 SysTick 定时器的计数值
//    OS_CPU_SysTickInit(cnts);                                   //调用 SysTick 初始化函数，设置定时器计数值和启动定时器

//    Mem_Init();                                                 //初始化内存管理组件（堆内存池和内存池表）

//#if OS_CFG_STAT_TASK_EN > 0u                                    //如果使能（默认使能）了统计任务
//    OSStatTaskCPUUsageInit(&err);                               //计算没有应用任务（只有空闲任务）运行时 CPU 的（最大）
//#endif                                                          //容量（决定 OS_Stat_IdleCtrMax 的值，为后面计算 CPU 
//                                                                //使用率使用）。
//    CPU_IntDisMeasMaxCurReset();                                //复位（清零）当前最大关中断时间

//    
//		/* 创建 AppTaskTmr 任务 */
//    OSTaskCreate((OS_TCB     *)&AppTaskTmrTCB,                             //任务控制块地址
//                 (CPU_CHAR   *)"App Task Tmr",                             //任务名称
//                 (OS_TASK_PTR ) AppTaskTmr,                                //任务函数
//                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
//                 (OS_PRIO     ) APP_TASK_TMR_PRIO,                         //任务的优先级
//                 (CPU_STK    *)&AppTaskTmrStk[0],                          //任务堆栈的基地址
//                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
//                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
//                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
//                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
//                 (void       *) 0,                                          //任务扩展（0表不扩展）
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
//                 (OS_ERR     *)&err);                                       //返回错误类型
//								 
//		OSTaskDel ( & AppTaskStartTCB, & err );                     //删除起始任务本身，该任务不再运行
//		
//		
//}


///*
//*********************************************************************************************************
//*                                          TMR TASK
//*********************************************************************************************************
//*/
//void TmrCallback (OS_TMR *p_tmr, void *p_arg) //软件定时器MyTmr的回调函数
//{
//	CPU_INT32U       cpu_clk_freq;	
//	CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
//											 //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
//											 //，开中断时将该值还原。  
//  printf ( "%s", ( char * ) p_arg );
//	
//	cpu_clk_freq = BSP_CPU_ClkFreq();                   //获取CPU时钟，时间戳是以该时钟计数
//	
//	macLED1_TOGGLE (); 
//	
//  ts_end = OS_TS_GET() - ts_start;     //获取定时后的时间戳（以CPU时钟进行计数的一个计数值）
//	                                     //，并计算定时时间。
//	OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断
//	
//	printf ( "\r\n定时1s，通过时间戳测得定时 %07d us，即 %04d ms。\r\n", 
//						ts_end / ( cpu_clk_freq / 1000000 ),     //将定时时间折算成 us 
//						ts_end / ( cpu_clk_freq / 1000 ) );      //将定时时间折算成 ms 
//	
//	OS_CRITICAL_EXIT();                               

//	ts_start = OS_TS_GET();                            //获取定时前时间戳
//	
//}


//static  void  AppTaskTmr ( void * p_arg )
//{
//	OS_ERR      err;
//	OS_TMR      my_tmr;   //声明软件定时器对象

//	
//	(void)p_arg;


//  /* 创建软件定时器 */
//  OSTmrCreate ((OS_TMR              *)&my_tmr,             //软件定时器对象
//               (CPU_CHAR            *)"MySoftTimer",       //命名软件定时器
//               (OS_TICK              )10,                  //定时器初始值，依10Hz时基计算，即为1s
//               (OS_TICK              )10,                  //定时器周期重载值，依10Hz时基计算，即为1s
//               (OS_OPT               )OS_OPT_TMR_PERIODIC, //周期性定时
//               (OS_TMR_CALLBACK_PTR  )TmrCallback,         //回调函数
//               (void                *)"Timer Over!",       //传递实参给回调函数
//               (OS_ERR              *)err);                //返回错误类型
//							 
//	/* 启动软件定时器 */						 
//  OSTmrStart ((OS_TMR   *)&my_tmr, //软件定时器对象
//              (OS_ERR   *)err);    //返回错误类型
//					 
//	ts_start = OS_TS_GET();                       //获取定时前时间戳
//							 
//	while (DEF_TRUE) {                            //任务体，通常写成一个死循环

//		OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //不断阻塞该任务

//	}
//	
//}














