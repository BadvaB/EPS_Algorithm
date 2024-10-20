/*
 * @file	EPS.h
 * @brief	EPS- Energy Powering System.This system is incharge of the energy consumtion
 * 			the satellite and switching on and off power switches(5V, 3V3)
 * @see		inspect logic flowcharts thoroughly in order to write the code in a clean manner.
 */
#ifndef EPS_H_
#define EPS_H_
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "EPSOperationModes.h"

#define OUTPUTFILE "outputs_A11_state_time_out.txt"
#define INPUTFILE "inputs_A11_state_time_out.txt"
#define MAX_LINE 1024
#define FINISH 3
#define _CRT_SECURE_NO_DEPRECATE
 /*
				   ______
			   ___|		|___
			  |				|
			  |	FULL MODE	|
			  |- - - - - - -	|	-> FULL UP = 7200
			  |- - - - - - - |	-> FULL DOWN = 7100
			  |				|
			  |	CRUISE MODE	|
			  |- - - - - - -	|	-> CRUISE UP = 7000
			  |- - - - - - - |	-> CRUISE DOWN = 6800
			  |				|
			  |	SAFE MODE	|
			  |- - - - - - -	| 	-> SAFE UP = 6600
			  |- - - - - - - |	-> SAFE DOWN = 6400
			  |				|
			  |	CRITICAL	|
			  |______________|
  */
#define DEFAULT_ALPHA_VALUE 0.7

#define NUMBER_OF_SOLAR_PANELS			6
#define NUMBER_OF_THRESHOLD_VOLTAGES 	6 		///< first 3 are charging voltages, last 3 are discharging voltages
#define NUNBER_OF_COMPONENT 8
#define MAX_NUMBERS 10  // מקסימום מספרים בשורה

  // FIXME - define time outs
#define DEFAULT_TIME_OUT_NORMAL 500 //21600 // [sec], 360 minutes - 4 revs
#define DEFAULT_TIME_OUT_SAFE 300 //16200 // [sec], 270 minutes - 3 revs
#define DEFAULT_TIME_OUT_CRITICAL 250 //10800 // [sec], 180 minutes - 2 revs

#define DUTY_CYCLE_TIME 5400 // [sec], 90 minutes - 1 rev

extern const int DEFAULT_EPS_THRESHOLD_VOLTAGES[NUMBER_OF_THRESHOLD_VOLTAGES];
extern const int DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_FULL_MODE[NUNBER_OF_COMPONENT];
extern const int DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_NORMAL_MODE[NUNBER_OF_COMPONENT];
extern const int DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_SAFE_MODE[NUNBER_OF_COMPONENT];
extern const int DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_CRITICAL_MODE[NUNBER_OF_COMPONENT];
extern const int DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_FULL_MODE[NUNBER_OF_COMPONENT];
extern const int DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_NORMAL_MODE[NUNBER_OF_COMPONENT];
extern const int DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_SAFE_MODE[NUNBER_OF_COMPONENT];
extern const int DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_CRITICAL_MODE[NUNBER_OF_COMPONENT];

extern double alpha;

extern unsigned int NormalMode_TimeOut;
extern unsigned int SafeMode_TimeOut;
extern unsigned int CriticalMode_TimeOut;

extern unsigned int Duty_Cycle_DC_time;
extern bool flag_payload_ON;
extern bool flag_beacon_ON;

extern FILE* inputs;
extern FILE* outputs;
extern char line[MAX_LINE];
extern int params[MAX_NUMBERS];
extern char file_name[MAX_LINE];
extern char file_path_inputs[MAX_LINE];
extern char file_path_outputs[MAX_LINE];

typedef enum {
	INDEX_DOWN_SAFE,
	INDEX_DOWN_CRUISE,
	INDEX_DOWN_FULL,
	INDEX_UP_SAFE,
	INDEX_UP_CRUISE,
	INDEX_UP_FULL
} EpsThresholdsIndex;



typedef struct {
	int Vdown_safe;
	int Vdown_cruise;
	int Vdown_full;
	int Vup_safe;
	int Vup_cruise;
	int Vup_full;
}EpsThreshVolt_t;

typedef struct {
	int OBC_DC;
	int TxRx_DC;
	int TxRx_Beacon_DC; // not 100
	int Rx_DC;
	int EPS_DC;
	int Payload_DC; // not 100
	int Payload_work_mode_DC; // not 100
	int Ants_DC; 
}DutyCycle4Mode;


