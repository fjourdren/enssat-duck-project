## Description
Projet scolaire d'algorithmique répartie utilisant les sockets TCP linux.

## Mode de jeu
Ce projet est un jeu collaboratif qui donne aux joueurs la mission de retrouver les canards cachés dans la scène. Pour se faire, les joueurs doivent s'aider des cris des canards. Une fois un joueur assez proche d'un canard, on le considère trouvé. Celui-ci devient alors visible par tous et arrête de cancaner. Les niveaux sont chargés par le serveur et envoyés à tous les joueurs au début de la partie. Le serveur sauvegarde le temps minimum qu'il a fallu pour trouver tous les canards (système de record).

## Installation des dépendances
Projet uniquement compatible avec les OS linux.

    FEDORA :
    sudo dnf install gcc-c++ glew-devel glfw-devel SDL2-devel SDL2_image-devel openal-soft-devel freealut-devel freeglut-devel

    DEBIAN :
    sudo apt install g++ mesa-utils libglew-dev libglfw3-dev libsdl2-dev libsdl2-image-dev libopenal-dev libalut-dev freeglut3-dev

## Compilation et Exécution
### Serveur
Compilation du serveur :
    make

Pour l'exécution sur le port par défaut 25000 on utilise la commande `make run` ou la commande `./main`.
Pour ne pas utiliser le port par défaut, on utilise la commande `./main 25000`.

### Client
Compilation du client :
    make cleanalllibs && make main

Ensuite, `make run` permet d'exécuter le client et de le connecter au serveur avec l'IP et le port par défaut : 127.0.0.1:25000. `./main` a le même comportement.
Si l'on souhaite configurer l'ip et le port on utilise la commande `./main 127.0.0.1 25000`.

## Auteurs
* [Flavien JOURDREN](https://github.com/fjourdren)
* [Hugues WIECZOREK](https://github.com/HuguesWieczorek)

## Fonctionnalités
* Système de configuration (positions des canards) lu côté serveur puis envoyé aux clients.
* Synchronisation des canards trouvés entre les clients.
* Calcul du temps nécessaire aux joueurs pour trouver tous les canards.
* Système de meilleurs temps sur le serveur (avec sauvegarde sur le disque dur).
* Socket TCP.
* Serveur threadé.
* Client threadé.
* Partage des accès aux données critiques entre les threads.
* Machine à état pour gérer la partie.
* Système de redémarrage de la partie automatique.

## Améliorations
* Aujourd’hui, la réception est limité à un buffer de taille 1050 (un message d’erreur apparaît si la sérialisation prend trop de caractère). Une des améliorations serait de retirer cette limite.
* Ajouter la gestion de plusieurs objets (autre que le canard) côté client (Fonctionnalité en partie implémentée : les objets sont alors des flags).
* Vérification de toutes les informations côté serveur (pour éviter la triche).
* Ajout de la distance du plus proche canard.
* Voir les autres joueurs.
* Interface de connexion sur le client.
* Afficher le timer en temps réel chez le client.
* Utiliser Boost ASIO pour être multiplate-forme.
* Utilisation de protobuf dans les échanges.
