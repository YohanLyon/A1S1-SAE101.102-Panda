# A1S1-SAE101.102

SAE of semester number 1 on the theme of programming
> Programming of an application allowing the analysis of the bamboo shoot. It is a simulation using a panda robot. That gives the statistics.</br>

## Sujet

Vous venez d'acheter une maison au bord d'un lac. Un jardin de bambous pousse à l'extérieur de la maison et obstrue la belle vue sur le lac. Pour résoudre ce problème, vous achetez un robot jardinier panda qui, une fois par jour, peut tailler instantanément un seul bambou. Comme vous avez mesuré le taux de croissance quotidien de chaque bambou du jardin, vous devez maintenant programmer le robot jardinier avec un programme approprié pour tailler les bambous afin de garder la plus belle vue.

L'objectif du projet est donc de programmer le robot panda afin d'optimiser la découpe des bambous, et maintenir la forêt en-dessous d'un seuil max. Chaque jour, le schéma est le même: les bambous poussent, le robot doit choisir quel bambou couper (peut-être n'y en a-t-il pas ?) et couper le bambou sélectionné.
Plusieurs algorithmes simples peuvent être implémentés: aléatoire, hauteur max, développement max (développement=hauteur+croissance),...
Pour évaluer les différents algorithmes, plusieurs critères pourront être pris en compte. Des critères sur la **taille** des bambous (la hauteur max des bambous restants, la hauteur min des bambous restants (hors celui coupé :smirk:), la hauteur moyenne des bambous) et des critères sur le **temps** d'exécution des différentes méthodes.

