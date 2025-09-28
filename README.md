

Partindo do esquelo de código proporcionado en https://gitlab.com/emiliojpg/sistemasembebidos, implementa na FRDM-KL46Z4 un autómata simple que, mediante o uso dos dous botóns principais da placa, acenda/apague os dous LEDs (a placa iniciaríase cos dous LEDs apagados) deste xeito:

    botón dereito: cada vez que se preme conmuta entre os seguintes estados (sempre nesta orde): LED vermello apagado e verde acendido,  os dous LEDs acendidos, LED vermello acendido e verde apagado, os dous LEDs apagados.
    botón esquerdo: invirte o estado actual dos LEDs. Vaia, que se un está apagado e o outro acendido, invírtense. Se os dous están apagado pasan a estar acendidos; e se están acendidos ambos, apáganse. A secuencia de estados previamente definida debe manterse tras o cambio.

É parte do exercicio crear un Makefile para construír o binario executable, que dispoña dunha regra para «flashealo» no dispositivo (make flash). Engade tamén unha regra 'clean', para eliminar os arquivos obxecto (.o) e unha 'cleanall' que elimine tamén o binario final.

O Makefile proporcionado non pode conter regras explícitas de compilación, ten que aproveitar as regras implícitas. Si pode ter unha única regra explícita de enlazado («linkado»). Ademais, o Makefile debe empregar variables implícitas, variables automáticas e variables propias.
Entrega

O entregable será unha URL a un repositorio git remoto (dá igual o servizo: gitlab, github ou en calquera outro servizo git remoto propio ou alleo). Nese repositorio teredes que ter unha rama que se identifique claramente como Práctica 1. Mantede o repositorio remoto como privado, invitándome a min (o profesor) para que o poida clonar.

Ide facendo commits según avanzades, non fagades un único commit con todo ao final. O repositorio debe reflectir o desenvolvemento da práctica.

Nota final: por favor, non metades binarios froito da compilación baixo control de versións!

