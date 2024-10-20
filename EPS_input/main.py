import pandas as pd
import os

s_name = ["Nominal: good look on the system - AVG DC, all state", "TO: State Time Out", "DC: WC and AVG tables"]
s_name.append("Outlier: check filter voltage")
time_out_arr = [0, 0, 0]
built_s = 0
name = input("enter run name (name+date):")
duration = input("enter run duration:")
duration = int(duration)
DC_duration = input(f"enter Duty Cycle duration (between 2 to {duration}):")
DC_duration = max(2, min(int(DC_duration), duration))
s_c_for_print = "enter check time for counting state changes"
state_change_time_threshold = input(f"{s_c_for_print} - for DC table switch (100 to {duration}, default 1300):")
state_change_time_threshold = int(state_change_time_threshold)
if (state_change_time_threshold < 100) or (state_change_time_threshold > duration):
    state_change_time_threshold = 0
else:
    state_change_time_threshold = max(100, min(state_change_time_threshold, duration))
alpha = input(f"enter alpha value for the voltage filter (between 0.1 to 1, default 0.5):")
alpha = float(alpha)
if (alpha < 0.1) or (alpha > 1):
    alpha = 0.5

print("which scenario would you like?")
for i in range(len(s_name)):
    print(f"{i}) {s_name[i]}")
print(f"{len(s_name)}) Build your own scenario!")
scenario = input("your choice:")
scenario = int(scenario)
if scenario in range(len(s_name)):
    built_s = 1
    change_TO = 0
    if(s_name[scenario] == "TO: State Time Out"):
        print("you selected the Time Out scenario! pay attention - the time gap between each state is 200")
        change_TO = input("do you want to change the time out of the states? [y/n]")
    if (change_TO == 'y') or (change_TO == 'Y'):
        time_out_arr[0] = input(
            f"enter new time out for Normal Mode (2 to {duration} - up to 32,000):")
        time_out_arr[1] = input(
            f"enter new time out for Safe Mode (2 to {duration} - up to 32,000):")
        time_out_arr[2] = input(
            f"enter new time out for Critical Mode (2 to {duration} - up to 32,000):")
        for j in range(3):
            time_out_arr[j] = int(time_out_arr[j])
            if (time_out_arr[j] < 2) or (time_out_arr[j] > duration) or (time_out_arr[j] > 32000):
                time_out_arr[j] = 0
            else:
                time_out_arr[j] = max(2, min(time_out_arr[j], duration))
    if (s_name[scenario] == "Outlier: check filter voltage"):
        change_alpha = input(f"do you want to change alpha (which now {alpha})? [y/n]")
        if (change_alpha == 'y') or (change_alpha == 'Y'):
            alpha = input(f"enter new alpha value for the voltage filter (between 0.1 to 1, default 0.5):")
            alpha = float(alpha)
            if (alpha < 0.1) or (alpha > 1):
                alpha = 0.5
else:
    print("Build your own scenario!")
    change_TO = input("do you want to change the time out of the states? [y/n]")
    if (change_TO == 'y') or (change_TO == 'Y'):
        time_out_arr[0] = input(
            f"enter new time out for Normal Mode (2 to {duration} - up to 32,000):")
        time_out_arr[1] = input(
            f"enter new time out for Safe Mode (2 to {duration} - up to 32,000):")
        time_out_arr[2] = input(
            f"enter new time out for Critical Mode (2 to {duration} - up to 32,000):")
        for j in range(3):
            time_out_arr[j] = int(time_out_arr[j])
            if (time_out_arr[j] < 2) or (time_out_arr[j] > duration) or (time_out_arr[j] > 32000):
                time_out_arr[j] = 0
            else:
                time_out_arr[j] = max(2, min(time_out_arr[j], duration))
    change_voltage = input("do you want linear voltage changes? [y/n]")
    if (change_voltage == 'y') or (change_voltage == 'Y'):
        change_voltage = 1
    else:
        change_voltage = 0

    num_change = input("enter the number of changes:")
    changes = []
    V_target = 8
    total_time_left = duration
    for x in range(int(num_change)):
        if (x == 0) or (change_voltage == 0):
            V_change = input(f"enter V [V] for change #{x}:")
            V_change = max(6, min(float(V_change), 8))
            if change_voltage == 1:
                V_target = float(input(f"enter V [V] target for #{x}:"))
                V_target = max(6, min(float(V_target), 8))
        else:
            V_change = V_target
            V_target = float(input(f"enter V target for #{x}:"))
            V_target = max(6, min(float(V_target), 8))
        time_V_change = min(total_time_left, int(input(f"enter time of this V (up to {total_time_left}):")))
        gap = change_voltage*(V_target - V_change)/time_V_change
        for i in range(int(time_V_change)):
            changes.append(V_change+gap*i)
        total_time_left = total_time_left-time_V_change
        if total_time_left == 0:
            break
    if len(changes) < duration:
        for i in range(duration-len(changes)):
            changes.append(V_change)
if built_s == 1:
    new_string = s_name[scenario].split(':')[0]
    file_path = fr"C:\Users\bluma\OneDrive\inputs_{new_string}_{name}.txt"
else:
    file_path = fr"C:\Users\bluma\OneDrive\inputs_{name}.txt"


with open(file_path, 'w') as file:
    file.write(f"{duration}\t")
    file.write(f"{DC_duration}\t")
    file.write(f"{int(alpha*100)}\t")
    file.write(f"{state_change_time_threshold}\t")
    for k in range(len(time_out_arr)):
        file.write(f"{time_out_arr[k]}\t")
    file.write("\n")
    if built_s == 1:
        scen_name = s_name[scenario].split(':')[0] + ".txt"
        with open(scen_name, 'r') as additional_file:
            content = additional_file.read()
            file.write(content)
    else:
        for i in range(len(changes)):
            file.write(f"{int(changes[i]*1000)}\n")

print(f"the file was created!\nthe file path: {file_path}")
