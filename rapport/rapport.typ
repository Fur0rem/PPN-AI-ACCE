#import "conf.typ": conf
#import "utils.typ": bench_show, code_snippet
#import "@preview/xarrow:0.3.0": xarrow

#show link: this => {
	let show-type = "box" // "box" or "filled", see below
	let label-color = green
	let default-color = blue
	
	if show-type == "box" {
		if type(this.dest) == label {
			// Make the box bound the entire text:
			// set text(bottom-edge: "bounds", top-edge: "bounds", stroke: label-color)
			// text(this, stroke: label-color)
			text(label-color, [#this])
			// box(this, stroke: label-color + 1pt)
		} 
		else {
			//set text(bottom-edge: "bounds", top-edge: "bounds", stroke: default-color)
			text(default-color, [#this])
			// box(this, stroke: default-color + 1pt)
		}
	} 
	else if show-type == "filled" {
		if type(this.dest) == label {
			text(this, fill: label-color)
		} 
		else {
			text(this, fill: default-color)
		}
	} else {
		this
	}
}

// set indentations
#set list(indent: 1cm)

#let project_name = "PPN-AI-ACCE"

#set page(
	margin: 1.8cm,
	header: [
		#set align(right + horizon)
		#emph[#project_name]
	], 
	footer: [
		#set align(right)
		#counter(page).display("1") / #locate(loc => {
			let lastpage = counter(page).final(loc)
			[#lastpage.at(0)]
		})
	],
	// fill: gradient.conic(..color.map.rainbow),
)

#show: conf.with(
	title: [
		Projet PPN: Estimateur de performance de code assembleur en cycles CPU par intelligence artificielle
	],
	project_name: [],
	authors: (
		(
			name: "BAUMANN Pierre",
			affiliation: "Master 1 CHPS",
		),
		(
			name: "DARDILL Hugo",
			affiliation: "Master 1 CHPS",
		),
		(
			name: "DELMAY Arthur",
			affiliation: "Master 1 CHPS",
		),
	),
	abstract: [
		Le but de ce projet est de créer un estimateur de performance de code assembleur par intelligence artificielle.
		Il devra être capable de prédire le nombre de cycles CPU qu'un noyau de calcul assembleur prendra à s'exécuter.
	]

)

#outline()

= Introduction

Être capable de prédire la performance d'un code assembleur est un enjeu majeur pour l'optimisation de code.
En effet, pouvoir estimer rapidement le nombre de cycles CPU qu'un noyau de calcul prendra à s'exécuter permet de choisir les meilleures optimisations pour ce code, comme le choix des instructions, la vectorisation, le parallélisme, etc...
#linebreak()
Cela est un enjeu critique pour les applications nécessitant des performances élevées comme la simulation numérique, l'intelligence artificielle, et surtout les compilateurs, qui doivent pouvoir rapidement choisir des optimisations pour un code donné.


La plupart des approches actuelles pour prédire la performance d'un code assembleur sont basées sur la simulation de l'exécution du code sur un processeur @llvm_mca, ce qui est très coûteux en temps de calcul et est dépendant de l'architecture du processeur, ou des micro-benchmarks @MAQAO, qui ne sont pas toujours représentatifs de la performance réelle du code à cause des effets de cache, de la prédiction de branchement, etc...
#linebreak()
Nous proposons donc une approche basée sur l'intelligence artificielle et les réseaux de neurones pour résoudre ce problème.


Dans ce rapport, nous allons dans une première partie expliquer ce qu'est un réseau de neurones, comment lui donner en entrée du code assembleur, comment récupérer des données, et les méthodes d'entrainement.
Dans une seconde partie, nous allons présenter les résultats obtenus et le travail restant à faire.

= Premières expériences

Pour commencer, nous avons vérifié si cette approche pouvait être viable en tentant de prédire une version simplifiée du problème sur des frameworks de machine learning déjà existants.
#linebreak()
Notre expérience consistait à associer à chaque lettre un nombre aléatoire entre 1 et 50, et faire la somme de ces nombres pour des phrases de longueurs différentes.
Cette tâche se rapproche sur 2 points de la prédiction de performance de code assembleur.
- Les instructions assembleur ont souvent un nombre de cycles CPU précis.
- Les codes peuvent être de tailles différentes.
Cependant, elle est beaucoup plus simple, et ne prend pas en compte les dépendances entre les instructions, les patterns d'accès mémoire, etc...

Nous avons donc utilisé pytorch @pytorch pour voir si un réseau de neurones pouvait apprendre cette tâche.

#figure(
	image("images/ascii.svg"),
	caption: "Apprentissage de la somme de valeurs aléatoires associées à des lettres"
)

Malgré l'oscillation de la courbe, nous remarquons que certains des pics les plus bas ont une erreur de l'ordre de $10^(-6)$, ce qui montre que le réseau de neurones arrive à apprendre cette tâche, et est donc bon signe pour la viabilité de notre approche.
#footnote("On suppose que l'oscillation de la courbe est due à la manière dont pytorch explore l'espace des solutions")

= Théorie des réseaux de neurones

Les réseaux de neurones (ou neural networks) sont des modèles statistiques inspirés du fonctionnement biologique du cerveau humain.
Ils sont conçus pour résoudre des problèmes complexes en apprenant à partir de données, et résoudre des tâches pour lesquelles un algorithme classique serait impossible à écrire.

