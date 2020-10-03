| n | area | serial | 1  | 16 | 32 | 64 |
|---|---|---|---|---|---|---|
| 1  | 4 | 0.000384 | 0.000479  | 0.000614  | 0.000443 | 0.000094 |
| 10  | 9.880000  |  0.000267 | 0.000699  | 0.002039 | 0.001795 | 0.002113  |
| 1000  | 10.658668 | 0.000320 |  0.039299 | 0.018907 | 0.000767  | 0.000094  |
| 1,000,000 | 10.666659  | 0.020366 |  0.019056 | 0.016053  | 0.006618 |  0.013558 |
| 1,000,000,000 | 10.666667  | 12.448663 | 11.436838  |  4.335590 | 4.055215 | 3.892265 |

Podemos concluir que la diferencia entre el rendimiento del programa serial y el concurrente no es mucha cuando se trata de _pocos_ rectángulos (>1,000,000)
pero al tratar con cantidades bastante grandes, como un billón, se nota bastante la diferencia en el tiempo de ejecución, siendo la velocidad del programa concurrente el doble de eficiente que el programa serial.

##Elaborado por:
##-Gianfranco Bagnarello Hernández B70866
##-Katherine González Arias B22867
