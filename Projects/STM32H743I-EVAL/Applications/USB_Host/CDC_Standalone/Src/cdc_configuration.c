/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/cdc_configuration.c 
  * @author  MCD Application Team
  * @brief   CDC Settings state machine
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private define ------------------------------------------------------------ */
#define NB_BAUD_RATES   10

/* Private typedef ----------------------------------------------------------- */
uint8_t *DEMO_CONFIGURATION_menu[] = {
  (uint8_t *)
    "      1 - Apply new settings                                                ",
  (uint8_t *)
    "      2 - Set Default settings                                              ",
  (uint8_t *)
    "      3 - Return                                                            ",
};

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
CDC_DEMO_SETTING_StateMachine CdcSettingsState;
static CDC_LineCodingTypeDef LineCoding;
static CDC_LineCodingTypeDef DefaultLineCoding;
uint8_t Prev_BaudRateIdx = 0;
uint8_t Prev_DataBitsIdx = 0;
uint8_t Prev_ParityIdx = 0;
uint8_t Prev_StopBitsIdx = 0;

const uint32_t BaudRateValue[NB_BAUD_RATES] =
  { 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 };

const uint8_t DataBitsValue[4] = { 5, 6, 7, 8 };
const uint8_t ParityArray[3][5] = { "NONE", "EVEN", "ODD" };
const uint8_t StopBitsArray[2][2] = { "1", "2" };

const uint8_t MSG_BITS_PER_SECOND[] =
  "               : Bit Per Second                       ";
const uint8_t MSG_DATA_BITS[] =
  "               : Data width                           ";
const uint8_t MSG_PARITY[] =
  "               : Parity                               ";
const uint8_t MSG_STOP_BITS[] =
  "               : Stop Bits                            ";

/* Private function prototypes ----------------------------------------------- */
static void ConfigurationMenu_Init(void);
static void InitSettingsItems(void);
static void ReturnFromConfigurationMenu(void);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Handles Configuration Menu. 
  * @param  None
  * @retval None
  */
void CDC_Handle_Configuration_Menu(void)
{
  if (CdcSelectMode == CDC_SELECT_MENU)
  {

    switch (CdcDemo.Configuration_state)
    {
    case CDC_CONFIGURATION_IDLE:
      InitSettingsItems();
      ConfigurationMenu_Init();
      CdcSettingsState.select = 0;
      CdcDemo.Configuration_state = CDC_CONFIGURATION_WAIT;
      CDC_SelectItem(DEMO_CONFIGURATION_menu, 0xFF);
      CdcDemo.select = 0;
      break;

    case CDC_CONFIGURATION_WAIT:
      if (CdcDemo.select != PrevSelect)
      {
        PrevSelect = CdcDemo.select;
        CDC_SelectItem(DEMO_CONFIGURATION_menu, CdcDemo.select & 0x7F);
        /* Handle select item */
        if (CdcDemo.select & 0x80)
        {
          switch (CdcDemo.select & 0x7F)
          {
          case 0:              /* Apply new settings */
            USBH_CDC_SetLineCoding(&hUSBHost, &LineCoding);
            ReturnFromConfigurationMenu();
            break;

          case 1:              /* Apply default settings */
            USBH_CDC_SetLineCoding(&hUSBHost, &DefaultLineCoding);
            ReturnFromConfigurationMenu();
            break;

          case 2:              /* Return from configuration menu */
            ReturnFromConfigurationMenu();
            break;

          default:
            break;
          }
        }
      }
      break;

    default:
      break;
    }
  }
  else if (CdcSelectMode == CDC_SELECT_CONFIG)
  {
    CDC_AdjustSettingMenu();
  }
  CdcDemo.select &= 0x7F;
}


/**
  * @brief  Returns default configuration. 
  * @param  None
  * @retval None
  */
void GetDefaultConfiguration(void)
{
  USBH_CDC_GetLineCoding(&hUSBHost, &LineCoding);
  DefaultLineCoding = LineCoding;
}

/**
  * @brief  Returns from Configuration Menu. 
  * @param  None
  * @retval None
  */
static void ReturnFromConfigurationMenu(void)
{
  CdcDemo.state = CDC_DEMO_IDLE;
  CdcDemo.select = 0;

  /* Restore main menu */
  LCD_ClearTextZone();
  UTIL_LCD_TRACE_UpdateDisplay();
  Menu_Init();
}

/**
  * @brief  Initializes Configuration Menu.
  * @param  None
  * @retval None
  */
