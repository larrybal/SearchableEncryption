import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file into a DataFrame
data = pd.read_csv("performance_results_static_range.csv")

# Plot 1: Dataset Size vs Query Time (SearchTime)
plt.figure(figsize=(10, 5))
plt.plot(data['DatasetSize'], data['SearchTime'], marker='o', color='b', label='Query Time (ms)')
plt.xlabel('Dataset Size')
plt.ylabel('Query Time (ms)')
plt.title('Dataset Size vs Query Time')
plt.grid(True)
plt.legend()
# plt.xscale('log')  # Optional: Use log scale for better visibility on large range
# plt.yscale('log')  # Optional: Use log scale for better visibility
plt.show()

plt.figure(figsize=(10, 5))
plt.plot(data['DatasetSize'], data['SetupTime'], marker='o', color='g', label='Setup Time (ms)')
plt.xlabel('Dataset Size')
plt.ylabel('Setup Time (ms)')
plt.title('Dataset Size vs Setup Time')
plt.grid(True)
plt.legend()
# plt.xscale('log')  # Optional: Use log scale for better visibility on large range
# plt.yscale('log')  # Optional: Use log scale for better visibility
plt.show()

# plt.figure(figsize=(10, 5))
# plt.plot(data['DatasetSize'], data['MemoryUsageKB'], marker='o', color='g', label='Memory Usage (KB)')
# plt.xlabel('Dataset Size')
# plt.ylabel('Memory Usage (KB)')
# plt.title('Dataset Size vs Memory Usage')
# plt.grid(True)
# plt.legend()
# # plt.xscale('log')  # Optional: Use log scale for better visibility
# # plt.yscale('log')  # Optional: Use log scale for better visibility
# plt.show()
