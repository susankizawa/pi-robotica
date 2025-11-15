# Lições Aprendidas

## Alimentação

* A alimentação impacta diretamente a estabilidade do robô.
* A escolha das **pilhas 18650 para o Arduino** foi baseada em recomendações de que esse tipo de pilha suporta alimentar robôs; em teoria, elas deveriam ser suficientes.
* No entanto, a unidade específica que utilizamos pode ter sido de qualidade inferior, fornecendo corrente de forma menos consistente.
* A combinação final que funcionou melhor: **Arduino com 2 pilhas 18650** e **motores com 4 pilhas AA Duracell 1.5V**.
* Pilhas 18650 sozinhas não suportavam bem motores em valores altos de PWM; pilhas AA ofereciam estabilidade, porém menor força.

## Sensores

* Sensores ultrassônicos podem gerar **valores atípicos** ou leituras fantasmas.
* Filtragem via **mediana de 3 leituras** ajudou a reduzir erros.
* Delays na lógica (código) de seguir linha prejudicam o acompanhamento da linha pelos sensores IR; é importante manter a leitura e resposta rápida.

## Mecânica

* A roda boba frontal introduz variações indesejadas durante desvio de obstáculos, afetando a **direção do robô**.
* Solução adotada: fixar a roda boba com fita isolante para reduzir instabilidade.
* Essa solução gerou mais atrito, aumentando o torque necessário dos motores para mudar a direção, portanto não é ideal do ponto de vista mecânico.
* Ajustes no chassi e na disposição de componentes foram importantes para estabilidade geral.

## Lógica de Controle

* Estratégias mais complexas de desvio se tornaram inviáveis devido à combinação específica de alimentação com pouca tensão e corrente, motores relativamente fracos e roda boba afetando a direção do robô.
* A tentativa de implementar desvio seguindo parede falhou por limitações de tempo e dificuldade de integração com a lógica de seguir linha.
* A técnica de **desvio em V/triângulo** foi escolhida por ser simples, consistente e a melhor opção para nossa situação.
* Integração entre seguidor de linha e desvio exigiu minimizar delays e filtrar leituras para evitar comportamento instável.

## Observações Gerais

* Documentar problemas e soluções ajuda futuras melhorias.
* Algumas decisões (como não usar sensores ultrassônicos laterais) foram tomadas por limitações de tempo, física e comportamento do robô.
* Testes finais permitiram consolidar técnicas mais estáveis e confiáveis.
