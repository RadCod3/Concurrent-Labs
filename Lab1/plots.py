import matplotlib.pyplot as plt
import numpy as np

# Define the number of threads as the x-axis
threads = np.array([1, 2, 4, 8])

# Case 1 data
serial_times_case1 = np.array([0.03169, np.nan, np.nan, np.nan])
mutex_times_case1 = np.array([0.03164, 0.04799, 0.06288, 0.06632])
rw_times_case1 = np.array([0.03272, 0.01982, 0.01608, 0.01144])

# Case 2 data
serial_times_case2 = np.array([0.04580, np.nan, np.nan, np.nan])
mutex_times_case2 = np.array([0.04865, 0.05639, 0.06212, 0.06518])
rw_times_case2 = np.array([0.04822, 0.02921, 0.01867, 0.02101])

# Case 3 data
serial_times_case3 = np.array([0.09737, np.nan, np.nan, np.nan])
mutex_times_case3 = np.array([0.09983, 0.08036, 0.07379, 0.06648])
rw_times_case3 = np.array([0.10052, 0.06630, 0.03626, 0.04899])

# Function to plot each case
def plot_case(case_number, serial_times, mutex_times, rw_times):
    plt.figure(figsize=(10, 6))
    valid_serial_time = serial_times[0]  

    # Plot a dotted line for the serial times across all threads
    plt.plot(threads, [valid_serial_time] * len(threads), 'k--', label='Serial (dotted)', marker='o')
    plt.plot(threads, mutex_times, label='Mutex', marker='o')
    plt.plot(threads, rw_times, label='RW Lock', marker='o')
    
    plt.title(f'Average Execution Time vs Number of Threads: Case {case_number}')
    plt.xlabel('Number of Threads')
    plt.ylabel('Average Execution Time (seconds)')
    plt.legend()
    plt.grid(True)
    plt.show()

# Plot for Case 1
plot_case(1, serial_times_case1, mutex_times_case1, rw_times_case1)

# Plot for Case 2
plot_case(2, serial_times_case2, mutex_times_case2, rw_times_case2)

# Plot for Case 3
plot_case(3, serial_times_case3, mutex_times_case3, rw_times_case3)
