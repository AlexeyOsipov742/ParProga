import matplotlib.pyplot as plt
import numpy as np

np_list = []
tau_list = []
time_list = []

# Считываем данные
with open("timing_log.txt") as f:
    for line in f:
        parts = line.strip().split()
        np_val = int(parts[0].split("=")[1])
        tau_val = float(parts[1].split("=")[1])
        time_val = float(parts[2].split("=")[1])

        np_list.append(np_val)
        tau_list.append(tau_val)
        time_list.append(time_val)

# Преобразуем в numpy-массивы
np_array = np.array(np_list)
tau_array = np.array(tau_list)
time_array = np.array(time_list)

# ============================================
# 1. График от количества процессов при фикс. tau
# ============================================
fixed_tau = np.min(tau_array)
mask_tau = tau_array == fixed_tau

np_vals = np_array[mask_tau]
times = time_array[mask_tau]

sorted_idx = np.argsort(np_vals)
np_vals = np_vals[sorted_idx]
times = times[sorted_idx]

speedup = times[0] / times
efficiency = speedup / np_vals

plt.figure(figsize=(10, 4))
plt.suptitle(f"Зависимость от числа процессов при tau = {fixed_tau}")

plt.subplot(1, 2, 1)
plt.plot(np_vals, speedup, marker="o")
plt.xlabel("Число процессов (p)")
plt.ylabel("Ускорение S(p)")
plt.grid(True)

plt.subplot(1, 2, 2)
plt.plot(np_vals, efficiency, marker="o")
plt.xlabel("Число процессов (p)")
plt.ylabel("Эффективность E(p)")
plt.grid(True)

plt.tight_layout()
plt.show()

# ============================================
# 2. График от tau при фикс. np = 4
# ============================================
fixed_np = 4
mask_np = np_array == fixed_np

tau_vals = tau_array[mask_np]
times = time_array[mask_np]

sorted_idx = np.argsort(tau_vals)
tau_vals = tau_vals[sorted_idx]
times = times[sorted_idx]

speedup = times[0] / times
efficiency = speedup / fixed_np

plt.figure(figsize=(10, 4))
plt.suptitle(f"Зависимость от tau при np = {fixed_np}")

plt.subplot(1, 2, 1)
plt.plot(tau_vals, speedup, marker="o")
plt.xscale("log")
plt.xlabel("tau")
plt.ylabel("Ускорение S(tau)")
plt.grid(True)

plt.subplot(1, 2, 2)
plt.plot(tau_vals, efficiency, marker="o")
plt.xscale("log")
plt.xlabel("tau")
plt.ylabel("Эффективность E(tau)")
plt.grid(True)

plt.tight_layout()
plt.show()
