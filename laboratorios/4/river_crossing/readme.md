Laboratorio 4
Elaborado por:
Gianfranco Bagnarello Hernández B70866
Katherine González Arias B22867

El problema river crossing o de cruzar el río se refiere a que es necesario que dos grupos distintos de entidades cruzen el río: la entidad tipo serf y la entidad tipo hacker. Para poder cruzar, usan un barco, en el que caben 4 de distintos tipos, pero solo ciertas combinaciones son permitidas. Pueden ser solo 4 serfs, o 4 hackers, o 2 serfs y 2 hackers en el mismo. Se establece que si se presentara el caso de que hay 3 de un tipo y 1 del otro, no se puede enviar el barco, porque es una combinación illegal. 

Se puede presentar el problema de starvation, debido a que si se generan por ejemplo 3 hilos hacker y 1 serf, el serf no va a poder montarse en el barco. Si se generan 4 hackers después de eso, y se siguen generando, y al tener la combinación de 3/1 illegal, el hilo serf va a quedarse esperando a que lleguen más serfs que puede que nunca lleguen; el programa va a seguir funcionando, pero ese hilo va a quedarse esperando solo mientras los otros se ejecutan.



