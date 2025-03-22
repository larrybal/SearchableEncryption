import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('performance_results_static_dataset.csv')

# Create a figure
plt.figure(figsize=(10, 6))

# Plot Search Time for different Range Sizes
dataset_sizes = df['DatasetSize'].unique()
for size in dataset_sizes:
    subset = df[df['DatasetSize'] == size]
    plt.plot(subset['RangeSize'], subset['SearchTime'], label=f'Dataset Size {size}', marker='o')

# Set labels and title for Search Time plot
plt.xlabel('Range Size (log scale)')
plt.ylabel('Search Time (ms) (log scale)')
plt.title('Search Time vs Range Size for Different Dataset Sizes for LOG-BRC')
plt.legend(title='Dataset Size')
plt.grid(True)
plt.xscale('log')  # Optional: Use log scale for better visibility on large range
plt.yscale('log')  # Optional: Use log scale for better visibility
# Adjust layout to prevent overlap
plt.tight_layout()

# Show the plot
plt.show()
