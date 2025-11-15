# Lógica do Seguidor de Linha - Detalhamento do Código

O modo **FOLLOWING_LINE** utiliza três sensores IR (`irL`, `irC`, `irR`) para determinar a posição da linha e ajustar a movimentação dos motores.

## Sequência de Operações do Código

1. **Leitura dos sensores**

```cpp
bool right = digitalRead(irR);
bool center = digitalRead(irC);
bool left = digitalRead(irL);
```

* `center` detecta a linha central.
* `left` detecta a linha à esquerda.
* `right` detecta a linha à direita.

2. **Linha no centro**

```cpp
if(center && !right && !left)
```

* A linha está centralizada.
* Ambos motores avançam em mesma velocidade (`speed`).
* O robô segue reto.

3. **Linha levemente à esquerda**

```cpp
else if(center && left && !right)
```

* Curva suave para a esquerda.
* Motor esquerdo reduz velocidade (`speed * 0.6`), motor direito mantém `speed`.
* O robô corrige a posição levemente.

4. **Linha levemente à direita**

```cpp
else if(center && right && !left)
```

* Curva suave para a direita.
* Motor direito reduz velocidade (`speed * 0.6`), motor esquerdo mantém `speed`.
* Correção leve para centralizar.

5. **Linha totalmente à esquerda**

```cpp
else if(left && !center && !right)
```

* Curva extrema: gira no lugar para a esquerda.
* Motor direito avança, motor esquerdo vai para trás com `turningSpeed`.

6. **Linha totalmente à direita**

```cpp
else if(right && !center && !left)
```

* Curva extrema: gira no lugar para a direita.
* Motor esquerdo avança, motor direito vai para trás com `turningSpeed`.

7. **Todos os sensores detectam linha**

```cpp
else if(right && center && left)
```

* Possível cruzamento ou linha muito larga.
* Ambos motores param (`RELEASE`).

## Observações de Código

* Delays na função prejudicam o seguimento da linha, pois atrasam a atualização das leituras IR.
* Ajustes de velocidade são usados para suavizar curvas ou fazer giros precisos.
* A lógica é chamada continuamente em loop, garantindo respostas rápidas às mudanças da linha.
