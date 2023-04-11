# web_serv

Créer un socket d'écoute pour accepter les connexions entrantes.

Accepter les connexions entrantes et créer des sockets pour chacune d'entre elles.

Lire et traiter les commandes entrantes de chaque client.

Gérer les commandes IRC spécifiques, telles que JOIN, PART, PRIVMSG, etc.

Transmettre les messages à tous les clients dans un canal lorsqu'un client envoie un message.

Implémenter des fonctions de gestion des utilisateurs, telles que l'identification, l'authentification, la définition d'un surnom, etc.

Implémenter des fonctions de gestion des canaux, telles que la création et la suppression de canaux, l'ajout et la suppression de membres de canal, etc.

Gérer les déconnexions de clients.

Gérer les erreurs et les exceptions.

----------------------------------------------------------------------------------------------------------------------------------------
## Requête

Créer une socket serveur :

Utilisez la fonction socket() pour créer une socket TCP/IP IPv4 ou IPv6, en fonction de vos besoins.
Utilisez la fonction setsockopt() pour définir l'option SO_REUSEADDR sur la socket, afin de réutiliser rapidement le port après la fermeture du serveur.
Utilisez la fonction bind() pour lier la socket à une adresse IP et au port spécifié en argument.
Utilisez la fonction listen() pour mettre la socket en mode d'écoute des connexions entrantes.
Accepter les connexions entrantes :

Utilisez la fonction accept() pour accepter une connexion entrante.
Cette fonction retourne une nouvelle socket qui sera utilisée pour la communication avec le client.
Lire les données envoyées par le client :

Utilisez la fonction recv() pour lire les données envoyées par le client sur la socket de communication.
Envoyer des données au client :

Utilisez la fonction send() pour envoyer des données au client sur la socket de communication.
Gérer la fin de la communication avec le client :

Fermez la socket de communication à l'aide de la fonction close().


----------------------------------------------------------------------------------------------------------------------------------------
## Fonction utile

- **execve()** : exécute un programme avec un ensemble de paramètres et un environnement définis.
Cette fonction est utile pour exécuter un programme externe à partir d'un programme en cours d'exécution.

- **dup()** et **dup2()** : dupliquent un descripteur de fichier existant.
dup() crée une copie d'un descripteur de fichier existant, en utilisant le plus petit numéro de descripteur disponible.
dup2() fait la même chose, mais permet à l'utilisateur de spécifier le numéro de descripteur de fichier à utiliser.

- **pipe()** : crée une paire de descripteurs de fichiers pour une communication unidirectionnelle entre processus.
Les données écrites dans un descripteur de fichier peuvent être lues depuis l'autre.

- **strerror()** et **gai_strerror()** : renvoient une chaîne de caractères décrivant une erreur système ou réseau.
strerror() prend en argument un code d'erreur et renvoie une chaîne de caractères décrivant l'erreur correspondante.
gai_strerror() prend en argument un code d'erreur d'adressage et renvoie une chaîne de caractères décrivant l'erreur correspondante.

- **errno** : variable globale qui contient le dernier code d'erreur de la dernière opération système.
Cette variable peut être utilisée pour récupérer le code d'erreur renvoyé par une fonction système.

- **fork()** : crée un nouveau processus en dupliquant le processus existant.
Le nouveau processus est identique au processus parent, à l'exception de son identifiant de processus (PID) et de son identifiant de groupe de processus (PGID).
Le processus parent et le nouveau processus continuent l'exécution à partir du point d'appel de la fonction fork().

- **htonl()**, **htons()**, **ntohl()** et **ntohs()** : convertissent des entiers entre l'ordre des octets d'hôte et l'ordre des octets du réseau.
Ces fonctions sont nécessaires pour convertir les entiers lors de la communication via le réseau.

- **select()**, **poll()** et **epoll()** : sont des mécanismes de gestion d'événements pour les sockets.
Ces fonctions sont utilisées pour surveiller plusieurs sockets en même temps, en attendant qu'un ou plusieurs événements se produisent.

- **socket()** : crée un nouveau socket.
Les sockets sont des canaux de communication bidirectionnels utilisés pour la communication entre processus sur un réseau.

- **accept()** : accepte une nouvelle connexion sur un socket.
Lorsqu'un client se connecte à un serveur, le serveur appelle la fonction accept() pour accepter la connexion.

- **listen()** : met un socket en mode écoute des connexions entrantes.
Cette fonction est appelée sur un socket côté serveur pour spécifier le nombre maximal de connexions en attente de traitement.

- **send()** et **recv()** : envoient et reçoivent des données sur un socket.
Ces fonctions sont utilisées pour envoyer et recevoir des données sur un socket.

- **bind()** : attache un socket à une adresse IP et à un port.
Cette fonction est utilisée pour spécifier l'adresse IP et le port sur lequel le socket doit écouter les conn

----------------------------------------------------------------------------------------------------------------------------------------
## Cree un socket



## Lien utile



----------------------------------------------------------------------------------------------------------------------------------------

## NORME RFC

