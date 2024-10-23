#include <Arduino.h>
#include <Servo.h> 
Servo monServo; 
const int brochePiezo= A0;
const int brocheInterrupteur= 2;
const int brocheLEDJaune= 3;
const int brocheLEDVerte= 4;
const int brocheLEDRouge= 5; 

int etatInterrupteur;
int intensiteCoup;

const int coupFaible= 10;
const int coupFort= 100;

boolean estVerrouille= false;
int nombreDeCoups= 0; 

void setup() {
  monServo.attach(9);
  pinMode(brocheLEDJaune, OUTPUT);
  pinMode(brocheLEDVerte, OUTPUT);
  pinMode(brocheLEDRouge, OUTPUT);
  pinMode(brocheInterrupteur, INPUT);

  Serial.begin(9600); 
  digitalWrite(brocheLEDVerte, HIGH); 
  monServo.write(0);
  Serial.println("La boite est déverrouillée!");
}

void loop() {
  if (estVerrouille == false) {
    etatInterrupteur = digitalRead(brocheInterrupteur); 
    if (etatInterrupteur == HIGH) {
      estVerrouille = true;
      nombreDeCoups = 0;
      digitalWrite(brocheLEDVerte, LOW );
      digitalWrite(brocheLEDRouge, HIGH); 
      monServo.write(90);
      Serial.println("La boite est verrouillée!");
      delay(1000);
    }
  }
  if (estVerrouille == true) {
    intensiteCoup = analogRead(brochePiezo);
    if (nombreDeCoups < 3 && intensiteCoup > 0) {
      if (verifierCoup(intensiteCoup) == true) {
        nombreDeCoups++; 
      }
      Serial.print(3-nombreDeCoups);
      Serial.println(" supplémentaire(s) à donner");
    }
    if (nombreDeCoups >= 3) { 
      estVerrouille = false;
      monServo.write(0);
      delay(20);
      digitalWrite(brocheLEDVerte, HIGH);
      digitalWrite(brocheLEDRouge, LOW);
      Serial.println("La boite est déverrouillée!");
    }
  }
}

boolean verifierCoup(int intensite) {
  if (intensite > coupFaible && intensite < coupFort) {
    digitalWrite(brocheLEDJaune, HIGH);
    delay(50);
    digitalWrite(brocheLEDJaune, LOW );
    Serial.print("Coup VALIDE, d'intensité ");
    Serial.println(intensite);
    return true;
  }
  else {
    Serial.print("Coup INVALIDE, d'intensité ");
    Serial.println(intensite);
    return false;
  }
}
  
  
