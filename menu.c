/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: menu.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090315  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include "menu.h"
//#include "glob_def.h"
#include "file.h"
///#include "tmodel.h"
#include "ui.h"
#include "string.h"
//#include "semphr.h"
/*****************************    Defines    *******************************/
enum MM_states // Main menu options
{
  CURRENT_POSITION,
  WANTED_POSITION,
  HOME_POSTION,
};
enum WP_states // Wanted position menu options
{
  MOTOR_1,
  MOTOR_2,
};
enum H_states // Home menu options
{
  HOME_M1,
  HOME_M2,
};

enum MM_states MM_state;

enum WP_states WP_state;

enum H_states H_state;

// Booleans used to enter menus each time it has time on the CPU and update the LCD
// Prevents code being stuck in one place.
BOOLEAN SHOW_CURRENT_POS;
BOOLEAN SHOW_WANTED_POS;
BOOLEAN SHOW_ENTER_POS_M1;
BOOLEAN SHOW_ENTER_POS_M2;
BOOLEAN SHOW_HOME;
BOOLEAN SHOW_ENTER_HOME_M1;
BOOLEAN SHOW_ENTER_HOME_M2;

BOOLEAN VALID_POSITION_M1;
BOOLEAN VALID_POSITION_M2;

INT16U M1_USER_POSITION;
INT16U M2_USER_POSITION;
INT16U HOME_M1_POS;
INT16U HOME_M2_POS;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
void menu_task(void *p)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  SHOW_CURRENT_POS = FALSE;
  SHOW_WANTED_POS = FALSE;
  SHOW_ENTER_POS_M1 = FALSE;
  SHOW_ENTER_POS_M2 = FALSE;
  SHOW_HOME = FALSE;
  SHOW_ENTER_HOME_M1 = FALSE;
  SHOW_ENTER_HOME_M2 = FALSE;

  VALID_POSITION_M1 = FALSE;
  VALID_POSITION_M2 = FALSE;

  MM_state = CURRENT_POSITION;
  WP_state = MOTOR_1;
  H_state = HOME_M1;

  while(1)
  {
    switch( MM_state ) // Switch case for main menu
    {
      case CURRENT_POSITION:  // If main menu is at current position submenu
        if (SHOW_CURRENT_POS == TRUE || get_square_key() == TRUE )
        {
          SHOW_CURRENT_POS = TRUE; // Makes sure we enter the if statement
          if (get_star_key() == TRUE)
          {
            SHOW_CURRENT_POS = FALSE; // This prevents entering the if statement
          }
        }
        if (get_star_key() == TRUE)
        {
          MM_state = WANTED_POSITION; // Go to next option in main menu
        }
        break;

      case WANTED_POSITION: // If main menu is at wanted position submenu
        if(SHOW_WANTED_POS == TRUE || get_square_key() == TRUE)
        {
          SHOW_WANTED_POS = TRUE; // Makes sure we enter the if statement
          switch (WP_state)
          {
            case MOTOR_1:
              if ((SHOW_ENTER_POS_M1 == TRUE) || (get_square_key() == TRUE))
              {
                SHOW_ENTER_POS_M1 = TRUE; // Makes sure we enter the if statement
                //KEY_BOARD_ACTIVE(); // Activate buttons for user inputs (For wanted position)
                if (get_square_key() == TRUE && VALID_POSITION_M1) // If user have entered a valid position and accepted
                {
                  //UPDATE_POSITION_M1(M1_USER_POSITION); // User enters the wanted position, and position is send to motor
                  SHOW_ENTER_POS_M1 = FALSE; // This prevents entering the if statement
                }
              }
              if (get_star_key() == TRUE)
              {
                WP_state = MOTOR_2; // Go to next option in Wanted position menu
                SHOW_ENTER_POS_M1 = FALSE; // This prevents entering the if statement
              }
              break;
            case MOTOR_2:
              if (SHOW_ENTER_POS_M2 == TRUE || get_square_key() == TRUE)
              {
                SHOW_ENTER_POS_M2 = TRUE; // Makes sure we enter the if statement
                //KEY_BOARD_ACTIVE(); // Activate buttons for user inputs (For wanted position)
                if (get_square_key() == TRUE && VALID_POSITION_M2) // If user have entered a valid position and accepted
                {
                  //UPDATE_POSITION_M2(M2_USER_POSITION); // User enters the wanted position, and position is send to motor
                  SHOW_ENTER_POS_M2 = FALSE; // This prevents entering the if statement
                }
              }
              if (get_star_key() == TRUE)
              {
                WP_state = MOTOR_1; // Go to next option in Wanted position menu
                SHOW_WANTED_POS = FALSE; // This prevents entering the if statement
                SHOW_ENTER_POS_M2 = FALSE; // This prevents entering the if statement
              }
              break;
          }
        }
        if (get_star_key() == TRUE)
        {
          MM_state = HOME_POSTION;
        }
        break;

      case HOME_POSTION: // If main menu is at home submenu
        if (SHOW_HOME == TRUE || get_square_key() == TRUE)
        {
          SHOW_HOME = TRUE; // Makes sure we enter the if statement
          switch (H_state) {
            case HOME_M1:
              if (SHOW_ENTER_HOME_M1 == TRUE || get_square_key() == TRUE)
              {
                SHOW_ENTER_HOME_M1 = TRUE; // Makes sure we enter the if statement
                if (get_square_key() == TRUE)
                {
                  //UPDATE_POSITION_M1(HOME_M1_POS); // Update position of M1 with Home Pos
                }
              }
              if (get_star_key() == TRUE)
              {
                H_state = HOME_M2;
                SHOW_ENTER_HOME_M1 = FALSE; // This prevents entering the if statement
              }
              break;
            case HOME_M2:
              if (SHOW_ENTER_HOME_M2 || get_square_key() == TRUE)
              {
                SHOW_ENTER_HOME_M2 = TRUE; // Makes sure we enter the if statement
                if (get_square_key() == TRUE)
                {
                  //UPDATE_POSITION_M2(HOME_M2_POS); // Update position of M2 with Home Pos
                }
              }
              if (get_star_key() == TRUE) {
                SHOW_HOME = FALSE;
                H_state = HOME_M1;
                SHOW_ENTER_HOME_M2 = FALSE; // This prevents entering the if statement
                SHOW_ENTER_HOME_M1 = FALSE; // This prevents entering the if statement
              }
              break;
          }
        }
        if (get_star_key() == TRUE)
        {
          MM_state = CURRENT_POSITION;
        }
        break;
      }
      vTaskDelay( 400 );
      xSemaphoreGive(SEM_MENU_UPDATED);
  }
}

