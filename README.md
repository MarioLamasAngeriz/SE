

->DEADLINE: luns 6 outubro 2025
->Entregable: URL ao repositorio git co traballo. Idealmente, o mesmo repositorio que na práctica 1, pero nunha rama diferente (por ex. «Práctica 2»)
->É unha práctica individual

Crear un proxecto a partir dos exemplos «led_blinky» e «hello_world» que veñen nas 'demo-apps' da SDK (collede a versión da SDK que dá soporte á nosa placa) que ten que compilar e xerar os correspondentes binarios, led_blinky e hello_world, para executar na nosa placa, empregando a Toolchain GNU (GCC + OpenOCD + GDB), co seu correspondente Makefile.

O Makefile terá que proporcionar estas dúas regras, para construír e meter na flash da placa calquera dos dous binarios:
  - flash_led
  - flash_hello

A estrutura de directorios para a práctica podería ser a seguinte:

+ /practica2_SE
    - /includes
    - /drivers <- metede aquí todos os drivers e demais que precisedes, e que podedes coller de devices/MKL46Z4
    - hello_world.c
    - led_blinky.c
    - makefile    <- que constrúa e 'flashee' os binarios
    - startup.c   <- collede o de clase e engadide a desactivación do watchdog na rutina de reset()
    - openocd.cfg <- para facer o «flash» dos binarios na placa
    - link.ld     <- usade o script de enlazado que vimos empregando ata agora

Un exemplo de driver que é preciso para construír os dous binarios é utilities/fsl_assert.c

Valorarase positivamente non incluír drivers de máis (é dicir, que non se precisan neses dous proxectos)