static void ConfigurationMenu_Init(void)
{
  GUI_SetTextColor(GUI_COLOR_GREEN);
  GUI_DisplayStringAtLine(13,
                              (uint8_t *)
                              "                                           ");
  GUI_DisplayStringAtLine(14,
                              (uint8_t *)
                              "[User Tamper] to switch menu                  ");
  GUI_DisplayStringAtLine(15,
                              (uint8_t *)
                              "[Joystick Left/Right] to change settings   ");
  GUI_DisplayStringAtLine(16,
                              (uint8_t *)
                              "[Joystick Up/Down] to change settings items");
  CDC_ChangeSelectMode(CDC_SELECT_CONFIG);
}

/**
  * @brief  Sets the CDC demo initial values.
  * @param  None
  * @retval None
  */
static void InitSettingsItems(void)
{
  /* Initialize baud rate index accordingly */
  switch (LineCoding.b.dwDTERate)
  {
  case 2400:
    CdcSettingsState.settings.BaudRateIdx = 0;
    break;

  case 4800:
    CdcSettingsState.settings.BaudRateIdx = 1;
    break;

  case 9600:
    CdcSettingsState.settings.BaudRateIdx = 2;
    break;

  case 19200:
    CdcSettingsState.settings.BaudRateIdx = 3;
    break;

  case 38400:
    CdcSettingsState.settings.BaudRateIdx = 4;
    break;

  case 57600:
    CdcSettingsState.settings.BaudRateIdx = 5;
    break;

  case 115200:
    CdcSettingsState.settings.BaudRateIdx = 6;
    break;

  case 230400:
    CdcSettingsState.settings.BaudRateIdx = 7;
    break;

  case 460800:
    CdcSettingsState.settings.BaudRateIdx = 8;
    break;

  case 921600:
    CdcSettingsState.settings.BaudRateIdx = 9;
    break;

  default:
    break;
  }

  /* Initialize data bits index accordingly */
  switch (LineCoding.b.bDataBits)
  {
  case 5:
    CdcSettingsState.settings.DataBitsIdx = 0;
    break;

  case 6:
    CdcSettingsState.settings.DataBitsIdx = 1;
    break;

  case 7:
    CdcSettingsState.settings.DataBitsIdx = 2;
    break;

  case 8:
    CdcSettingsState.settings.DataBitsIdx = 3;
    break;

  default:
    break;
  }

  /* Initialize stop bits index accordingly */
  switch (LineCoding.b.bCharFormat)
  {
  case 1:
    CdcSettingsState.settings.StopBitsIdx = 0;
    break;

  case 2:
    CdcSettingsState.settings.StopBitsIdx = 1;
    break;

  default:
    break;
  }

  /* Initialize parity index accordingly */
  switch (LineCoding.b.bParityType)
  {
  case 0:
    CdcSettingsState.settings.ParityIdx = 0;
    break;

  case 1:
    CdcSettingsState.settings.ParityIdx = 1;
    break;

  case 2:
    CdcSettingsState.settings.ParityIdx = 2;
    break;

  default:
    break;
  }

  Prev_BaudRateIdx = CdcSettingsState.settings.BaudRateIdx;
  Prev_DataBitsIdx = CdcSettingsState.settings.DataBitsIdx;
  Prev_ParityIdx = CdcSettingsState.settings.StopBitsIdx;
  Prev_StopBitsIdx = CdcSettingsState.settings.ParityIdx;
  PrevSelect = 0;
}


/**
  * @brief  Manages the setting menu on the screen.
  * @param  item: Selected item to be highlighted
  * @retval None
  */
