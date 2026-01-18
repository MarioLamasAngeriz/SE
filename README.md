Traballo tutelado 2. PWM + LED + Sensor

Traballo individual

Prazo: 19 de xaneiro, 2026

Programade unha aplicación para a placa FRDM-KL46Z que faga uso de PWM para acender os LEDs con maior ou menor intensidade, en función do valor dun sensor da placa.

    Magnetómetro + acelerómetro: Empregando eses dous sensores calcularemos a nosa orientación ao norte en graos:
        0° → Norte
        90° → Leste
        180° → Sur
        270° → Oeste

    Por un lado, mostraremos esa información (os graos) no LCD da placa, e por outro lado faremos que o acendido e intensidade dos LEDs dependa da nosa orientación, deste xeito: orientados a norte, o LED verde estará acendido coa máxima intensidade, e o LED vermello estará apagado. Orientados a sur, será xusto ao revés. Cando a orientación vai mudando de norte a sur, o LED verde vai diminuíndo a súa intensidade e o vermello incrementándoa, e á inversa ao pasar de sur a norte. No Leste e no Oeste, os dous LEDs están acendidos cunha intensidade media. O magnetómetro da nosa placa (MAG3110) está conectado por I2C, igual que o acelerómetro (ver slides09.pdf)

