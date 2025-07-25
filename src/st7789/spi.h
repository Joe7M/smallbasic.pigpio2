//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2025

#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>

int32_t SPI_Open(char *device);
int32_t SPI_Close(int32_t fd);
int32_t SPI_SetSpeed(int32_t fd, uint32_t Speed);
int32_t SPI_SetLSBFirst(int32_t fd);
int32_t SPI_SetMSBFirst(int32_t fd);
int32_t SPI_SetBitsPerWord(int32_t fd, uint32_t BitsPerWord);
int32_t SPI_SetMode(int32_t fd, uint32_t Mode);
int32_t SPI_Write(int32_t fd, uint8_t *WriteBuffer, uint32_t Bytes, uint32_t PageSize);

#endif /* !_SPI_H_ */
