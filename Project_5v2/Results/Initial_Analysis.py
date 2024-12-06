import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv('network_latency_data.csv')

# Ensure numeric columns are correctly typed
df['Packet Loss (%)'] = pd.to_numeric(df['Packet Loss (%)'], errors='coerce')
df['RTT Min (ms)'] = pd.to_numeric(df['RTT Min (ms)'], errors='coerce')
df['RTT Avg (ms)'] = pd.to_numeric(df['RTT Avg (ms)'], errors='coerce')
df['RTT Max (ms)'] = pd.to_numeric(df['RTT Max (ms)'], errors='coerce')
df['Hop Count'] = pd.to_numeric(df['Hop Count'], errors='coerce')

# Drop rows with missing or invalid data if necessary
df = df.dropna(subset=['RTT Avg (ms)', 'Packet Loss (%)', 'Hop Count'])

# 1. Compute and display the average RTT by region
avg_rtt_by_region = df.groupby('Region')['RTT Avg (ms)'].mean().sort_values()
print("Average RTT by region (ms):")
print(avg_rtt_by_region)
print()

# 2. Compute and display the average packet loss by region
avg_loss_by_region = df.groupby('Region')['Packet Loss (%)'].mean().sort_values()
print("Average Packet Loss by region (%):")
print(avg_loss_by_region)
print()

# 3. Find the top 5 servers with the highest average RTT
top5_servers_by_rtt = df.groupby('Server')['RTT Avg (ms)'].mean().sort_values(ascending=False).head(5)
print("Top 5 servers by highest average RTT (ms):")
print(top5_servers_by_rtt)
print()

# 4. Distribution of RTT across regions
# Plot a boxplot of average RTT by region
plt.figure(figsize=(10,6))
df.boxplot(column='RTT Avg (ms)', by='Region', grid=False)
plt.title('RTT Distributions by Region')
plt.suptitle('')
plt.xlabel('Region')
plt.ylabel('RTT Avg (ms)')
plt.tight_layout()
plt.show()

# 5. Packet Loss distribution
# Plot histogram of packet loss
plt.figure(figsize=(10,6))
df['Packet Loss (%)'].hist(bins=20)
plt.title('Packet Loss Distribution')
plt.xlabel('Packet Loss (%)')
plt.ylabel('Count')
plt.tight_layout()
plt.show()

# 6. Hop Count distribution across regions
avg_hop_by_region = df.groupby('Region')['Hop Count'].mean().sort_values()
print("Average Hop Count by Region:")
print(avg_hop_by_region)

# Plot bar chart of average hop count by region
avg_hop_by_region.plot(kind='bar', figsize=(8,5), title='Average Hop Count by Region')
plt.ylabel('Average Hop Count')
plt.tight_layout()
plt.show()
