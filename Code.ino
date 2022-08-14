#include <Stepper.h>

const int s0 = 8; //Verbindung der Farbsensorkontakte mit dem Arduino festlegen
const int s1 = 9;
const int s2 = 12;
const int s3 = 11;
const int out = 10;

unsigned long startTime;
int indu = 42; // Eingänge und Ausgänge deklarieren
int hub1 = 22;
int hub2 = 26;
int hub3 = 24;int Start = 45;

int rot = 0; //Variablen für LEDs benennen
int gruen = 0;
int blau = 0;
int val;
int go ;

int steps = 2048; // Schrittmotor in Schritte unterteilen
Stepper Schrittmotor(steps, 3, 5, 4, 6);

void setup()

{

  Serial.begin(9600); //Serielle Kommunikation starten

  pinMode(s0, OUTPUT); //Die Kontakte des Farbsensors werden als Output oder...
  pinMode(s1, OUTPUT); // (Warum als Output? Die Frequenzwerte (s0-s3) sollen im
  pinMode(s2, OUTPUT); // seriellen Monitor ausgegeben werden.)
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT); // ...Input festgelegt
  pinMode(hub1, OUTPUT);
  pinMode(hub2, OUTPUT);
  pinMode(hub3, OUTPUT);
  pinMode(indu, INPUT);
  pinMode(Start, INPUT);
  digitalWrite(s0, HIGH); //Die weißen LED am Farbsensor an
  digitalWrite(s1, HIGH);
  Schrittmotor.setSpeed(5); // Geschwindigkeit vom Schrittmotor
}

void loop()

{
  digitalWrite(hub1, LOW);
  digitalWrite(hub2, LOW);
  digitalWrite(hub3, LOW);

  color();//Diese Funktion wird am Ende des Codes festgelegt (s.“void color();“)

  Serial.print(" Rot: "); //Auf dem seriellen Monitor soll jeweils „Wert“
  Serial.print(rot, DEC); //mit der entsprechenden Farbe angezeigt
  Serial.print(" Gruen: ");//werden und dahinter der Wert, welcher in der
  Serial.print(gruen, DEC); //void color(); Funktion ausgelesen wurde.
  Serial.print(" Blau: ");
  Serial.println(blau, DEC);

  val = digitalRead(indu); // Induktivsensor auslesen und in val schreiben
  Serial.print(val);
  Serial.print(go);

  go = digitalRead(Start); // Einlesen von Start Taster und Wert speichern in Variable go
  if (go == HIGH) {    //wenn Taster gedrückt dann...

    if (rot < blau && rot < gruen && rot < 35) //Wenn der Filterwert für rot //kleiner ist als alle anderen Werte..
    {
      Serial.println(" – (Rote Farbe)"); //..soll “Rote Farbe“ am seriellen //Monitor angezeigt werden und..
      digitalWrite(hub1, HIGH); //…soll Hubmagnet 1 und 2 anziehen
      digitalWrite(hub2, HIGH); //
      delay(2000);
      digitalWrite(hub1, LOW);
      digitalWrite(hub2, LOW);
    }
    else if (blau < rot && blau < gruen && blau < 40) //Das gleiche bei Blau
    {
      Serial.println(" – (Blaue Farbe)");
      digitalWrite(hub1, HIGH);
      delay(2000);
      digitalWrite(hub1, LOW);
    }
    else {
      Serial.println(); //..nichts auf dem seriellen Monitor anzeigen und..
    }
    delay(2000);
    if (val == 0) {   // Wenn Induktivsensor Metall erkennt, dann...
      Schrittmotor.step(-360);
      Schrittmotor.step(360);// ...Schrittmotor 360 Schritte drehen
      digitalWrite(hub1, HIGH);// ...alle Hubmagnete anziehen
     digitalWrite(hub2, HIGH);
     digitalWrite(hub3, HIGH);

    delay(500);
      digitalWrite(hub1, LOW);
      delay(500);//... nach einer Sekunde, alle Hubmagnete aus
      digitalWrite(hub2, LOW);
      delay(500);
      digitalWrite(hub3, LOW);
      delay(500);
      //Schrittmotor.step(360); // ... Schrittmotor zürck drehen
      
    }else {    // wenn kein Metall erkannt --> Kunststoff, dann...
      Schrittmotor.step(-360); // ....Schrittmotor drehen, 0,2 Sekunden warten, zurückdrehen
      delay(200);
      Schrittmotor.step(360);

    }
    delay(1500);
  }
}

void color() //Hier werden die Werte vom Farbsensor ausgelesen und unter den //entsprechenden Variablen gespeichert
{
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  rot = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  blau = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  gruen = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}
