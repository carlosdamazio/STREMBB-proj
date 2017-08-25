#include <Thread.h>
#include <ThreadController.h>
#include <Ultrasonic.h>
#define echoPin 9 // Define a porta 9 do Arduino como recebimento do Echo do
                  // sensor Ultrassônico.
#define trigPin 8 // Define a porta 8 como emissor do pulso ultrassônico
                  // do sensor.

  Ultrasonic ultrasonic(8, 9);

  // Definição das portas Arduino ligadas a ponte H.
  int IN1 = 13;
  int IN2 = 12;
  int IN3 = 11;
  int IN4 = 10;

  // Definição da variável de recebimento dos dados do sensor Ultrassônico.
  float sen;

  // Definição das threads para o controle das rodas e leitura do sensor.
  Thread sonicThread = Thread();
  Thread wheelThread = Thread();

  // Definição do controlador de threads.
  ThreadController controllet = ThreadController();

  // ------------------------Seção das funções---------------------------------

  // Função de leitura do sensor Ultrassônico.
  void captSensor(){
    sen = ultrasonic.Ranging(CM); // Armazena o valor de leitura do sensor em
                                  // centímetros.
    Serial.println(sen);
  }

  void controlRodas(){
    if (sen <= 30){ // Se o sensor identificar obstáculo a 30 cm ou menos...
      Serial.println("Alvo identificado, estou andando pra frente");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else {
      Serial.println("Estou andando em círculos");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
  }

  // ------------------------Seção de setur do Arduino-------------------------
  void setup(){
    Serial.begin(9600); // Inicializa comunicação serial para Debug na IDE.
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    wheelThread.onRun(controlRodas);
    wheelThread.setInterval(10); // Seta ativação de thread em 10 ms.

    sonicThread.onRun(captSensor);
    sonicThread.setInterval(6); // Seta ativação de thread em 6 ms.

    controller.add(&wheelThread);
    controller.add(&sonicThread);

    delay(5000); // Espera 5000 ms (5 segundos) para o robô iniciar ação.
  }

  void loop(){
    controller.run();
  }
