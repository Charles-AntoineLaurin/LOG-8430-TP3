# Installation
Cette section portera sur les installations à faire pour la réalisation de cette étude.

Il faut tout d'abord installer java, maven et docker comme suit : 
```
sudo apt-get update
sudo apt update 

# install Git
sudo apt install git -y

# install C
sudo apt install build-essential -y

#install Python
sudo apt install python2.7 -y

# install Java
sudo apt install default-jre -y
sudo apt install default-jdk -y

# install Maven
sudo apt install maven -y

# install Docker
sudo apt install apt-transport-https ca-certificates curl software-properties-common -y
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu focal stable" -y
sudo apt install docker-ce -y
sudo usermod -aG docker ${USER}
```
**Un redémarage est nécessaire pour que les modifications soient appliquées**

Pour finir l'installation, il suffit d'installer docker-compose et YCSB comme suit :
```
# install docker-compose
sudo curl -L "https://github.com/docker/compose/releases/download/1.29.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose

#install YCSB
sudo curl -O --location https://github.com/brianfrankcooper/YCSB/releases/download/0.17.0/ycsb-0.17.0.tar.gz
tar xfvz ycsb-0.17.0.tar.gz -C ./YCSB
rm ycsb-0.17.0.tar.gz
```

# Déploiement de la base de données
Cette section portera sur le déploiment de la base de données MongoDB grâce à Docker.

```
# Téléchargement du code source
git clone https://github.com/Charles-AntoineLaurin/LOG-8430-TP3.git

# Mise en service de la base de données
cd Mongo
docker-compose up
docker exec -it primary mongosh --eval "rs.initiate(
    {
        _id: \"myReplicaSet\", 
        members: [
            {
                _id: 0, 
                host: \"primary\"
            },
            {
                _id: 1, 
                host: \"secondary1\"
            },
            {
                _id: 2, 
                host: \"secondary2\"
            },
            {
                _id: 3, 
                host: \"secondary3\"
            }
        ]
    })"
docker exec -it primary mongosh --eval "rs.status()"
```

## Modification du fichier /etc/hosts
Avec l'éditeur de votre choix (Nano, vim, ...), modifier le fichier /etc/hosts pour que le contenu de la première partie ressemble à ceci: 
```
127.0.0.1 localhost
192.168.5.2 primary
192.168.5.3 secondary1
192.168.5.4 secondary2
192.168.5.5 secondary3
```

# Phase de tests
Cette section portera sur les étapes à suivre pour réaliser les tests de charges.

```
cd /home/ubuntu/LOG-8430-TP3/Mongo

# Creation des folders pour la sauvegarde des résultats
./folder_results.sh

# Compilation du code pour la génération des charges de travail
make


# Execution du processus de génération de données
./process
```

# Reset des containers
Une fois les manipulations réalisées, on peut shutdown la base de données en allant dans le répertoire où se trouve le docker-compose.yml et exécuté la commande suivante
```
# Avant de fermer les containers, on sauvegarde les id de containers
docker container ls >> /home/results/container.csv

docker-compose down
```

# Récuprération des données pour l'analyse
Pour récupérer les données sauvegardées dans l'instance AWS, il ne suffit que d'utiliser la commande suivante dans le terminal de votre l'ordinateur
```
scp -i votre_clef.pem -r ubuntu@ec2-0-0-0-0.compute-1.amazonaws.com:/home/ubuntu/results votre/path/en/local 
```

# Fichier recceillis 
## stats.csv
Fichier contenant les statistiques des containers Docker durant l'execution des workloads ainsi que les timestamp associés à chaque mesure.
## container.csv
Fichier contenant les données sur les containers Docker. L'essentiel est d'avoir le ID de container et le nom rattacher à ce dernier, il sera utile pour identifier les containers affichés dans le fichier stats.csv
## /x/output.csv (où x est a, b ou c)
Fichier contenant les différentes données recceillis pendant l'exécution workloads. Chaque workload à été exécuté 5 fois et nécessite 2 opérations (Load et Run). Chaque fichier output.csv comprend donc 10 sections de résultats.