void CDC_SelectSettingsItem(uint8_t item)
{
  uint8_t str_temp[40];

  GUI_SetTextColor(GUI_COLOR_WHITE);

  switch (item)
  {
  case 0:
    {
      GUI_SetBackColor(GUI_COLOR_MAGENTA);
      GUI_DisplayStringAtLine(9, (uint8_t *) MSG_BITS_PER_SECOND);
      if (LineCoding.b.dwDTERate <= 9600)
        sprintf((char *)str_temp, "         %lu", LineCoding.b.dwDTERate);
      else if (LineCoding.b.dwDTERate <= 57600)
        sprintf((char *)str_temp, "        %lu", LineCoding.b.dwDTERate);
      else
        sprintf((char *)str_temp, "       %lu", LineCoding.b.dwDTERate);
      GUI_DisplayStringAtLine(9, str_temp);

      /* Display the data bits */
      GUI_SetBackColor(GUI_COLOR_BLUE);
      GUI_DisplayStringAtLine(10, (uint8_t *) MSG_DATA_BITS);
      sprintf((char *)str_temp, "            %d", LineCoding.b.bDataBits);
      GUI_DisplayStringAtLine(10, str_temp);

      /* Display the parity bits */
      GUI_DisplayStringAtLine(11, (uint8_t *) MSG_PARITY);
      if (LineCoding.b.bParityType == 2)
        sprintf((char *)str_temp, "          %s",
                ParityArray[LineCoding.b.bParityType]);
      else
        sprintf((char *)str_temp, "         %s",
                ParityArray[LineCoding.b.bParityType]);
      GUI_DisplayStringAtLine(11, str_temp);

      /* Display the Stop bits */
      GUI_DisplayStringAtLine(12, (uint8_t *) MSG_STOP_BITS);
      sprintf((char *)str_temp, "            %s",
              StopBitsArray[LineCoding.b.bCharFormat]);
      GUI_DisplayStringAtLine(12, str_temp);
    }
    break;

  case 1:
    {
      GUI_SetBackColor(GUI_COLOR_BLUE);
      GUI_DisplayStringAtLine(9, (uint8_t *) MSG_BITS_PER_SECOND);
      if (LineCoding.b.dwDTERate <= 9600)
        sprintf((char *)str_temp, "         %lu", LineCoding.b.dwDTERate);
      else if (LineCoding.b.dwDTERate <= 57600)
        sprintf((char *)str_temp, "        %lu", LineCoding.b.dwDTERate);
      else
        sprintf((char *)str_temp, "       %lu", LineCoding.b.dwDTERate);
      GUI_DisplayStringAtLine(9, str_temp);
      GUI_DisplayStringAtLine(9, str_temp);

      /* Display the data bits */
      GUI_SetBackColor(GUI_COLOR_MAGENTA);
      GUI_DisplayStringAtLine(10, (uint8_t *) MSG_DATA_BITS);
      sprintf((char *)str_temp, "            %d", LineCoding.b.bDataBits);
      GUI_DisplayStringAtLine(10, str_temp);


      /* Display the parity bits */
      GUI_SetBackColor(GUI_COLOR_BLUE);
      GUI_DisplayStringAtLine(11, (uint8_t *) MSG_PARITY);
      if (LineCoding.b.bParityType == 2)
        sprintf((char *)str_temp, "          %s",
                ParityArray[LineCoding.b.bParityType]);
      else
        sprintf((char *)str_temp, "         %s",
                ParityArray[LineCoding.b.bParityType]);
      GUI_DisplayStringAtLine(11, str_temp);

      /* Display the Stop bits */
      GUI_DisplayStringAtLine(12, (uint8_t *) MSG_STOP_BITS);
      sprintf((char *)str_temp, "            %s",
              StopBitsArray[LineCoding.b.bCharFormat]);
      GUI_DisplayStringAtLine(12, str_temp);
    }
    break;

  case 2:
    {
      GUI_SetBackColor(GUI_COLOR_BLUE);
      GUI_DisplayStringAtLine(9, (uint8_t *) MSG_BITS_PER_SECOND);
      if (LineCoding.b.dwDTERate <= 9600)
        sprintf((char *)str_temp, "         %lu", LineCoding.b.dwDTERate);
      else if (LineCoding.b.dwDTERate <= 57600)
        sprintf((char *)str_temp, "        %lu", LineCoding.b.dwDTERate);
      else
        sprintf((char *)str_temp, "       %lu", LineCoding.b.dwDTERate);
      GUI_DisplayStringAtLine(9, str_temp);

      /* Display the data bits */
      GUI_DisplayStringAtLine(10, (uint8_t *) MSG_DATA_BITS);
      sprintf((char *)str_temp, "            %d", LineCoding.b.bDataBits);
      GUI_DisplayStringAtLine(10, str_temp);

      /* Display the parity bits */
      GUI_SetBackColor(GUI_COLOR_MAGENTA);
      GUI_DisplayStringAtLine(11, (uint8_t *) MSG_PARITY);
      if (LineCoding.b.bParityType == 2)
        sprintf((char *)str_temp, "          %s",
                ParityArray[LineCoding.b.bParityType]);
      else
        sprintf((char *)str_temp, "         %s",
                ParityArray[LineCoding.b.bParityType]);
      GUI_DisplayStringAtLine(11, str_temp);

      /* Display the Stop bits */
      GUI_SetBackColor(GUI_COLOR_BLUE);
      GUI_DisplayStringAtLine(12, (uint8_t *) MSG_STOP_BITS);
      sprintf((char *)str_temp, "            %s",
              StopBitsArray[LineCoding.b.bCharFormat]);
      GUI_DisplayStringAtLine(12, str_temp);
    }
    break;

  case 3:
    {
      GUI_SetBackColor(GUI_COLOR_BLUE);
      GUI_DisplayStringAtLine(9, (uint8_t *) MSG_BITS_PER_SECOND);
      if (LineCoding.b.dwDTERate <= 9600)
        sprintf((char *)str_temp, "         %lu", LineCoding.b.dwDTERate);
      else if (LineCoding.b.dwDTERate <= 57600)
        sprintf((char *)str_temp, "        %lu", LineCoding.b.dwDTERate);
      else
        sprintf((char *)str_temp, "       %lu", LineCoding.b.dwDTERate);
      GUI_DisplayStringAtLine(9, str_temp);
      GUI_DisplayStringAtLine(9, str_temp);

      /* Display the data bits */
      GUI_DisplayStringAtLine(10, (uint8_t *) MSG_DATA_BITS);
      sprintf((char *)str_temp, "            %d", LineCoding.b.bDataBits);
      GUI_DisplayStringAtLine(10, str_temp);

      /* Display the parity bits */
      GUI_DisplayStringAtLine(11, (uint8_t *) MSG_PARITY);
      if (LineCoding.b.bParityType == 2)
        sprintf((char *)str_temp, "          %s",
                ParityArray[LineCoding.b.bParityType]);
      else
        sprintf((char *)str_temp, "         %s",
                ParityArray[LineCoding.b.bParityType]);
      GUI_DisplayStringAtLine(11, str_temp);

      /* Display the Stop bits */
      GUI_SetBackColor(GUI_COLOR_MAGENTA);
      GUI_DisplayStringAtLine(12, (uint8_t *) MSG_STOP_BITS);
      sprintf((char *)str_temp, "            %s",
              StopBitsArray[LineCoding.b.bCharFormat]);
      GUI_DisplayStringAtLine(12, str_temp);
    }
    break;

  default:
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAtLine(9, (uint8_t *) MSG_BITS_PER_SECOND);
    if (LineCoding.b.dwDTERate <= 9600)
      sprintf((char *)str_temp, "         %lu", LineCoding.b.dwDTERate);
    else if (LineCoding.b.dwDTERate <= 57600)
      sprintf((char *)str_temp, "        %lu", LineCoding.b.dwDTERate);
    else
      sprintf((char *)str_temp, "       %lu", LineCoding.b.dwDTERate);
    GUI_DisplayStringAtLine(9, str_temp);
    GUI_DisplayStringAtLine(9, str_temp);

    /* Display the data bits */
    GUI_DisplayStringAtLine(10, (uint8_t *) MSG_DATA_BITS);
    sprintf((char *)str_temp, "            %d", LineCoding.b.bDataBits);
    GUI_DisplayStringAtLine(10, str_temp);

    /* Display the parity bits */
    GUI_DisplayStringAtLine(11, (uint8_t *) MSG_PARITY);
    if (LineCoding.b.bParityType == 2)
      sprintf((char *)str_temp, "          %s",
              ParityArray[LineCoding.b.bParityType]);
    else
      sprintf((char *)str_temp, "         %s",
              ParityArray[LineCoding.b.bParityType]);
    GUI_DisplayStringAtLine(11, str_temp);

    /* Display the Stop bits */
    GUI_DisplayStringAtLine(12, (uint8_t *) MSG_STOP_BITS);
    sprintf((char *)str_temp, "            %s",
            StopBitsArray[LineCoding.b.bCharFormat]);
    GUI_DisplayStringAtLine(12, str_temp);
    break;
  }
}

