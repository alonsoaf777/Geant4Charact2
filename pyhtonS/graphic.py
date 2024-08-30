import pandas as pd
import matplotlib.pyplot as plt

# Leer los datos del archivo Sim0.csv
df = pd.read_csv('Sim0.csv')

# Crear la figura y el eje 2D
plt.figure(figsize=(8, 6))

# Graficar los puntos en 2D
sc = plt.scatter(df['PositionX'], df['PositionY'], c=df['Energy'], cmap='Spectral', s=1)

# Etiquetas de los ejes
plt.xlabel('PositionX')
plt.ylabel('PositionY')

# Cambiar los rangos de los ejes
plt.xlim(-100, 100)  # Rango para el eje X
plt.ylim(-100, 100)  # Rango para el eje Y

# Añadir la barra de color para la energía
cbar = plt.colorbar(sc)
cbar.set_label('Energy')

# Mostrar el gráfico
plt.show()
