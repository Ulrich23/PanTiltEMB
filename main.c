/*****************************************************************************

SDU Semesterproject 4 Group 1

*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

// Systick Timer used for software TICK timer by FreeRTOS
// (used for context switching and time measuring)
#include "systick_frt.h"

// FreeRTOS include files
#include "FreeRTOS.h"   // IMPORTANT!!! Always include FreeRTOS.h before task.h or queue.h etc.
#include "task.h"
#include "queue.h"
#include "semphr.h"

// SPI protocol setup and functions.
#include "spi_config.h"

// Tasks
#include "tasktest.h" // (An example of a setup of a task, use this for reference)
#include "spiTask.h"
#include "menu.h"
#include "lcd.h"
#include "ui.h"
#include "key.h"
#include "regulering.h" //Regulerings task

// Display Color (For Debugging Purposes)
#include "display_color.h"

// Initialize GPIO pins for on board LEDs, Button Matrix and LCD.
#include "gpio.h"

// Others
#include "taskHandlers.h"
#include "queueHandlers.h"
#include "semaphoreHandlers.h"
#include "file.h"
#include "protocol_function.h"
//#include "tmodel.h"

/*****************************    Defines    *******************************/
//#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  init_systick();       // Initialize Real time clock SystickTimer for Ticks
  init_gpio();          // Initialize GPIO pins for on board LEDs,
                        // Button Matrix and LCD.
  init_files();
  init_master_spi();    // Initialize the spi protocol
}



int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{

  setupHardware();

  // Create the queues
  // ----------------

  Q_KEY = xQueueCreate(QUEUE_SIZE, sizeof(INT8U));
  Q_LCD = xQueueCreate(QUEUE_SIZE, sizeof(INT8U));
  Q_SPIDATAM1 = xQueueCreate(1, sizeof(INT16U));
  Q_SPIDATAM2 = xQueueCreate(1, sizeof(INT16U));
  Q_SPIDATATXM1 = xQueueCreate(1, sizeof(INT16U));
  Q_SPIDATATXM2 = xQueueCreate(1, sizeof(INT16U));
  Q_NEWPOSM1 = xQueueCreate(1, sizeof(INT16U));
  Q_NEWPOSM2 = xQueueCreate(1, sizeof(INT16U));

  // Create the semaphore
  // ----------------
  SEM_MENU_UPDATED = xSemaphoreCreateMutex();


  // Used for testing
  // ----------------
  // INT16U testM1 = 240;
  // INT16U testM2 = 128;
  // xQueueOverwrite(Q_SPIDATAM1,&testM1);
  // xQueueOverwrite(Q_SPIDATAM2,&testM2);

  // Start the tasks.
  // ----------------
  xTaskCreate(myTaskTest, "taskTest", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &myTaskTestHandle);
//  xTaskCreate(spiTask, "spiTask", configMINIMAL_STACK_SIZE, NULL, HIGH_PRIO, &spiTaskHandle);
  xTaskCreate(lcd_task, "lcdTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &lcdTaskHandle);
  xTaskCreate(key_task, "keyTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &keyTaskHandle);
  xTaskCreate(display_menu_task, "displayMenuTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &displayMenuTaskHandle);
  xTaskCreate(menu_task, "menuTask", configMINIMAL_STACK_SIZE, NULL, LOW_PRIO, &menuTaskHandle);
  xTaskCreate(reg_Task, "regTask", ( unsigned short ) 150 , NULL, HIGH_PRIO, &regTaskHandle);


  // Start the scheduler.
  // --------------------
  vTaskStartScheduler();

  // Will only get here, if there was insufficient memory.
  // -----------------------------------------------------
  return 1;
}

/****************************** End Of Module *******************************/
