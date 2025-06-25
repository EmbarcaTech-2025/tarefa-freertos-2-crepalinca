# Tradutor de Texto para Código Morse com FreeRTOS na Raspberry Pi Pico

**Autores:** Antonio Crepaldi e Bianca Andrade
**Data:** 25 de Junho de 2025

## 1. Resumo do Projeto

Este projeto transforma uma Raspberry Pi Pico em um tradutor de Código Morse em tempo real. Utilizando o sistema operacional de tempo real **FreeRTOS**, o sistema lê texto enviado através do terminal serial (USB), o converte para Código Morse e o reproduz de forma síncrona através de um LED e um buzzer.

É um exemplo prático de multitarefa, comunicação inter-tarefas (usando filas) e controle de hardware com PWM para geração de áudio.

## 2. Funcionalidades

- **Tradução em Tempo Real:** Converte caracteres alfanuméricos (A-Z, 0-9) para Código Morse.
- **Saída Dupla:** O código Morse é sinalizado simultaneamente de duas formas:
  - **Visual:** Um LED pisca no ritmo dos pontos e traços.
  - **Auditiva:** Um buzzer emite tons na mesma cadência do LED.
- **Interface Serial:** A entrada de texto é feita via um terminal serial conectado à porta USB da Pico, facilitando o teste e a interação.
- **Velocidade e Tom Ajustáveis:** A velocidade da transmissão Morse e o tom (frequência) do buzzer podem ser facilmente alterados através de constantes no código.
- **Arquitetura Multitarefa:** Utiliza duas tarefas distintas do FreeRTOS para separar a lógica de entrada (leitura do console) da lógica de saída (geração do Morse), comunicando-se através de uma fila (Queue).
