/* Code written by Chia Jiun Wei @ 24 Jul 2017
 * <J.W.Chia@tudelft.nl>
 
 * MB85RS: a library to provide high level APIs to interface
 * with the Fujitsu FRAM. It is possible to use this library in
 * Energia (the Arduino port for MSP microcontrollers) or in 
 * other toolchains.
 
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * version 3, both as published by the Free Software Foundation.
  
 */
 
#ifndef MB85RS_H
#define MB85RS_H

#include <driverlib.h>
#include "DSPI.h"

// Status register
#define WPEN       0x80
#define BP1        0x08
#define BP0        0x04
#define WEL        0x02

// OP-CODE
#define WREN       0x06
#define WRDI       0x04
#define RDSR       0x05
#define WRSR       0x01
#define READ       0x03
#define WRITE      0x02
#define RDID       0x9F
#define SLEEP      0xB9

#define MEM_SIZE_SMALL   0x8000	//Memory size
#define MEM_SIZE_BIG     0x20000

class MB85RS
{
public:
    enum Device { MB85RS256A, MB85RS1MT, CY15B104QN50SXI};

	MB85RS( DSPI &spi, unsigned long csPort, unsigned long csPin, Device dev);
	virtual ~MB85RS( ) {};
		
	void init();
	unsigned long getID();
	bool ping();
	void writeEnable();
	void writeProtect();
	unsigned char getStatus();
	void setStatus(char val);
	void read(unsigned int address, unsigned char *Ptr, unsigned int size);
	void write(unsigned int address, unsigned char *Ptr, unsigned int size);
	void erase();

protected:
    DSPI &line;
    unsigned long CSPort;
    unsigned long CSPin;
    Device device;

    //Compatibility workaround
    bool threeByteAddress;

private:

};

#endif	// MB85RS_H
