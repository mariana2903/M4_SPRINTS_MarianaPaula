// definição das portas de entrada do led
#define led1 21
#define led2 45
#define led3 36
#define led4 39

//definição das portas do buzzer, sensor e botões
#define Buzzer 2
#define Button1 16
#define Button2 18
#define LDR 5

// declaração de variável que recebe 4 valores
int entrada[4];

int vector[100];
int x = 0;


void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Button1, INPUT_PULLUP); 
  pinMode(Button1, INPUT_PULLUP);
  pinMode(LDR, INPUT); 
}

void conversor(int valor) {
  int i = 0;
  while (i < 4) {
    entrada[i] = valor % 2;
    valor = valor / 2;
    i += 1;
  }
}

void armazenarf(int freq) {
  tone(Buzzer, freq * 200, 250);
  vector[x] = freq;
  x += 1;
}

void play() {
  int i = 0;
  while (i <= (x - 1)) {
    tone(Buzzer, vector[i]*200, 250);
    conversor(vector[i]);
    delay(1000);
    ligaled();
    i += 1;
  }
}


void ligaled() {
  if (entrada[0] == 1) {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }
  if (entrada[1] == 1) {
    digitalWrite(led2, HIGH);
  } else {
    digitalWrite(led2, LOW);
  }
  if (entrada[2] == 1) {
    digitalWrite(led3, HIGH);
  } else {
    digitalWrite(led3, LOW);
  }
  if (entrada[3] == 1) {
    digitalWrite(led4, HIGH);
  } else {
    digitalWrite(led4, LOW);
  }
}


void loop() {
  int ldr = analogRead(LDR);

  int intervalo = (ldr / 260); //separando os intervalos 0-15
  conversor(intervalo);

  ligaled();

  if (digitalRead(Button1) == LOW) {
    armazenarf(intervalo);
  }

  if (digitalRead(Button2) == LOW) {
    play();
  }
  delay(100);
}