extern DutyCycle4Mode DutyCycleFullMode_WC;
extern DutyCycle4Mode DutyCycleNormalMode_WC;
extern DutyCycle4Mode DutyCycleSafeMode_WC;
extern DutyCycle4Mode DutyCycleCriticalMode_WC;

extern DutyCycle4Mode DutyCycleFullMode_AVG;
extern DutyCycle4Mode DutyCycleNormalMode_AVG;
extern DutyCycle4Mode DutyCycleSafeMode_AVG;
extern DutyCycle4Mode DutyCycleCriticalMode_AVG;

extern DutyCycle4Mode DutyCycleFullMode;
extern DutyCycle4Mode DutyCycleNormalMode;
extern DutyCycle4Mode DutyCycleSafeMode;
extern DutyCycle4Mode DutyCycleCriticalMode;

extern DutyCycle4Mode currentDutyCycle;

/*!
 * @brief initializes the EPS subsystem.
 * @return	0 on success
 * 			-1 on failure of init
 */
int EPS_Init();

/*!
 * @brief EPS logic. controls the state machine of which subsystem
 * is on or off, as a function of only the battery voltage
 * @return	0 on success
 * 			-1 on failure setting state of channels
 */
int EPS_Conditioning();

/*!
 * @brief returns the current voltage on the battery
 * @param[out] vbat he current battery voltage
 * @return	0 on success
 * 			Error code according to <hal/errors.h>
 */
int GetBatteryVoltage(int* vbat);

/*!
 * @brief setting the new EPS logic threshold voltages on the FRAM.
 * @param[in] thresh_volts an array holding the new threshold values
 * @return	0 on success
 * 			-1 on failure setting new threshold voltages
 * 			-2 on invalid thresholds
 * 			ERR according to <hal/errors.h>
 */
int UpdateThresholdVoltages(EpsThreshVolt_t* thresh_volts);

/*!
 * @brief getting the EPS logic threshold  voltages on the FRAM.
 * @param[out] thresh_volts a buffer to hold the threshold values
 * @return	0 on success
 * 			-1 on NULL input array
 * 			-2 on FRAM read errors
 */
int GetThresholdVoltages(EpsThreshVolt_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES]);


/*!
 * @brief setting the new voltage smoothing factor (alpha) on the FRAM.
 * @param[in] new_alpha new value for the smoothing factor alpha
 * @note new_alpha is a value in the range - (0,1)
 * @return	0 on success
 * 			-1 on failure setting new smoothing factor
 * 			-2 on invalid alpha
 * @see LPF- Low Pass Filter at wikipedia: https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization
 */
int UpdateAlpha(double new_alpha);

/*!
 * @brief setting the new voltage smoothing factor (alpha) to be the default value.
 * @return	0 on success
 * 			-1 on failure setting new smoothing factor
 * @see DEFAULT_ALPHA_VALUE
 */
int RestoreDefaultAlpha();

/*!
 * @brief	setting the new EPS logic threshold voltages on the FRAM to the default.
 * @return	0 on success
 * 			-1 on failure setting smoothing factor
  * @see EPS_DEFAULT_THRESHOLD_VOLTAGES
 */
//int RestoreDefaultThresholdVoltages();

 /*! //FIXME - new function
  * @Updates the Time Out for the states - each state has its own command
  * @return	0 on success
  * 			-1 on failure
  */
int UpdateTimeOut_NormalMode(unsigned int New_TimeOut);

int UpdateTimeOut_SafeMode(unsigned int New_TimeOut);

int UpdateTimeOut_CriticalMode(unsigned int New_TimeOut);

int UpdateDutyCycleTable_WorstOrAverage(bool average); //FIXME - maybe add update function

int Duty_Cycle_implement(); //FIXME - lemamesh this function

int GetSystemCurrent(int* SystemCurrent); //FIXME - check functionality

int PrintParams();  //FIXME

int update_Duty_Cycle_DC_time(unsigned int new_DC); //FIXME

bool compare_modes_DC(DutyCycle4Mode DC1, DutyCycle4Mode DC2); // FIXME - new function

int updateEpsThreshVolt_t(int threshVolt[NUMBER_OF_THRESHOLD_VOLTAGES]);

int updateDutyCycle(DutyCycle4Mode* dc, int data[NUNBER_OF_COMPONENT]);

int updateDutyCyclefromTable(DutyCycle4Mode* dc, int data[NUNBER_OF_COMPONENT]);

int updateDutyCyclefromDiffDCtable(DutyCycle4Mode *dc1, DutyCycle4Mode *dc2);

int updateFileName(char name[MAX_LINE]);

int GetSimParams();

#endif