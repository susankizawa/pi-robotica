# Etapas do Projeto

## 1. Pesquisa e Estudo

* Estudo de robôs seguidores de linha e técnicas de desvio de obstáculos.
* Análise dos componentes eletrônicos e mecânicos: sensores IR e ultrassônicos, motores DC, pontes H, alimentação e estrutura do robô.
* Planejamento geral do projeto e levantamento de materiais necessários.

## 2. Projeto Físico

* Escolha de chassi, rodas e suportes.
* Planejamento da disposição de motores, sensores e alimentação.
* Preparação do robô para montagem do protótipo.

## 3. Testes Individuais

* Verificação do funcionamento de cada sensor separadamente.
* Testes dos motores com PWM e alimentação adequada.
* Ajustes mecânicos iniciais e identificação de problemas potenciais.

## 4. Protótipo Funcional

* Montagem do robô com Arduino, Motor Shield, alimentação, sensores IR e ultrassônico.
* Componentes fixos ou ajustáveis conforme necessidade de mudanças para testes futuros.
* Robô pronto para receber programação e iniciar testes de código.

## 5. Programação

* Implementação do código de seguidor de linha.
* Desenvolvimento da lógica de desvio de obstáculos.
* Criação da **máquina de estados** para integrar as duas lógicas.
* Ajustes de delays, leituras de sensores e controle de motores.

## 6. Ajustes e Melhorias

* Filtragem de leituras atípicas do ultrassônico (mediana de 3 leituras).
* Calibração dos sensores IR e ajustes de PWM dos motores.
* Soluções para problemas mecânicos, como a roda boba frontal afetar a direção do robô.

## 7. Testes Finais

* Avaliação de desempenho em trajetos com curvas e obstáculos.
* Validação da técnica de desvio em V.
* Registro de limitações e pontos de atenção.
* Documentação posterior do processo.
