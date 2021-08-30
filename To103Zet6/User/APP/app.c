#include <includes.h>

#define Task_Start_TASK_PRIO  2 //任务优先级
#define Task_LED1_Task_Prio 2 
#define Task_LED2_Task_Prio 2 
#define Task_LED3_Task_Prio 2 

static  OS_ERR  err; 

static  CPU_STK App_Task_StartStk[APP_TASK_START_STK_SIZE];//定义任务栈
static  CPU_STK App_Task_Led1Stk[APP_TASK_LED1_STK_SIZE];//定义任务栈
static  CPU_STK App_Task_Led2Stk[APP_TASK_LED2_STK_SIZE];//定义任务栈
static  CPU_STK App_Task_Led3Stk[APP_TASK_LED3_STK_SIZE];//定义任务栈

static OS_TCB AppTaskStartTCB; //TCB-任务总控制
static OS_TCB AppTaskLed1TCB ;
static OS_TCB AppTaskLed2TCB ;
static OS_TCB AppTaskLed3TCB ;

static void AppTaskStat(void* p_arg);
static void AppTaskLed1(void* p_arg);
static void AppTaskLed2(void* p_arg);
static void AppTaskLed3(void* p_arg);

static void voidLedTask (void* parameter) //一般即采用死循环的方式来进行任务的设计，对于只执行一次的任务执行完之后要进行删除
{
	  CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)parameter;

    //BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

    CPU_IntDisMeasMaxCurReset();
	while(1)
	{
	  macLED1_ON();
		OSTimeDly(500,OS_OPT_TIME_DLY,&err); //调用OS的阻塞任务延时 -> 三个参数的具体含义没清楚
	  macLED1_OFF();
		OSTimeDly(500,OS_OPT_TIME_DLY,&err);
	}

}

int main()
{
	
	OSInit(&err);/*系统初始化*/

	/*--创建初始任务---*/
	OSTaskCreate ( (OS_TCB     *) &AppTaskStartTCB ,
									(CPU_CHAR      *) "AppTaskStartTCB", //任务名称 字符串形式
									(OS_TASK_PTR)    AppTaskStat,
									(void          *)0,
									(OS_PRIO)     Task_Start_TASK_PRIO,
									(CPU_STK       *)&App_Task_StartStk[0],//任务栈起始地址
									(CPU_STK_SIZE)   APP_TASK_START_STK_SIZE/10,//任务栈的限定位置 表示任务栈在剩余10%的即可认为任务栈已满
									(CPU_STK_SIZE)   APP_TASK_START_STK_SIZE,//任务栈大小
									     (OS_MSG_QTY) 5u,//消息队列大小
									(OS_TICK)        0,//时间片
									(void          *)0,//内存位置指针
									(OS_OPT)   (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR) ,//用户任务特定选项  check and clear stk;
									(OS_ERR*)        &err);

	OSStart(&err);
	
	
}