Pour mesurer l'efficacité d'un réseau de neurones, nous utilisons une fonction de coût, souvent appelée Loss et notée $C$, est une fonction qui mesure l'erreur entre la prédiction du réseau et la valeur réelle.
Il existe différentes fonctions de coût, ici nous allons utiliser la fonction MSER (Mean Squared Error Relative) 
#linebreak()
MSER = $frac(1,n) sum_(i=1)^n frac((y_i - y'_i)^2, y_i)$, où $y_i$ est la valeur réelle, $y'_i$ est la prédiction du réseau, et $n$ est le nombre de données.
#linebreak()
Elle permet d'accorder plus d'importance aux grosses erreurs, et normalise l'erreur par la valeur réelle, afin de prendre en compte les différentes échelles des données.
#linebreak()
Notre but va être de minimiser cette fonction.

#pagebreak()
Tout d'abord, il existe 2 principaux types de réseaux de neurones:
- Les classifieurs, qui prennent en entrée des données et les séparent en différentes classes ou catégories, souvent utilisés pour la reconnaissance d'images, d'espèces végétales, etc...
- Les régresseurs, qui prennent en entrée des données et retournent une valeur continue, souvent utilisés pour la prédiction de prix, de température, etc...
#linebreak()
Un régresseur semble bien plus adapté à notre problème, car nous voulons prédire un nombre de cycles CPU, qui est une valeur continue.
Ensuite, expliquons la structure d'un tel objet, un réseau de neurones est composé de plusieurs couches, appelées layers, qui sont elles-mêmes composées de neurones.
- La couche d'entrée, c'est la toute première couche où les données initiales sont introduites dans le réseau.
- Une ou plusieurs couches cachées, qui traitent et transforment les données grâce à des liens pondérés entre les neurones, afin de pouvoir trouver des relations complexes entre les données.
- La couche de sortie, qui produit le résultat final, et est donc notre prédiction.
Nous appelons topologie du réseau la suite des tailles des différentes couches: (taille entrée, taille couche cachée 1, taille couche cachée 2, ..., taille sortie)

Les neurones, ou nœuds, sont les unités de calcul du réseau de neurones, tous interconnectés entre eux.
#linebreak()
Les connexions entre les neurones sont pondérées, c'est-à-dire que chaque connexion a un poids noté $w$ qui multiplie la valeur de sortie du neurone précédent, cela permet de donner plus ou moins d'importance à certaines relations entre les neurones.
#linebreak()
Chaque neurone possède aussi un biais noté $b$, qui est une valeur constante ajoutée à la somme pondérée des valeurs d'entrée, il permet de représenter la contribution et l'importance de ce neurone dans le réseau.

#grid(
  columns: (1fr, 1fr),
  [
    #figure(
      image("neuron.png", width: 100%, fit: "contain"),
      caption: "Neurone d'un cerveau et son équivalent mathématique"
    )
  ],
  [
    #figure(
      image("network.png", width: 100%, fit: "contain"),
      caption: "Structure d'un réseau de neurones"
    )
  ]
)

#pagebreak()

Les fonctions d'activation sont des fonctions qui sont appliquées aux sorties des neurones, elles permettent de donner au réseau de neurones la capacité de modéliser des relations plus complexes entre les données.
Le plus important est que ces fonctions soient non-linéaires, c'est-à-dire d'une forme différente d'une droite, car sinon un réseau de neurones serait équivalent à une application linéaire, et ne pourrait donc pas apprendre des patterns non-linéaires, ce qui est souvent le cas dans la réalité.
#linebreak()
De plus elles imitent le fonctionnement des neurones biologiques qui sont stimulés ou non, et avoir différentes réponses en fonction du signal reçu.
#linebreak()
Deux de ces fonctions d'activation les plus utilisées sont la fonction sigmoïde et la fonction ReLU, nous allons les étudier ainsi que leur dérivée, car nous allons en avoir besoin pour l'entrainement du réseau de neurones.
$ "relu" (x) = max(0, x) $
#figure(
	image("relu.png", width: 50%, fit: "contain"),
	caption: "Fonction d'activation ReLU"
)
La fonction ReLU (Rectified Linear Unit) est la plus utilisée, car elle est simple, rapide à calculer, permet de vite converger, et permet d'éviter le problème du vanishing gradient, qui est un problème où la dérivée de la fonction de coût par rapport aux poids et aux biais devient très petite, ce qui rend l'entrainement du réseau très difficile.
#linebreak()
La dérivée de la fonction ReLU est aussi très simple à calculer, car elle est égale à 1 si $x > 0$ et 0 sinon.

Cependant, elle possède un inconvénient, elle peut "tuer" des neurones, c'est-à-dire que si un neurone a une valeur négative, il va toujours retourner 0, et donc ne plus apprendre.

La fonction sigmoïde est une autre fonction d'activation, qui est souvent utilisée pour les problèmes de classification, car elle permet de donner une probabilité à une classe, et est dérivable en tout point.
$ #sym.sigma (x) = 1 / (1 + e^(-x)) $
#figure(
	image("sigmoid.png", width: 50%, fit: "contain"),
	caption: "Fonction d'activation sigmoïde"
)
La fonction sigmoïde est une fonction en forme de S, qui prend en entrée un nombre réel et retourne un nombre entre 0 et 1.
Cependant, elle est bien plus coûteuse en temps de calcul que la fonction ReLU, à cause de l'exponentielle.
Sa dérivée est aussi plus complexe à calculer, car elle est égale à $ #sym.sigma (x) * (1 - #sym.sigma (x))$.

//#pagebreak()

