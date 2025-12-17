
const int pinVelocita = A0; //pin analogico deputato a leggere i valori del potenziometro
const int IN_1 = 12; // Devono essere complementari per avere la marcia
const int IN_2 = 13; //
const int pin_PWM_A = 11; //pin di controllo della velocità per il motore A

//variabili usate per gestire e mostrare i valori di operaizone
int velocita = 0;  //valore letto dal pin A0
int duty_Cycle = 0;  //valore in uscita dal pin 11

void setup() {
  //inizializzo la comunicazione seriale cosi da mostrare i valori nel Mointor Seriale
  Serial.begin(9600);

  //definisco tipologia pin
  pinMode(pinVelocita, INPUT); //input in quanto legge il valore analogico del pin A0
  pinMode(IN_1, OUTPUT); //output perche' definisce lo stato logico del pin IN1 del modulo L298N
  pinMode(IN_2, OUTPUT); //output perche' definisce lo stato logico del pin IN2 del modulo L298N
  pinMode(pin_PWM_A, OUTPUT);  //output perche' definisce il valore PWM del pin EN1 del modulo L298N

  //pullDown sensore pinAcceleratore
  digitalWrite(pinVelocita, LOW);

  //Sati logici di controllo del modulo
  /*
   inA |   inB  | Evento
  -----|-------|----------------------
  LOW  | LOW   | fermo
  LOW  | HIGH  | Movimento in un senso
  HIGH | LOW   | Movimento senso opposto
  HIGH | HIGH  | Fermo
  */
  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH); // Marcia avanti

}


void loop() {
  
  //leggo la tensione di controllo in ingresso al piedino A0.
  velocita = analogRead(pinVelocita);
  
  /*
  Il range dei valori analogici e' da 0 a 1023 mentre il range dei valori PWM  e' da 0 a 255
  per mantenere questa proporzionalita' eseguo la mappatura dei valori
  */
   duty_Cycle = map(velocita, 0, 1024, 0, 255);

  //Invio i dati letti al Monitro seriale, cosi da poterli vedere a video
  Serial.print("Velocità = ");
  Serial.print(velocita);
  Serial.print(" Duty Cycle = ");
  Serial.println(duty_Cycle);
  
  //invio costantemente il valore PWM della potenza in modo da far variare la velocita' del motore in base alla posizione del potenziometro
  analogWrite(pin_PWM_A, duty_Cycle);
}
