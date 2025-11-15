# Lógica de Desvio de Obstáculos - Detalhamento do Código

O modo **AVOIDING_OBSTACLE** é ativado quando o sensor ultrassônico frontal detecta um obstáculo dentro da distância segura (`obstacleDistance`). O objetivo é contornar o obstáculo e retomar a linha.

## Sequência de Operações

1. **Filtragem de distância**

```cpp
a = usF.read();
b = usF.read();      
c = usF.read();      

distance = (a + b + c) - max(max(a,b),c) - min(min(a,b),c);
```

* Três leituras do sensor frontal são feitas.
* Mediana simples elimina valores fantasmas.
* Apenas distâncias > 5 cm são consideradas válidas.

2. **Transição para desvio**

```cpp
if(distance > 5 && distance < obstacleDistance)
  currentState = AVOIDING_OBSTACLE;
```

3. **Movimentos do desvio**

```cpp
// Ré
motorR.run(BACKWARD);
motorL.run(BACKWARD);
delay(500);

// Giro inicial
motorR.run(FORWARD);
motorL.run(BACKWARD);
delay(750);

// Avanço paralelo
motorR.run(FORWARD);
motorL.run(FORWARD);
delay(1000);

// Giro de retorno
motorR.run(BACKWARD);
motorL.run(FORWARD);
delay(850);

// Avanço final curto
motorR.run(FORWARD);
motorL.run(FORWARD);
delay(250);
```

* **Ré (500 ms):** cria espaço antes de contornar o obstáculo.
* **Giro inicial (750 ms):** inicia o V para contornar o obstáculo.
* **Avanço paralelo (1000 ms):** segue adiante contornando o obstáculo.
* **Giro de retorno (850 ms):** gira na direção esperada da linha.
* **Avanço final curto (250 ms):** segue reto e retorna para seguir linha.

4. **Retorno ao FOLLOWING_LINE**

* Após o avanço final curto, a lógica de seguir linha reassume, permitindo a detecção da faixa assim que ela aparece.

## Observações

* Delays controlam os tempos de cada manobra.
* Filtragem do ultrassônico previne acionamentos falsos.
* A lógica de desvio é temporária; após o V, o robô retoma seguir linha normalmente.
