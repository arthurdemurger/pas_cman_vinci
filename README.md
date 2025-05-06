
# Projet Pas Cman

## Description

Le projet Pas Cman est un jeu multijoueur en réseau où plusieurs clients peuvent se connecter à un serveur pour interagir dans un environnement partagé. Ce projet comprend des composants serveur, client et un programme de test `pas_labo` pour valider le comportement du jeu.

## Fonctionnalités

- **Serveur** : Gère les connexions des clients et orchestre le jeu.
- **Client** : Permet aux joueurs de se connecter au serveur et d'interagir dans le jeu.
- **Tests** : Utilise le programme `pas_labo` pour exécuter des tests automatisés sur le jeu.

## Prérequis

- Compilateur GCC
- Make (pour la compilation)

## Installation

1. Clonez le dépôt du projet sur votre machine locale.
2. Accédez au répertoire du projet.
3. Exécutez `make` pour compiler les binaires du serveur, du client et du programme de test.

```bash
git clone <URL_DU_DEPOT>
cd pas-cman
make
```

## Exécution Normale

Pour lancer le jeu Pas Cman, suivez ces étapes :

### Serveur

Ouvrez un terminal et lancez le serveur avec la commande suivante :

```bash
./pas_server 9090 ./resources/map3.txt
```

- `9090` : Port d'écoute du serveur (en localhost).
- `./resources/map3.txt` : Chemin vers la carte utilisée pour le jeu.

### Client

Ouvrez deux terminaux distincts pour lancer deux clients :

#### Client 1

```bash
./pas_client localhost 9090
```

#### Client 2

```bash
./pas_client localhost 9090
```

- `localhost` : Adresse du serveur.
- `9090` : Port sur lequel le serveur écoute.

## Exécution des Tests

Pour exécuter des tests automatisés, utilisez le programme `pas_labo` :

```bash
./pas_labo 9090 ./test/map.txt ./test/joueur1.txt ./test/joueur2.txt
```

- `9090` : Port d'écoute du serveur (en localhost).
- `./test/map.txt` : Carte utilisée pour le test.
- `./test/joueur1.txt` : Fichier contenant les mouvements du joueur 1 pour le test.
- `./test/joueur2.txt` : Fichier contenant les mouvements du joueur 2 pour le test.

### Encodage des Mouvements

Les mouvements des joueurs sont encodés dans des fichiers texte avec les caractères suivants :

- `'v'` : DOWN
- `'>'` : RIGHT
- `'<'` : LEFT
- `'^'` : UP

## Contribuer

1. Forkez le projet.
2. Créez une branche pour votre fonctionnalité (`git checkout -b feature/nouvelle-fonctionnalite`).
3. Committez vos modifications (`git commit -am 'Ajout d'une nouvelle fonctionnalité'`).
4. Poussez la branche (`git push origin feature/nouvelle-fonctionnalite`).
5. Ouvrez une Pull Request.
