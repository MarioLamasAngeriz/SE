# Traballo Tutelado I: Benchmarking de implementacións de reverse_int()

## Implementa catro das versións mostradas de reverse_int()

- **reverse1.c** - A versión en C mostrada na diapositiva 2
- **reverse2.s** - A versión en ASM máis optimizada da diapositiva 5
- **reverse3.s** - A versión ASM sen bucle da diapositiva 7
- **reverse4.c** - A versión sen bucle en C (diapositiva 8)

Mide os ciclos de execución de cada unha delas usando un temporizador que non sexa o SYSTICK.

Podes usar unha aproximación deste estilo:

```c
uint32_t start = timestamp();
reverse1(INPUT);
uint32_t end = timestamp();
uint32_t elapsed_ticks = start - end;
```

Para as versións en C, obtén resultados de execución compilando sen optimizacións, `-O0`, e co máximo nivel de optimización, `-Ofast`.

---

## Ejecución de reverse1.c (O0)

```
Introduce un número enteiro, max 9 dixitos
1234
Número a invertir bit a bit: 1234
Número en binario: 0b00000000 00000000 00000100 11010010 
Número invertido bit a bit: 1260388352
Número en binario: 0b01001011 00100000 00000000 00000000 
Número de ciclos de CPU para a execución de la función de inversion de bits = 998
```

---

## Ejecución de reverse1.c (Ofast)

```
Introduce un número enteiro, max 9 dixitos
1234
Número a invertir bit a bit: 1234
Número en binario: 0b00000000 00000000 00000100 11010010 
Número invertido bit a bit: 1260388352
Número en binario: 0b01001011 00100000 00000000 00000000 
Número de ciclos de CPU para a execución de la función de inversion de bits = 296
```

---

## Ejecución de reverse2.s (Ofast)

```
Introduce un número enteiro, max 9 dixitos
1234
Número a invertir bit a bit: 1234
Número en binario: 0b00000000 00000000 00000100 11010010 
Número invertido bit a bit: 1260388352
Número en binario: 0b01001011 00100000 00000000 00000000 
Número de ciclos de CPU para a execución de la función de inversion de bits = 212
```

---

## Ejecución de reverse3.s (Ofast)

```
Introduce un número enteiro, max 9 dixitos
1234
Número a invertir bit a bit: 1234
Número en binario: 0b00000000 00000000 00000100 11010010 
Número invertido bit a bit: 1260388352
Número en binario: 0b01001011 00100000 00000000 00000000 
Número de ciclos de CPU para a execución de la función de inversion de bits = 54
```

---

## Ejecución de reverse4.c (O0)

```
Introduce un número enteiro, max 9 dixitos
1234
Número a invertir bit a bit: 1234
Número en binario: 0b00000000 00000000 00000100 11010010 
Número invertido bit a bit: 1260388352
Número en binario: 0b01001011 00100000 00000000 00000000 
Número de ciclos de CPU para a execución de la función de inversion de bits = 120
```

---

## Ejecución de reverse4.c (Ofast)

```
Introduce un número enteiro, max 9 dixitos
1234
Número a invertir bit a bit: 1234
Número en binario: 0b00000000 00000000 00000100 11010010 
Número invertido bit a bit: 1260388352
Número en binario: 0b01001011 00100000 00000000 00000000 
Número de ciclos de CPU para a execución de la función de inversion de bits = 8
```
