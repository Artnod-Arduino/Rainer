#ifndef Rainer_h
#define Rainer_h

#include <EEPROM.h>

#define RAINER_MIN		0
#define RAINER_TIME		1
#define RAINER_BREAK	2
#define RAINER_ENABLE	3
#define RAINER_EEPROM_LEN  7

class Rainer
{
	public:
		// Déclaration de l'objet
		Rainer(int IO_Pin, int ee_addr = 0);
		
		// renvoie la valeur du paramètre
		int		getValue(int type);
		// modifie la valeur du paramètre
		void	setValue(int type, int value);
		// sauvegarde la valeur du parametre dans l'EEPROM
		void	saveValue(int type);
		
		// lance l'analyse
		void	run(unsigned long currentTime, int currentHum);
		// renvoie un booléen signifiant sont état
		bool	isWorking(void);
		
		// active ou desactive le timer
		void	enable(bool value = true);		
		//renvoie un booléen s'il est activé
		bool	isEnable(void);
		
		// renvoie l'adresse EEPROM
		int		getEEPROM(void);
		// renvoie le prochain octet libre
		int		getNextEEPROM(void);
		// modifier l'adresse EEPROM
		void	setEEPROM(int addr);
		//charge la conf depuis l'EEPROM
		void	loadAll(void);	
		//sauvegarde la conf dans l'EEPROM
		void	saveAll(void);
		
	private:
		int		_IO_Pin;
		int		_ee_addr;
		
		int		_data[3];
		bool	_isWorking;
		bool	_isEnable;
		unsigned long _lastRain;
		void	activateRelay(void);
		void	desactivateRelay(void);
};
#endif