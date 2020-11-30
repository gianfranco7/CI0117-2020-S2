# jaccard_omp
# Gianfranco Bagnarello Hernández B70866

- Responda lo siguiente (5 pts):

- Explique cada una de las directivas de OpenMP que utilizó y cómo funcionan dentro del programa

```sh
#pragma omp parallel for 
```
Lo anterior especifica que es un ciclo _for_ y que debe dividir sus iteraciones en hilos para que corran en paralelo.

```sh
default(none) private(i) shared(matrix_row,test_vector) 
```
El uso de default(none) obliga a especificar las variables que serán utilizadas como memoria compartida, en este caso serán matrix_row que tiene que ser compartida para que cada hilo en el for pueda comparar si ya llegó a su última iteración o no. Además debe usar esa variable para hacer las operaciones lógicas en los ifs del ciclo. El i es privado porque cada hilo trabaja con su propia iteración i y no deben tener el mismo varos hilos.

```sh
reduction(+: words_union, words_intersection) \
        schedule(static,1)
```
Se utiliza un reduction para que se divida entre los hilos y el schedule es estático y de forma que le toque una iteración a cada hilo. Se usa una reducción en words_union y words_intersection ya que son los datos que están editando en el ciclo con una suma.

- Explique de forma general cómo se está dando la repartición de tareas entre hilos en su solución

Al usar el schedule de forma estática, la división de tareas es de forma que se asigna antes de la ejecución del programa que a cada hilo le toca una iteración.

Además, para garantizar que no haya condiciones de carrera, se pusieron mutexes con 

```sh
#pragma omp critical(words_intersection)
```

de manera que varios hilos no compitan para editar la memoria al mismo tiempo.