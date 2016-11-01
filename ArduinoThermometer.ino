/*
 * Список необходимых библиотек: / List of needed libraries:
 * - U8glib
 * - OneWire
 * - DallasTemperature 
 */

extern uint8_t temp_icon[]; // значок температуры / temperature icon

/* 
   Подключение экрана OLED 0.91" / Connection of OLED 0.91" display
   SCK -> Вывод A5 / Pin A5
   SDA -> Вывод A4 / Pin A4
   VCC -> Питание 5В / +5V
   GND -> Земля / Ground
*/

#include "U8glib.h" // библиотека работы с экраном / display library
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // инициализация объекта для работы с экраном по шине I2C / initialize object for handling display

/*
   Подключение датчиков температуры Dallas DS18B20 / Dallas DS18B20 temperature sensors connection
   1 - GND -> Земля / Ground
   2 - DQ -> Вывод 4 / Pin 4
   3 - VDD -> Питание 5В / +5V
   Также необходимо подключить резистор 10кОм между выводами 2 и 3 у DS18B20 / Also needed 10kOhm resistor between DS18B20 pins 2 and 3
*/

#include <OneWire.h>  // библиотека для работы по протоколу OneWire (необходима для работы библиотеки DallasTemperature.h) / OneWire protocol library (DallasTemperature.h dependency)
#include <DallasTemperature.h>  // библиотека для работы с датчиками Dallas DS18B20 / Dallas DS18B20 sensors library

#define ONE_WIRE_BUS 4  // номер вывода Arduino, к которому подключен датчик температуры / Pin connected to DS18B20 sensor data pin

OneWire oneWire(ONE_WIRE_BUS);  // инициализация объекта для работы по протоколу OneWire / initialize object for handling onewire bus
DallasTemperature sensors(&oneWire);  // инициализация объекта для работы с датчиками DS18B20 / initialize object for handling DS18B20 sensors

// метод для предварительной настройки оборудования / setup
void setup() {
  // Старт обмена с датчиками / Start sensors bus
  sensors.begin();

  // Отрисовка экрана заставки / Draw splashscreen
  drawSplashScreen();
  delay(5000);
}

// основной рабочий цикл / Main loop
void loop() {
  int sensorsCount = sensors.getDeviceCount(); // количество подключенных датчиков / number of connected sensors
  if (sensorsCount == 0) { // проверка на количество подключенных датчиков / check for connected sensors
    drawError(-1);
  }
  else {
    sensors.requestTemperatures(); // получение значений температуры с датчиков / request temperatures

    for (int i = 0; i < sensorsCount; i++) {  // перебор подключенных датчиков / loop by connected sensors
      float temperature = sensors.getTempCByIndex(i); // получение температуры с датчика / get temperature fo sensor
      if (temperature == -127.0 || temperature == 85.0) { //проверка на пропавшую связь с датчиком / check for sensor lost connection
        drawError(i);
      }
      else {
        drawTemperature(temperature, i, sensorsCount);
      }

      delay(8000);  // задержка между отображениями показаний / delay between screens
    }
  }
}

#include "rus7x13.h"  // подключение русских шрифтов / import russian fonts

// метод для отрисовки экрана заставки / draw splashscreen
void drawSplashScreen() {
  u8g.firstPage();
  do {
    u8g.setFont(rus7x13); // задание шрифта / set font
    u8g.drawStr( 0, 16, "       W201");
    u8g.drawStr( 0, 25, "   Mercedes-Benz");
    
    invertScreenColors(true);
    setContrast(0x00);
  } while ( u8g.nextPage() );
}

#include <Wire.h>
#define OLED_I2C_ADDRESS   0x3C // Константа адреса контроллера экрана / Display I2C address
#define OLED_CONTROL_BYTE_CMD_STREAM  0x00 // Константа старта отсылки сообщений контроллеру экрана / Control byte (start stream)
#define OLED_CMD_DISPLAY_NORMAL      0xA6 // Константа для нормального отображения экрана (белые символы на черном фоне) / Normal display color mode
#define OLED_CMD_DISPLAY_INVERTED     0xA7 // Константа для инвертированного отображения экрана (черные символы на белом фоне) / Inverted display color mode

// метод для инвертирования цветов экрана / Set display color mode
void invertScreenColors(bool invertFlag) {
  Wire.beginTransmission(OLED_I2C_ADDRESS);
  Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
  if (invertFlag) {
    Wire.write(OLED_CMD_DISPLAY_INVERTED);
  }
  else {
    Wire.write(OLED_CMD_DISPLAY_NORMAL);
  }
  Wire.endTransmission();
}

char buffer[25];  // буфер для преобразования значения температуры из float в строку / String buffer
#include "digits32.h"  // подключение шрифта для отображения температуры / import digit font

// метод для отрисовки показаний температурных датчиков / Draw temperature
void drawTemperature(float temperature, int index, int count) {
  u8g.firstPage();
  do {
    u8g.drawBitmapP( 5, 0, 3, 32, temp_icon); // отрисовка значка температуры / draw temperature graphic symbol

    if (count > 1) {
      u8g.setFont(rus7x13); // задание шрифта / set font
      sprintf(buffer, "%d", index + 1);
      u8g.drawStr(10 - u8g.getStrWidth(buffer), 12, buffer);
    }

    u8g.setFont(digits32); // задание шрифта отображения температуры / set temperature font
    dtostrf(temperature, 5, 1, buffer);
    u8g.drawStr(34, 32, buffer); // отображение показаний температуры / draw temperature

    invertScreenColors(false);

  } while ( u8g.nextPage() );
}

// метод для задания контрастности (0...255) / set display contrast
void setContrast(int contrast) {
  Wire.beginTransmission(OLED_I2C_ADDRESS);
  Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
  Wire.write(0x81);
  Wire.write(contrast);
  Wire.endTransmission();
}

// метод для отображения ошибки связи с датчиком / draw error
void drawError(int index) {
  u8g.firstPage();
  do {
    u8g.setFont(rus7x13); // задание шрифта / set font
    u8g.drawRFrame(0, 0, 128, 13, 3);
    u8g.drawStr( 43, 11, "ОШИБКА"); // ERROR
    if (index == -1) {
      u8g.drawStr( 0, 21, "датчик температуры"); // temperature sensor
      u8g.drawStr( 0, 32, "   не подключен");    //   not connected
    }
    else {
      u8g.drawStr( 0, 21, "   потеря связи");    //  connection error
      sprintf(buffer, "  с датчиком  N%d", index + 1); // sensor number
      u8g.drawStr( 0, 32, buffer);
    }
  } while ( u8g.nextPage() );
}
