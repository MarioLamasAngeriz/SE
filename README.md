# Práctica 4 - SE 24/25

Nesta práctica veremos como empregar algo de código ensamblador nos nosos proxectos. En xeral, isto faise cando é preciso optimizar algunha parte do código respecto ao código máquina que o compilador proporciona, pero no noso caso imos facelo para un caso concreto e sinxelo.

Proporciona unha implementación en ensamblador para unha función que rote todos os bits dun número enteiro que recibe como parámetro: reverse_int(). Ofrece unha implementación que *ti* penses que pode ser máis eficiente que a ofrecida polo compilador cando traduce este código C co nivel de optimización -Ofast:

```c
unsigned int reverse_int(unsigned int in)
{
  unsigned int out = 0;
  // Devolve o enteiro invertido bit a bit

  for (unsigned int i=0; i<32; i++) {
    out = out << 1;
    out |= in & 1;
    in = in >> 1;
  }

  return out;
}
```

Fai dúas implementacións desa función:

1. Función C con código ensamblador embebido no código C (inline ASM).

2. Función completa en ensamblador nun arquivo .s propio que haberá que ensamblar para obter o código obxecto que logo enlazar co resto.

Inclúe na entrega os arquivos .c e .s correspondentes, xunto a un arquivo de texto explicando a túa mellora sobre a versión ofrecida polo compilador. Non pasa nada se a túa mellora, ao final, non é máis eficiente, pero xustifica as túas decisións.