Donc la sortie d'un neurone est donnée par la formule suivante:
$ f(x) = #sym.alpha (w #sym.dot.op x + b), " avec" #sym.alpha "la fonction d'activation" $
Et la sortie d'un réseau de neurones est donnée par la formule suivante:
$ f(x) = #sym.alpha (w_n #sym.alpha (w_{n-1} #sym.alpha ( ... #sym.alpha (w_1 #sym.alpha (w_0 x + b_0) + b_1) ... ) + b_{n-1}) + b_n) $

En pratique, ces équations sont implémentées sous forme de produits matriciels, en effet nous pouvons représenter les poids et les biais des connexions entre les neurones sous forme de matrices, et les données d'entrée et de sortie sous forme de vecteurs, ce qui permet de calculer les sorties des neurones et les erreurs de manière très efficace.
Par exemple la valeur de sortie d'un neurone revient à faire un produit scalaire $ x = #sym.alpha (vec(e_1, e_2, ..., e_n) #sym.dot.op vec(w_1, w_2, ..., w_n) + b) $
#linebreak()
Pour l'implémentation, nous utilisons la bibliothèque Eigen @eigen, qui est une bibliothèque C++ de calcul matriciel, qui appelle la bibliothèque BLAS derrière, ce qui permet d'avoir des calculs très rapides.

Nous pouvons modéliser cette transformation avec les schémas suivants:
// Grid with 3 columns
#figure(
	grid(
		columns: (1fr, 1fr, 1fr),
		[
			#figure(
				image("images/start_space.png", width: 50%, fit: "contain"),
				caption: "Espace d'entrée"
			),
		],[
			#figure(
				image("images/transform.png", width: 43%, fit: "contain"),
				caption: "Transformation de l'espace avec les poids et les biais"
			),
		],[
			#figure(
				image("images/activation.png", width: 58%, fit: "contain"),
				caption: "Application de la fonction d'activation"
			)
		]
	)
)

Tout le but de l'entrainement d'un réseau de neurones est d'ajuster les poids et les biais des connexions entre les neurones pour minimiser la fonction de coût, et donc faire de meilleures prédictions, en utilisant un algorithme d'optimisation afin de trouver les valeurs modélisant au mieux les données d'entrainement.
Le réseau de neurones va donc itérativement tenter de minimiser cette fonction de coût, ces itérations sont appelées des époques, ou epochs.
#pagebreak()
Pour cela, nous utilisons deux techniques:

/*- La propagation avant
	La propagation avant est le processus où les données sont introduites dans le
	réseau de neurones, et sont transmises de couche en couche jusqu'à la couche de sortie.

- La rétropropagation
	La rétropropagation est le processus où l'erreur est calculée entre la prédiction du réseau et la valeur réelle, et est transmise de la couche de sortie à la couche d'entrée, afin de mettre à jour les poids et les biais des connexions entre les neurones.
	Pour cela, nous utilisons l'algorithme de rétropropagation du gradient, qui calcule le gradient de la fonction de coût par rapport aux poids et aux biais, et met à jour ces valeurs en fonction du gradient et d'un taux d'apprentissage.
	Ce qui fait que le réseau va au fur et à mesure se diriger de lui-même vers les valeurs optimales des poids et des biais.
	Les 4 équations de la rétropropagation sont les suivantes:
	$ delta^L = #sym.nabla{a} C #sym.dot.op #sym.alpha'(z^L) $, qui est l'erreur de la couche de sortie
	$ delta^l = (w^(l+1))^T delta^(l+1) #sym.dot.op #sym.alpha'(z^l) $, qui est l'erreur de la couche $l$ en fonction de l'erreur de la couche $l+1$
	$ frac(#sym.partial C, #sym.partial b^l_j) = delta^l_j $, qui est la dérivée de la fonction de coût par rapport au biais de la couche $l$
	$ frac(#sym.partial C, #sym.partial w^l_(j,k)) = a^(l-1)_k delta^l_j $, qui est la dérivée de la fonction de coût par rapport au poids de la connexion entre le neurone $k$ de la couche $l-1$ et le neurone $j$ de la couche $l$
*/

La première est la descente de gradient. Un réseau de neurones est un problème d'optimisation, c'est-à-dire que nous pouvons le voir comme une très grande fonction mathématique où chaque poids et chaque biais sont des paramètres, et le but est de trouver les valeurs de ces paramètres qui minimisent une fonction de coût.
	#linebreak()
	La descente de gradient est un algorithme adapté en multi-dimension de la méthode de Newton, qui permet de trouver les minimums locaux d'une fonction, en suivant la dérivée de la fonction.
	Le principe de cet algorithme est similaire à faire rouler une bille sur une montagne, la bille va descendre le long de la montagne en suivant la pente la plus raide, jusqu'à se stabiliser dans un creux.
	Ici, la bille est notre configuration de poids et de biais, la montagne est la fonction de coût, et la pente est le gradient, de la fonction de coût par rapport aux poids et aux biais.
	#figure(
		image("gradient.png", width: 50%, fit: "contain"),
		caption: "Descente de gradient"
	)

