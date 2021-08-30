#include <includes.h>

#define Task_Start_TASK_PRIO  2 //�������ȼ�
#define Task_LED1_Task_Prio 2 
#define Task_LED2_Task_Prio 2 
#define Task_LED3_Task_Prio 2 

static  OS_ERR  err; 

static  CPU_STK App_Task_StartStk[APP_TASK_START_STK_SIZE];//��������ջ
static  CPU_STK App_Task_Led1Stk[APP_TASK_LED1_STK_SIZE];//��������ջ
static  CPU_STK App_Task_Led2Stk[APP_TASK_LED2_STK_SIZE];//��������ջ
static  CPU_STK App_Task_Led3Stk[APP_TASK_LED3_STK_SIZE];//��������ջ

static OS_TCB AppTaskStartTCB; //TCB-�����ܿ���
static OS_TCB AppTaskLed1TCB ;
static OS_TCB AppTaskLed2TCB ;
static OS_TCB AppTaskLed3TCB ;

static void AppTaskStat(void* p_arg);
static void AppTaskLed1(void* p_arg);
static void AppTaskLed2(void* p_arg);
static void AppTaskLed3(void* p_arg);

static void voidLedTask (void* parameter) //һ�㼴������ѭ���ķ�ʽ�������������ƣ�����ִֻ��һ�ε�����ִ����֮��Ҫ����ɾ��
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
		OSTimeDly(500,OS_OPT_TIME_DLY,&err); //����OS������������ʱ -> ���������ľ��庬��û���
	  macLED1_OFF();
		OSTimeDly(500,OS_OPT_TIME_DLY,&err);
	}

}

int main()
{
	
	OSInit(&err);/*ϵͳ��ʼ��*/

	/*--������ʼ����---*/
	OSTaskCreate ( (OS_TCB     *) &AppTaskStartTCB ,
									(CPU_CHAR      *) "AppTaskStartTCB", //�������� �ַ�����ʽ
									(OS_TASK_PTR)    AppTaskStat,
									(void          *)0,
									(OS_PRIO)     Task_Start_TASK_PRIO,
									(CPU_STK       *)&App_Task_StartStk[0],//����ջ��ʼ��ַ
									(CPU_STK_SIZE)   APP_TASK_START_STK_SIZE/10,//����ջ���޶�λ�� ��ʾ����ջ��ʣ��10%�ļ�����Ϊ����ջ����
									(CPU_STK_SIZE)   APP_TASK_START_STK_SIZE,//����ջ��С
									     (OS_MSG_QTY) 5u,//��Ϣ���д�С
									(OS_TICK)        0,//ʱ��Ƭ
									(void          *)0,//�ڴ�λ��ָ��
									(OS_OPT)   (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR) ,//�û������ض�ѡ��  check and clear stk;
									(OS_ERR*)        &err);

	OSStart(&err);
	
	
}

