import traci
import time
import socket
import json
from pyproj import Proj, Transformer, transform
import math
import tkinter as tk
from tkinter import ttk
from threading import Thread

laty0 = 47.728709
longx0 = 7.290897

waitingTime = 0.02

coloriage = False

client_socket = None
server_socket = None
shouldRun = True
limit = 100
already_closed = False
vehicle_list = []

vehicle_to_send = "0"

def extract_number(vehicle_id):
    return int(vehicle_id[3:])


def on_close():
    close_simulation()
    root.destroy()
    


def start_simulation():
    global shouldRun
    shouldRun = True
    start_button.config(state='disabled')
    stop_button.config(state='normal')
    close_button.config(state='normal')
    global stop
    stop = False
    simulation_thread = Thread(target=run_simulation)
    lock = False
    simulation_thread.start()

def close_simulation():
    global client_socket
    global already_closed

    notice = {'veh_id': "-2"}
    try:
        if(client_socket != None and already_closed != True):
            client_socket.send((json.dumps(notice) + "\n").encode('utf-8'))
            already_closed = True
    except socket.error as e:
        print()
    try:
        traci.close()
    except Exception as e:
        print()
    
    global shouldRun
    shouldRun = False
    show_connections_button.config(state='disabled')
    start_button.config(state='normal')
    stop_button.config(state='disabled')
    reprendre_button.config(state='disabled')
    close_button.config(state='disabled')
    
def stop_simulation():
    reprendre_button.config(state='normal')
    stop_button.config(state='disabled')
    global stop
    stop = True

def reprendre_simulation():
    reprendre_button.config(state='disabled')
    stop_button.config(state='normal')
    global stop
    stop = False

# Fonction pour modifier la valeur de waitingTime
def set_speed(speed):
    global waitingTime
    if speed == 'lent':
        waitingTime = 0.08
        if(decoloriage_button.cget('state') == 'disabled'):
            coloriage_button.config(state='normal')
    elif speed == 'normal':
        waitingTime = 0.04
        if(decoloriage_button.cget('state') == 'disabled'):
            coloriage_button.config(state='normal')
    elif speed == 'rapide':        
        waitingTime = 0
        coloriage_button.config(state='disabled')


def update_vehicle_list(new_list):
    global vehicle_list

    show_connections_button.config(state='normal')
    sorted_vehicle_ids = sorted(new_list, key=extract_number)
    vehicle_list = ['Aucune'] + sorted_vehicle_ids
    # Mise à jour du menu déroulant
    vehicle_combobox['values'] = vehicle_list

def update_limit():
    global limit
    try:
        # Mise à jour de la limite avec la valeur entrée par l'utilisateur
        limit = int(limit_entry.get())
        print(f"La nouvelle limite est {limit}")
    except ValueError:
        print("Veuillez entrer un nombre valide.")

def toggle_coloriage(para):
        
        if(para):
            coloriage_button.config(state='disabled')
            fast_button.config(state='disabled')
            decoloriage_button.config(state='normal')
            
        else:
            coloriage_button.config(state='normal')
            fast_button.config(state='normal')
            decoloriage_button.config(state='disabled')

        
        global client_socket
        global coloriage
        coloriage = not coloriage
        print("Coloriage " + ("activé" if coloriage else "désactivé"))

        if not coloriage:
            if(client_socket != None and already_closed != True):

                notice = {'veh_id': "0"}
                client_socket.send((json.dumps(notice) + "\n").encode('utf-8'))



def send_connexion_notice():
    global vehicle_to_send
    vehicule_selectionne = vehicle_combobox.get()
    if(vehicule_selectionne == 'Aucune'):
        vehicle_to_send = "0"
    elif(vehicule_selectionne != ""):
        vehicle_to_send= vehicule_selectionne

def get_color_by_id(id):
    # Générer une couleur RGB en fonction de l'ID
    
    red = (int(id[3:]) * 17 % 256)/255
    green = (int(id[3:]) * 71 % 256)/255
    blue = (int(id[3:]) * 3 * 7 * 11 % 256)/255

    # Retourner la couleur sous forme de tuple (R, G, B)
    return red, green, blue

