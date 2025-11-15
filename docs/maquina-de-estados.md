# Máquina de Estados

O robô opera com uma **máquina de estados simples**, responsável por alternar entre os dois comportamentos principais: seguir a linha e desviar de obstáculos. Essa abordagem organiza o fluxo do programa e permite controlar cada lógica de maneira independente.

## Estados

### **1. FOLLOWING_LINE**

* Lê continuamente os três sensores IR.
* Mantém a trajetória na linha usando curvas suaves ou correções mais intensas.
* Permanece neste estado enquanto **nenhum obstáculo válido** é detectado pelo sensor ultrassônico frontal.

### **2. AVOIDING_OBSTACLE**

* Ativado quando o ultrassônico frontal detecta um obstáculo dentro da distância segura.
* Executa a sequência de desvio em formato de V.
* Retorna ao estado FOLLOWING_LINE após o avanço final curto.

## Transições

| Estado Atual      | Condição de Transição                                | Próximo Estado    |
| ----------------- | ---------------------------------------------------- | ----------------- |
| FOLLOWING_LINE    | Sensor ultrassônico frontal detecta obstáculo válido | AVOIDING_OBSTACLE |
| AVOIDING_OBSTACLE | Sequência completa de desvio (inclui avanço final)   | FOLLOWING_LINE    |

## Observações

* Leituras menores que 5 cm são ignoradas para evitar valores fantasmas do ultrassônico.
* O avanço final curto após o desvio permite que a lógica de seguir linha volte rapidamente a atuar, garantindo que os sensores IR detectem a faixa assim que ela reaparecer.
* A máquina de estados foi escolhida por ser a abordagem mais adequada quando o robô precisa alternar entre duas lógicas distintas (seguir linha e desviar obstáculos). Ela organiza o fluxo do programa e evita misturar comportamentos no `loop()`, facilitando manutenção e expansão futura.