Le problème majeur de la descente de gradient est que nous devons utiliser les dérviées partielles $frac(#sym.partial C, #sym.partial y_i)$ pour chaque poids et chaque biais, pour chaque entrée, ce qui est très coûteux en temps de calcul.
#linebreak()
C'est ici qu'entre en jeu la rétropropagation, qui va être une estimation de ces dérivées partielles, en utilisant la règle de la chaîne, qui permet de décomposer une dérivée en plusieurs dérivées plus simples.
Cela permet d'avoir à calculer moins de dérivées à chaque étape et d'avoir un entrainement plus rapide.
#linebreak()
#linebreak()
Nous commençons par mettre en entrée les données dans le réseau de neurones, et les transmettre de couche en couche jusqu'à la couche de sortie afin d'obtenir une prédiction, c'est ce qu'on appelle la propagation avant.
#linebreak()
Ensuite, l'erreur est calculée entre la prédiction du réseau et la valeur réelle, et chaque couche va modifier les poids et les biais des connexions des couches d'avant afin d'indiquer ce qu'elles auraient dû faire pour minimiser l'erreur, c'est ce qu'on appelle la rétropropagation.
	Cela permet d'avoir à calculer moins de dérivées à chaque étape et d'avoir un entrainement plus rapide.
	#linebreak()
	Les 4 équations de la rétropropagation sont les suivantes:
	- L'erreur de la couche de sortie : $delta^L = #sym.nabla{a} C #sym.dot.op alpha'(z^L)$ #linebreak()
	// $ delta^l = (w^(l+1))^T delta^(l+1) #sym.dot.op alpha'(z^l) $, qui est l'erreur de la couche $l$ en fonction de l'erreur de la couche $l+1$
	// $ frac(#sym.partial C, #sym.partial b^l_j) = delta^l_j $, qui est la dérivée de la fonction de coût par rapport au biais de la couche $l$
	// $ frac(#sym.partial C, #sym.partial w^l_(j,k)) = a^(l-1)_k delta^l_j $, qui est la dérivée de la fonction de coût par rapport au poids de la connexion entre le neurone $k$ de la couche $l-1$ et le neurone $j$ de la couche $l$
	- L'erreur de la couche $l$ : $delta^l = (w^(l+1))^T delta^(l+1) #sym.dot.op alpha'(z^l)$ #linebreak()
	- La dérivée de la fonction de coût par rapport au biais de la couche $l$ : $frac(#sym.partial C, #sym.partial b^l_j) = delta^l_j$ #linebreak()
	- La dérivée de la fonction de coût par rapport au poids de la connexion entre le neurone $k$ de la couche $l-1$ et le neurone $j$ de la couche $l$ : $frac(#sym.partial C, #sym.partial w^l_(j,k)) = a^(l-1)_k delta^l_j$

Le taux d'apprentissage $gamma$ est un hyperparamètre du réseau de neurones, qui détermine la taille des pas que l'algorithme d'optimisation va faire pour trouver les valeurs optimales des poids et des biais. Ce qui correspondrait à notre pas de temps pour simuler la descente de la bille sur la montagne.
Il est souvent choisi entre 0.1 et 0.0001, et doit être ajusté en fonction du problème et de la taille du réseau de neurones, et peut être modifié au cours de l'entrainement.
Un taux d'apprentissage trop grand peut faire diverger l'entrainement, et un taux d'apprentissage trop petit peut faire stagner l'entrainement.
#figure(
	image("learning_rate.png", width: 50%, fit: "contain"),
	caption: "Impact du taux d'apprentissage sur l'entrainement d'un réseau de neurones"
)

Grâce à toutes ces méthodes, le réseau de neurones va pouvoir converger vers les valeurs optimales des poids et des biais, et donc "apprendre" à partir des données d'entrainement.

= Transformer le code assembleur en données exploitables

Comme dit précédemment, un réseau de neurones prend en entrée un vecteur de nombres réels entre 0 et 1.
Il est donc nécessaire de transformer le code assembleur en un vecteur de nombres réels pour pouvoir l'exploiter.
Pour cela, 2 approches sont possibles:

Traiter le code assembleur comme du texte et donc utiliser des méthodes de traitement de texte.
#linebreak()
Cette approche possède quelques inconvénients, déjà les Large Language Models, les modèles de traitement de texte les plus performants, sont très lourds et nécessitent des ressources importantes pour être utilisés, et vont donc être très longs à entraîner.
#linebreak()
Aussi l'assembleur possède une structure assez différente du texte classique et même des autres langages de programmation, donc difficile de prévoir s'il va être adapté à ces modèles.
#linebreak()
Il y a aussi des syntaxes différentes pour un même code assembleur, comme Intel et AT&T pour l'ordre des opérandes, les registres pouvant être écrits de différentes manières, les commentaires de code, etc..., 
donc il aurait fallu prétraiter le code assembleur pour le mettre dans un format standard, ce qui nécessite tout un travail de parsing et tokenisation du code.

#figure(
	grid(
		columns: (1fr, 1fr),
		[
			#code_snippet(
				lang: "asm",
				code: "section .data
    num1 dd 5
    num2 dd 10

section .text
    global _start

_start:
    mov eax, [num1]   
    add eax, [num2]   
    mov ebx, eax      

    mov eax, 1       
    int 0x80         ",
			),
		],
		[
			#code_snippet(
				lang: "asm",
				code: ".section .data
    num1: .long 5
    num2: .long 10

.section .text
    .globl _start

_start:
    movl num1, %eax
    addl num2, %eax
    movl %eax, %ebx

    movl $1, %eax  
    int $0x80      ",
			),
		]
	)
		, caption: "Deux codes assembleurs additionnant 2 nombres équivalents",
)



Nous avons donc choisi une approche plus simple.
#linebreak()
En fait, l'assembleur est une transcription directe du binaire en langage plus compréhensible par le cerveau humain.
#linebreak()
Donc nous pouvons directement transformer le code assembleur en binaire via un compilateur, et ensuite mettre en entrée ces zéros et uns dans le réseau de neurones.
Nous avons utilisé l'outil nasm pour compiler le code assembleur en hexadécimal, puis nous avons converti l'hexadécimal en binaire.

#figure(
	
