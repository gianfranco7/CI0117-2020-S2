Elaborado por:
Gianfranco Bagnarello Hernández B70866

Ejecute el programa con diferentes valores de *n* (*n = 1*, *n = 1,000* y *n = 1,000,000*, ..., *n = 1,000,000,000*) y utilizando valores fijos de *a* y *b* en cada ejecución. Realice ejecuciones usando 1, 16, 32, 64 y 128 hilos.
Apunte la duración de cada ejecución y asegúrese de obtener las mismas áreas estimadas en cada prueba. Agregue los datos de la comparación al archivo .md de este ejercicio, por ej:
| n | area | 1 | 16  | 32 | 64 | 128 |
|---|---|---|---|---|---|---|
| 1  | 4.0  | 0.000078  | 0.000480  | 0.003427  | 0.005038  | 0.015369  |
| 10  | 9.88  | 0.000131  | 0.001431  | 0.003332  | 0.007303  | 0.011511  |
| 1000  | 10.658668  | 0.000044  | 0.001450  | 0.003654  | 0.006163  | 0.014836  |
| 1000000  | 10.666659  |  0.044129 | 0.021274  | 0.105758 | 0.108517  | 0.030636 |
| 100000000  | 10.666667  |  1.679289 | 0.370120 | 0.375199  | 0.367840  | 0.361289 |
| 1000000000  | 10.666667   | 16.542770 | 3.161907| 3.268884  | 3.501298  | 3.258907 |

Se obtuvieron resultados similares a los obtenidos en el laboratorio anterior, donde se paralelizó con pthreads.
