const int pinVelocita = A0; //pin analogico deputato a leggere i valori del potenziometro
const int IN_1 = 12; // Devono essere complementari per avere la marcia
const int IN_2 = 13; //
const int pin_PWM_A = 11; //pin di controllo della velocità per il motore A

const int pin_canaleA = 2;
const int pin_canaleB = 3;

//variabili usate per gestire e mostrare i valori di operaizone
int velocita = 0;  //valore letto dal pin A0
int duty_Cycle = 0;  //valore in uscita dal pin 11

volatile int contaImpulsi = 0;

volatile boolean A = 0;
volatile boolean B = 0;


int  rifImpulsi = 0;
int deltaImpulsi = 0; 

long int rifTemporale = 0;

void isrA()
{
 if(digitalRead(pin_canaleB)) 
      contaImpulsi--; //Se B è alto al cambio di A il senso di rotazione è antiorario e diminuisco il numero di impulsi contati 
 else
      contaImpulsi++; //Se B è basso al cambio di A il senso di rotazione è orario e aumento il numero di impulsi contati in avanti

}

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

  //turn on pullup resistor - le uscite dell'encoder sono open-collector
     
  pinMode(pin_canaleA, INPUT_PULLUP); //tutti gli encoder sono open collector
  pinMode(pin_canaleB, INPUT_PULLUP);

  //Abilito l'interrupt sui pin 2 e dichiaro la funzione da richiamare e quando richiamarle
    
  attachInterrupt(digitalPinToInterrupt(pin_canaleA), isrA, RISING); 

  rifTemporale = millis(); //memorizza il tempo attuale

}


void loop() {


   if(millis()-rifTemporale>=1000){
    deltaImpulsi = contaImpulsi-rifImpulsi;       //conta gli impulsi nell'ultimo secondo
    rifImpulsi   = contaImpulsi;                 // aggiorno il riferimento di conteggio al valore attuale
    rifTemporale = millis(); 
    }                    //aggiorno il tempo di riferimento

    Serial.print("Giri al minuto: ");  
    //ogni giro di riduttore ho 19 giri di motore. con 4 poli del magnete ho 52 impulsi/giro
    Serial.println((deltaImpulsi/52)*60); 
  
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
  Serial.print("RPM stimati: ");
  Serial.println((duty_Cycle/255.0)*789);
  //invio costantemente il valore PWM della potenza in modo da far variare la velocita' del motore in base alla posizione del potenziometro
  analogWrite(pin_PWM_A, duty_Cycle);
  delay(200);
}