static void AppTaskStat(void *p_arg)
{
	  CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
	  (void ) p_arg;
	
	BSP_Init();/*初始化一次就行*/
	CPU_Init();
	cpu_clk_freq = BSP_CPU_ClkFreq();//获取CPU内核时钟频率
	cnts = cpu_clk_freq/(CPU_INT32U)OSCfg_TickRate_Hz;//根据用户设定的时钟街拍频率计算systicl定时器的计数值
	OS_CPU_SysTickInit(cnts);//设定定时器数值和启动定时器
	Mem_Init();//内存管理初始化
	
	#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif
	
	 CPU_IntDisMeasMaxCurReset();//将最大关中断时间设置为0
	
	  OSSchedRoundRobinCfg ((CPU_BOOLEAN)DEF_ENABLED,     //启用时间片轮转调度
                            (OS_TICK)       0,//还不明白
                             &err);
	 
	 	OSTaskCreate ((OS_TCB     *) &AppTaskLed1TCB ,
									(CPU_CHAR      *) "AppTaskLed1TCB", //任务名称 字符串形式
									(OS_TASK_PTR)    AppTaskLed1,
									(void          *)0,
									(OS_PRIO)     Task_LED1_Task_Prio,
									(CPU_STK       *)&App_Task_Led1Stk[0],//任务栈起始地址
									(CPU_STK_SIZE)   APP_TASK_LED1_STK_SIZE/10,//任务栈的限定位置 表示任务栈在剩余10%的即可认为任务栈已满
									(CPU_STK_SIZE)   APP_TASK_LED1_STK_SIZE,//任务栈大小
									     (OS_MSG_QTY) 5u,//消息队列大小
									(OS_TICK)        0,//时间片
									(void          *)0,//内存位置指针
									(OS_OPT)   (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR) ,//用户任务特定选项  check and clear stk;
									(OS_ERR*)        &err);
									
		 	OSTaskCreate ((OS_TCB     *) &AppTaskLed2TCB ,
									(CPU_CHAR      *) "AppTaskLed2TCB", //任务名称 字符串形式
									(OS_TASK_PTR)    AppTaskLed2,
									(void          *)0,
									(OS_PRIO)     Task_LED2_Task_Prio,
									(CPU_STK       *)&App_Task_Led2Stk[0],//任务栈起始地址
									(CPU_STK_SIZE)   APP_TASK_LED2_STK_SIZE/10,//任务栈的限定位置 表示任务栈在剩余10%的即可认为任务栈已满
									(CPU_STK_SIZE)   APP_TASK_LED2_STK_SIZE,//任务栈大小
									     (OS_MSG_QTY) 5u,//消息队列大小
									(OS_TICK)        0,//时间片
									(void          *)0,//内存位置指针
									(OS_OPT)   (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR) ,//用户任务特定选项  check and clear stk;
									(OS_ERR*)        &err);
			
		OSTaskCreate ((OS_TCB     *) &AppTaskLed3TCB ,
									(CPU_CHAR      *) "AppTaskLed3TCB", //任务名称 字符串形式
									(OS_TASK_PTR)    AppTaskLed3,
									(void          *)0,
									(OS_PRIO)     Task_LED3_Task_Prio,
									(CPU_STK       *)&App_Task_Led3Stk[0],//任务栈起始地址
									(CPU_STK_SIZE)   APP_TASK_LED3_STK_SIZE/10,//任务栈的限定位置 表示任务栈在剩余10%的即可认为任务栈已满
									(CPU_STK_SIZE)   APP_TASK_LED3_STK_SIZE,//任务栈大小
									     (OS_MSG_QTY) 5u,//消息队列大小
									(OS_TICK)        0,//时间片
									(void          *)0,//内存位置指针
									(OS_OPT)   (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR) ,//用户任务特定选项  check and clear stk;
									(OS_ERR*)        &err);

   OSTaskDel( 0,(OS_ERR*)   &err);
	
}
static void AppTaskLed1(void* p_arg)
{
	OS_ERR err;
	OS_REG value;
	(void ) p_arg;
	
	while (DEF_TRUE) //任务死循环
	{
	  macLED1_TOGGLE();
		value = OSTaskRegGet(0,0,&err);//获取自身任务寄存器的值
		if(value <10)
		{
		 OSTaskRegSet(0,0,++value,&err);//累加任务寄存器
		}else{
		 OSTaskRegSet(0,0,0,&err);//将自身任务寄存器的值设为0
		 OSTaskResume (&AppTaskLed2TCB,&err);
		 OSTaskResume (&AppTaskLed3TCB,&err);
		}
	  OSTimeDly(1000,OS_OPT_TIME_DLY,&err);//周期1000个时钟节拍一次
		
	
	}
}
static void AppTaskLed2(void* p_arg)
{
	
	OS_ERR err;
	OS_REG value;
	(void ) p_arg;
	while(DEF_TRUE)
	{
		macLED2_TOGGLE();
		value = OSTaskRegGet(0,0,&err);
		if (value < 5)
		{
		 OSTaskRegSet(0,0,++value,&err);
		}else {
		OSTaskRegSet(0,0,0,&err);
		OSTaskSuspend(0,&err);
		}
	 OSTimeDly(100,OS_OPT_TIME_DLY,&err);
	}

}
static void AppTaskLed3(void* p_arg)
{

	OS_ERR err;
	OS_REG value;
	(void ) p_arg;
	while(DEF_TRUE)
	{
		macLED3_TOGGLE();
		value = OSTaskRegGet(0,0,&err);
		if (value < 5)
		{
		 OSTaskRegSet(0,0,++value,&err);
		}else {
		OSTaskRegSet(0,0,0,&err);
		OSTaskSuspend(0,&err);
		}
	 OSTimeDly(100,OS_OPT_TIME_DLY,&err);
	}

}
