#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif
#include "Rainer.h"

Rainer::Rainer(int IO_Pin, int ee_addr)
{
	_IO_Pin		= IO_Pin;
	_ee_addr	= ee_addr;
	_isWorking	= false;
	_isEnable	= false;
	_lastRain	= 0;
	pinMode(_IO_Pin, OUTPUT);
	digitalWrite(_IO_Pin, LOW);
}


int Rainer::getValue(int type)
{
	return _data[type];
}

void Rainer::setValue(int type, int value)
{
	_data[type] = value;
}

void Rainer::saveValue(int type)
{
	int loc = _ee_addr + (sizeof(int) * type);
	switch (type)
	{
		case RAINER_MIN:
			EEPROM.put(loc, _data[RAINER_MIN]);
			break;
		
		case RAINER_TIME:
			EEPROM.put(loc, _data[RAINER_TIME]);
			break;
		
		case RAINER_BREAK:
			EEPROM.put(loc, _data[RAINER_BREAK]);
			break;
		
		case RAINER_ENABLE:
			EEPROM.write(loc, (_isEnable ? 1 : 0));
			break;
		
		default:
			break;
	}
}


void Rainer::run(unsigned long currentTime, int currentHum)
{
	if(_isEnable)
	{
		if(! _isWorking) // si pompe eteinte
		{
			if(currentHum <= _data[RAINER_MIN]) // si humidite basse
			{
				if(currentTime >= (_lastRain + _data[RAINER_BREAK])) // si pause entre 2 arrosage faite
				{
					// allumer pompe
					_isWorking = true;
					_lastRain = currentTime;
					digitalWrite(_IO_Pin, HIGH);
				}
			}
		}
		else // sinon pompe allumee
		{
			if(currentTime >= (_lastRain + _data[RAINER_TIME])) // si duree arrosage depassee
			{
				// eteindre pompe
				_isWorking = false;
				_lastRain = currentTime;
				digitalWrite(_IO_Pin, LOW);
			}
		}
	}
}

bool Rainer::isWorking(void)
{
	return _isWorking;
}


void Rainer::enable(bool value)
{
	if(value != _isEnable)
	{
		if(_isWorking)
		{
			_isWorking = false;
			digitalWrite(_IO_Pin, LOW);
		}
		_isEnable = value;
	}
}

bool Rainer::isEnable(void)
{
	return _isEnable;
}


int Rainer::getEEPROM(void)
{
	return _ee_addr;
}

int Rainer::getNextEEPROM(void)
{
	return _ee_addr + RAINER_EEPROM_LEN;
}

void Rainer::setEEPROM(int addr)
{
	_ee_addr = addr;
}

void Rainer::loadAll(void)
{
	int loc = _ee_addr;
	EEPROM.get(loc, _data[RAINER_MIN]);
	loc += sizeof(int);
	EEPROM.get(loc, _data[RAINER_TIME]);
	loc += sizeof(int);
	EEPROM.get(loc, _data[RAINER_BREAK]);
	loc += sizeof(int);
	_isEnable = EEPROM.read(loc) & 1;
}

void Rainer::saveAll(void)
{
	int loc = _ee_addr;
	EEPROM.put(loc, _data[RAINER_MIN]);
	loc += sizeof(int);
	EEPROM.put(loc, _data[RAINER_TIME]);
	loc += sizeof(int);
	EEPROM.put(loc, _data[RAINER_BREAK]);
	loc += sizeof(int);
	EEPROM.write(loc, (_isEnable ? 1 : 0));
}
