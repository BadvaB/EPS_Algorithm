
#include "EPSOperationModes.h"
#include "EPS.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int ChangeState(EpsState_t state) {
	switch (state) {
	case FullMode:
		EnterFullMode();
		break;
	case CruiseMode:
		EnterCruiseMode();
		break;
	case SafeMode:
		EnterSafeMode();
		break;
	case CriticalMode:
		EnterCriticalMode();
		break;
	default:
		EnterCriticalMode();
	}
	return 0;
}

int EnterFullMode() //FIXME
{
	if (state == FullMode) {
		if (compare_modes_DC(currentDutyCycle, DutyCycleFullMode)) { //FIXME - checking case - WORST or AVERAGE
			return 0;
		}
	}
	UpdateTime_ChangeState();
	updateDutyCyclefromDiffDCtable(&currentDutyCycle,&DutyCycleFullMode); //FIXME
	state = FullMode;
	EpsSetLowVoltageFlag(false);
	return 0;
}

int EnterCruiseMode()
{
	if (state == CruiseMode) {
		if (compare_modes_DC(currentDutyCycle, DutyCycleNormalMode)) { //FIXME - checking case - WORST or AVERAGE
			return 0;
		}
	}
	UpdateTime_ChangeState();
	updateDutyCyclefromDiffDCtable(&currentDutyCycle,&DutyCycleNormalMode); //FIXME
	state = CruiseMode;
	state_time_out = NormalMode_TimeOut; //FIXME
	EpsSetLowVoltageFlag(false);
	return 0;
}

int EnterSafeMode()
{
	if (state == SafeMode) {
		if (compare_modes_DC(currentDutyCycle, DutyCycleSafeMode)) { //FIXME - checking case - WORST or AVERAGE
			return 0;
		}
	}
	UpdateTime_ChangeState();
	updateDutyCyclefromDiffDCtable(&currentDutyCycle,&DutyCycleSafeMode);
	state = SafeMode;
	state_time_out = SafeMode_TimeOut;
	EpsSetLowVoltageFlag(false);
	return 0;
}

int EnterCriticalMode()
{
	if (state == CriticalMode) {
		if (compare_modes_DC(currentDutyCycle, DutyCycleCriticalMode)) { //FIXME - checking case - WORST or AVERAGE
			return 0;
		}
	}
	UpdateTime_ChangeState();
	updateDutyCyclefromDiffDCtable(&currentDutyCycle,&DutyCycleCriticalMode);
	state = CriticalMode;
	state_time_out = CriticalMode_TimeOut;
	EpsSetLowVoltageFlag(true);
	return 0;
}


EpsState_t GetSystemState()
{
	return state;
}


bool EpsGetLowVoltageFlag()
{
	return g_low_volt_flag;
}

void EpsSetLowVoltageFlag(bool low_volt_flag)
{
	g_low_volt_flag = low_volt_flag;
}

bool CheckTimeOut() { //FIXME - new function
	if (state == FullMode) {
		return false; // FullMode doesnt have a time out
	}
	return(curr_time - start_mode_time > state_time_out);
}

int Count_StateChange() { //FIXME - new function that returns if there were a lot of state changes in the past X time or not
	unsigned int idx_arr = min(state_change_count, STATE_CHANGE_ARR_MAX), i = 0;
	state_change_counter = 0;
	for (i = 0; i < idx_arr; i++) {
		if (curr_time - state_change_arr[i] < state_change_time_threshold) {
			state_change_counter++;
		}
		if (state_change_counter == STATE_CHANGE_ARR_MAX) {
			return 1;
		}
	}
	return 0;
}

int UpdateTime_ChangeState() { //FIXME - new function for update time
	start_mode_time = curr_time;
	state_change_arr[state_change_count % STATE_CHANGE_ARR_MAX] = start_mode_time;
	state_change_count++;
	return 0;
}

//void logError(const char* errorMessage) {
//	FILE* logFile = fopen("error_log.txt", "a"); // פתיחת קובץ יומן השגיאות במצב הוספה
//	if (logFile == NULL) {
//		perror("Unable to open log file");
//		return;
//	}
//
//	// מקבל את הזמן הנוכחי
//	time_t currentTime = time(NULL);
//	char* timeStr = ctime(&currentTime);  // המרת הזמן לפורמט מחרוזת
//	timeStr[strlen(timeStr) - 1] = '\0';  // הסרת תו ה-newline בסוף הזמן
//
//	// כתיבת הודעת השגיאה לקובץ היומן
//	fprintf(logFile, "[%s] Error: %s\n", timeStr, errorMessage);
//	fclose(logFile);  // סגירת הקובץ
//}