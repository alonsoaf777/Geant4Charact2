import os
import subprocess
import ROOT  # Asegúrate de tener PyROOT instalado
import re

# Configuración
sim_dir = r"C:\Users\conej\Documents\Universidad\Geant4\Projects\Git\G4-PCM\build\Release"
exec_file = "PCM.exe"
root_filename = "Sim.root"
mac_template = """\
/run/initialize
/gun/energy {energy} keV
/det/t {thickness:.2f} nm
/run/reinitializeGeometry
/run/beamOn 10000
"""

# Función para crear el archivo .mac
def create_mac_file(energy, thickness):
    mac_file_path = os.path.join(sim_dir, "bisection.mac")
    mac_content = mac_template.format(energy=energy, thickness=thickness)
    with open(mac_file_path, 'w') as f:
        f.write(mac_content)
    return mac_file_path, mac_content

# Función para ejecutar la simulación
def run_simulation(mac_file_path):
    subprocess.run([os.path.join(sim_dir, exec_file), mac_file_path], cwd=sim_dir, check=True)

# Función para contar eventos en el archivo ROOT
def count_events():
    file_path = os.path.join(sim_dir, root_filename)
    if not os.path.exists(file_path):
        print("Error: El archivo ROOT no existe.")
        return -1  # No se encontró el archivo
    
    try:
        root_file = ROOT.TFile(file_path)
        
        if root_file.IsZombie():
            print("Error: El archivo ROOT está corrupto.")
            return -1  # Archivo ROOT corrupto
        
        ntuple = root_file.Get("G4_PCM")
        if not ntuple:
            print("Error: La ntuple 'G4_PCM' no se encontró.")
            return -1  # No se encontró la ntuple
        
        # Crear un histograma para contar los eventos con energía mayor a 0
        energy_histogram = ROOT.TH1F("energy_histogram", "Energy > 0", 10000, 0, 10000)
        ntuple.Draw("Energy>>energy_histogram", "Energy > 0")  # Filtro para contar solo energías > 0
        
        event_count = int(energy_histogram.GetEntries())
        print(f"Conteo de eventos con energía > 0: {event_count}")
        return event_count  # Retorna el número de eventos con energía > 0

    except Exception as e:
        print(f"Error al procesar el archivo ROOT: {e}")
        return -1

    finally:
        # Asegurarse de cerrar el archivo ROOT
        root_file.Close()

# Función para imprimir el valor de 't' del archivo .mac
def print_thickness_from_mac(mac_content):
    match = re.search(r'/det/t (\d+\.\d+) nm', mac_content)
    if match:
        thickness = match.group(1)
        print(f"\nEl valor de 't' en el archivo .mac es: {thickness} nm")
    else:
        print("No se encontró el valor de 't' en el archivo .mac.")

# Función para la optimización por bisección
def bisection_optimization(initial_energy, t_min, t_max, target_count=4000, tol=500, max_iter=100):
    low = t_min
    high = t_max
    iterations = 0
    optimal_t = low
    
    while high - low > tol and iterations < max_iter:
        mid = (low + high) / 2.0
        mid = int(round(mid))  # Redondear al entero más cercano
        mac_file_path, mac_content = create_mac_file(initial_energy, mid)
        run_simulation(mac_file_path)
        
        event_count = count_events()
        if event_count == -1:
            print("Error: No se encontró el archivo ROOT o la ntuple.")
            break
        
        print(f"t = {mid} nm: {event_count} eventos con energía > 0")
        
        # Ajustar límites según el número de eventos
        if abs(event_count - target_count) <= tol:
            optimal_t = mid
            print(f"Se cumple la condición: Número de eventos está dentro del rango objetivo.")
            break
        
        if event_count >= target_count:
            optimal_t = mid  # El grosor actual cumple el objetivo
            low = mid + 1  # Ajustar el límite inferior para buscar valores mayores
        else:
            high = mid - 1  # Ajustar el límite superior para buscar valores menores

        # Verifica si el cambio en el límite superior e inferior es muy pequeño para evitar oscilaciones
        if abs(high - low) < tol:
            print(f"Se detiene la búsqueda debido a que la diferencia entre los límites es menor que {tol:.2f}")
            break

        iterations += 1

    return optimal_t

# Parámetros iniciales
initial_energy = 10  # keV
t_min = 0  # nm
t_max = 20000000  # nm

# Ejecutar optimización
optimal_t = bisection_optimization(initial_energy, t_min, t_max)
print(f"Optimización completada: t óptimo = {optimal_t} nm")

# Imprimir el archivo .mac generado
mac_file_path = os.path.join(sim_dir, "bisection.mac")
with open(mac_file_path, 'r') as f:
    mac_content = f.read()

print("\nContenido del archivo .mac generado:")
print(mac_content)

# Imprimir el valor de 't' del archivo .mac
print_thickness_from_mac(mac_content)

