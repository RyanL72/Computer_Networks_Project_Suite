import pandas as pd
import matplotlib.pyplot as plt

# Load the dataset
df = pd.read_csv('network_latency_data.csv')

# Count the number of rows per server (this gives us how many times each server was measured)
counts_by_server = df['Server'].value_counts()

# Plot as a bar chart
plt.figure(figsize=(10,6))
counts_by_server.plot(kind='bar', color='skyblue')
plt.title('Number of Measurements per Server')
plt.xlabel('Server')
plt.ylabel('Count')
plt.tight_layout()
plt.show()
