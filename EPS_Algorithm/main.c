
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdbool.h>
#include "EPS.h"
#include "EPSOperationModes.h"
#include <direct.h>

int main() {
	printf("Enter the input_file path: ");
	int res = scanf("%1024s", file_path_inputs);
	printf("Enter the output file name (add '.txt' at the end!!!): ");
	res = scanf("%1024s", file_path_outputs);
	EPS_Init();
	int dur = GetSimParams(); //FIXME - update the first line of the sim input and the function before using!
	//int dur = params[0];
	//update_Duty_Cycle_DC_time(params[1]);
	res = 0;
	printf("starting!\n");
	for (int i = 0; i < dur; i++) {
		if (i % 100 == 0) {
			printf("iteration %d started..\n", i);
		}
		res = EPS_Conditioning();
		if (res == FINISH) {
			break;
		}
	}
	fclose(inputs);
	printf("run ended successfully!\n");
	char cwd[MAX_LINE];
	if (_getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working directory: %s\n", cwd);
	}
	printf("output file location: %s\\%s\n", cwd, file_path_outputs);
	return 0;
}