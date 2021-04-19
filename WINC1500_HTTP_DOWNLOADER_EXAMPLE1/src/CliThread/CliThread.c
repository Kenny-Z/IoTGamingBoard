/**************************************************************************//**
* @file      CliThread.c
* @brief     File for the CLI Thread handler. Uses FREERTOS + CLI
* @author    Eduardo Garcia
* @date      2020-02-15

******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include "CliThread.h"

/******************************************************************************
* Defines
******************************************************************************/
#define DAEMON_STACK    512
#define DAEMON_PRIORITY (configMAX_PRIORITIES - 1)

/******************************************************************************
* Variables
******************************************************************************/
static const int8_t * const pcWelcomeMessage =
"FreeRTOS CLI.\r\nType Help to view a list of registered commands.\r\n";

/******************************************************************************
* Forward Declarations
******************************************************************************/

/******************************************************************************
* Callback Functions
******************************************************************************/




void vCommandConsoleTask( void *pvParameters )
{

uint8_t cRxedChar, cInputIndex = 0;
BaseType_t xMoreDataToFollow;
/* The input and output buffers are declared static to keep them off the stack. */
static int8_t pcOutputString[ MAX_OUTPUT_LENGTH_CLI  ], pcInputString[ MAX_INPUT_LENGTH_CLI ];

    /* This code assumes the peripheral being used as the console has already
    been opened and configured, and is passed into the task as the task
    parameter.  Cast the task parameter to the correct type. */

    /* Send a welcome message to the user knows they are connected. */
    SerialConsoleWriteString( pcWelcomeMessage);

    for( ;; )
    {
        /* This implementation reads a single character at a time.  Wait in the
        Blocked state until a character is received. */
        int recv = SerialConsoleReadCharacter(&cRxedChar);
		if(recv == -1) //If no characters in the buffer, thread goes to sleep for a while
		{
			vTaskDelay( CLI_TASK_DELAY);
		}else if( cRxedChar == '\n' )
        {
            /* A newline character was received, so the input command string is
            complete and can be processed.  Transmit a line separator, just to
            make the output easier to read. */
            SerialConsoleWriteString("\r\n");

            /* The command interpreter is called repeatedly until it returns
            pdFALSE.  See the "Implementing a command" documentation for an
            explanation of why this is. */
            do
            {
                /* Send the command string to the command interpreter.  Any
                output generated by the command interpreter will be placed in the
                pcOutputString buffer. */
                xMoreDataToFollow = FreeRTOS_CLIProcessCommand
                              (
                                  pcInputString,   /* The command string.*/
                                  pcOutputString,  /* The output buffer. */
                                  MAX_OUTPUT_LENGTH_CLI/* The size of the output buffer. */
                              );

                /* Write the output generated by the command interpreter to the
                console. */
				//Ensure it is null terminated
				pcOutputString[MAX_OUTPUT_LENGTH_CLI - 1] = 0;
                SerialConsoleWriteString(pcOutputString);

            } while( xMoreDataToFollow != pdFALSE );

            /* All the strings generated by the input command have been sent.
            Processing of the command is complete.  Clear the input string ready
            to receive the next command. */
            cInputIndex = 0;
            memset( pcInputString, 0x00, MAX_INPUT_LENGTH_CLI );
        }
        else
        {
            /* The if() clause performs the processing after a newline character
            is received.  This else clause performs the processing if any other
            character is received. */

            if( cRxedChar == '\r' )
            {
                /* Ignore carriage returns. */
            }
            else if( cRxedChar == ASCII_BACKSPACE || cRxedChar == ASCII_DELETE )
            {
				char erase[4] = {0x08, 0x20, 0x08, 0x00};
				SerialConsoleWriteString(erase);
                /* Backspace was pressed.  Erase the last character in the input
                buffer - if there are any. */
                if( cInputIndex > 0 )
                {
                    cInputIndex--;
                    pcInputString[ cInputIndex ] = 0;
                }
            }
            else
            {
                /* A character was entered.  It was not a new line, backspace
                or carriage return, so it is accepted as part of the input and
                placed into the input buffer.  When a n is entered the complete
                string will be passed to the command interpreter. */
                if( cInputIndex < MAX_INPUT_LENGTH_CLI )
                {
                    pcInputString[ cInputIndex ] = cRxedChar;
                    cInputIndex++;
                }

					//Order Echo
					SerialConsoleWriteString(&cRxedChar);
            }
        }
    }
}