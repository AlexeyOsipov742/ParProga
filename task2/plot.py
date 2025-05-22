import matplotlib.pyplot as plt
import numpy as np
from collections import defaultdict

time_by_np = defaultdict(list)

with open("timing_log.txt") as f:
    for line in f:
        if not line.startswith("np="):
            continue
        try:
            parts = line.strip().split()
            np_val = int(parts[0].split("=")[1])
            eps_val = float(parts[1].split("=")[1])
            time_val = float(parts[2].split("=")[1])
            time_by_np[np_val].append((eps_val, time_val))
        except (IndexError, ValueError):
            continue

plt.figure(figsize=(12, 5))

# Ускорение
plt.subplot(1, 2, 1)
for np_val, data in sorted(time_by_np.items()):
    data.sort()
    eps = np.array([e for e, _ in data])
    times = np.array([t for _, t in data])
    base_time = times[0]
    speedup = base_time / times
    plt.plot(eps, speedup, marker='o', label=f"np = {np_val}")

plt.xlabel("Погрешность ε")
plt.ylabel("Ускорение S(ε)")
plt.title("Ускорение")
plt.xscale("log")
plt.grid(True)
plt.legend()

# Эффективность
plt.subplot(1, 2, 2)
for np_val, data in sorted(time_by_np.items()):
    data.sort()
    eps = np.array([e for e, _ in data])
    times = np.array([t for _, t in data])
    base_time = times[0]
    speedup = base_time / times
    efficiency = speedup / np_val
    plt.plot(eps, efficiency, marker='o', label=f"np = {np_val}")

plt.xlabel("Погрешность ε")
plt.ylabel("Эффективность E(ε)")
plt.title("Эффективность")
plt.xscale("log")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()