static void AppTaskStat(void *p_arg)
{
	  CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
	  (void ) p_arg;
	
	BSP_Init();/*��ʼ��һ�ξ���*/
	CPU_Init();
	cpu_clk_freq = BSP_CPU_ClkFreq();//��ȡCPU�ں�ʱ��Ƶ��
	cnts = cpu_clk_freq/(CPU_INT32U)OSCfg_TickRate_Hz;//�����û��趨��ʱ�ӽ���Ƶ�ʼ���systicl��ʱ���ļ���ֵ
	OS_CPU_SysTickInit(cnts);//�趨��ʱ����ֵ��������ʱ��
	Mem_Init();//�ڴ�����ʼ��
	
	#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif
	
	 CPU_IntDisMeasMaxCurReset();//�������ж�ʱ������Ϊ0
	
	  OSSchedRoundRobinCfg ((CPU_BOOLEAN)DEF_ENABLED,     //����ʱ��Ƭ��ת����
                            (OS_TICK)       0,//��������
                             &err);
	 
	 	OSTaskCreate ((OS_TCB     *) &AppTaskLed1TCB ,
									(CPU_CHAR      *) "AppTaskLed1TCB", //�������� �ַ�����ʽ
									(OS_TASK_PTR)    AppTaskLed1,
									(void          *)0,
									(OS_PRIO)     Task_LED1_Task_Prio,
									(CPU_STK       *)&App_Task_Led1Stk[0],//����ջ��ʼ��ַ
									(CPU_STK_SIZE)   APP_TASK_LED1_STK_SIZE/10,//����ջ���޶�λ�� ��ʾ����ջ��ʣ��10%�ļ�����Ϊ����ջ����
									(CPU_STK_SIZE)   APP_TASK_LED1_STK_SIZE,//����ջ��С
									     (OS_MSG_QTY) 5u,//��Ϣ���д�С
									(OS_TICK)        0,//ʱ��Ƭ
									(void          *)0,//�ڴ�λ��ָ��
									(OS_OPT)   (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR) ,//�û������ض�ѡ��  check and clear stk;
									(OS_ERR*)        &err);
									
		 	OSTaskCreate ((OS_TCB     *) &AppTaskLed2TCB ,
									(CPU_CHAR      *) "AppTaskLed2TCB", //�������� �ַ�����ʽ
									(OS_TASK_PTR)    AppTaskLed2,
									(void          *)0,
									(OS_PRIO)     Task_LED2_Task_Prio,
									(CPU_STK       *)&App_Task_Led2Stk[0],//����ջ��ʼ��ַ
									(CPU_STK_SIZE)   APP_TASK_LED2_STK_SIZE/10,//����ջ���޶�λ�� ��ʾ����ջ��ʣ��10%�ļ�����Ϊ����ջ����
									(CPU_STK_SIZE)   APP_TASK_LED2_STK_SIZE,//����ջ��С
									     (OS_MSG_QTY) 5u,//��Ϣ���д�С
									(OS_TICK)        0,//ʱ��Ƭ
									(void          *)0,//�ڴ�λ��ָ��
									(OS_OPT)   (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR) ,//�û������ض�ѡ��  check and clear stk;
									(OS_ERR*)        &err);
			
		OSTaskCreate ((OS_TCB     *) &AppTaskLed3TCB ,
									(CPU_CHAR      *) "AppTaskLed3TCB", //�������� �ַ�����ʽ
									(OS_TASK_PTR)    AppTaskLed3,
									(void          *)0,
									(OS_PRIO)     Task_LED3_Task_Prio,
									(CPU_STK       *)&App_Task_Led3Stk[0],//����ջ��ʼ��ַ
									(CPU_STK_SIZE)   APP_TASK_LED3_STK_SIZE/10,//����ջ���޶�λ�� ��ʾ����ջ��ʣ��10%�ļ�����Ϊ����ջ����
									(CPU_STK_SIZE)   APP_TASK_LED3_STK_SIZE,//����ջ��С
									     (OS_MSG_QTY) 5u,//��Ϣ���д�С
									(OS_TICK)        0,//ʱ��Ƭ
									(void          *)0,//�ڴ�λ��ָ��
									(OS_OPT)   (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR) ,//�û������ض�ѡ��  check and clear stk;
									(OS_ERR*)        &err);

   OSTaskDel( 0,(OS_ERR*)   &err);
	
}
static void AppTaskLed1(void* p_arg)
{
	OS_ERR err;
	OS_REG value;
	(void ) p_arg;
	
	while (DEF_TRUE) //������ѭ��
	{
	  macLED1_TOGGLE();
		value = OSTaskRegGet(0,0,&err);//��ȡ��������Ĵ�����ֵ
		if(value <10)
		{
		 OSTaskRegSet(0,0,++value,&err);//�ۼ�����Ĵ���
		}else{
		 OSTaskRegSet(0,0,0,&err);//����������Ĵ�����ֵ��Ϊ0
		 OSTaskResume (&AppTaskLed2TCB,&err);
		 OSTaskResume (&AppTaskLed3TCB,&err);
		}
	  OSTimeDly(1000,OS_OPT_TIME_DLY,&err);//����1000��ʱ�ӽ���һ��
		
	
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
