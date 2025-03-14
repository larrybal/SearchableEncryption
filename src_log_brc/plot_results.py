import pandas as pd
import matplotlib.pyplot as plt

# Load the data
data = pd.read_csv("results.csv")

# Create the plots
plt.figure(figsize=(8, 6))
plt.plot(data["DatasetSize"], data["Accuracy"], marker='o')
plt.xlabel("Dataset Size")
plt.ylabel("Accuracy")
plt.title("Accuracy vs. Dataset Size")
plt.grid(True)
plt.show()

plt.figure(figsize=(8, 6))
plt.plot(data["DatasetSize"], data["QueryTime"], marker='o')
plt.xlabel("Dataset Size")
plt.ylabel("Query Time (seconds)")
plt.title("Query Time vs. Dataset Size")
plt.grid(True)
plt.show()

plt.figure(figsize=(8, 6))
plt.plot(data["DatasetSize"], data["MemoryUsage"], marker='o')
plt.xlabel("Dataset Size")
plt.ylabel("Memory Usage (bytes)")
plt.title("Memory Usage vs. Dataset Size")
plt.grid(True)
plt.show()