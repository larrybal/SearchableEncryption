import pandas as pd
import matplotlib.pyplot as plt

# Load performance results
df = pd.read_csv("performance_results.csv")

# Plot performance trends
plt.figure(figsize=(10, 6))
plt.plot(df["DatasetSize"], df["AvgSetupTime"], label="Setup Time", marker="o")
plt.plot(df["DatasetSize"], df["AvgPointQueryTime"], label="Point Query Time", marker="s")
plt.plot(df["DatasetSize"], df["AvgRangeQueryTime"], label="Range Query Time", marker="^")

# Labels and title
plt.xlabel("Dataset Size")
plt.ylabel("Time (seconds)")
plt.title("Performance vs Dataset Size")
plt.legend()
plt.grid()
plt.show()

# Plot accuracy trends
plt.figure(figsize=(10, 6))
plt.plot(df["DatasetSize"], df["AvgPointAccuracy"], label="Point Query Accuracy", marker="o")
plt.plot(df["DatasetSize"], df["AvgRangeAccuracy"], label="Range Query Accuracy", marker="s")

plt.xlabel("Dataset Size")
plt.ylabel("Accuracy (1.0 = 100%)")
plt.title("Accuracy vs Dataset Size")
plt.legend()
plt.grid()
plt.show()

# Plot memory usage trends
plt.figure(figsize=(10, 6))
plt.plot(df["DatasetSize"], df["AvgMemorySetup"], label="Setup Memory", marker="o")
plt.plot(df["DatasetSize"], df["AvgMemorySearch"], label="Point Query Memory", marker="s")
plt.plot(df["DatasetSize"], df["AvgMemoryRange"], label="Range Query Memory", marker="^")

plt.xlabel("Dataset Size")
plt.ylabel("Memory Usage (MB)")
plt.title("Memory Usage vs Dataset Size")
plt.legend()
plt.grid()
plt.show()
