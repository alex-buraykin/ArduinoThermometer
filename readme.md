# Dallas DS18B20 Arduino Thermometer with I2C OLED display

## Materials

1.Arduino Pro Mini 5V - 1 ps.
2.DS18B20 - 1...N ps.
3.OLED 0.91" I2C (SSD1306 controller) - 1 ps.
4.Resistor 4,7-10kOhm

## Connection

> Dallas DS18B20 temperature sensors connection
>
> 1.GND -> Ground
> 2.DQ -> Pin 4
> 3.VDD -> +5V
>
> Also needed 10kOhm resistor between DS18B20 pins 2 and 3

----------

> Connection of OLED 0.91" I2C display
>
> - SCK -> Pin A5
> - SDA -> Pin A4 A4
> - VCC -> +5V
> - GND -> Ground

## Features

1.Multiple DS18B20 sensors support
2.Draw error if sensor missed during work
3.Draw error if no connected sensors
4.Print sensor number (in case multiple DS18B20 sensors)

----------

# Термометр с датчиками Dallas DS18B20

## Материалы

1.Arduino Pro Mini 5V - 1шт.
2.DS18B20 - 1шт. (или больше)
3.OLED 0.91" I2C - 1шт.
4.Резистор 4,7-10кОм

## Коммутация

> Подключение датчика температуры Dallas DS18B20
>
> 1.GND -> Земля
> 2.DQ -> Вывод 4
> 3.VDD -> Питание 5В
>
> Также необходимо подключить резистор 10кОм между выводами 2 и 3 у DS18B20

----------

> Подключение экрана OLED 0.91"
>
> - SCK -> Вывод A5
> - SDA -> Вывод A4
> - VCC -> Питание 5В
> - GND -> Земля

## Возможности

1.Работа с несколькими датчиками DS18B20
2.Отображение ошибки, если потеряна связь с датчиком во время работы
3.Отображение ошибки, если нет подключенных датчиков
4.Отображение номера датчика, в случае несколких подключенных датчиков