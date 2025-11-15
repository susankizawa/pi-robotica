# 🤖 Robô Seguidor de Linha com Desvio de Obstáculos

## 📘 Visão Geral

Este projeto consiste em um robô autônomo capaz de **seguir uma linha** no chão usando sensores infravermelhos (IR) e **desviar de obstáculos** usando um sensor ultrassônico frontal. O foco do desenvolvimento foi alcançar um comportamento estável e confiável em situações reais, simplificando a lógica sempre que necessário.

## ⚙️ Hardware Utilizado

| Componente                 | Descrição                                                        |
| -------------------------- | ---------------------------------------------------------------- |
| Arduino UNO                | Microcontrolador principal                                       |
| Motor Shield Ponte H       | Controle dos motores (Adafruit v1)                               |
| Motores DC                 | 2 unidades                                                       |
| Sensores IR                | 3 unidades para seguir linha                                     |
| Sensores ultrassônicos     | 1 frontal + 2 laterais (laterais não utilizados na versão final) |
| Roda boba frontal          | Roda tipo carrinho de supermercado                               |
| Alimentação Arduino        | 2× pilhas 18650                                                  |
| Alimentação Motores        | 4× pilhas AA Duracell 1.5V                                       |
| Estrutura                  | Rodas, chassi, suporte e estrutura do robô                       |

## 🧠 Visão Geral da Lógica do Robô

O robô atua com uma **máquina de estados simples**, organizada em dois modos principais:

### **1. FOLLOWING_LINE**

Neste estado, o robô:

* Lê continuamente os três sensores IR.
* Mantém respostas rápidas (sem atrasos) para não perder a linha.
* Executa **curvas suaves** quando percebe que está levemente descentralizado.
* Executa **correções mais fortes** quando a linha é detectada apenas pelos sensores externos ou está quase sendo perdida.
* Permanece nesse estado enquanto o sensor ultrassônico **frontal** não detectar um obstáculo.

### **2. AVOIDING_OBSTACLE**

Ativado quando o ultrassônico frontal confirma um obstáculo dentro da distância segura.

Neste estado, o robô:

* Interrompe temporariamente o seguimento de linha.
* Executa um desvio curto em formato de **V**: primeiro se afasta da linha, depois inicia o retorno.
* No ponto de retorno, gira na direção esperada da linha e avança por **250 ms** com comandos diretos.
* Após isso, a lógica de seguir linha reassume e o robô recupera o trajeto naturalmente quando os sensores voltam a detectar a faixa.

## 🌿 Branches

* **desvio-de-obstaculos** – controle do carrinho através de sensores ultrassônicos.
* **desvio-de-obstaculos-por-seguir-parede** – tentativa experimental de seguir parede para desviar de obstáculos, comportamento instável.

## 📦 Dependências

* [Adafruit Motor Shield Library v1](https://github.com/adafruit/Adafruit-Motor-Shield-library)
* [Ultrasonic Library](https://github.com/ErickSimoes/Ultrasonic)

## 📚 Etapas do Projeto

1. Pesquisa e estudo
2. Projeto físico e montagem
3. Testes individuais de sensores e motores
4. Protótipo funcional
5. Programação do seguidor de linha
6. Programação do desvio de obstáculos
7. Integração e ajustes
8. Testes finais

## 💻 Tecnologias e Ferramentas

* [Arduino IDE](https://www.arduino.cc/en/software)
* C/C++

## 👥 Equipe

* Guilherme Mafioletti Quintana
* Jonathan Almeida de Camargo
* Leonardo Augusto Sanzovo
* Susan Kaori Izawa
* Tiago Unlauf Boing Nack

## 📝 Lições Aprendidas

* A estabilidade da alimentação impacta diretamente o comportamento do robô. A combinação final (Arduino com 18650 e motores com AA) foi a mais confiável.
* Sensores ultrassônicos geram leituras fantasmas; a mediana de três medições resolveu o problema.
* A roda boba frontal introduz variações indesejadas durante o desvio, dificultando movimentos precisos.
* Estratégias de desvio complexas (hexágono, seguir parede) criaram muitas variáveis difíceis de controlar. O desvio em V foi o mais consistente.
* A integração entre seguir linha e desviar obstáculos exigiu minimizar delays para que IR e ultrassônico respondessem corretamente.

## 📄 Documentação Complementar (WIP)

Arquivos sugeridos:

* `docs/logica-seguidor.md`
* `docs/logica-desvio.md`
* `docs/maquina-de-estados.md`
* `docs/notas-de-engenharia.md`

(Conteúdo a ser preenchido conforme avançarmos.)
