# osn2017-virtmem
Operating Systems and Networks - Virtual Memory Assignment (Tarea 2)

Algoritmos de reemplazo de pagina implementados:
- FIFO: conocido, saca de memoria la pagina que lleva mas tiempo cargada y la guarda en disco
- CUSTOM: debido a que los accesos a memoria son hechos directamente desde el programa y como programadores no podemos contarlos,
solo podemos monitorear los fallos generados por acceso a cada pagina. Es por esto que se decidió implementar un algoritmo que 
reemplaza la pagina que ha generado menos fallas a lo largo de la ejecucion del programa.
