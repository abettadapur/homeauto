/**************************************************************************************************
  Filename:       switchFlipper.c
  Revised:        $Date: 2010-08-06 08:56:11 -0700 (Fri, 06 Aug 2010) $
  Revision:       $Revision: 23333 $

  Description:    This file contains the Simple BLE Peripheral sample application
                  for use with the CC2540 Bluetooth Low Energy Protocol Stack.

  Copyright 2010 - 2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"

#include "OnBoard.h"
#include "hal_adc.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_lcd.h"

#include "gatt.h"

#include "hci.h"

#include "gapgattserver.h"
#include "gattservapp.h"
#include "devinfoservice.h"
#include "GATTprofile.h"

#include "gpio.h"

#include "peripheral.h"

#include "gapbondmgr.h"

#include "SwitchFlipper.h"

#if defined FEATURE_OAD
  #include "oad.h"
  #include "oad_target.h"
#endif

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// How often to perform periodic event
#define SBP_PERIODIC_EVT_PERIOD                   5000

// What is the advertising interval when device is discoverable (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL          160

// Limited discoverable mode advertises for 30.72s, and then stops
// General discoverable mode advertises indefinitely

#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_LIMITED

// Minimum connection interval (units of 1.25ms, 80=100ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     80

// Maximum connection interval (units of 1.25ms, 800=1000ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     800

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY         0

// Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT          1000

// Whether to enable automatic parameter update request when a connection is formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         TRUE

// Connection Pause Peripheral time value (in seconds)
#define DEFAULT_CONN_PAUSE_PERIPHERAL         6

// Company Identifier: Texas Instruments Inc. (13)
#define TI_COMPANY_ID                         0x000D

#define INVALID_CONNHANDLE                    0xFFFF

// Length of bd addr as a string
#define B_ADDR_STR_LEN                        15

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8 switchFlipper_TaskID;   // Task ID for internal task/event processing

static uint8 PWM_state;   //


static gaprole_States_t gapProfileState = GAPROLE_INIT;

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8 scanRspData[] =
{
  // complete name
  0xB,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  0x46,0x6c,0x69,0x70,0x53,0x77,0x69,0x74,0x63,0x68
  //"FlipSwitch"
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
static uint8 advertData[] =
{
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // service UUID, to notify central devices what services are included
  // in this peripheral
  0x03,   // length of this data
  GAP_ADTYPE_16BIT_MORE,      // some of the UUID's, but not all
  LO_UINT16( SIMPLEPROFILE_SERV_UUID ),
  HI_UINT16( SIMPLEPROFILE_SERV_UUID ),

};

// GAP GATT Attributes
static uint8 attDeviceName[GAP_DEVICE_NAME_LEN] = "Switch Flipper";

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void switchFlipper_ProcessOSALMsg( osal_event_hdr_t *pMsg );
static void peripheralStateNotificationCB( gaprole_States_t newState );
static void performPeriodicTask( void );
static void simpleProfileChangeCB( uint8 paramID );
static void initGPIO( void );
//static void initPWMTimer( void );
static void switchFlipper_HandleKeys( uint8 shift, uint8 keys );
//static void startPWM( void );
//static void stopPWM( void );

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t switchFlipper_PeripheralCBs =
{
  peripheralStateNotificationCB,  // Profile State Change Callbacks
  NULL                            // When a valid RSSI is read from controller (not used by application)
};

// GAP Bond Manager Callbacks
static gapBondCBs_t switchFlipper_BondMgrCBs =
{
  NULL,                     // Passcode callback (not used by application)
  NULL                      // Pairing / Bonding state Callback (not used by application)
};

// Simple GATT Profile Callbacks
static simpleProfileCBs_t switchFlipper_SimpleProfileCBs =
{
  simpleProfileChangeCB    // Charactersitic value change callback
};
/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SwitchFlipper_Init
 *
 * @brief   Initialization function for the Simple BLE Peripheral App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void SwitchFlipper_Init( uint8 task_id )
{
  switchFlipper_TaskID = task_id;

  // Setup the GAP
  VOID GAP_SetParamValue( TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL );
  
  // Setup the GAP Peripheral Role Profile
  {
      uint8 initial_advertising_enable = FALSE;

    // By setting this to zero, the device will go into the waiting state after
    // being discoverable for 30.72 second, and will not being advertising again
    // until the enabler is set back to TRUE
    uint16 gapRole_AdvertOffTime = 0;

    uint8 enable_update_request = DEFAULT_ENABLE_UPDATE_REQUEST;
    uint16 desired_min_interval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
    uint16 desired_max_interval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
    uint16 desired_slave_latency = DEFAULT_DESIRED_SLAVE_LATENCY;
    uint16 desired_conn_timeout = DEFAULT_DESIRED_CONN_TIMEOUT;

    // Set the GAP Role Parameters
    GAPRole_SetParameter( GAPROLE_ADVERT_ENABLED, sizeof( uint8 ), &initial_advertising_enable );
    GAPRole_SetParameter( GAPROLE_ADVERT_OFF_TIME, sizeof( uint16 ), &gapRole_AdvertOffTime );

    GAPRole_SetParameter( GAPROLE_SCAN_RSP_DATA, sizeof ( scanRspData ), scanRspData );
    GAPRole_SetParameter( GAPROLE_ADVERT_DATA, sizeof( advertData ), advertData );

    GAPRole_SetParameter( GAPROLE_PARAM_UPDATE_ENABLE, sizeof( uint8 ), &enable_update_request );
    GAPRole_SetParameter( GAPROLE_MIN_CONN_INTERVAL, sizeof( uint16 ), &desired_min_interval );
    GAPRole_SetParameter( GAPROLE_MAX_CONN_INTERVAL, sizeof( uint16 ), &desired_max_interval );
    GAPRole_SetParameter( GAPROLE_SLAVE_LATENCY, sizeof( uint16 ), &desired_slave_latency );
    GAPRole_SetParameter( GAPROLE_TIMEOUT_MULTIPLIER, sizeof( uint16 ), &desired_conn_timeout );
  }

  // Set the GAP Characteristics
  GGS_SetParameter( GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName );

  // Set advertising interval
  {
    uint16 advInt = DEFAULT_ADVERTISING_INTERVAL;

    GAP_SetParamValue( TGAP_LIM_DISC_ADV_INT_MIN, advInt );
    GAP_SetParamValue( TGAP_LIM_DISC_ADV_INT_MAX, advInt );
    GAP_SetParamValue( TGAP_GEN_DISC_ADV_INT_MIN, advInt );
    GAP_SetParamValue( TGAP_GEN_DISC_ADV_INT_MAX, advInt );
  }

  // Setup the GAP Bond Manager
  {
    uint32 passkey = 0; // passkey "000000"
    uint8 pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
    uint8 mitm = TRUE;
    uint8 ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
    uint8 bonding = TRUE;
    GAPBondMgr_SetParameter( GAPBOND_DEFAULT_PASSCODE, sizeof ( uint32 ), &passkey );
    GAPBondMgr_SetParameter( GAPBOND_PAIRING_MODE, sizeof ( uint8 ), &pairMode );
    GAPBondMgr_SetParameter( GAPBOND_MITM_PROTECTION, sizeof ( uint8 ), &mitm );
    GAPBondMgr_SetParameter( GAPBOND_IO_CAPABILITIES, sizeof ( uint8 ), &ioCap );
    GAPBondMgr_SetParameter( GAPBOND_BONDING_ENABLED, sizeof ( uint8 ), &bonding );
  }

  // Initialize GATT attributes
  GGS_AddService( GATT_ALL_SERVICES );            // GAP
  GATTServApp_AddService( GATT_ALL_SERVICES );    // GATT attributes
  DevInfo_AddService();                           // Device Information Service
  SimpleProfile_AddService( GATT_ALL_SERVICES );  // Simple GATT Profile
#if defined FEATURE_OAD
  VOID OADTarget_AddService();                    // OAD Profile
#endif

  // Setup the SimpleProfile Characteristic Values
  {
    //TODO
    uint8 charValue1 = 1;
    uint8 charValue2 = 2;
    uint8 charValue3 = 3;
    uint8 charValue4 = 4;
    uint8 charValue5[SIMPLEPROFILE_CHAR5_LEN] = { 1, 2, 3, 4, 5 };
    SimpleProfile_SetParameter( SIMPLEPROFILE_CHAR1, sizeof ( uint8 ), &charValue1 );
    SimpleProfile_SetParameter( SIMPLEPROFILE_CHAR2, sizeof ( uint8 ), &charValue2 );
    SimpleProfile_SetParameter( SIMPLEPROFILE_CHAR3, sizeof ( uint8 ), &charValue3 );
    SimpleProfile_SetParameter( SIMPLEPROFILE_CHAR4, sizeof ( uint8 ), &charValue4 );
    SimpleProfile_SetParameter( SIMPLEPROFILE_CHAR5, SIMPLEPROFILE_CHAR5_LEN, charValue5 );
  }


  SK_AddService( GATT_ALL_SERVICES ); // Simple Keys Profile

  // Register for all key events - This app will handle all key events
  RegisterForKeys( switchFlipper_TaskID );

  // makes sure LEDs are off
  //HalLedSet( (HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_FLASH );
  initGPIO();
 
  // Register callback with SimpleGATTprofile
  VOID SimpleProfile_RegisterAppCBs( &switchFlipper_SimpleProfileCBs );

  // Enable clock divide on halt
  // This reduces active current while radio is active and CC254x MCU
  // is halted
  HCI_EXT_ClkDivOnHaltCmd( HCI_EXT_ENABLE_CLK_DIVIDE_ON_HALT );
  // Setup a delayed profile startup
  osal_set_event( switchFlipper_TaskID, SBP_START_DEVICE_EVT );
}

static void initGPIO( void ){
  P0SEL = 0; // Configure Port 0 as GPIO
  P1SEL = 0; // Configure Port 1 as GPIO
  P2SEL = 0; // Configure Port 2 as GPIO

  P0DIR = 0xFC; // Port 0 pins P0.0 and P0.1 as input (buttons),
                // all others (P0.2-P0.7) as output
  P1DIR = 0xFF; // All port 1 pins (P1.0-P1.7) as output
  P2DIR = 0x1F; // All port 2 pins (P2.0-P2.4) as output

  P0 = 0x03; // All pins on port 0 to low except for P0.0 and P0.1 (buttons)
  P1 &= 0x7F ;
  P2 = 0;   // All pins on port 2 to low
  
  P1IEN=0x00; // Disable all Interrupts on Port 1
}


void pwmInit(void)
{
    PERCFG |= 0x20;             // Timer 3 Alternate location 2
    P1DIR |= 0x80;              // P1_7 = output
    P1SEL |= 0x80;              // Peripheral function on P1_7

    T3CTL &= ~0x10;             // Stop timer 3 (if it was running)
    T3CTL |= 0x04;              // Clear timer 3
    T3CTL &= ~0x08;             // Disable Timer 3 overflow interrupts
    T3CTL |= 0x03;              // Timer 3 mode = 3 - Up/Down
  
    T3CCTL1 &= ~0x40;           // Disable channel 0 interrupts
    T3CCTL1 |= 0x04;            // Ch1 mode = compare
    T3CCTL1 |= 0x10;            // Ch0 output compare mode = toggle on compare
    
    //CLKCONSTA&= 0xC7;
    CLKCONCMD|= 0x04<<3;
    while((CLKCONSTA & 0x38)!=0x20);
}

/** \brief	Stops the buzzer
*
* Turn off the buzzer
*
*/
void pwmStop(void)
{
    T3CTL &= ~0x10;             // Stop timer 3
    HCI_EXT_ClkDivOnHaltCmd( HCI_EXT_ENABLE_CLK_DIVIDE_ON_HALT );
    osal_pwrmgr_device( PWRMGR_BATTERY );
}