def get_pt_by_id(id):
    
    pt = ((int(id[3:]) * 17) % 150 ) +100
    return pt


def start_sumo_simulation():
    traci.start(['sumo', '-c', './sumocfg/osm.sumocfg'])

def run_simulation():
    global client_socket
    global server_socket
    global vehicle_to_send
    start_sumo_simulation()
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)   
    server_socket.bind(('localhost', 12345))
    server_socket.listen()

     # Définir un timeout pour accept()
    server_socket.settimeout(1.0)

    while  shouldRun:
        try:
            client_socket, addr = server_socket.accept()
            break
            
        except socket.timeout:
            # Continue la boucle et re-vérifie la condition d'arrêt
            continue

    if shouldRun:
        previous_vehicle_ids = set()
    
        try:
            while traci.simulation.getMinExpectedNumber() > 0 and shouldRun:
                traci.simulationStep()

                current_vehicle_ids = set(traci.vehicle.getIDList()[:limit])
                vehicle_ids_as_strings = list(map(str, current_vehicle_ids))
                update_vehicle_list(vehicle_ids_as_strings)
                vehicles_that_left = previous_vehicle_ids - current_vehicle_ids

                for veh_id in vehicles_that_left:
                    # Traiter la disparition du véhicule
                    data = {'veh_id': veh_id, 'lat': '0', 'lon': '0'}
                    client_socket.send((json.dumps(data) + "\n").encode('utf-8'))
                    if(veh_id == vehicle_to_send):
                        vehicle_to_send = "0"
                        notice = {'veh_id': "-4"}
                        client_socket.send((json.dumps(notice) + "\n").encode('utf-8'))

                


                for veh_id in current_vehicle_ids:
                    position = traci.vehicle.getPosition(veh_id)
                    red, green, blue = get_color_by_id(veh_id)
                    pt = get_pt_by_id(veh_id)
                    lon, lat = traci.simulation.convertGeo(position[0], position[1])
                    data = {'veh_id': veh_id, 'lat': str(lat), 'lon': str(lon), 'red': str(red), 'green': str(green), 'blue': str(blue), "pt": str(pt)}
                    
                    
                    client_socket.send((json.dumps(data) + "\n").encode('utf-8'))
                    
                    
                    


                    while(stop):
                        time.sleep(0.01)
                

                if(coloriage):
                    notice = {'veh_id': "-1"}
                    client_socket.send((json.dumps(notice) + "\n").encode('utf-8'))
                if(vehicle_to_send != "0"):
                    notice = {'veh_id': "-3", 'lat': vehicle_to_send}
                    client_socket.send((json.dumps(notice) + "\n").encode('utf-8'))
                
                previous_vehicle_ids = current_vehicle_ids
                if not current_vehicle_ids:
                    print("Aucun véhicule détecté, arrêt de la simulation.")
                    stop_simulation()
                    break
                time.sleep(waitingTime)  # Pause d'une seconde entre chaque étape de simulation qui reduit en fonction du nombre de vehicule        
                
        except Exception as e:
            print("Erreur de connexion :", e)
        finally:
            client_socket.close()
            server_socket.close()
            try:
                traci.close()
            except Exception:
                print()
    else: 
        server_socket.close()       

# Création de l'interface graphique
root = tk.Tk()
root.title("Contrôle de la Simulation")

# Configurez la grille
root.columnconfigure(0, weight=1)
root.columnconfigure(1, weight=1)

# Cadre pour l'état de la simulation
state_frame = tk.LabelFrame(root, text="État de la Simulation", padx=10, pady=10)
state_frame.grid(row=0, column=0, columnspan=2, sticky="ew", padx=10, pady=10)

# Bouton pour démarrer la simulation
start_button = tk.Button(state_frame, text="Démarrer la Simulation", command=start_simulation)
start_button.grid(row=0, column=0, padx=5, pady=5)

# Bouton pour fermer la simulation
close_button = tk.Button(state_frame, text="Fermer la Simulation", command=close_simulation, state='disabled')
close_button.grid(row=0, column=1, padx=5, pady=5)

