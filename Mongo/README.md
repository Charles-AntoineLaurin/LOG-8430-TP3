# Installation
Cette section portera sur les installations à faire pour la réalisation de cette étude.

Il faut tout d'abord installer java, maven et docker comme suit : 
```
sudo apt-get update
sudo apt update 

# install Java
sudo apt install default-jre -y
sudo apt install default-jdk -y

# install Maven
sudo apt install maven -y

# install Docker
sudo apt install apt-transport-https ca-certificates curl software-properties-common -y
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "dev [arch=amd64] https://download.docker.com/linux/ubuntu focal stable" -y
sudo apt install docker-ce -y
sudo usermod -aG docker ${USER}
```
**Un redémarage est nécessaire pour que les modifications soient appliquées**

Pour finir l'installation, il suffit d'installer docker-compose et YCSB comme suit :
```
# install docker-compose
sudo curl -L "https://github.com/docker/compose/release/download/1.29.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose

#install YCSB
sudo curl -O --location https://github.com/brianfrankcooper/YCSB/releases/download/0.17.0/ycsb-0.17.0.tar.gz
tar xfvz ycsb-0.17.0.tar.gz -C ./YCSB
rm ycsb-0.17.0.tar.gz
```

# Déploiement de la base de données
Cette section portera sur le déploiment de la base de données MongoDB grâce à Docker.

```
# Téléchargement du docker-compose.yml utilisé pour initialisé la base de données
mkdir database
cd database
sudo curl https://raw.githubusercontent.com/Charles-AntoineLaurin/LOG-8430-TP3/main/Mongo/docker-compose.yml -o docker-compose.yml

# Mise en service de la base de données
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
cd YCSB
mkdir results

# Création des dossiers pour la sauvegarde des données
cd results
mkdir A
mkdir B
mkdir C
mkdir D
mkdir E
mkdir F
cd ..

# Exécution des tests de charges
for i in {1..3}
do
./bin/ycsb load mongodb-async -s -P workloads/workloada -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/A/outputLoad.csv
./bin/ycsb run mongodb-async -s -P workloads/workloada -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/A/outputRun.csv

./bin/ycsb load mongodb-async -s -P workloads/workloadb -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/B/outputLoad.csv
./bin/ycsb run mongodb-async -s -P workloads/workloadb -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/B/outputRun.csv

./bin/ycsb load mongodb-async -s -P workloads/workloadc -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/C/outputLoad.csv
./bin/ycsb run mongodb-async -s -P workloads/workloadc -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/C/outputRun.csv

./bin/ycsb load mongodb-async -s -P workloads/workloadd -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/D/outputLoad.csv
./bin/ycsb run mongodb-async -s -P workloads/workloadd -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/D/outputRun.csv

./bin/ycsb load mongodb-async -s -P workloads/workloade -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/E/outputLoad.csv
./bin/ycsb run mongodb-async -s -P workloads/workloade -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/E/outputRun.csv

./bin/ycsb load mongodb-async -s -P workloads/workloadf -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/F/outputLoad.csv
./bin/ycsb run mongodb-async -s -P workloads/workloadf -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet >> ./results/F/outputRun.csv
done
```

# Reset des containers
Une fois les manipulations réalisées, on peut shutdown la base de données en allant dans le répertoire où se trouve le docker-compose.yml et exécuté la commande suivante
```
docker-compose down
```


