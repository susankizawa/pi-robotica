# 🚀 Possíveis Melhorias Futuras do Projeto

Esta seção reúne ideias para quem quiser continuar o desenvolvimento do robô. Elas não são obrigatórias nem necessariamente simples — funcionam como um ponto de partida para novas versões, experimentos ou otimizações.

## 🔋 Alimentação e Drivers

* **Melhorar a fonte de alimentação**: usar packs de boa qualidade que forneçam tensão e corrente mais estáveis ou pilhas 18650 de melhor qualidade.
* **Testar outros drivers de motor**: alguém pode investigar se drivers diferentes se comportam melhor para este conjunto de motores.

## ⚙️ Mecânica

* **Reposicionar ou trocar a roda boba**: colocar atrás ou substituir por uma bola deslizante reduz interferências na direção.
* **Montar o chassi já considerando duas fontes de alimentação**: o robô depende de duas fontes separadas, então vale projetar a estrutura já levando isso em conta.

## 🧰 Organização Elétrica

* **Distribuição de energia usando mini protoboards**: útil quando faltam pinos de alimentação no Arduino ou no motor shield, especialmente ao alimentar vários sensores.

## 🧭 Sensores e Navegação

* **Adicionar odometria ou sensores de movimento (encoders/IMU)**: ajudaria a tornar movimentos fixos mais previsíveis e melhoraria o desvio de obstáculos.
* **Melhorar o desvio de obstáculos**: incluir sensores laterais funcionais, detectar rotas bloqueadas ou tentar seguir paredes.
* **Adicionar mais sensores IR**: permite correções mais precisas e suaves durante o seguimento da linha.

## 🧠 Controle e Código

* **Experimentar controle proporcional ou PID**: pode melhorar suavidade e estabilidade, dependendo dos testes.
* **Refatorar o código**: modularizar e organizar melhor para facilitar evoluções.
* **Adaptar o robô para operar em velocidades maiores**: exige ajustes na lógica e talvez sensores adicionais, já que alta velocidade aumenta a chance de derrapagens e perda da linha — mas seria divertido ver alguém tentar! 🚀
