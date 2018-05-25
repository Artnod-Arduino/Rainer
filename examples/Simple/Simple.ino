#include <Rainer.h>
#include <Wire.h>
#include "RTClib.h"

#define RAIN_PIN 10
#define SOIL_PIN A1

RTC_DS1307 rtc;
DateTime now;
Rainer arrosage(RAIN_PIN);

void setup()
{	
	Wire.begin();
	rtc.begin();
	
	arrosage.setValue(RAINER_MIN,   400);
	arrosage.setValue(RAINER_TIME,  60);
	arrosage.setValue(RAINER_BREAK, 3600);
	arrosage.enable();
}

void loop()
{
	now = rtc.now();
  int soilHum = analogRead(SOIL_PIN);
	arrosage.run(now.unixtime(), soilHum);
}
