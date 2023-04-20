# -*- coding: utf-8 -*-

import subprocess
import time
import sys
from datetime import datetime as dt

# Commande à exécuter
cmd = ["docker", "stats", "--no-stream", "--format", "{{.Container}}, {{.CPUPerc}},{{.MemUsage}}"]

# Ouvrir un fichier pour stocker les statistiques
filepath = "/home/ubuntu/results/stats.csv"
print(filepath)

with open(filepath, "a") as f:
    # Ajouter l'en-tête du fichier s'il est vide
    if f.tell() == 0:
        f.write("Timestamp, Container, CPU (%),RAM (MiB)\n")

    # Boucle principale
    while True:
        # Exécuter la commande et récupérer les statistiques
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
        output, _ = p.communicate()
        lines = output.decode("utf-8").strip().split("\n")
        
        for line in lines:
            stats = line.split(",")
            print(stats)

            # Ajouter les statistiques au fichier
            if len(stats) > 2:
                f.write("{},  {},  {},  {}\n".format(dt.now(),stats[0], stats[1], stats[2].split()[0]))

        # Attendre avant de récupérer les statistiques à nouveau
        time.sleep(100/1000)
