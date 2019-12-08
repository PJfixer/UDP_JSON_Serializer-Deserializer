
## Fichiers sources :
* soft/udpapi.h et .c => fichiers sources de l'api (classe C++)
* soft/samplexxx.c => exemples d'utilisation client et serveur

## Compiler (sortie dans lib/):
```bash
make
```

## Exemple d'utilisation :
```bash
# lib/sampleserver.x local_port
lib/sampleserver.x 8889
# lib/sampleclient.x local_port  dest_ip  dest_port  message timeout
lib/sampleclient.x 8888 127.0.0.1 8889 coucou 2000

