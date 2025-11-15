# 🤖 Robô Seguidor de Linha com Desvio de Obstáculos

## 📘 Visão Geral

Este projeto consiste em um robô autônomo capaz de **seguir uma linha** no chão usando sensores infravermelhos (IR) e **desviar de obstáculos** usando um sensor ultrassônico frontal. O foco do desenvolvimento foi alcançar um comportamento estável e confiável em situações reais, simplificando a lógica sempre que necessário.

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

1. Interrompe temporariamente o seguimento de linha.
2. Executa um desvio curto em formato de **V**: primeiro se afasta da linha, depois inicia o retorno.
3. No ponto de retorno, gira na direção esperada da linha e avança por **250 ms** com comandos diretos.
4. **Retorno ao FOLLOWING_LINE**: após o avanço final curto, a lógica de seguir linha reassume, permitindo a detecção da faixa assim que ela aparece.

Para mais detalhes sobre a lógica do robô:

* [`logica-seguidor.md`](docs/logica-seguidor.md)
* [`logica-desvio.md`](docs/logica-desvio.md)
* [`maquina-de-estados.md`](docs/maquina-de-estados.md)
* [`melhorias.md`](docs/melhorias.md)

## ⚙️ Hardware Utilizado

| Componente             | Descrição                                                        |
| ---------------------- | ---------------------------------------------------------------- |
| Arduino UNO            | Microcontrolador principal                                       |
| Motor Shield Ponte H   | Controle dos motores (Adafruit v1)                               |
| Motores DC             | 2 unidades                                                       |
| Sensores IR            | 3 unidades para seguir linha                                     |
| Sensores ultrassônicos | 1 frontal + 2 laterais (laterais não utilizados na versão final) |
| Roda boba frontal      | Roda tipo carrinho de supermercado                               |
| Alimentação Arduino    | 2× pilhas 18650                                                  |
| Alimentação Motores    | 4× pilhas AA Duracell 1.5V                                       |
| Estrutura              | Rodas, chassi, suporte e estrutura do robô                       |

## 🌿 Branches

* **desvio-de-obstaculos** – controle do carrinho através de sensores ultrassônicos.
* **desvio-de-obstaculos-por-seguir-parede** – tentativa experimental de seguir parede para desviar de obstáculos, comportamento instável.

## 📦 Dependências

* [Adafruit Motor Shield Library v1](https://github.com/adafruit/Adafruit-Motor-Shield-library)
* [Ultrasonic Library](https://github.com/ErickSimoes/Ultrasonic)

## 💻 Tecnologias e Ferramentas

* [Arduino IDE](https://www.arduino.cc/en/software)
* C/C++

## 📄 Documentação Complementar

* [Etapas do Projeto](docs/etapas.md) – detalhes de cada etapa do desenvolvimento.
* [Lições Aprendidas](docs/licoes-aprendidas.md) – problemas, soluções e aprendizados do projeto.
* [Lógica do Seguidor de Linha](docs/logica-seguidor.md) – funcionamento detalhado do seguidor de linha.
* [Lógica de Desvio de Obstáculos](docs/logica-desvio.md) – funcionamento detalhado do desvio de obstáculos.
* [Máquina de Estados](docs/maquina-de-estados.md) – estrutura e transições entre estados.
* [Possíveis Melhorias Futuras](docs/melhorias.md) – ideias para evolução e otimização do robô.

## 👥 Equipe

* Guilherme Mafioletti Quintana
* Jonathan Almeida de Camargo
* Leonardo Augusto Sanzovo
* Susan Kaori Izawa
* Tiago Unlauf Boing Nack
