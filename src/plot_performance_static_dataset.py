import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('performance_results_static_dataset.csv')

# Create a figure with two subplots
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 12))

# Plot Search Time for different Range Sizes
dataset_sizes = df['DatasetSize'].unique()
for size in dataset_sizes:
    subset = df[df['DatasetSize'] == size]
    ax1.plot(subset['RangeSize'], subset['SearchTime'], label=f'Dataset Size {size}', marker='o')

# Set labels and title for Search Time plot
ax1.set_xlabel('Range Size')
ax1.set_ylabel('Search Time (ms)')
ax1.set_title('Search Time vs Range Size')
ax1.legend(title='Dataset Size')
ax1.grid(True)

# Plot Accessed Records for different Range Sizes
for size in dataset_sizes:
    subset = df[df['DatasetSize'] == size]
    ax2.plot(subset['RangeSize'], subset['AccessedRecords'], label=f'Dataset Size {size}', marker='x')

# Set labels and title for Accessed Records plot
ax2.set_xlabel('Range Size')
ax2.set_ylabel('Accessed Records')
ax2.set_title('Accessed Records vs Range Size')
ax2.legend(title='Dataset Size')
ax2.grid(True)

# Adjust layout to prevent overlap
plt.tight_layout()

# Show the plots
plt.show()