grid(
columns: (1fr, 0.5fr, 1fr, 0.5fr, 1fr, 0.5fr, 1fr),
[
#code_snippet(
lang: "asm",
code: "xor rdx, rdx
movss xmm0, [rax + rcx * 4]
")
],
[#xarrow("nasm")],
[0x 48 31 D2 ... E0],
[#xarrow("binaire")],
[0b 01001000 00110001 11010010 ... 11100000],
[#xarrow("réels")],
[0.0 1.0 0.0 0.0 1.0 0.0 0.0 0.0 ... 1.0 1.0 1.0 0.0 0.0 0.0 0.0 0.0]
)
, caption: "Transformation du code assembleur en vecteur de nombres réels"
)

Cependant, une séquence binaire n'aura pas forcément la même signification selon les bits précédents.
Par exemple, 
#grid(
align: center,
columns: (1fr, 1fr),
[
#code_snippet(
lang: "asm",
code: "	ret;
	end_branch:
		add r8, r9;
", tabwidth: 13
)
sera compilée en c3 *4d 01 c8*
],
[
#code_snippet(
lang: "asm",
code: "	mov r10, 0x4d01c8;


", tabwidth: 15
)
sera compilée en 49 c7 c2 *4d 01 c8* 00
]
)
Donc lorsque notre réseau rencontre la séquence 4d 01 c8, il devra prendre en compte les instructions précédentes pour déterminer sa signification.
De plus, les instructions en x86 sont de tailles variables, donc impossible de découper le code en blocs de taille fixe ou d'utiliser la position de l'instruction dans le code pour déterminer sa signification.
#linebreak()
Nous avons donc choisi une balise arbitraire pour séparer les instructions, le nombre 0.5, car nous n'avons mis en entrée que 0 et 1 pour les bits pour l'instant.
#code_snippet(code: "ret;		-> 1 1 0 0 0 0 1 1
;		   -> 0.5
add r8, r9;	 -> 0 1 0 0 1 1 0 1 0 0 0 0 0 0 0 1 1 1 0 0 1 0 0 0 
;		   -> 0.5
", lang: "asm", tabwidth: 8)

Il reste un dernier problème, la taille du code.
En effet, un réseau de neurones prend en entrée un vecteur de taille fixe, or nous pouvons traiter des codes de tailles différentes, car les noyaux de calcul peuvent exécuter un nombre différent d'instructions, et la vectorisation du code peut aussi rajouter plus ou moins d'instructions.


Pour résoudre cela, nous avons défini un nombre maximal de tokens arbitraire pour l'entrée du réseau, ensuite nous marquons la première entrée du vecteur avec le nombre d'instructions du code, encodé par $frac(n, max)$, où $n$ est le nombre de bits de la séquence binaire et $max$ est le nombre maximal de tokens.
Ensuite nous complétons le vecteur avec une valeur arbitraire, ici 0, pour atteindre la taille maximale.
Par exemple, avec un nombre maximal de tokens de 10, l'entrée [0 1 1 0 1 0] sera transformée en [0.6 0 1 1 0 1 0 0 0 0 0]

En mettant toutes ces transformations bout à bout, nous pouvons transformer n'importe quel code assembleur en une entrée exploitable par un réseau de neurones.



= Collecte des données et benchmarking

Nos données sont donc des noyaux de calcul en assembleur, que nous avons benchmarkés pour connaître le nombre de cycles CPU qu'ils prennent.
#linebreak()
Notre but est d'avoir différentes versions de ces noyaux, avec des optimisations différentes, de la vectorisation, ect... afin de comparer les performances des différentes instructions utilisées, l'impact des optimisations et de la vectorisation, en plus d'avoir plus de données pour entraîner notre réseau de neurones.

Pour mesurer les cycles CPU, nous avons utilisé l'outil nanobench @nanobench, qui est un outil de micro-benchmarking.

Prennons par exemple un noyau de calcul qui additionne 2 vecteurs de taille $n$, et faisons varier la vectorisation du code, avec du code non vectorisé, SIMD 128 bits, et SIMD 256 bits, avec des nombres flottants 32 bits et 64 bits.
#footnote("N'ayant pas accès au calculateur Haswell, nous avons réalisé ces benchmarks sur un de nos ordinateurs personnels, qui possède un processeur AMD Ryzen 5 5000, n'allant que jusqu'à 256 bits de vectorisation")

#pagebreak()

#code_snippet(
	lang: "c",
	code: "
// Version non vectorisée
void vec_add_f32_no_smid() {
	for (int i = 0; i < N; i++) {
		c[i] = a[i] + b[i];
	}
}

// Version SIMD 128 bits
void vec_add_f32_simd128() {
	for (int i = 0; i < N; i += 4) {
		__m128 a4 = _mm_load_ps(&a[i]);
		__m128 b4 = _mm_load_ps(&b[i]);
		__m128 c4 = _mm_add_ps(a4, b4);
		_mm_store_ps(&c[i], c4);
	}
}

// Version SIMD 256 bits
void vec_add_f32_simd256() {
	for (int i = 0; i < N; i += 8) {
		__m256 a8 = _mm256_load_ps(&a[i]);
		__m256 b8 = _mm256_load_ps(&b[i]);
		__m256 c8 = _mm256_add_ps(a8, b8);
		_mm256_store_ps(&c[i], c8);
	}
}
"
)

