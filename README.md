# INF295-2024-2
Proyecto para el ramo de Inteligencia Artificial, implementacion de un Algoritmo Genetico para resolver el problema OPHS

## Instrucciones de ejecucion
Instrucciones base de compilacion y ejecucion:
```
make
make exe
```
Los valores para la instancia a leer, como los parametros del algoritmo vienen previamente definidos, pero es posible modificarlos, a continuacion se presenta un ejemplo de como estos pueden ser modificados:
```
make exe instance=instance.txt results=results.txt cr=0.9 mr=0.1 ps=1e+1 max_iter=1e+5 seed=123 debug=true
```
Para limpiar cualquier archivo remanente de la compilacion y/o ejecucion del programa utilizar el comando:
```
make clean
```
