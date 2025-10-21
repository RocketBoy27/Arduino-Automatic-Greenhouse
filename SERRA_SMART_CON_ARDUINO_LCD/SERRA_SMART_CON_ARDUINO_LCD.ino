/*COLLEGARE IL PIN 3V3 DI ARDUINO CON IL SUO PIN AREF!!! (analog
reference)*/
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

#define SENS_TEMP 2
#define PULSANTE_PIN 12
#define SENS_TERRA A0
#define MAX_VALUE 693
#define MIN_VALUE 293
#define SENS_LUCE A1
#define buzzer 10
#define ERROR_LED 4
#define POMPA_ACQUA 8
#define STRISCIA_LED 9
#define PULSANTE_UV 5
#define PULSANTE_VENTOLA 3
#define VENTOLA 7

int schermata_lcd = 0;
bool stato_uv = false;
bool stato_aria = true;
unsigned long t1, dt1;
unsigned long t2, dt2;
unsigned long t3, dt3;
unsigned long t4, dt4;

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(2, DHT11);

void setup() {
  analogReference(AR_EXTERNAL);
  lcd.init();
  lcd.backlight();
  dht.begin();
  lcd.setCursor(0, 0);
  lcd.print("Serra smart");
  lcd.setCursor(0, 1);
  lcd.print("con Arduino");
  lcd.setCursor(14, 1);
  lcd.print(":)");
  pinMode(SENS_TEMP, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(PULSANTE_PIN, INPUT);
  pinMode(ERROR_LED, OUTPUT);
  pinMode(POMPA_ACQUA, OUTPUT);

  pinMode(STRISCIA_LED, OUTPUT);
  pinMode(PULSANTE_UV, INPUT);
  pinMode(VENTOLA, OUTPUT);
  pinMode(PULSANTE_VENTOLA, INPUT);
  Serial.begin(9600);
  delay(3000);

  t1 = millis();
  t2 = millis();
  t3 = millis();
  t4 = millis();
}
void loop() {
  dt1 = millis() - t1;
  dt2 = millis() - t2;
  dt3 = millis() - t3;
  dt4 = millis() - t4;

  float t = dht.readTemperature();
  int h = dht.readHumidity();
  int valore = analogRead(SENS_TERRA);
  int luce = analogRead(SENS_LUCE);
  int th = map(valore, MIN_VALUE, MAX_VALUE, 100, 0);
  th = constrain(th, 0, 100);
  int UMID_TERRA = th;
  int light = map(luce, 170, 1023, 0, 100);
  light = constrain(light, 0, 100);

  Serial.println(valore);
  Serial.println(luce);

  if (digitalRead(PULSANTE_UV) == HIGH) {
    stato_uv = !stato_uv;
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(150);
    if (stato_uv == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Illuminazione UV");
      lcd.setCursor(0, 1);
      lcd.print("ON");
      delay(2300);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Illuminazione UV");
      lcd.setCursor(0, 1);
      lcd.print("OFF");
      delay(2300);
    }
  }

  if (stato_uv == true) {
    digitalWrite(STRISCIA_LED, HIGH);
  } else {
    digitalWrite(STRISCIA_LED, LOW);
  }
  if (digitalRead(PULSANTE_PIN) == HIGH) {
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(150);
    schermata_lcd++;
    if (schermata_lcd > 3) {
      schermata_lcd = 0;
    }
  }

  if (digitalRead(PULSANTE_VENTOLA) == HIGH) {
    stato_aria = !stato_aria;
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(150);
    digitalWrite(buzzer, LOW);
    if (stato_aria == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ventilazione");
      lcd.setCursor(0, 1);
      lcd.print("auto");
      lcd.setCursor(14, 1);
      lcd.print("ON");
      delay(2300);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ventilazione");
      lcd.setCursor(0, 1);
      lcd.print("auto");
      lcd.setCursor(13, 1);
      lcd.print("OFF");
      delay(2300);
    }
  }

  if (stato_aria == true) {
    if (t >= 26.50 or h >= 70) {
      digitalWrite(VENTOLA, HIGH);
    } else {
      digitalWrite(VENTOLA, LOW);
    }
  } else {
    digitalWrite(VENTOLA, LOW);
  }

  if (valore > 600) {
    digitalWrite(ERROR_LED, HIGH);
  } else {
    digitalWrite(ERROR_LED, LOW);
  }

  if (UMID_TERRA < 35) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Irrigazione in");
    lcd.setCursor(0, 1);
    lcd.print("corso...");
    digitalWrite(POMPA_ACQUA, HIGH);
    delay(1000);
    digitalWrite(POMPA_ACQUA, LOW);
    delay(3500);
    lcd.clear();
  } else {
    digitalWrite(POMPA_ACQUA, LOW);
  }

  if (schermata_lcd == 1) {
    if (dt1 >= 1000) {
      t1 = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Umidita' del");
      lcd.setCursor(0, 1);
      lcd.print("terreno:" + String(UMID_TERRA) + "%");
      lcd.setCursor(12, 1);
      lcd.print("soil");
    }
  }

  if (schermata_lcd == 0) {
    if (dt2 >= 1000) {
      t2 = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp:" + String(t) + char(0xDF) + "c");
      lcd.setCursor(0, 1);
      lcd.print("Umidita':" + String(h) + "%");
      lcd.setCursor(13, 1);
      lcd.print("air");
    }
  }

  if (schermata_lcd == 2) {
    if (dt3 >= 1000) {
      t3 = millis();
      lcd.clear();
      lcd.setCursor(0, 0);

      lcd.print("quantita' di");
      lcd.setCursor(0, 1);
      lcd.print("luce:" + String(light) + "%");
      lcd.setCursor(11, 1);
      lcd.print("light");
    }
  }
  
  if (schermata_lcd == 3) {
    if (dt4 >= 1000) {
      t4 = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Info:Luce UV ");
      lcd.setCursor(0, 1);
      lcd.print("Ventilazione ");
      if (stato_uv == true) {
        lcd.setCursor(13, 0);
        lcd.print("ON");
      } else {
        lcd.setCursor(13, 0);
        lcd.print("OFF");
      }
      if (stato_aria == true) {
        lcd.setCursor(13, 1);
        lcd.print("ON");
      } else {
        lcd.setCursor(13, 1);
        lcd.print("OFF");
      }
    }
  }
}