void display_menu_task(void *p)
{
  while (1)
  {
    if(SEM_MENU_UPDATED != NULL)
    {
      if (xSemaphoreTake(SEM_MENU_UPDATED, 0) == pdTRUE)
      {
        if(MM_state == CURRENT_POSITION && SHOW_CURRENT_POS == FALSE)
        {
          gfprintf( COM2, "%c%cMenu            ", 0x1B, 0x80); // Menu (L1)
          gfprintf( COM2, "%c%c- Current pos   ", 0x1B, 0xC0); // - Nuværende pos (L2)
        }

        else if (MM_state == WANTED_POSITION && SHOW_WANTED_POS == FALSE)
        {
          gfprintf( COM2, "%c%cMenu            ", 0x1B, 0x80); // Menu (L1)
          gfprintf( COM2, "%c%c- Wanted pos    ", 0x1B, 0xC0); // - Ønskede pos (L2)
        }

        else if (MM_state == HOME_POSTION && SHOW_HOME == FALSE)
        {
          gfprintf( COM2, "%c%cMenu            ", 0x1B, 0x80); // Menu (L1)
          gfprintf( COM2, "%c%c- Home          ", 0x1B, 0xC0); // - Home (L2)
        }

        else if (MM_state == CURRENT_POSITION && SHOW_CURRENT_POS == TRUE)
        {
          gfprintf( COM2, "%c%cPos M1:         %c", 0x1B, 0x80/*, getCurrentPos(M1)*/); // Pos M1: "aktuel pos hentede fra FPGA" (L1)
          gfprintf( COM2, "%c%cPos M2:         %c", 0x1B, 0xC0/*, getCurrentPos(M2)*/); // Pos M2: "aktuel pos hentede fra FPGA" (L2)
        }

        else if (MM_state == WANTED_POSITION && SHOW_WANTED_POS == TRUE && WP_state == MOTOR_1 && SHOW_ENTER_POS_M1 == FALSE)
        {
          gfprintf( COM2, "%c%cWanted pos       ", 0x1B, 0x80); // Ønskede pos (L1)
          gfprintf( COM2, "%c%c- Motor 1        ", 0x1B, 0xC0); // - Motor 1 (L2)
        }

        else if (MM_state == WANTED_POSITION && SHOW_WANTED_POS == TRUE && WP_state == MOTOR_2 && SHOW_ENTER_POS_M2 == FALSE)
        {
          gfprintf( COM2, "%c%cWanted pos       ", 0x1B, 0x80); // Ønskede pos (L1)
          gfprintf( COM2, "%c%c- Motor 2        ", 0x1B, 0xC0); // - Motor 2 (L2)
        }

        else if (MM_state == WANTED_POSITION && SHOW_WANTED_POS == TRUE && WP_state == MOTOR_1 && SHOW_ENTER_POS_M1 == TRUE)
        {
          gfprintf( COM2, "%c%cEnter wanted pos", 0x1B, 0x80); // Indtast ønskede pos (L1)
          gfprintf( COM2, "%c%cM1 pos: %c      ", 0x1B, 0xC0/*, getWantedPos(M1)*/); // M1 pos: "her indtaster man ønskede pos" (L2)
        }

        else if (MM_state == WANTED_POSITION && SHOW_WANTED_POS == TRUE && WP_state == MOTOR_2 && SHOW_ENTER_POS_M2 == TRUE)
        {
          gfprintf( COM2, "%c%cEnter wanted pos", 0x1B, 0x80); // Indtast ønskede pos (L1)
          gfprintf( COM2, "%c%cM2 pos: %c      ", 0x1B, 0xC0/*, getWantedPos(M2)*/); // M2 pos: "her indtaster man ønskede pos" (L2)
        }

        else if (MM_state == HOME_POSTION && SHOW_HOME == TRUE && H_state == HOME_M1)
        {
          gfprintf( COM2, "%c%cHome M1?        ", 0x1B, 0x80); // Home M1 (L1)
          gfprintf( COM2, "%c%cY/N             ", 0x1B, 0xC0); // Y/N (L2)
        }

        else if (MM_state == HOME_POSTION && SHOW_HOME == TRUE && H_state == HOME_M2)
        {
          gfprintf( COM2, "%c%cHome M2?        ", 0x1B, 0x80); // Home M2 (L1)
          gfprintf( COM2, "%c%cY/N             ", 0x1B, 0xC0); // Y/N (L2)
        }
        //xSemaphoreGive(SEM_MENU_UPDATED);
      }
    }
  }
}

/****************************** End Of Module *******************************/
