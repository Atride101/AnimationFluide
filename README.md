BoyerMooreHorspool
==================

Ce programme C++ permet de faire l'animation et la modélisation de liquides.

La simulation de l’écoulement du liquide se fait par la méthode Smoothed Particle Hydrodynamics (SPH). Cette technique divise la masse totale du liquide en plus petites masses, appelées particules, qui sont animées de telle sorte qu’elles respectent une équation différentielle. Cette équation permet de quantifier les différentes forces qui agissent sur chaque particule, telles que la force de pression et la force de viscosité, en tenant compte de ses voisins.

La modélisation du liquide lui-même se fait par la technique des Marching Tetrahedras. Cette technique se base sur un échantillonage régulier de l'espace pour en former une grille régulière.

Lorsque le logiciel est démarré, il ouvre une fenêtre d’affichage où il est possible de visualiser la scène interactivement en mode OpenGL. Voici les touches et contrôles:

p : Met l’animation en pause
0 : Remet à zéro la vélocité des particules
m : Active/désactive l’affichage de la surface par Marching Tetrahedra 
r : Active/désactive l’effet de réfraction   approximative du liquide
espace+souris : Applique une rotation au contenant
