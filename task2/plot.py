import matplotlib.pyplot as plt
import numpy as np
from collections import defaultdict

# Словари: ключ — np, значение — списки h и time
time_by_np = defaultdict(list)

with open("timing_log.txt") as f:
    for line in f:
        parts = line.strip().split()
        np_val = int(parts[0].split("=")[1])
        h_val = float(parts[1].split("=")[1])
        time_val = float(parts[2].split("=")[1])
        time_by_np[np_val].append((h_val, time_val))

plt.figure(figsize=(12, 5))

# 1. Ускорение
plt.subplot(1, 2, 1)
for np_val, data in sorted(time_by_np.items()):
    data.sort()  # сортировка по h
    hs = np.array([h for h, _ in data])
    times = np.array([t for _, t in data])
    base_time = times[0]  # время при np=1 на том же h
    speedup = base_time / times
    plt.plot(hs, speedup, marker="o", label=f"np = {np_val}")

plt.xlabel("Шаг интегрирования h")
plt.ylabel("Ускорение S(h)")
plt.title("Ускорение vs шаг")
plt.xscale("log")
plt.grid(True)
plt.legend()

# 2. Эффективность
plt.subplot(1, 2, 2)
for np_val, data in sorted(time_by_np.items()):
    data.sort()
    hs = np.array([h for h, _ in data])
    times = np.array([t for _, t in data])
    base_time = times[0]
    speedup = base_time / times
    efficiency = speedup / np_val
    plt.plot(hs, efficiency, marker="o", label=f"np = {np_val}")

plt.xlabel("Шаг интегрирования h")
plt.ylabel("Эффективность E(h)")
plt.title("Эффективность vs шаг")
plt.xscale("log")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()


