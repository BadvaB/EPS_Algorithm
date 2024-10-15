#define _CRT_SECURE_NO_DEPRECATE
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


#include "EPS.h"
#include "EPSOperationModes.h"

#define MAX_NUMBERS 10  // מקסימום מספרים בשורה

// y[i] = a * x[i] +(1-a) * y[i-1]
int prev_avg = 0;		// y[i-1]
double alpha = 0;			//<! smoothing constant
int curr_voltage = 0;
int filtered_voltage = 0;		// the currently filtered voltage; y[i]

unsigned int NormalMode_TimeOut = DEFAULT_TIME_OUT_NORMAL;
unsigned int SafeMode_TimeOut = DEFAULT_TIME_OUT_SAFE;
unsigned int CriticalMode_TimeOut = DEFAULT_TIME_OUT_CRITICAL;
clock_t start_time;
clock_t end_time;
double time_taken;

const int DEFAULT_EPS_THRESHOLD_VOLTAGES[NUMBER_OF_THRESHOLD_VOLTAGES] = { 6400, 6800, 7100, 6600, 7000, 7200 }; // D: s,c,f , U:s,c,f
const int DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_FULL_MODE[NUNBER_OF_COMPONENT] = { 100, 15, 7, 100, 100, 100, 1, 100};
const int DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_NORMAL_MODE[NUNBER_OF_COMPONENT] = { 100, 15, 2, 100, 100, 85 , 1, 100 };
const int DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_SAFE_MODE[NUNBER_OF_COMPONENT] = { 100, 15, 1, 100, 100, 50, 1, 100 };
const int DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_CRITICAL_MODE[NUNBER_OF_COMPONENT] = { 100, 15, 0, 100, 100, 0, 0, 100 };
const int DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_FULL_MODE[NUNBER_OF_COMPONENT] = { 100, 0, 8, 100, 100, 100, 1, 100 };
const int DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_NORMAL_MODE[NUNBER_OF_COMPONENT] = { 100, 0, 7, 100, 100, 80, 1, 100 };
const int DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_SAFE_MODE[NUNBER_OF_COMPONENT] = { 100, 0, 5, 100, 100, 45, 1, 100 };
const int DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_CRITICAL_MODE[NUNBER_OF_COMPONENT] = { 100, 0, 1, 100, 100, 0, 0, 100 };


unsigned int Duty_Cycle_DC_time = DUTY_CYCLE_TIME;
bool flag_payload_ON = false;
bool flag_beacon_ON = false;

FILE* inputs = NULL;
FILE* outputs = NULL;
char line[MAX_LINE];
int params[MAX_LINE] = { 0 };
char file_path_inputs[MAX_LINE] = INPUTFILE;
char file_path_outputs[MAX_LINE] = OUTPUTFILE;

DutyCycle4Mode currentDutyCycle;

EpsState_t state = CriticalMode;
bool g_low_volt_flag = false;

// FIXME - add a time parameter: int time = 0 [sec] or something like that
unsigned int start_mode_time = 0; // in [sec]
unsigned int curr_time = 0;
unsigned int state_time_out = 10800; //FIXME - change to DEFAULT_TIME_OUT_SAFE when its allowed
// FIXME - new array for follow the states changes
unsigned int state_change_arr[STATE_CHANGE_ARR_MAX];
unsigned int state_change_time_threshold = STATE_CHANGE_TIME_TRSH;
unsigned int state_change_count = 0;
unsigned int state_change_counter = 0;
// FIXME - new parameters for the DC Duty Cycle table changes
bool change_avg_wc_dc_table = false;

// holds all 6 default values for eps_threshold
EpsThreshVolt_t eps_threshold_voltages;

////Components Duty Cycles - update to default
DutyCycle4Mode DutyCycleFullMode_WC;
DutyCycle4Mode DutyCycleNormalMode_WC;
DutyCycle4Mode DutyCycleSafeMode_WC;
DutyCycle4Mode DutyCycleCriticalMode_WC;

DutyCycle4Mode DutyCycleFullMode_AVG;
DutyCycle4Mode DutyCycleNormalMode_AVG;
DutyCycle4Mode DutyCycleSafeMode_AVG;
DutyCycle4Mode DutyCycleCriticalMode_AVG;

DutyCycle4Mode DutyCycleFullMode;
DutyCycle4Mode DutyCycleNormalMode;
DutyCycle4Mode DutyCycleSafeMode;
DutyCycle4Mode DutyCycleCriticalMode;

DutyCycle4Mode currentDutyCycle;


