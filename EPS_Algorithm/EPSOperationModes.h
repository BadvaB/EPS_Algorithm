#define _CRT_SECURE_NO_DEPRECATE

#ifndef EPSOPERATIONMODES_H_
#define EPSOPERATIONMODES_H_

#include "EPS.h"
#include <stdio.h>
#include <stdbool.h>

//FIXME define number of 
#define STATE_CHANGE_ARR_MAX 	 6
#define STATE_CHANGE_TIME_TRSH 	 5400 // [sec], 90 minutes - 1 rev

typedef enum {
	CriticalMode,
	SafeMode,
	CruiseMode,
	FullMode
}EpsState_t;

//channel_t g_system_state; FIXME- is needed???
extern EpsState_t state;
extern bool g_low_volt_flag; // set to true if in low voltage


extern unsigned int start_mode_time;
extern unsigned int curr_time;
extern unsigned int state_time_out;
extern unsigned int state_change_arr[STATE_CHANGE_ARR_MAX];
extern unsigned int state_change_count;
extern unsigned int state_change_counter;
extern bool change_avg_wc_dc_table;

//// FIXME - add a time parameter: int time = 0 [sec] or something like that
//unsigned int start_mode_time = 0; // in [sec]
//unsigned int curr_time = 0;
//unsigned int state_time_out = 10800; //FIXME - change to DEFAULT_TIME_OUT_SAFE when its allowed
//// FIXME - new array for follow the states changes
//unsigned int state_change_arr[STATE_CHANGE_ARR_MAX];
//unsigned int state_change_count = 0;
//unsigned int state_change_counter = 0;
//// FIXME - new parameters for the DC Duty Cycle table changes
//bool change_avg_wc_dc_table = false;


/*!
 * @brief Executes the necessary procedure in order to initiate the system into Full mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterFullMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Cruise mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCruiseMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Safe mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterSafeMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Critical mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCriticalMode();

/*!
 * @brief Sets the channel state according to the bitwise 'logic on'
 * if the 2'nd bit is '1' the second channel will turn on (channel = 0b01000000)
 * @note please use the defines defined in this header to turn on/off channels
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
//int SetEPS_Channels(channel_t channel);

/*!
 * returns the current system state according to the EpsState_t enumeration
 * @return system state according to EpsState_t
 */
EpsState_t GetSystemState();

/*
 * Gets the current system channel state
 * @return current system channel state
 */
//channel_t GetSystemChannelState();

bool EpsGetLowVoltageFlag();

void EpsSetLowVoltageFlag(bool low_volt_flag);

/*! //FIXME - new function
 * checking if we pass the time out
 * @return true if we pass the time out (changing state), else return false (staying in the same state)
 */
bool CheckTimeOut();

/*! //FIXME - new function
 * forcing state change
 * @return 0 if executed correctly
 */
int ChangeState(EpsState_t state);

/*! //FIXME - new function
 * counting state changes in a X time
 * @return 0 if the number of changes is below the max treshold
 * @return 1 if the number of changes is equal the max treshold (so we need to change to WC DC)
 */
int Count_StateChange();

/*! //FIXME - new function
 * updating time and changes counter upon state change
 * @return 0 if executed correctly
 */
int UpdateTime_ChangeState();



#endif /* EPSOPERATIONMODES_H_ */