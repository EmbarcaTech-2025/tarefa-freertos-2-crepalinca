# Relatório de Análise: Sistema Multitarefa com FreeRTOS

**Autor:** Antonio Crepaldi
**Projeto:** embarcatech-freertos-tarefa-1
**Data:** 14 de Junho de 2025

---

## 1. O que acontece se todas as tarefas tiverem a mesma prioridade?

Se todas as tarefas compartilham a mesma prioridade no FreeRTOS, o escalonador adota a estratégia de **Round-Robin com Divisão de Tempo (Time Slicing)**. Nesse cenário, a CPU distribui o tempo de execução em pequenos intervalos, alternando entre as tarefas que estão prontas para serem executadas. Isso assegura um equilíbrio no uso do processador, evitando que uma única tarefa domine o sistema e dando a impressão de que todas estão rodando ao mesmo tempo.

---

## 2. Qual tarefa consome mais tempo da CPU?

A tarefa que mais utiliza o tempo da CPU é a **`button_task`**. Isso ocorre devido à sua **frequência de ativação mais alta**. Diferentemente das tarefas relacionadas ao LED e ao buzzer, que são ativadas apenas 2 vezes por segundo, a `button_task` é executada **10 vezes por segundo** para monitorar os botões por meio de verificação contínua (*polling*). Essa repetição constante e as trocas de contexto frequentes fazem com que ela acumule um maior consumo de recursos do processador.

---

## 3. Quais seriam os riscos de usar polling?

O método de *polling* (monitoramento periódico), embora prático, traz alguns perigos significativos em sistemas embarcados. Vou destacar três pontos críticos:

Primeiro, há um **desperdício de CPU e energia**, que é o maior problema. A tarefa é ativada repetidamente, consumindo recursos mesmo quando não há eventos ou ações a serem processadas, o que é ineficiente. Segundo, existe uma **latência elevada**, já que a detecção de um evento, como o pressionamento de um botão, pode demorar até 100 ms (o intervalo de verificação), o que pode ser inadequado para aplicações que exigem respostas rápidas. Por fim, há o **risco de perder eventos**: se um evento for muito breve, como um toque no botão que dure menos de 100 ms, ele pode passar despercebido entre os ciclos de verificação.

Uma solução mais eficiente para esses problemas seria implementar **interrupções**, que só ativam o processamento quando um evento de fato acontece, eliminando as limitações do polling.

---

Espero que essa reescrita atenda ao que você esperava! Se precisar de mais ajustes ou tiver outras solicitações, é só avisar. Quanto ao arquivo em formato Markdown, o conteúdo acima já está formatado dessa forma. Caso queira que eu gere um documento específico, posso ajudar com isso.