FILE* openFile(FILE* file, char file_path[]) {
	errno_t err = fopen_s(&file, file_path, "r");

	if (err != 0) {
		perror("Error opening file");
		return NULL;
	}
	return file;
}

FILE* openFileOutput(FILE* file, char file_path[]) {
	errno_t err = fopen_s(&file, file_path, "w+");

	if (err != 0) {
		perror("Error opening file");
		return NULL;
	}
	return file;
}

//FIXME - need to add a timer option or add in enter mode the first time we enter it, and check for time out

int line2params() {
	return 0;
}

int GetBatteryVoltage(int* vbatt)
{
	int volt_eps = params[0];
	*vbatt = volt_eps;
	return 0;
}


int EPS_Init()
{
	inputs = openFile(inputs,file_path_inputs);
	outputs = openFileOutput(outputs, file_path_outputs);
	updateEpsThreshVolt_t(DEFAULT_EPS_THRESHOLD_VOLTAGES);
	updateDutyCycle(&DutyCycleFullMode_WC, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_FULL_MODE);
	updateDutyCycle(&DutyCycleNormalMode_WC, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_NORMAL_MODE);
	updateDutyCycle(&DutyCycleSafeMode_WC, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_SAFE_MODE);
	updateDutyCycle(&DutyCycleCriticalMode_WC, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_CRITICAL_MODE);
	updateDutyCycle(&DutyCycleFullMode_AVG, DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_FULL_MODE);
	updateDutyCycle(&DutyCycleNormalMode_AVG, DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_NORMAL_MODE);
	updateDutyCycle(&DutyCycleSafeMode_AVG, DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_SAFE_MODE);
	updateDutyCycle(&DutyCycleCriticalMode_AVG, DEFAULT_DUTY_CYCLE_TABLE_AVERAGE_CASE_CRITICAL_MODE);
	updateDutyCycle(&DutyCycleFullMode, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_FULL_MODE);
	updateDutyCycle(&DutyCycleNormalMode, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_NORMAL_MODE);
	updateDutyCycle(&DutyCycleSafeMode, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_SAFE_MODE);
	updateDutyCycle(&DutyCycleCriticalMode, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_CRITICAL_MODE);
	updateDutyCycle(&currentDutyCycle, DEFAULT_DUTY_CYCLE_TABLE_WORST_CASE_CRITICAL_MODE);

	if (GetThresholdVoltages(&eps_threshold_voltages)) return -1;
	alpha = DEFAULT_ALPHA_VALUE;
	prev_avg = 6000;
	//GetBatteryVoltage(&prev_avg);
	start_mode_time=0;
	curr_time=0;
	state_change_count = 0;
	state = CriticalMode;
	flag_beacon_ON = false;
	flag_payload_ON = false;
	//EPS_Conditioning();
	return 0;
}

#define GetFilterdVoltage(curr_voltage) (int) (alpha * curr_voltage + (1 - alpha) * prev_avg)

int EPS_Conditioning()
{
	if (!(fgets(line, sizeof(line), inputs))) {
		return FINISH;
	}
	start_time = clock();
	params[0]= atoi(line);

	//UPDATE DUTY CYCLES DC
	if (Count_StateChange()==1) { // if there were a lot of state changes
		if (!change_avg_wc_dc_table) {
			UpdateDutyCycleTable_WorstOrAverage(false); //FIXME change to WC DC table
			change_avg_wc_dc_table = true;
		}
	}
	else {
		UpdateDutyCycleTable_WorstOrAverage(true); // FIXME change state to AVG table
		change_avg_wc_dc_table = false;
	}
	//Duty_Cycle_implement();

	//Get voltages
	GetBatteryVoltage(&curr_voltage);
	filtered_voltage = GetFilterdVoltage(curr_voltage);

	// CHANGE SATELLITE MODE/STATE
	if (filtered_voltage < prev_avg) {
		if (filtered_voltage < eps_threshold_voltages.Vdown_safe) {
			EnterCriticalMode();
		}
		else if (filtered_voltage < eps_threshold_voltages.Vdown_cruise) {
			EnterSafeMode();
		}
		else if (filtered_voltage < eps_threshold_voltages.Vdown_full) {
			EnterCruiseMode();
		}
		else if (filtered_voltage > eps_threshold_voltages.Vup_full) {
			EnterFullMode();
		}

	}
	if (filtered_voltage > prev_avg) {
		if (filtered_voltage > eps_threshold_voltages.Vup_full) {
			EnterFullMode();
		}
		else if (filtered_voltage > eps_threshold_voltages.Vup_cruise) {
			EnterCruiseMode();
		}
		else if (filtered_voltage > eps_threshold_voltages.Vup_safe) {
			EnterSafeMode();
		}
	}

	//TIME OUT TO STATE
	if (CheckTimeOut()) {
		ChangeState(state + 1); //moving up //FIXME - maybe opposite
	}

	Duty_Cycle_implement();
	Sleep(10); //artifital addition so we can spot the real time. this time was deducted from the final result
	end_time = clock();
	time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC - 0.01; //second
	time_taken = time_taken *1000.0; // new its in ms and not second
	PrintParams();
	curr_time++;
	prev_avg = filtered_voltage+1;
	return 0;
}

