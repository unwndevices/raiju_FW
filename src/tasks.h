#ifndef TASKS_H
#define TASKS_H
#include "Arduino.h"
#include "Interface.h"
#include "systemTimer.h"
//#include <esp_task_wdt.h>
//try esp_task_wdt_reset();
TaskHandle_t interfaceTaskInit; //task handle creation
TaskHandle_t interfaceTask;     //task handle creation

void interfaceTaskRoutine(void *parameter) //interface task on core 0
{
  for (;;)
  {
    vTaskDelay(10);
    interfaceUpdate();
  }
}

void interfaceInterruptDeploy(void *parameter) //interface task on core 0
{
  //vTaskDelay(10);
  initInterfaceTimer();
  interfaceInit();
  vTaskDelete(interfaceTaskInit); //delete the task
}

void taskInit()
{
  //interrupt creation task
  xTaskCreatePinnedToCore(
      interfaceInterruptDeploy,
      "interr",
      10000,
      NULL,
      0,
      &interfaceTaskInit,
      0);

  //interface task
  xTaskCreatePinnedToCore(
      interfaceTaskRoutine,
      "interface",
      10000,
      NULL,
      0,
      &interfaceTask,
      0);
}

#endif // !TASKS_H