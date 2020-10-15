void *fight(void *args)
{
	// detener hilos (barrier)
	// Order
	// -> Sem t
	// -> Mutex / Variable de condición -> Envia señal al siguiente
	// -> Array de mutex / array de sem
	//obtener la informacion del ataque (power, el cooldown, energy, energy gain, tipo)

	//mutex lock del mio
	while (tenga vida / tenga puntos de vida)
	{
		// oponente cambie

		//atacar cargado 
		if (se puede hacer ataque cargado)
		{
			// metodo sinc. para asegurarse de que mientras se haga un ataque cargado el oponente se detenga
			// sleep (animation)

			damage = calcular daño(power, effective, bonus)
						 oponente.hp -= damage
				energia acumulada -= energia
					sleep(cooldown)
		}
		else
		 atacar rápido {
			// sleep(time animation)

			damage = calcular daño(power, effective, bonus)
						 oponente.hp -= damage
				energia acumulada += gane de energia
					sleep(cooldown)
		}
	}
	//mutex unlock(siguiente)
}