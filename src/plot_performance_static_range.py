import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file into a DataFrame
data = pd.read_csv("performance_results_static_range.csv")

# Plot 1: Dataset Size vs Query Time (SearchTime)
plt.figure(figsize=(10, 5))
plt.plot(data['DatasetSize'], data['SearchTime'], marker='o', color='b', label='Query Time (ms)')
plt.xlabel('Dataset Size')
plt.ylabel('Search (ms)')
plt.title('Dataset Size vs Search Time')
plt.grid(True)
plt.legend()
# plt.xscale('log')  # Optional: Use log scale for better visibility on large range
# plt.yscale('log')  # Optional: Use log scale for better visibility
plt.show()
# Plot 2: Dataset Size vs Accessed Records
plt.figure(figsize=(10, 5))
plt.plot(data['DatasetSize'], data['AccessedRecords'], marker='o', color='r', label='Accessed Records')
plt.xlabel('Dataset Size')
plt.ylabel('Accessed Records')
plt.title('Dataset Size vs Accessed Records')
plt.grid(True)
plt.legend()
# plt.xscale('log')  # Optional: Use log scale for better visibility on large range
# plt.yscale('log')  # Optional: Use log scale for better visibility
plt.show()
