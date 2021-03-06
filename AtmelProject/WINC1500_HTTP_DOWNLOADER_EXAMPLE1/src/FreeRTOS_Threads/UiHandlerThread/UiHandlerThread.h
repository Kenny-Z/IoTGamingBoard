/**************************************************************************//**
* @file      UiHandlerThread.h
* @brief     File that contains the task code and supporting code for the UI Thread for ESE516 Spring (Online) Edition
* @author    Kenny Zhang and Chen Chen
* @date      2021-05-13

******************************************************************************/

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include "FreeRTOS_Threads/WifiHandlerThread/WifiHandler.h"
/******************************************************************************
* Defines
******************************************************************************/
#define UI_TASK_SIZE			410//<Size of stack to assign to the UI thread. In words
#define UI_TASK_PRIORITY		(configMAX_PRIORITIES - 2)

/******************************************************************************
* Structures and Enumerations
******************************************************************************/
typedef enum uiStateMachine_state
{
	UI_STATE_HANDLE_BUTTONS = 0, ///<State used to handle buttons
	UI_STATE_IGNORE_PRESSES, ///<State to ignore button presses
	UI_STATE_SHOW_MOVES, ///<State to show opponent's moves
	UI_STATE_MAX_STATES	///<Max number of states

}uiStateMachine_state;

/******************************************************************************
* Global Function Declaration
******************************************************************************/
void vUiHandlerTask( void *pvParameters );
void UiOrderShowMoves(struct GameDataPacket *packetIn);
bool UiPlayIsDone(void);
struct GameDataPacket *UiGetGamePacketOut(void);
void UIChangeColors(uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif