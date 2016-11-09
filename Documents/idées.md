# Roguelike, idées

[Sujet](SujetsProjets2016.pdf)

## Fonctionnement de base

**But du jeu** : Le jeu prend place dans un donjon, le joueur doit se rendre tout en haut, pour aller chercher coffre rempli d'or et de diamants.

- Générateur de niveaux (juste les pièces et couloirs). Génère en fonction d'un nombre de pièces donné (min et max).
- Personnage qui se déplace d'étages en étages.
- Sauvegarde de la partie dans un fichier.
- Monstres, pièges, points de vie, faim, ...
- Gestion de la lumière.
- Mini-AI des monstres.
- Personnages différents pour le joueur (humains, nains, magicien).

## Règles

- Génération des objets sur la map, aléatoirement, mais lié au niveau (plus difficile dans les niveaux élevés).
- Temps qui s'incrémente à chaque action.
- 3 sauvegardes différentes sur le jeu.
- Les dégats éffectués à un monstre est proportionnel à la faim, et à la vie du joueur.
- Habilité pour un humain :
  > Force : 10

  > Vie   : 50

  > Habilité : 20% (esquive 20% des coups par défaut & donne des coups 20% plus fort)

  > Armure : 0 par défaut (esquive en plus 0% des coups)

## Affichage

- Utilisation d'une TUI ? `ncurses` ?
- Lancement du jeu : texte explicatif de l'histoire
- Zone de jeu
- Zone d'informations (statistiques du joueur)
- Zone de logs : déroulement du jeu, description de la situation


1. Joueur : `@`
2. Escaliers : descendre : `>` et monter : `<`
3. Portes : fermée : `+` ouverte : `-` ou `|`
4. Ouvrir et fermer les portes : `o` et `c`. Le jeu demande alors quelle porte nous voulons ouvrir (avec les flèches).

- Mouvements : flèches, valider entrée, cramer un temps : entrée.


```
############
#  <       #
######## + #
#          #          #######
#          ############     #
#                     +     #
#          ############     #
#          #          #   @ #
############          #######


```




## Déroulement du jeu :

- ~ Gestionnaire de parties (avec lancement d'une partie déjà crée, ou création d'une nouvelle partie sur un emplacement de sauvegarde).
- ~ Si nouvelle partie : vue du chateau
- ~ Histoire du jeu si nouvelle partie.
- Génération de tous les niveaux (avec leur sauvegardes dans un fichier).
- Enregistrement des infos du joueur : position, étage, stats.
- Le joueur apparait au premier étage, ou à l'endroit où il était auparavant.
- Sauvegarde du jeu à chaque actions.
- Intéractions avec le jeu.


## Structures & Énumérations
