// Датчик DHT11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Дисплей
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 2 
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup()
{
  Serial.begin(9600);

  dht.begin();
  
  // Отримуємо інформацію про датчик для правильної затримки
  sensor_t sensor;
  dht.temperature().getSensor(&sensor); 
  delayMS = sensor.min_delay / 1000;
  
  if (delayMS < 2000) delayMS = 2000; 

  // Ініціалізація I2C (SDA = 8, SCL = 9)
  Wire.begin(8, 9);
  
  // Ініціалізація дисплея
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Зупиняємо роботу, якщо дисплей не знайдено
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.display(); // Оновлюємо екран
}

void loop()
{
  delay(delayMS);
  sensors_event_t event;
  
  display.clearDisplay();
  display.setCursor(0, 0);

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
    display.println(F("Temp Error!"));
  }
  else
  {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));

    display.print(F("Temperature: "));
    display.print(event.temperature);
    display.println(F(" \xF8" "C"));
  }

  // 3. Читання і запис вологості
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
    display.println(F("Hum Error!"));
  }
  else
  {
    Serial.print(F("Humidity:    "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));

    display.print(F("Humidity:    "));
    display.print(event.relative_humidity);
    display.println(F(" %"));
  }

  display.display();
}