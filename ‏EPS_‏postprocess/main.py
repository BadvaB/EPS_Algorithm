import matplotlib.pyplot as plt
import numpy as np
import argparse
import re
import os
import time

def DataPlot(input_file):
    # Initialize lists to store data
    curr_time_list = []
    curr_voltage_list = []
    state_list = []
    filtered_voltage_list = []
    prev_avg_list = []
    state_time_out_list = []
    state_change_count_list = []
    state_change_counter_list = []
    duty_cycle_payload_list = []
    flag_payload_on_list = []
    start_mode_time_list = []
    g_low_volt_flag_list = []
    flag_beacon_on_list = []
    change_avg_wc_dc_table_list = []
    time_taken_list = []
    duty_cycle_txrx_beacon_list = []

    # Parse the data
    with open(input_file, "r") as fp:
        for line in fp:
            if line.startswith('#$X'):  # Assuming this is still the start of relevant data lines
                parts = line.split('\t')
                for param in parts:
                    if re.search("curr_time", param):
                        curr_time = int(param.split(':')[1])
                        curr_time_list.append(curr_time)
                    if re.search("curr_voltage", param):
                        curr_voltage = float(param.split(':')[1])/1000.0
                        curr_voltage_list.append(curr_voltage)
                    if re.search("sat_state", param):
                        state = int(param.split(':')[1])
                        state_list.append(state)
                    if re.search("filtered_voltage", param):
                        filtered_voltage = float(param.split(':')[1])/1000.0
                        filtered_voltage_list.append(filtered_voltage)
                    if re.search("prev_avg", param):
                        prev_avg = float(param.split(':')[1])/1000.0
                        prev_avg_list.append(prev_avg)
                    if re.search("state_time_out", param):
                        state_time_out = int(param.split(':')[1])
                        state_time_out_list.append(state_time_out)
                    if re.search("state_change_count_", param):
                        state_change_count = int(param.split(':')[1])
                        state_change_count_list.append(state_change_count)
                    if re.search("state_change_counter", param):
                        state_change_counter = int(param.split(':')[1])
                        state_change_counter_list.append(state_change_counter)
                    if re.search("DutyCycle.Payload_DC", param):
                        duty_cycle_payload = int(param.split(':')[1])
                        duty_cycle_payload_list.append(duty_cycle_payload)
                    if re.search("flag_payload_ON", param):
                        flag_payload_on = int(param.split(':')[1])
                        flag_payload_on_list.append(flag_payload_on)
                    if re.search("start_mode_time", param):
                        start_mode_time = int(param.split(':')[1])
                        start_mode_time_list.append(start_mode_time)
                    if re.search("g_low_volt_flag", param):
                        g_low_volt_flag = int(param.split(':')[1])
                        g_low_volt_flag_list.append(g_low_volt_flag)
                    if re.search("flag_beacon_ON", param):
                        flag_beacon_on = int(param.split(':')[1])
                        flag_beacon_on_list.append(flag_beacon_on)
                    if re.search("change_avg_wc_dc_table", param):
                        change_avg_wc_dc_table = int(param.split(':')[1])
                        change_avg_wc_dc_table_list.append(change_avg_wc_dc_table)
                    if re.search("time_taken", param):
                        time_taken = float(param.split(':')[1])
                        time_taken_list.append(time_taken)
                    if re.search("DutyCycle.TxRx_Beacon_DC", param):
                        duty_cycle_txrx_beacon = int(param.split(':')[1])
                        duty_cycle_txrx_beacon_list.append(duty_cycle_txrx_beacon)

        # Convert lists to numpy arrays
        curr_time_array = np.array(curr_time_list, dtype=np.uint32)
        curr_voltage_array = np.array(curr_voltage_list, dtype=np.float32)
        state_array = np.array(state_list, dtype=np.int32)
        filtered_voltage_array = np.array(filtered_voltage_list, dtype=np.float32)
        prev_avg_array = np.array(prev_avg_list, dtype=np.float32)
        state_time_out_array = np.array(state_time_out_list, dtype=np.uint32)
        state_change_count_array = np.array(state_change_count_list, dtype=np.uint32)
        state_change_counter_array = np.array(state_change_counter_list, dtype=np.uint32)
        duty_cycle_payload_array = np.array(duty_cycle_payload_list, dtype=np.int32)
        flag_payload_on_array = np.array(flag_payload_on_list, dtype=np.int32)
        start_mode_time_array = np.array(start_mode_time_list, dtype=np.uint32)
        g_low_volt_flag_array = np.array(g_low_volt_flag_list, dtype=np.int32)
        flag_beacon_on_array = np.array(flag_beacon_on_list, dtype=np.int32)
        change_avg_wc_dc_table_array = np.array(change_avg_wc_dc_table_list, dtype=np.int32)
        time_taken_array = np.array(time_taken_list, dtype=np.float32)
        duty_cycle_txrx_beacon_array = np.array(duty_cycle_txrx_beacon_list, dtype=np.int32)

        # Plotting for all parameters
        # (Add plotting code for each of the new arrays similar to the previous plots)

        # Plotting for all parameters
        # Plot for curr_time_array
        plt.figure(figsize=(10, 4))
        plt.plot(curr_time_array, label='Current Time', color='blue')
        plt.xlabel('Index')
        plt.ylabel('Current Time')
        plt.title('Current Time Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Current_Time.png'))
        plt.close()

        # Plot for curr_voltage_array
        plt.figure(figsize=(10, 4))
        plt.plot(curr_voltage_array, label='Current Voltage', color='green')
        plt.xlabel('Index')
        plt.ylabel('Current Voltage (V)')
        plt.title('Current Voltage Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Current_Voltage.png'))
        plt.close()

        # Plot for state_array
        plt.figure(figsize=(10, 4))
        plt.plot(state_array, label='Satellite State', color='orange')
        plt.xlabel('Index')
        plt.ylabel('Satellite State')
        plt.title('Satellite State Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Satellite_State.png'))
        plt.close()

        # Plot for filtered_voltage_array
        plt.figure(figsize=(10, 4))
        plt.plot(filtered_voltage_array, label='Filtered Voltage', color='red')
        plt.xlabel('Index')
        plt.ylabel('Filtered Voltage (V)')
        plt.title('Filtered Voltage Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Filtered_Voltage.png'))
        plt.close()

        # Plot for prev_avg_array
        plt.figure(figsize=(10, 4))
        plt.plot(prev_avg_array, label='Previous Average', color='purple')
        plt.xlabel('Index')
        plt.ylabel('Previous Average (V)')
        plt.title('Previous Average Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Previous_Average.png'))
        plt.close()

        # Plot for state_time_out_array
        plt.figure(figsize=(10, 4))
        plt.plot(state_time_out_array, label='State Time Out', color='cyan')
        plt.xlabel('Index')
        plt.ylabel('State Time Out (ms)')
        plt.title('State Time Out Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'State_Time_Out.png'))
        plt.close()

        # Plot for state_change_count_array
        plt.figure(figsize=(10, 4))
        plt.plot(state_change_count_array, label='State Change Count', color='magenta')
        plt.xlabel('Index')
        plt.ylabel('State Change Count')
        plt.title('State Change Count Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'State_Change_Count.png'))
        plt.close()

        # Plot for state_change_counter_array
        plt.figure(figsize=(10, 4))
        plt.plot(state_change_counter_array, label='State Change Counter', color='brown')
        plt.xlabel('Index')
        plt.ylabel('State Change Counter')
        plt.title('State Change Counter Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'State_Change_Counter.png'))
        plt.close()

        # Plot for duty_cycle_payload_array
        plt.figure(figsize=(10, 4))
        plt.plot(duty_cycle_payload_array, label='Duty Cycle Payload', color='black')
        plt.xlabel('Index')
        plt.ylabel('Duty Cycle Payload (%)')
        plt.title('Duty Cycle Payload Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Duty_Cycle_Payload.png'))
        plt.close()

        # Plot for flag_payload_on_array
        plt.figure(figsize=(10, 4))
        plt.plot(flag_payload_on_array, label='Flag Payload ON', color='gray')
        plt.xlabel('Index')
        plt.ylabel('Flag Payload ON')
        plt.title('Flag Payload ON Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Flag_Payload_ON.png'))
        plt.close()

        # Plot for start_mode_time_array
        plt.figure(figsize=(10, 4))
        plt.plot(start_mode_time_array, label='Start Mode Time', color='pink')
        plt.xlabel('Index')
        plt.ylabel('Start Mode Time (ms)')
        plt.title('Start Mode Time Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Start_Mode_Time.png'))
        plt.close()

        # Plot for g_low_volt_flag_array
        plt.figure(figsize=(10, 4))
        plt.plot(g_low_volt_flag_array, label='G Low Volt Flag', color='lime')
        plt.xlabel('Index')
        plt.ylabel('G Low Volt Flag')
        plt.title('G Low Volt Flag Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'G_Low_Volt_Flag.png'))
        plt.close()

        # Plot for flag_beacon_on_array
        plt.figure(figsize=(10, 4))
        plt.plot(flag_beacon_on_array, label='Flag Beacon ON', color='violet')
        plt.xlabel('Index')
        plt.ylabel('Flag Beacon ON')
        plt.title('Flag Beacon ON Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Flag_Beacon_ON.png'))
        plt.close()

        # Plot for change_avg_wc_dc_table_array
        plt.figure(figsize=(10, 4))
        plt.plot(change_avg_wc_dc_table_array, label='Change Avg WC DC Table', color='teal')
        plt.xlabel('Index')
        plt.ylabel('Change Avg WC DC Table')
        plt.title('Change Avg WC DC Table Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Change_Avg_WC_DC_Table.png'))
        plt.close()

        # Plot for time_taken_array
        plt.figure(figsize=(10, 4))
        plt.plot(time_taken_array, label='Time Taken', color='navy')
        plt.xlabel('Index')
        plt.ylabel('Time Taken (ms)')
        plt.title('Time Taken Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Time_Taken.png'))
        plt.close()

        # Plot for duty_cycle_txrx_beacon_array
        plt.figure(figsize=(10, 4))
        plt.plot(duty_cycle_txrx_beacon_array, label='Duty Cycle TxRx Beacon', color='coral')
        plt.xlabel('Index')
        plt.ylabel('Duty Cycle TxRx Beacon (%)')
        plt.title('Duty Cycle TxRx Beacon Over Index')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Duty_Cycle_TxRx_Beacon.png'))
        plt.close()

        # Plotting combinations of parameters

        # Combination: prev_avg, filtered_voltage, curr_voltage
        plt.figure(figsize=(10, 4))
        plt.plot(curr_voltage_array, label='Current Voltage', color='green')
        plt.plot(prev_avg_array, label='Previous Average', color='purple')
        plt.plot(filtered_voltage_array, label='Filtered Voltage', color='red')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Previous Average, Filtered Voltage, Current Voltage')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_prev_avg_filtered_voltage_curr_voltage.png'))
        plt.close()

        # Combination: sat_state, state_time_out, state_change_count
        plt.figure(figsize=(10, 4))
        plt.plot(state_list, label='Satellite State', color='orange')
        plt.plot(state_change_counter_array, label='State Change Counter', color='brown')
        plt.plot(state_change_count_array, label='State Change Count', color='magenta')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Satellite State, State Time Out, State Change Count')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_sat_state_state_change_counter_state_change_count.png'))
        plt.close()

        # Combination: flag_change_counter, DutyCycle, Payload_count
        plt.figure(figsize=(10, 4))
        plt.plot(state_change_counter_array, label='Flag Change Counter',
                 color='brown')  # Assuming this is flag_change_counter
        plt.plot(duty_cycle_payload_array, label='Duty Cycle Payload', color='black')
        plt.plot(flag_payload_on_array, label='Payload Count', color='gray')  # Assuming this is Payload_count
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Flag Change Counter, Duty Cycle, Payload Count')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_flag_change_counter_duty_cycle_payload_count.png'))
        plt.close()

        # Combination: flag_payload_ON, DutyCycle, Payload_DC
        plt.figure(figsize=(10, 4))
        plt.plot(flag_payload_on_array, label='Flag Payload ON', color='gray')
        plt.plot(duty_cycle_payload_array, label='Duty Cycle Payload', color='black')
        plt.plot(duty_cycle_payload_array, label='Payload DC', color='purple')  # Using the same array for illustration
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Flag Payload ON, Duty Cycle, Payload DC')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_flag_payload_ON_duty_cycle_payload_DC.png'))
        plt.close()

        # Combination: state_change_count_, start_mode_time
        plt.figure(figsize=(10, 4))
        plt.plot(state_change_count_array, label='State Change Count', color='magenta')
        plt.plot(start_mode_time_array, label='Start Mode Time', color='pink')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: State Change Count, Start Mode Time')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_state_change_count_start_mode_time.png'))
        plt.close()

        # Combination: state_change_count_, sat_state
        plt.figure(figsize=(10, 4))
        plt.plot(state_change_count_array, label='State Change Count', color='magenta')
        plt.plot(state_list, label='Satellite State', color='orange')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: State Change Count, Satellite State')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_state_change_count_sat_state.png'))
        plt.close()

        # Combination: state_change_count_, sat_state
        plt.figure(figsize=(10, 4))
        plt.plot(filtered_voltage_array, label='Filtered Voltage', color='magenta')
        plt.plot(state_list, label='Satellite State', color='orange')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Filtered Voltage, Satellite State')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_Filtered_Voltage_sat_state.png'))
        plt.close()

        # Combination: start_mode_time, sat_state, g_low_volt_flag
        plt.figure(figsize=(10, 4))
        plt.plot(state_list, label='Satellite State', color='orange')
        plt.plot(g_low_volt_flag_array, label='G Low Volt Flag', color='lime')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Satellite State, G Low Volt Flag')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_sat_state_g_low_volt_flag.png'))
        plt.close()

        # Combination: curr_voltage, g_low_volt_flag
        plt.figure(figsize=(10, 4))
        plt.plot(curr_voltage_array, label='Current Voltage', color='green')
        plt.plot(g_low_volt_flag_array, label='G Low Volt Flag', color='lime')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Current Voltage, G Low Volt Flag')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_curr_voltage_g_low_volt_flag.png'))
        plt.close()

        # Combination: filtered_voltage, DutyCycle, Payload_DC
        plt.figure(figsize=(10, 4))
        plt.plot(filtered_voltage_array, label='Filtered Voltage', color='red')
        plt.plot(duty_cycle_payload_array, label='Duty Cycle Payload', color='black')
        plt.plot(duty_cycle_payload_array, label='Payload DC', color='purple')  # Using the same array for illustration
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Filtered Voltage, Duty Cycle, Payload DC')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_filtered_voltage_duty_cycle_payload_DC.png'))
        plt.close()

        # Combination: avgworse, flag, time_taken (assuming avgworse is calculated elsewhere)
        plt.figure(figsize=(10, 4))
        plt.plot(time_taken_array, label='Time Taken', color='navy')
        plt.plot(flag_payload_on_array, label='Flag', color='gray')  # Assuming flag is represented by flag_payload_on
        plt.plot(filtered_voltage_array, label='Avg Worse',
                 color='brown')  # Using filtered_voltage as avgworse for illustration
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Avg Worse, Flag, Time Taken')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_avgworse_flag_time_taken.png'))
        plt.close()

        # Combination: sat_state
        plt.figure(figsize=(10, 4))
        plt.plot(state_list, label='Satellite State', color='orange')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Satellite State')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_sat_state_single.png'))
        plt.close()

        # Combination: filtered_voltage, curr_voltage
        plt.figure(figsize=(10, 4))
        plt.plot(filtered_voltage_array, label='Filtered Voltage', color='red')
        plt.plot(curr_voltage_array, label='Current Voltage', color='green')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Filtered Voltage, Current Voltage')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_filtered_voltage_curr_voltage.png'))
        plt.close()

        # Combination: sat_state, DutyCycle, Payload_DC
        plt.figure(figsize=(10, 4))
        plt.plot(state_list, label='Satellite State', color='orange')
        plt.plot(duty_cycle_payload_array, label='Duty Cycle Payload', color='black')
        plt.plot(duty_cycle_payload_array, label='Payload DC', color='purple')  # Using the same array for illustration
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Satellite State, Duty Cycle, Payload DC')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_sat_state_duty_cycle_payload_DC.png'))
        plt.close()

        # Combination: flag_Payload_ON
        plt.figure(figsize=(10, 4))
        plt.plot(flag_payload_on_array, label='Flag Payload ON', color='gray')
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Flag Payload ON')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_flag_payload_ON_single.png'))
        plt.close()

        # Combination: flag_beacon_ON, flag_TRX_BEACON_ON
        plt.figure(figsize=(10, 4))
        plt.plot(flag_beacon_on_array, label='Flag Beacon ON', color='violet')
        plt.plot(flag_payload_on_array, label='Flag TRX Beacon ON', color='brown')  # Assuming this is the equivalent
        plt.xlabel('Index')
        plt.ylabel('Values')
        plt.title('Combination: Flag Beacon ON, Flag TRX Beacon ON')
        plt.legend()
        plt.grid(True)
        plt.savefig(os.path.join(output_dir, 'Combination_flag_beacon_ON_flag_TRX_BEACON_ON.png'))
        plt.close()


def main():
    args = parser.parse_args()
    start_time = time.time()
    print("start plotting..")
    DataPlot(args.serial_output)
    end_time = time.time()
    elapsed_time = end_time - start_time
    current_directory = os.getcwd()
    output_dir_file = output_dir + "_log.txt"
    output_dir_file = current_directory + rf"\{output_dir_file}"
    with open(output_dir_file, 'w') as file:
        file.write(f"the input directory: {current_directory}\{input_dir}\n")
        file.write(f"the plot were successfully created! it took {elapsed_time:.2f} seconds\n")
        file.write(rf"plotting file path: {current_directory}\{output_dir}")
        file.write("\n")
    print(f"the plot were successfully created! it took only {elapsed_time:.2f} seconds")
    print(rf"plotting file path: {current_directory}\{output_dir}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    input_dir = input("Please enter the directory for the input (copy from output of EPS ALG): ")
    output_dir = input("Please enter the directory name to save plots: ")
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    parser.add_argument("--serial_output", type=str, default=input_dir, help="Path file")
    main()