/**
  * @brief  Adjusts CDC configuration setting Menu.
  * @param  None
  * @retval None
  */
void CDC_AdjustSettingMenu(void)
{
  uint8_t str_temp[40];

  GUI_SetBackColor(GUI_COLOR_MAGENTA);

  if (CdcSettingsState.select != PrevSelect)
  {
    PrevSelect = CdcSettingsState.select;
    CDC_SelectSettingsItem(CdcSettingsState.select);
  }

  if (CdcSettingsState.settings.BaudRateIdx != Prev_BaudRateIdx)
  {
    Prev_BaudRateIdx = CdcSettingsState.settings.BaudRateIdx;

    if (BaudRateValue[CdcSettingsState.settings.BaudRateIdx] <= 9600)
      sprintf((char *)str_temp, "         %lu",
              BaudRateValue[CdcSettingsState.settings.BaudRateIdx]);
    else if (BaudRateValue[CdcSettingsState.settings.BaudRateIdx] <= 57600)
      sprintf((char *)str_temp, "        %lu",
              BaudRateValue[CdcSettingsState.settings.BaudRateIdx]);
    else
      sprintf((char *)str_temp, "       %lu",
              BaudRateValue[CdcSettingsState.settings.BaudRateIdx]);

    GUI_DisplayStringAtLine(9, str_temp);

    LineCoding.b.dwDTERate =
      BaudRateValue[CdcSettingsState.settings.BaudRateIdx];
  }

  if (CdcSettingsState.settings.DataBitsIdx != Prev_DataBitsIdx)
  {
    Prev_DataBitsIdx = CdcSettingsState.settings.DataBitsIdx;
    sprintf((char *)str_temp, "            %d",
            DataBitsValue[CdcSettingsState.settings.DataBitsIdx]);
    GUI_DisplayStringAtLine(10, str_temp);
    LineCoding.b.bDataBits =
      DataBitsValue[CdcSettingsState.settings.DataBitsIdx];
  }

  if (CdcSettingsState.settings.ParityIdx != Prev_ParityIdx)
  {
    Prev_ParityIdx = CdcSettingsState.settings.ParityIdx;
    if (CdcSettingsState.settings.ParityIdx == 2)
      sprintf((char *)str_temp, "          %s",
              (uint8_t *) ParityArray[CdcSettingsState.settings.ParityIdx]);
    else
      sprintf((char *)str_temp, "         %s",
              (uint8_t *) ParityArray[CdcSettingsState.settings.ParityIdx]);
    GUI_DisplayStringAtLine(11, str_temp);
    LineCoding.b.bParityType = CdcSettingsState.settings.ParityIdx;
  }

  if (CdcSettingsState.settings.StopBitsIdx != Prev_StopBitsIdx)
  {
    Prev_StopBitsIdx = CdcSettingsState.settings.StopBitsIdx;
    sprintf((char *)str_temp, "            %s",
            StopBitsArray[CdcSettingsState.settings.StopBitsIdx]);
    GUI_DisplayStringAtLine(12, str_temp);
    LineCoding.b.bCharFormat = CdcSettingsState.settings.StopBitsIdx;
  }
  GUI_SetBackColor(GUI_COLOR_BLUE);
}

