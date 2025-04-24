import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # неявный импорт нужен

# Загружаем данные
data = np.loadtxt("solution_conseq.txt")  # или другой файл

# Размеры сетки
K, M = data.shape
X = 1.0
T = 1.0
x = np.linspace(0, X, M)
t = np.linspace(0, T, K)

# Создаём сетку координат
T_grid, X_grid = np.meshgrid(t, x, indexing='ij')

# Построение 3D-графика
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

surf = ax.plot_surface(X_grid, T_grid, data, cmap='viridis')

ax.set_xlabel("x")
ax.set_ylabel("t")
ax.set_zlabel("u(x, t)")
ax.set_title("3D-график решения уравнения переноса")
fig.colorbar(surf, shrink=0.5, aspect=10)

plt.tight_layout()
plt.show()