/** \brief	Starts the buzzer
*
* Starts the buzzer with given frequency
*
* \param[in]       frequency
*     The frequency in Hertz of the sound to output
* @return  1 successful - 0 if frequency invalid
*/
uint8 pwmStart(uint16 frequency)
{
    HCI_EXT_ClkDivOnHaltCmd( HCI_EXT_DISABLE_CLK_DIVIDE_ON_HALT );
    osal_pwrmgr_device( PWRMGR_ALWAYS_ON );

//    pwmInit();
    uint8 prescaler = 0;

    // Get current Timer tick divisor setting
    uint8 tickSpdDiv = (CLKCONSTA & 0x38)>>3;

    // Check if frequency too low
    if (frequency < (244 >> tickSpdDiv)){   // 244 Hz = 32MHz / 256 (8bit counter) / 4 (up/down counter and toggle on compare) / 128 (max timer prescaler)
        pwmStop();                       // A lower tick speed will lower this number accordingly.
        return 0;
    }

    // Calculate nr of ticks required to achieve target frequency
    uint32 ticks = (8000000/frequency) >> tickSpdDiv;      // 8000000 = 32M / 4;

    // Fit this into an 8bit counter using the timer prescaler
    while ((ticks & 0xFFFFFF00) != 0)
    {
        ticks >>= 1;
        prescaler += 32;
    }

    // Update registers
    T3CTL &= ~0xE0;
    T3CTL |= prescaler;
    T3CC0 = (uint8)ticks;

    // Start timer
    T3CTL |= 0x10;
    
    return 1;
}