/**
  * @brief  Probes the CDC Settings joystick state.
  * @param  state: Joystick state
  * @retval None
  */
void CDC_Settings_ProbeKey(uint32_t state)
{
  /* Handle Configuration inputs */
  /**** Vertical selection ****/
  if ((state == JOY_UP) && (CdcSettingsState.select > 0))
  {
    CdcSettingsState.select--;
  }
  else if ((state == JOY_DOWN) && (CdcSettingsState.select < 3))
  {
    CdcSettingsState.select++;
  }
  /**** Horizontal selection ****/
  else if (state == JOY_RIGHT)
  {
    if ((CdcSettingsState.select == 0) &&
        (CdcSettingsState.settings.BaudRateIdx < 9))
    {
      CdcSettingsState.settings.BaudRateIdx++;
    }

    if ((CdcSettingsState.select == 1) &&
        (CdcSettingsState.settings.DataBitsIdx < 3))
    {
      CdcSettingsState.settings.DataBitsIdx++;
    }

    if ((CdcSettingsState.select == 2) &&
        (CdcSettingsState.settings.ParityIdx < 2))
    {
      CdcSettingsState.settings.ParityIdx++;
    }

    if ((CdcSettingsState.select == 3) &&
        (CdcSettingsState.settings.StopBitsIdx < 1))
    {
      CdcSettingsState.settings.StopBitsIdx++;
    }
  }
  else if (state == JOY_LEFT)
  {
    if ((CdcSettingsState.select == 0) &&
        (CdcSettingsState.settings.BaudRateIdx > 0))
    {
      CdcSettingsState.settings.BaudRateIdx--;
    }

    if ((CdcSettingsState.select == 1) &&
        (CdcSettingsState.settings.DataBitsIdx > 0))
    {
      CdcSettingsState.settings.DataBitsIdx--;
    }

    if ((CdcSettingsState.select == 2) &&
        (CdcSettingsState.settings.ParityIdx > 0))
    {
      CdcSettingsState.settings.ParityIdx--;
    }

    if ((CdcSettingsState.select == 3) &&
        (CdcSettingsState.settings.StopBitsIdx > 0))
    {
      CdcSettingsState.settings.StopBitsIdx--;
    }
  }
}

/**
  * @brief  Informs user that Settings have been changed.
  * @param  phost: Host handle
  * @retval None
  */
void USBH_CDC_LineCodingChanged(USBH_HandleTypeDef * phost)
{
  LCD_DbgTrace("New CDC Settings applied!\n");
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/