API simple de communication UDP en C++
En reception, l'api est bloquante, avec timer reglable

fichiers sources :
------------------
* soft/udpapi.h et .c => fichiers sources de l'api (classe C++)
* soft/samplexxx.c => exemples d'utilisation client et serveur

lignes de compil (sortie dans lib/):
------------------
g++ -I"./soft" -o lib/sampleserver.x soft/sampleserver.cpp soft/udpapi.cpp
g++ -I"./soft" -o lib/sampleclient.x soft/sampleclient.cpp soft/udpapi.cpp

exemple d'utilisation :
-----------------------
lib/sampleserver.x 8889					(local_port)
lib/sampleclient.x 8888 127.0.0.1 8889 coucou 2000	(local_port  dest_ip  dest_port  message timeout)