/*
static void initPWMTimer( void ){
  PERCFG=0x00;// Reset PERCFG
  PERCFG |=(1<<5);//Timer 3 Alternativ 2
  P2SEL |= 0xF0;//Give Timer 3 preference
  CLKCONSTA&= 0xC7;
  CLKCONSTA|= 7<<3;
  T3CCTL1 = (7 << 6) | (5 << 3) | (1 << 2);//Set Timer 3 channel 1 to compare mode with toggling in up and down
  T3CTL= (7 << 3) | (1 << 2) | 0x00;// Free running, clear counter, no interrupt, lots of divisions
  T3CC1 = 0xBF;//Duty 
}

static void startPWM( void )
{
  T3CTL|=(1<<4); //Start timer 3
}

static void stopPWM( void )
{
 T3CTL&=~(1<<4);// Stop Timer 1
 T3CTL|=(1 << 2);// Reset T1CNT=0x0000 
}
*/

/*********************************************************************
 * @fn      SwitchFlipper_ProcessEvent
 *
 * @brief   Simple BLE Peripheral Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16 SwitchFlipper_ProcessEvent( uint8 task_id, uint16 events )
{

  VOID task_id; // OSAL required parameter that isn't used in this function

  if ( events & SYS_EVENT_MSG )
  {
    uint8 *pMsg;

    if ( (pMsg = osal_msg_receive( switchFlipper_TaskID )) != NULL )
    {
      switchFlipper_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );

      // Release the OSAL message
      VOID osal_msg_deallocate( pMsg );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  if ( events & SBP_START_DEVICE_EVT )
  {
    // Start the Device
    VOID GAPRole_StartDevice( &switchFlipper_PeripheralCBs );

    // Start Bond Manager
    VOID GAPBondMgr_Register( &switchFlipper_BondMgrCBs );

    // Set timer for first periodic event
      pwmInit();
      PWM_state = 0;
     // osal_start_timerEx( switchFlipper_TaskID, SBP_PWM_EVT, 25 );

    return ( events ^ SBP_START_DEVICE_EVT );
  }
  
  if ( events & SBP_PWM_STOP_EVT ){     
     pwmStop();
     PWM_state=0;
           HalLedSet( (HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_OFF );

    //P1 ^= (0x01<<7) ;
    return (events ^ SBP_PWM_STOP_EVT);
  }

  // Discard unknown events
  return 0;
}

/*********************************************************************
 * @fn      switchFlipper_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void switchFlipper_ProcessOSALMsg( osal_event_hdr_t *pMsg )
{
  switch ( pMsg->event )
  {
    case KEY_CHANGE:
      switchFlipper_HandleKeys( ((keyChange_t *)pMsg)->state, ((keyChange_t *)pMsg)->keys );
      break;
  default:
    // do nothing
    break;
  }
}

/*********************************************************************
 * @fn      switchFlipper_HandleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
static void switchFlipper_HandleKeys( uint8 shift, uint8 keys )
{
  uint8 SK_Keys = 0;

  VOID shift;  // Intentionally unreferenced parameter

  if ( keys & HAL_KEY_SW_1 )
  {
    SK_Keys |= SK_KEY_LEFT;
    HalLedSet( HAL_LED_1, HAL_LED_MODE_ON );
    pwmStart(100);
    osal_start_timerEx( switchFlipper_TaskID, SBP_PWM_STOP_EVT, 400 ); 
  }

  if ( keys & HAL_KEY_SW_2 )
  {
    SK_Keys |= SK_KEY_RIGHT;
    HalLedSet( HAL_LED_2, HAL_LED_MODE_ON );
    pwmStart(500);
    osal_start_timerEx( switchFlipper_TaskID, SBP_PWM_STOP_EVT, 400 ); 
    // if device is not in a connection, pressing the right key should toggle
    // advertising on and off

    if( gapProfileState != GAPROLE_CONNECTED )
    {
      uint8 current_adv_enabled_status;
      uint8 new_adv_enabled_status;

      //Find the current GAP advertisement status
      GAPRole_GetParameter( GAPROLE_ADVERT_ENABLED, &current_adv_enabled_status );

      if( current_adv_enabled_status == FALSE )
      {
        new_adv_enabled_status = TRUE;
      }
      else
      {
        new_adv_enabled_status = FALSE;
      }

      //change the GAP advertisement status to opposite of current status
      GAPRole_SetParameter( GAPROLE_ADVERT_ENABLED, sizeof( uint8 ), &new_adv_enabled_status );
    }
  }

  // Set the value of the keys state to the Simple Keys Profile;
  // This will send out a notification of the keys state if enabled
  SK_SetParameter( SK_KEY_ATTR, sizeof ( uint8 ), &SK_Keys );
}

/*********************************************************************
 * @fn      peripheralStateNotificationCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void peripheralStateNotificationCB( gaprole_States_t newState )
{
#ifdef PLUS_BROADCASTER
  static uint8 first_conn_flag = 0;
#endif // PLUS_BROADCASTER
  
  
  switch ( newState )
  {
    case GAPROLE_STARTED:
      {
        uint8 ownAddress[B_ADDR_LEN];
        uint8 systemId[DEVINFO_SYSTEM_ID_LEN];

        GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);

        // use 6 bytes of device address for 8 bytes of system ID value
        systemId[0] = ownAddress[0];
        systemId[1] = ownAddress[1];
        systemId[2] = ownAddress[2];

        // set middle bytes to zero
        systemId[4] = 0x00;
        systemId[3] = 0x00;

        // shift three bytes up
        systemId[7] = ownAddress[5];
        systemId[6] = ownAddress[4];
        systemId[5] = ownAddress[3];

        DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);

      }
      break;

    case GAPROLE_ADVERTISING:
      {
        //HalLedSet( HAL_LED_1, HAL_LED_MODE_FLASH );
        //HalLedSet( HAL_LED_2, HAL_LED_MODE_FLASH );
      }
      break;

    case GAPROLE_CONNECTED:
      {        
          
#ifdef PLUS_BROADCASTER
        // Only turn advertising on for this state when we first connect
        // otherwise, when we go from connected_advertising back to this state
        // we will be turning advertising back on.
        if ( first_conn_flag == 0 ) 
        {
          uint8 adv_enabled_status = 1;
          GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8), &adv_enabled_status); // Turn on Advertising
          first_conn_flag = 1;
        }
#endif // PLUS_BROADCASTER
      }
      break;

    case GAPROLE_CONNECTED_ADV:
      {
        
      }
      break;      
    case GAPROLE_WAITING:
      {}
      break;
      
    case GAPROLE_WAITING_AFTER_TIMEOUT:
      {
          
#ifdef PLUS_BROADCASTER
        // Reset flag for next connection.
        first_conn_flag = 0;
#endif //#ifdef (PLUS_BROADCASTER)
      }
      break;

    case GAPROLE_ERROR:
      {
      }
      break;

    default:
      {
      }
      break;

  }

  gapProfileState = newState;

#if !defined( CC2540_MINIDK )
  VOID gapProfileState;     // added to prevent compiler warning with
                            // "CC2540 Slave" configurations
#endif


}

/*********************************************************************
 * @fn      performPeriodicTask
 *
 * @brief   Perform a periodic application task. This function gets
 *          called every five seconds as a result of the SBP_PERIODIC_EVT
 *          OSAL event. In this example, the value of the third
 *          characteristic in the SimpleGATTProfile service is retrieved
 *          from the profile, and then copied into the value of the
 *          the fourth characteristic.
 *
 * @param   none
 *
 * @return  none
 */