### Formalisation
Pour ceux que cela intéresse, une formalisation de ce problème peut être trouvée [ici](https://drops.dagstuhl.de/opus/volltexte/2020/12766/pdf/LIPIcs-FUN-2021-5.pdf)
![](https://codimd.math.cnrs.fr/uploads/upload_9aa63a8755091a23844a9806845d5690.png)


### Stratégies
Deux stratégies émergent naturellement : **Reduce-Max()** et **Reduce-Fastest(x)** :
- **Reduce-Max()** coupe le plus grand bambou du moment. Dans ce cas, on remarque expérimentalement que cela permet de maintenir la hauteur max. de la forêt en dessous de *2H*, où *H* est la croissance de la forêt entière (càd, la somme des taux de croissance journaliers). 
- **Reduce-Fastest(x)** sélectionne le bambou qui grandit le plus vite parmi ceux qui ont une taille supérieure à *xH*, où x>1. Le choix de *x* qui minimise la taille max de la forêt est *x = 1 + 1/sqrt(5) ≈ 1.45*. Cet algorithme garantit une taille max de *2.62H* mais en ne coupant pas de bambous à chaque tour (on économise la batterie du robot panda !).


Un autre aspect critique est le fait que votre robot a des capacités et une mémoire limitée ! Il est alors important de déterminer rapidement le prochain bambou à couper, en un temps au plus linéaire (fonction du nombre de bambous). Ce problème sera abordé plus loin dans la partie avancée concernant les **extensions**. 

---

## Démo
https://www.isnphard.com/g/bamboo-garden-trimming/

![](https://codimd.math.cnrs.fr/uploads/upload_7940c4248aa533db363db19f339eb515.png =400x250)

---

## Travail demandé

### Niveau 0

En mode console, votre programme doit pouvoir à chaque étape:
- programmer la croissance journalière des bambous,
- déplacer le robot panda (plusieurs implémentations d'algorithmes),
- calculer les statistiques de taille des bambous, la moyenne, la taille max. atteinte, etc... et les afficher dans la console.

Votre programme doit également permettre de paramétrer l'application (choix de l'algorithme, le nombre de bambous,...).

---

### Niveau 1

Les éléments du niveau 0 peuvent être repris en SDL afin d'afficher les étapes du déroulement de l'algorithme choisi ainsi que les courbes de statistiques. L'interface peut contenir les éléments suivants :
- **affichage des bambous**, qui s'adapte au nombre et à la taille max. des bambous
- **choix de la stratégie**, et choix de *x* pour *Reduce-Fastest*
- **affichage des courbes de statistiques journalières**, ainsi que le max déjà atteint. Pour les statistiques, on pourra se limiter aux T=100 dernières mesures
- et tout élément qui vous semble pertinent pour améliorer l'affichage et l'interaction avec l'utilisateur

:::info 
*Aide pour affichage en valeurs glissantes*

 * si T est la taille du tableau (T=100)
 * tant que la table n'est pas pleine (si i < T valeurs) les afficher, de 0 à i
 * si T valeurs ou plus (i >= T), enregistrer les nouvelles valeurs en repartant du début, sans effacer les suivantes. Pour la ième valeur, l'enregistrer simplement dans la case i%T, et afficher les valeurs de (i+1)%T à (i+T)%T
 * par exemple si T=100 et i=10, pour la ième valeur, enregistrer dans la case 10, et afficher de 11 à 111 %100, ce qui donnera bien 100 valeurs de 11 à 99 puis 0 à 10. Et la dernière valeur affichée est bien celle se trouvant à la case 10.
:::

Évidemment, vous avez toutes libertés esthétiques pour votre application, mais cela ne sera pas le point principal d'évaluation. Elle doit aussi, et surtout, être la plus ergonomique possible.

---

### Niveau 2

Dès lors, de nombreuses options peuvent être envisagées. À vous de compléter votre application avec une/des option(s) possible(s):
- boutons SDL pour **lancer/arrêter l'animation**
- ajouter une **interaction utilisateur**. Dans ce mode manuel, on ne programme plus le robot-panda, c'est l'utilisateur qui choisit quel bambou couper, par exemple :
    - avec les flèches, pour déplacer le panda, et appui sur une touche pour couper
    - directement avec un clic souris sur le bambou choisi
- pouvoir **enregistrer une configuration** (le nombre de bambous et le tableau de croissance, par exemple), et la **recharger depuis un fichier**. Affichage de la configuration en SDL
    - on peut également ajouter un mode édition, soit texte (on rentre les valeurs dans une zone de saisie SDL), soit avec des *sliders*, soit graphique (on affiche les bambous après 1 jour, et on modifie leur taille à l'aide de la souris)

---

### Niveau 3

- Critères de **temps de calcul**. Outre les courbes de statistiques sur la taille des bambous, il est intéressant d'évaluer la performance de vos algorithmes. Ajoutez une courbe présentant les temps d'exécution à chaque étape de vos algorithmes.
- **Contraintes supplémentaires**. Il est possible d'ajouter des contraintes pour le robot:
    - le déplacement du robot est **limité à quelques cases** autour de la position actuelle (cela demande d'anticiper un déplacement plus lointain pour ne pas rester localisé autour d'une position). Pensez à définir une position de départ,
    - le robot étant alimenté, il peut nécessiter de revenir à son point de départ pour effectuer une **recharge** des batteries :
        - régulière, tous les 7 jours par exemple
        - ou dépendant de la distance parcourue (on pourra supposer que les bambous sont régulièrement espacés. Penser aussi à garder la position précédente du panda pour calculer la distance effectuée chaque jour.)
- malgré un algorithme performant, la forêt vous gâche toujours la vue, vous achetez un **deuxième robot**...

---

### Niveau +
En extension, on pourra aussi : 
- travailler en **2 dimensions**. Dans ce cas, les bambous seront placés sur une grille de *L* lignes et *C* colonnes.
    - reprendre l'application pour utiliser des tableaux à 2 dimensions
    - adapter l'affichage SDL
    - ajouter des contraintes spécifiques, par exemple le panda ne peut se déplacer qu'en ligne ou colonne pour son déplacement journalier
- implémenter des algorithmes **plus performants**... 

Chaque jour, le robot-panda doit examiner tous les bambous. Les 2 algorithmes `Reduce-Max()` et `Reduce-Fastest(x)` donc ont une complexité linéaire en `O(n)`, où `n` est le nombre de bambous.

Il est bien sûr possible d'améliorer cela en optimisant la recherche du bambou à couper. Par exemple, si l'on sait qu'une partie de la forêt pousse moins vite, on peut la tester moins souvent. Mais il y a beaucoup d'autres possibilités !

Cet article [(Cutting Bamboo down to Size, 2021)](https://drops.dagstuhl.de/opus/volltexte/2020/12766/pdf/LIPIcs-FUN-2021-5.pdf) en propose quelques uns, qui nécessitent cependant des structures de données en arbre, que vous n'avez pas encore étudiées en algorithmique...

:::warning
[***PROPOSITIONS D'ALGORITHMES À VENIR***]
:::

Et si l'on considère que le robot-panda a une autonomie limitée, on peut aussi rechercher des algorithmes qui minimisent le nombre de bambous qu'il coupe, ou qui minimisent ses déplacements.

---

## Analogie avec l'informatique

Un parallèle peut être fait entre ce problème de taille de bambous et la gestion de files d'attente.
Un système d’attente est composé d’un espace de service (comportant un ou plusieurs serveurs) et d'un espace d'attente dans lequel se forme une éventuelle file d'attente.
La littérature distingue :
- les systèmes ouverts, où la file (le nombre d'entrées en attente) est de longueur infinie, et
- les systèmes fermés, où la file d'attente est de taille finie.

Plusieurs méthodes peuvent alors être mises en évidence pour la gestion de ces files d'attente.

Étant donné une file d'attente, plusieurs caractéristiques sont identifiables afin de mesurer l'efficacité de la méthode choisie :
- la longueur moyenne de la file,
- la durée moyenne de service
- le taux moyen d'inactivité des serveurs,
- le temps moyen d'attente dans une file
- le temps moyen total de service dans le système (ou « temps de résidence » en informatique).

---

## Matériel et logiciels nécessaires
Un PC, Visual Studio C++ (ou n'importe quel autre IDE équivalent) de développment C++, la bibliothèque graphique SDL, Git.

Pour un projet VSC++, vous devez installer les **nuggets SDL**, mais ils ne seront pas intégrés au dépôt Git.
Avant de faire le *push*, assurez vous que le fichier **.gitignore** est correctement configuré, pour ne pas mettre tout le dossier *.vs* sur le dépôt Git ! (Menu ```Git>Paramètres>Général>.gitignore```)

Quand vous **clonez** un projet depuis un dépôt Git sur votre poste de travail :
- pour la configuration dans VSC++, souvent un simple clic sur le fichier *.sln* suffit
- il faut aussi bien sûr installer les *nuggets* SDL pour votre projet local

Ceci n'est à faire qu'une seule fois.

Pour **Git**, toutes les informations ici : [https://iutbg-gitlab.iutbourg.univ-lyon1.fr/fabrice.jaillet/ateliergit](https://iutbg-gitlab.iutbourg.univ-lyon1.fr/fabrice.jaillet/ateliergit)

---

### Distanciel...
Git, jitsi, discord, etc...

---

## Groupes {#planning_groupes}
Vous travaillerez en groupes de 3 ou 4:
* les groupes sont déterminés **aléatoirement**
* dans un même groupe TD
* vous devez choisir un **“chef de projet”** dont le rôle officieux sera
    * de répartir les tâches de chacun
    * d’intégrer les différents codes
    * d’évaluer l’avancée du groupe