Nous avons donc benchmarké ce noyau pour un N fixé, et nous obtenons ces résultats:
/*#cmarker.render("
|               ns/op |                op/s |    err% |          ins/op |          cyc/op |    IPC |         bra/op |   miss% |     total | benchmark
|--------------------:|--------------------:|--------:|----------------:|----------------:|-------:|---------------:|--------:|----------:|:----------
|              517.13 |        1,933,765.68 |    0.3% |        7,177.00 |        2,075.59 |  3.458 |       1,027.00 |    0.1% |      0.01 | `vec_add_f32_no_smid`
|              133.21 |        7,506,981.41 |    0.1% |        1,801.00 |          535.60 |  3.363 |         259.00 |    0.4% |      0.01 | `vec_add_f32_simd128`
|               70.87 |       14,110,601.83 |    1.3% |          906.00 |          284.85 |  3.181 |         131.00 |    0.8% |      0.01 | `vec_add_f32_simd256`
|              312.48 |        3,200,195.08 |    0.3% |        7,177.00 |        1,253.48 |  5.726 |       1,027.00 |    0.1% |      0.01 | `vec_add_f64_no_smid`
|              159.01 |        6,288,784.20 |    0.2% |        3,593.00 |          638.74 |  5.625 |         515.00 |    0.2% |      0.01 | `vec_add_f64_simd128`
|               83.85 |       11,926,400.78 |    0.0% |        1,802.00 |          338.11 |  5.330 |         259.00 |    0.4% |      0.01 | `vec_add_f64_simd256`
")*/

#figure(
	table(
  columns: 10,
  table.header(
    [ns/op],
    [op/s],
    [err%],
    [ins/op],
    [cyc/op],
    [IPC],
    [bra/op],
    [miss%],
    [total],
    [benchmark],
  ),
  align: center,
  [517.13], [1,933,765.68], [0.3%], [7,177.00], [2,075.59], [3.458], [1,027.00], [0.1%], [0.01], [`vec_add_f32_no_smid`],
  [133.21], [7,506,981.41], [0.1%], [1,801.00], [535.60], [3.363], [259.00], [0.4%], [0.01], [`vec_add_f32_simd128`],
  [70.87], [14,110,601.83], [1.3%], [906.00], [284.85], [3.181], [131.00], [0.8%], [0.01], [`vec_add_f32_simd256`],
  [312.48], [3,200,195.08], [0.3%], [7,177.00], [1,253.48], [5.726], [1,027.00], [0.1%], [0.01], [`vec_add_f64_no_smid`],
  [159.01], [6,288,784.20], [0.2%], [3,593.00], [638.74], [5.625], [515.00], [0.2%], [0.01], [`vec_add_f64_simd128`],
  [83.85], [11,926,400.78], [0.0%], [1,802.00], [338.11], [5.330], [259.00], [0.4%], [0.01], [`vec_add_f64_simd256`],
)
, caption: "Résultats du benchmarking du noyau de calcul d'addition de 2 vecteurs"
)
Les 2 colonnes qui nous intéressent sont cyc/op, qui est le nombre de cycles CPU moyen que prend le noyau de calcul à s'exécuter, et err%, qui est l'erreur relative de la mesure, afin de savoir si la mesure est fiable.
#linebreak()
Les résultats montrent que les versions SIMD du noyau de calcul sont plus performantes que les versions non SIMD, et que la performance évolue linéairement avec la taille des vecteurs.