int UpdateAlpha(double new_alpha)
{
	if (new_alpha < 0 || new_alpha > 1) {
		return -2; // logError(-2, "UpdateAlpha");
	}
	alpha = new_alpha;
	return 0;
}

// check: what happens first time when there are no values in the FRAM
int GetThresholdVoltages(EpsThreshVolt_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES])
{
	if (NULL == thresh_volts) {
		return -2;//logError(E_INPUT_POINTER_NULL, "GetThresholdVoltages");
	}

	if (true) {//(logError(FRAM_read((unsigned char*)thresh_volts, EPS_THRESH_VOLTAGES_ADDR, EPS_THRESH_VOLTAGES_SIZE), "GetThresholdVoltages-FRAM_read"))return -1;
		return 0;
	}
}

int RestoreDefaultAlpha()
{
	double def_alpha = DEFAULT_ALPHA_VALUE;
	UpdateAlpha(def_alpha);
	return 0;
}

int UpdateDutyCycleTable_WorstOrAverage(bool average) { //FIXME
	if (average) {
		updateDutyCyclefromDiffDCtable(&DutyCycleFullMode, &DutyCycleFullMode_AVG);
		updateDutyCyclefromDiffDCtable(&DutyCycleNormalMode, &DutyCycleNormalMode_AVG); 
		updateDutyCyclefromDiffDCtable(&DutyCycleSafeMode, &DutyCycleSafeMode_AVG); 
		updateDutyCyclefromDiffDCtable(&DutyCycleCriticalMode, &DutyCycleCriticalMode_AVG); 
		return 0;
	}
	updateDutyCyclefromDiffDCtable(&DutyCycleFullMode, &DutyCycleFullMode_WC);
	updateDutyCyclefromDiffDCtable(&DutyCycleNormalMode, &DutyCycleNormalMode_WC);
	updateDutyCyclefromDiffDCtable(&DutyCycleSafeMode, &DutyCycleSafeMode_WC);
	updateDutyCyclefromDiffDCtable(&DutyCycleCriticalMode, &DutyCycleCriticalMode_WC);
	return 0;
}



int UpdateTimeOut_NormalMode(unsigned int New_TimeOut) { //FIXME - new
	NormalMode_TimeOut = New_TimeOut; //time out [sec]
	return 0;
}

int UpdateTimeOut_SafeMode(unsigned int New_TimeOut) { //FIXME - new
	SafeMode_TimeOut = New_TimeOut; //time out [sec]
	return 0;
}

int UpdateTimeOut_CriticalMode(unsigned int New_TimeOut) { //FIXME - new
	CriticalMode_TimeOut = New_TimeOut; //time out [sec]
	return 0;
}

int Duty_Cycle_implement() {
	// payload implement
	if ((100*(curr_time % Duty_Cycle_DC_time)) < (Duty_Cycle_DC_time * currentDutyCycle.Payload_DC)) { // FIXME - definitions
		flag_payload_ON = true;
	}
	else {
		flag_payload_ON = false;
	}
	// beacon implement
	if ((100 * (curr_time % Duty_Cycle_DC_time)) < (Duty_Cycle_DC_time * currentDutyCycle.TxRx_Beacon_DC)) { // FIXME - definitions
		flag_beacon_ON = true;
	}
	else {
		flag_beacon_ON = false;
	}
	return 0;
}

