import matplotlib.pyplot as plt
import numpy as np

# Define the number of threads and the data from each case
threads = np.array([1, 2, 4, 8])

# Data for Mutex and RW lock across cases
mutex_times_case1 = np.array([0.03164, 0.04799, 0.06288, 0.06632])
rw_times_case1 = np.array([0.03272, 0.01982, 0.01608, 0.01144])

mutex_times_case2 = np.array([0.04865, 0.05639, 0.06212, 0.06518])
rw_times_case2 = np.array([0.04822, 0.02921, 0.01867, 0.02101])

mutex_times_case3 = np.array([0.09983, 0.08036, 0.07379, 0.06648])
rw_times_case3 = np.array([0.10052, 0.06630, 0.03626, 0.04899])

# Define colors for each case
colors = ['blue', 'red', 'green']

# Function to plot data for each case
def plot_case_data(threads, mutex_times, rw_times, case_number, color):
    plt.plot(threads, mutex_times, color=color, marker='o', linestyle='-', label=f'Mutex Case {case_number}')
    plt.plot(threads, rw_times, color=color, marker='o', linestyle='--', label=f'RW Lock Case {case_number}')

# Create the plot
plt.figure(figsize=(12, 8))
plot_case_data(threads, mutex_times_case1, rw_times_case1, 1, colors[0])
plot_case_data(threads, mutex_times_case2, rw_times_case2, 2, colors[1])
plot_case_data(threads, mutex_times_case3, rw_times_case3, 3, colors[2])

# Adding details to the plot
plt.title('Performance Analysis of Mutex vs RW Lock Across Different Cases')
plt.xlabel('Number of Threads')
plt.ylabel('Average Execution Time (seconds)')
plt.legend()
plt.grid(True)
plt.show()