Pour extraire le code assembleur, nous pouvons mettre des labels spécifiques avec #linebreak() ```c __asm__ volatile("label:"); ``` pour pouvoir les retrouver plus facilement dans le code compilé.
#linebreak()
Ensuite, nous pouvons utiliser des outils comme objdump extraire le code.
#figure(
```asm
000000000000e0e0 <_Z19vec_add_f64_no_smidv>:
    e0e0:	48 8d 05 19 62 01 00 	lea    0x16219(%rip),%rax        # 24300 <f64a>
    e0e7:	48 8d 15 12 82 01 00 	lea    0x18212(%rip),%rdx        # 26300 <f64b>
    e0ee:	48 8d 35 0b a2 01 00 	lea    0x1a20b(%rip),%rsi        # 28300 <f64c>
    e0f5:	bf 00 04 00 00       	mov    $0x400,%edi
    e0fa:	48 c7 c1 00 00 00 00 	mov    $0x0,%rcx
    e101:	f2 0f 10 04 c8       	movsd  (%rax,%rcx,8),%xmm0
    e106:	f2 0f 10 0c ca       	movsd  (%rdx,%rcx,8),%xmm1
    e10b:	f2 0f 58 c1          	addsd  %xmm1,%xmm0
    e10f:	f2 0f 11 04 ce       	movsd  %xmm0,(%rsi,%rcx,8)
    e114:	48 83 c1 01          	add    $0x1,%rcx
    e118:	48 39 f9             	cmp    %rdi,%rcx
    e11b:	7c e4                	jl     e101 <_Z19vec_add_f64_no_smidv+0x21>

000000000000e11d <vec_add_f64_no_smid_end>:
    e11d:	c3                   	ret
    e11e:	66 90                	xchg   %ax,%ax

000000000000e120 <_Z19vec_add_f64_simd128v>:
    e120:	48 8d 05 d9 61 01 00 	lea    0x161d9(%rip),%rax        # 24300 <f64a>
    e127:	48 8d 15 d2 81 01 00 	lea    0x181d2(%rip),%rdx        # 26300 <f64b>
    e12e:	48 8d 35 cb a1 01 00 	lea    0x1a1cb(%rip),%rsi        # 28300 <f64c>
    e135:	bf 00 04 00 00       	mov    $0x400,%edi
    e13a:	48 c7 c1 00 00 00 00 	mov    $0x0,%rcx
    e141:	66 0f 28 04 c8       	movapd (%rax,%rcx,8),%xmm0
    e146:	66 0f 28 0c ca       	movapd (%rdx,%rcx,8),%xmm1
    e14b:	66 0f 58 c1          	addpd  %xmm1,%xmm0
    e14f:	66 0f 29 04 ce       	movapd %xmm0,(%rsi,%rcx,8)
    e154:	48 83 c1 02          	add    $0x2,%rcx
    e158:	48 39 f9             	cmp    %rdi,%rcx
    e15b:	7c e4                	jl     e141 <_Z19vec_add_f64_simd128v+0x21>

000000000000e15d <vec_add_f64_simd128_end>:
    e15d:	c3                   	ret
    e15e:	66 90                	xchg   %ax,%ax

000000000000e160 <_Z19vec_add_f64_simd256v>:
    e160:	48 8d 05 99 61 01 00 	lea    0x16199(%rip),%rax        # 24300 <f64a>
    e167:	48 8d 15 92 81 01 00 	lea    0x18192(%rip),%rdx        # 26300 <f64b>
    e16e:	48 8d 35 8b a1 01 00 	lea    0x1a18b(%rip),%rsi        # 28300 <f64c>
    e175:	bf 00 04 00 00       	mov    $0x400,%edi
    e17a:	48 c7 c1 00 00 00 00 	mov    $0x0,%rcx
    e181:	c5 fd 28 04 c8       	vmovapd (%rax,%rcx,8),%ymm0
    e186:	c5 fd 28 0c ca       	vmovapd (%rdx,%rcx,8),%ymm1
    e18b:	c5 fd 58 c1          	vaddpd %ymm1,%ymm0,%ymm0
    e18f:	c5 fd 29 04 ce       	vmovapd %ymm0,(%rsi,%rcx,8)
    e194:	48 83 c1 04          	add    $0x4,%rcx
    e198:	48 39 f9             	cmp    %rdi,%rcx
    e19b:	7c e4                	jl     e181 <_Z19vec_add_f64_simd256v+0x21>

000000000000e19d <vec_add_f64_simd256_end>:
    e19d:	c5 f8 77             	vzeroupper
    e1a0:	c3                   	ret
    e1a1:	66 66 66 66 66 66 2e 	data16 data16 data16 data16 data16 cs nopw 0x0(%rax,%rax,1)
    e1a8:	0f 1f 84 00 00 00 00 
    e1af:	00 
```
, caption: "Code assembleur extrait des différentes versions du noyau de calcul d'addition de 2 vecteurs"
)

Nous voyons bien nos différentes versions du noyau de calcul, et les instructions vectorielles SIMD utilisées.
#linebreak()
Nous avons donc maintenant un moyen de générer des données pour entraîner notre réseau de neurones, avec un code assembleur et le nombre de cycles CPU qu'il prend à s'exécuter.

= Résultats et validation

Pour l'instant, nous n'avons pas encore de résultats à présenter, car nous n'avons pas encore assez de données pour entraîner un réseau de neurones.
Cependant, pour vérifier notre implémentation de notre réseau de neurones, nous l'avons entraîné sur le problème du XOR, qui est un problème classique pour vérifier si un réseau de neurones fonctionne correctement.
Ce test consiste à entraîner un réseau de neurones à prédire de la porte logique XOR à 2 bits, c'est-à-dire cette table:
#show table.cell.where(y: 0): strong
#set table(
  stroke: (x, y) => if y == 0 {
    (bottom: 0.7pt + black)
  },
  align: (x, y) => (
    if x > 0 { center }
    else { left }
  )
)

#figure(
	table(
		columns: 3,
		table.header(
			[Entrée 1],
			[Entrée 2],
			[Sortie],
		),
		align: center,
		[0], [0], [0],
		[0], [1], [1],
		[1], [0], [1],
		[1], [1], [0],
	table.vline(x: 2),
	)
	,caption: "Table de vérité de la porte logique XOR"
)

Nous avons entraîné notre réseau de neurones sur cette table de vérité en utilisant un réseau de neurones à topologie (1, 3, 2), avec une fonction d'activation sigmoïde, et un taux d'apprentissage $gamma = 0.1$.

#figure(
	image("images/xor.svg"),
	caption: "Apprentissage du XOR par notre réseau de neurones"
)

Comme nous pouvons le voir, notre réseau de neurones arrive à rapidement converger vers une solution, ce qui est un bon signe pour la suite du projet.

= Premier entrainement sur des noyaux de calcul

La prochaine étape a donc été d'entraîner notre réseau de neurones sur les noyaux de calcul que nous avons benchmarkés précédemment, ici un noyau d'addition de 2 vecteurs et un noyau de copie de mémoire, avec différentes optimisations et vectorisations.
#linebreak()
Nous avons utilisé un réseau de neurones avec une topologie de (2000, 3000, 2000, 500, 1), avec des fonctions d'activation ReLU, et un taux d'apprentissage de $gamma = 0.2$ (qui est assez élevé, mais cela nous permet de tester plus rapidement différentes topologies et configurations).
#figure(
	image("images/asm.svg"),
	caption: "Apprentissage des noyaux de calcul par notre réseau de neurones"
)

Sur ce graphique, nous pouvons voir que notre réseau de neurones a 3 phases distinctes:
- Une phase d'exploration, où la fonction de coût fait des hauts et des bas, avec des sauts importants, ce qui est normal pour un réseau de neurones non entraîné.
- Une phase de convergence, où la fonction de coût diminue de manière régulière, ce qui signifie que le réseau de neurones commence à trouver une bonne solution.
- Une phase de stabilisation, où la fonction de coût stagne, ce qui signifie que le réseau de neurones a convergé vers une solution optimale.