# Bouton pour arrêter la simulation
stop_button = tk.Button(state_frame, text="Arrêter la Simulation", command=stop_simulation, state='disabled')
stop_button.grid(row=1, column=0, padx=5, pady=5)

# Bouton pour reprendre la simulation
reprendre_button = tk.Button(state_frame, text="Reprendre la Simulation", command=reprendre_simulation, state='disabled')
reprendre_button.grid(row=1, column=1, padx=5, pady=5)

# Cadre pour la mise à jour de la limite
limit_frame = tk.LabelFrame(root, text="Nombre de véhicules à l'écran", padx=10, pady=10)
limit_frame.grid(row=2, column=0, columnspan=2, pady=10)

# Champ de saisie pour la limite
limit_entry = tk.Entry(limit_frame)
limit_entry.pack(side=tk.LEFT, padx=5)

# Bouton pour mettre à jour la limite
update_limit_button = tk.Button(limit_frame, text="Mettre à Jour la Limite", command=update_limit)
update_limit_button.pack(side=tk.LEFT, padx=5)



# Cadre pour les boutons de vitesse
speed_frame = tk.LabelFrame(root, text="Vitesse de la Simulation", padx=10, pady=10)
speed_frame.grid(row=3, column=0, columnspan=2, sticky="ew")

# Créez un cadre intermédiaire pour contenir les boutons
button_frame = tk.Frame(speed_frame)
button_frame.pack(expand=True)

# Créer les boutons
lent_button = tk.Button(button_frame, text="Lente", command=lambda: set_speed('lent'))
normal_button = tk.Button(button_frame, text="Normale", command=lambda: set_speed('normal'))
fast_button = tk.Button(button_frame, text="Rapide", command=lambda: set_speed('rapide'))

# Empaquetez les boutons
lent_button.pack(side=tk.LEFT)
normal_button.pack(side=tk.LEFT)
fast_button.pack(side=tk.LEFT)

# Cadre pour le coloriage des mailles
coloriage_frame = tk.LabelFrame(root, text="Coloriage des mailles", padx=10, pady=10)
coloriage_frame.grid(row=4, column=0, columnspan=2, sticky="ew", padx=10, pady=10)

# Bouton pour activer le coloriage
coloriage_button = tk.Button(coloriage_frame, text="Activer", command=lambda: toggle_coloriage(True))
coloriage_button.pack(side=tk.LEFT, expand=True, padx=10)

# Bouton pour désactiver le coloriage
decoloriage_button = tk.Button(coloriage_frame, text="Désactiver", command=lambda: toggle_coloriage(False), state='disabled')
decoloriage_button.pack(side=tk.RIGHT, expand=True, padx=10)


# Créer un LabelFrame pour les connexions entre véhicules
connections_frame = tk.LabelFrame(root, text="Affichage des connexions entre véhicules", padx=10, pady=10)
connections_frame.grid(row=0, column=2, columnspan=4, sticky="ew", padx=10, pady=10)

# Créer et ajouter le combobox au LabelFrame
vehicle_combobox = ttk.Combobox(connections_frame, values=vehicle_list)
vehicle_combobox.grid(row=0, column=0, padx=10, pady=10)

# Créer et ajouter un bouton pour afficher les connexions
show_connections_button = tk.Button(connections_frame, text="Afficher les connexions pour cette voiture",command=send_connexion_notice, state='disabled')
show_connections_button.grid(row=0, column=1, padx=10, pady=10)


# Créer le widget Text
text_widget = tk.Text(root, height=10, width=50, bg=root.cget('bg'), bd=0)  # Ajustez la hauteur et la largeur selon vos besoins
text_widget.grid(row=1, column=2, rowspan=5,columnspan=4, padx=10, pady=10)

# Pour ajouter du texte dans le widget Text
text_widget.insert(tk.END, 'Veuillez mettre la simulation en pause avant de \nsélectionner une voiture spécifique depuis \nle menu déroulant. Cela garantira que les \ninformations affichées correspondent au véhicule  sélectionné au moment de votre choix.')
text_widget.config(state='disabled')
# Lancer l'interface graphique

root.protocol("WM_DELETE_WINDOW", on_close)
root.mainloop()