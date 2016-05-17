#include "FreeRTOS.h"
#include "stm32f10x.h"

#include "led.h"
#include "tim.h"
#include "usart.h"

#include "task.h"
#include "queue.h"

#include "semphr.h"

#define USART_HANDLE_PRIORITY				( tskIDLE_PRIORITY + 6 )
#define USART_HANDLE_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )
#define USART_HANDLE_DELAY					( ( TickType_t ) 500 / portTICK_PERIOD_MS )		//ÑÓÊ±500ms

//LED1
#define LED1_PRIORITY				( tskIDLE_PRIORITY + 4 )
#define LED1_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )
#define LED1_DELAY					( ( TickType_t ) 500 / portTICK_PERIOD_MS )		//ÑÓÊ±500ms

//LED2
#define LED2_PRIORITY				( tskIDLE_PRIORITY + 5 )
#define LED2_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )
#define LED2_DELAY					( ( TickType_t ) 500 / portTICK_PERIOD_MS )

void NVIC_Configuration(u8 num);

static void LED1_Task( void *pvParameters );
static void LED2_Task( void *pvParameters );
//static void USART_Handle_Task( void *pvParameters );

xQueueHandle xQueue;
xSemaphoreHandle xSemaphore;

u32 Wcount=0,Rcount=0;

/*
	01234567
	[x,xxxx]
*/
int main()
{

	LED_Configuration();
	USART1_Configuration(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

	//xQueue=xQueueCreate(5,8);
	//xSemaphore=xSemaphoreCreateMutex();



	xTaskCreate( LED1_Task, \
							"LED1", \
							LED1_STACK_SIZE, \
							NULL, \
							LED1_PRIORITY, \
							NULL );

	xTaskCreate( LED2_Task, \
							"LED2", \
							LED2_STACK_SIZE, \
							NULL, \
							LED2_PRIORITY, \
							NULL );

//	xTaskCreate( USART_Handle_Task, \
//							"USART Handle", \
//							USART_HANDLE_STACK_SIZE, \
//							NULL, \
//							USART_HANDLE_PRIORITY, \
//							NULL );

	/* Configure the timers used by the fast interrupt timer test. */
	vTaskStartScheduler();

	/* Will only get here if there was not enough heap space to create the
	idle task. */
	while(1);
	return 0;
}

static void LED1_Task( void *pvParameters )
{
	portTickType xLastWakeTime;
	u8 LED1_val=0;

	xLastWakeTime=xTaskGetTickCount();		//»ñÈ¡µ±Ç°ÐÄÌøÖµ

  for( ;; )
	{
		//printf("ABC\r\n");
		LED1(LED1_val);
		LED1_val=!LED1_val;
		/* Perform this Task1 every mainCHECK_DELAY milliseconds. */
		//vTaskDelay( TASK1_DELAY );
		printf("LED1\r\n");
		vTaskDelayUntil(&xLastWakeTime,LED1_DELAY);
	}
}

static void LED2_Task( void *pvParameters )
{
	portTickType xLastWakeTime;
	u8 LED2_val=1;

	xLastWakeTime=xTaskGetTickCount();		//»ñÈ¡µ±Ç°ÐÄÌøÖµ

  for( ;; )
	{
		//printf("ABC\r\n");
		LED2(LED2_val);
		LED2_val=!LED2_val;
		/* Perform this Task1 every mainCHECK_DELAY milliseconds. */
		//vTaskDelay( TASK2_DELAY );
		printf("LED2\r\n");
		vTaskDelayUntil(&xLastWakeTime,LED2_DELAY);
	}
}

//static void USART_Handle_Task( void *pvParameters )
//{
//	u8 ReceBuff[10];
//	//printf("%s\r\n",pvParameters);
//	for( ;; )
//	{
//		while(xQueueReceive(xQueue,(long *)ReceBuff,5)!=pdPASS);
//		switch(ReceBuff[1])
//		{
//			case '0':
//			{
//				//printf("This is 0\r\n");
//				USART1_SendString("This is 0\r\n");
//				break;
//			}
//			case '1':
//			{
//				break;
//			}
//			case '2':		//¶ÁµÚ¶þÐÐÄÚÈÝ
//			{
//				USART1_SendString("This is 2\r\n");
//				break;
//			}
//			default:
//			{
//				//printf("This is default\r\n");
//				break;
//			}
//		}
//	}
//}

void NVIC_Configuration(u8 num)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
}