Le problème est que nous arrivons à un coût de l'ordre de $10^(-16)$, ce qui est "trop parfait".
Cela signifie que notre réseau de neurones a appris par cœur les données d'entrainement, et fait donc du surapprentissage plutôt que de la généralisation.
Donc, sur des données inconnues, le réseau de neurones ne sera sûrement pas très performant.

Cependant, cela reste un bon signe, et montre que notre réseau de neurones est capable d'apprendre à partir de données, et que la performance de code assembleur est bien prédisible par un réseau de neurones.

#pagebreak()

= Travail restant

Il reste encore beaucoup de travail à faire pour arriver à un estimateur de performance de code assembleur fonctionnel.
La tâche la plus importante est d'avoir plus de données pour entraîner le réseau de neurones, nous n'avons quasiment pas de données pour l'instant, à part un noyau calculant la somme de 2 vecteurs, et un noyau de copie de mémoire, avec plus ou moins de vectorisation du code par instructions SIMD, ce qui n'est clairement pas suffisant pour entraîner un réseau de neurones.
Il faudra donc récolter plein d'autres noyaux de calcul différents, avec des tailles différentes, des types de calculs différents, des optimisations différentes, etc... pour avoir une bonne hétérogénéité des données.

Pour l'instant nous avons pensé à 2 sources :
	- Les enseignants et chercheurs du Master CHPS, qui ont sûrement déjà travaillé sur plein de noyaux de calcul différents, notamment pour les TP d'architectures parallèles.
	- Le site #link("https://benchmarksgame-team.pages.debian.net/benchmarksgame/index.html")[The Computer Language Benchmarks Game], qui propose différentes tâches à implémenter, comme la simulation de N corps célestes, génération de l'ensemble de Mandelbrot, ect..., en différents langages de programmation fait par la communauté afin de comparer les performances de ces langages, et donc plein de variantes d'un même noyau de calcul.
		En plus, le site possède une section "Hand-written vector instructions" qui propose des implémentations avec différents niveaux de vectorisation avec les extensions SIMD des processeurs modernes, ce qui est parfait pour notre projet.
	#grid(
	columns: (1fr, 1fr),
	[
		#figure(
			image("images/nbody_rankings.png"),
			caption: "Classement des langages de programmation pour la simulation de N corps",
		)
	],
	[
		#figure(
			image("images/nbody_code.png"),
			caption: "Partie du code le plus rapide pour la simulation de N corps",
		)
	]
	)

Ensuite, il faudra entrainer le réseau de neurones sur ces données, et tester son efficacité sur des noyaux de calcul inconnus.
Pour cela, nous avons penser à utiliser la méthode de validation croisée, qui consiste à diviser les données en 2 parties, une pour l'entrainement et une pour le test, et à répéter l'opération plusieurs fois pour avoir une idée de la performance du réseau de neurones sur des données inconnues.

Une fois ces données récoltées, nous pourrons explorer différentes méthodes pour améliorer notre réseau de neurones.
- Pour combattre le surapprentissage, nous pourrons utiliser des techniques de régularisation, comme de ne prendre à chaque époque qu'une petite partie des données pour l'entrainement, afin de reconnaître les patterns généraux du code et non pas les patterns spécifiques à un noyau de calcul.
- Utiliser un optimiseur afin d'avoir un taux d'apprentissage adaptatif et converger plus rapidement vers les solutions optimales, comme la descente de gradient stochastique, l'adam, l'adagrad, mini-batch, ect...
- Explorer de nouvelles méthodes de transformation de l'assembleur qui pourrait améliorer la prédiction.
- Explorer d'autres types de réseaux de neurones comme les réseaux de convolution, les réseaux récurrents, les auto-encodeurs, afin de mieux modéliser l'analyse du code et des dépendances entre les instructions.
- Porter le code sur GPU pour améliorer la performance et la parallélisation. Nous avons vu que Eigen proposait du calcul avec CUDA, donc peut-être que cela ne sera pas très long.

= Travaux similaires et autres recherches sur les réseaux de neurones et l'assembleur

Même s'il n'y a pas beaucoup de recherches sur l'utilisation des réseaux de neurones pour prédire la performance de code assembleur, ils ont déjà été utilisés pour d'autres tâches liées à l'assembleur comme celles-ci:
- Exploration of Convolutional Neural Network models for source code classification @cnn_code pour classer des codes sources selon sur quelle unité de calcul (CPU, GPU, FPGA, etc...) ils seraient le plus performants.
- Recognising Functions in Binaries with Neural Networks @binrecog pour reconnaître et analyser des fonctions dans du code assembleur.
- Asm2Vec @asm2vec pour reconnaître des fonctions sémantiquement similaires dans du code assembleur malgré les différentes optimisations vectorielles, de registres, etc...
- Machine Learning on Machines: Building a Model to Evaluate CPU Performance @ml_on_machines pour prédire la performance d'un code en fonction des caractéristiques de la machine.

= Conclusion

Malgré les problèmes rencontrés dû au manque de données, nous avons toutes les bases pour pouvoir mener à bien ce projet, et nous avons montré la viabilité de notre approche.
Toute la structure du code est en place, le réseau de neurones est fonctionnel, et nous avons tout préparé pour récolter des données.
#linebreak()
Ce premier semestre nous a permis de bien comprendre les bases des réseaux de neurones et du code assembleur vectorisé, et nous sommes prêts pour le mettre en pratique et le perfectionner pour le semestre prochain.

#bibliography("references.bib")

		