static void performPeriodicTask( void )
{
  uint8 valueToCopy;
  uint8 stat;
//TODO
  // Call to retrieve the value of the third characteristic in the profile
  //stat = SimpleProfile_GetParameter( SIMPLEPROFILE_CHAR3, &valueToCopy);

  //if( stat == SUCCESS )
  //{
    /*
     * Call to set that value of the fourth characteristic in the profile. Note
     * that if notifications of the fourth characteristic have been enabled by
     * a GATT client device, then a notification will be sent every time this
     * function is called.
     */
  //  SimpleProfile_SetParameter( SIMPLEPROFILE_CHAR4, sizeof(uint8), &valueToCopy);
  //}
}

/*********************************************************************
 * @fn      simpleProfileChangeCB
 *
 * @brief   Callback from SimpleBLEProfile indicating a value change
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  none
 */
static void simpleProfileChangeCB( uint8 paramID )
{
  uint8 newValue;

  switch( paramID )
  {
    case SIMPLEPROFILE_CHAR1:
      SimpleProfile_GetParameter( SIMPLEPROFILE_CHAR1, &newValue );
      pwmStart(500);
      osal_start_timerEx( switchFlipper_TaskID, SBP_PWM_STOP_EVT, 400 ); 
      break;

    case SIMPLEPROFILE_CHAR3:
      SimpleProfile_GetParameter( SIMPLEPROFILE_CHAR3, &newValue );
      pwmStart(100);
      osal_start_timerEx( switchFlipper_TaskID, SBP_PWM_STOP_EVT, 400 ); 
      break;

    default:
      // should not reach here!
      break;
  }
}

/*********************************************************************
*********************************************************************/
