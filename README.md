
Traballo Tutelado I: Benchmarking} de implementacións de reverse_int()

    Implementa catro das versións mostradas de reverse_int():
        reverse1.c A versión en C mostrada na diapositiva 2
        reverse2.s A versión en ASM máis optimizada da diapositiva 5
        reverse3.s A versión ASM sen bucle da diapositiva 7
        reverse4.c A versión sen bucle en C (diapositiva 8)
    Mide os ciclos de execución de cada unha delas usando un temporizador que non sexa o SYSTICK
        Podes usar unha aproximación deste estilo:

              uint32_t start = timestamp();

              reverse1(INPUT);

              uint32_t end = timestamp();
              uint32_t elapsed_ticks = start - end;

    Para as versións en C, obtén resultados de execución compilando sen optimizacións, -O0, e co máximo nivel de
        optimización, -Ofast.