int PrintParams() {
	fprintf(outputs, "#$X\t");
	fprintf(outputs, "curr_time:%u\t", curr_time);
	fprintf(outputs, "curr_voltage:%d\t", curr_voltage);
	fprintf(outputs, "sat_state:%d\t", state);
	fprintf(outputs, "filtered_voltage:%d\t", filtered_voltage);
	fprintf(outputs, "prev_avg:%d\t", prev_avg);
	fprintf(outputs, "state_time_out:%u\t", state_time_out);
	fprintf(outputs, "state_change_count_:%u\t", state_change_count);
	fprintf(outputs, "state_change_counter:%u\t", state_change_counter);
	fprintf(outputs, "DutyCycle.Payload_DC:%d\t", currentDutyCycle.Payload_DC);
	fprintf(outputs, "flag_payload_ON:%d\t", flag_payload_ON); // indicate if the component is on or off
	fprintf(outputs, "start_mode_time:%u\t", start_mode_time);
	fprintf(outputs, "g_low_volt_flag:%d\t", g_low_volt_flag);
	fprintf(outputs, "flag_beacon_ON:%d\t", flag_beacon_ON); // indicate if the component is on or off
	fprintf(outputs, "change_avg_wc_dc_table:%d\t", change_avg_wc_dc_table);
	fprintf(outputs, "time_taken:%.2f\t", time_taken);
	fprintf(outputs, "DutyCycle.TxRx_Beacon_DC:%d\t", currentDutyCycle.TxRx_Beacon_DC);
	fprintf(outputs, "\n"); //end of parameters
	return 0;
}

int update_Duty_Cycle_DC_time(unsigned int new_DC) {
	Duty_Cycle_DC_time = new_DC;
	return 0;
}

bool compare_modes_DC(DutyCycle4Mode DC1, DutyCycle4Mode DC2) {
	return (DC1.OBC_DC == DC2.OBC_DC &&
		DC1.TxRx_DC == DC2.TxRx_DC &&
		DC1.TxRx_Beacon_DC == DC2.TxRx_Beacon_DC &&
		DC1.Rx_DC == DC2.Rx_DC &&
		DC1.EPS_DC == DC2.EPS_DC &&
		DC1.Payload_DC == DC2.Payload_DC &&
		DC1.Payload_work_mode_DC == DC2.Payload_work_mode_DC &&
		DC1.Ants_DC == DC2.Ants_DC);
}

int updateDutyCycle(DutyCycle4Mode *dc, int data[NUNBER_OF_COMPONENT]) {
    dc->OBC_DC = data[0];
    dc->TxRx_DC = data[1];
	dc->TxRx_Beacon_DC = data[2];
    dc->Rx_DC = data[3];
    dc->EPS_DC = data[4];
    dc->Payload_DC = data[5];
	dc->Payload_work_mode_DC = data[6];
    dc->Ants_DC = data[7];
    return 0;
}


int updateDutyCyclefromDiffDCtable(DutyCycle4Mode *dc1, DutyCycle4Mode *dc2) { //FIXME
	dc1->OBC_DC = dc2->OBC_DC;
	dc1->TxRx_DC = dc2->TxRx_DC;
	dc1->TxRx_Beacon_DC = dc2->TxRx_Beacon_DC;
	dc1->Rx_DC = dc2->Rx_DC;
	dc1->EPS_DC = dc2->EPS_DC;
	dc1->Payload_DC = dc2->Payload_DC;
	dc1->Payload_work_mode_DC = dc2->Payload_work_mode_DC;
	dc1->Ants_DC = dc2->Ants_DC;
	return 0;
}

int updateEpsThreshVolt_t(int threshVolt[NUMBER_OF_THRESHOLD_VOLTAGES]) { // D: s,c,f , U:s,c,f
	eps_threshold_voltages.Vdown_safe = threshVolt[0];
	eps_threshold_voltages.Vdown_cruise = threshVolt[1];
	eps_threshold_voltages.Vdown_full = threshVolt[2];
	eps_threshold_voltages.Vup_safe = threshVolt[3];
	eps_threshold_voltages.Vup_cruise = threshVolt[4];
	eps_threshold_voltages.Vup_full = threshVolt[5];
	return 0;
}

int updateFileName(char name[MAX_LINE]) {
	//strcpy(file_path, name); FIXME - find safer function
	return 0;
}

int GetSimParams() {
	fgets(line, sizeof(line), inputs);
	char* token;
	int i = 0;
	float new_alpha_user;
	token = strtok(line, "\t");
	while (token != NULL && i < MAX_NUMBERS) {
		params[i] = atoi(token);
		i++;
		token = strtok(NULL, "\t");
	}
	update_Duty_Cycle_DC_time(params[1]);
	new_alpha_user = (float)params[2] / 100.0;
	UpdateAlpha(new_alpha_user);
	if (params[3] != 0) {
		state_change_time_threshold = params[3];
	}
	return params[0]; //duration
}



