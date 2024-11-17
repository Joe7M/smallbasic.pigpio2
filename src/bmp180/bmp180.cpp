/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT license
 * Joerg Siebenmorgen, 2024
*/

/*
 * BMP180 source code is based on
 * https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/bmp180-ohne-bibliothek
*/

#include "config.h"

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "bmp180.h"


void bmp180_close(void);
int32_t Read_2Bytes(uint8_t Reg);

int fd;
int16_t ac1, ac2, ac3;
uint16_t ac4, ac5, ac6;
int16_t  b1, b2, mb, mc, md;
uint8_t Oversampling = 0;

int32_t Read_2Bytes(uint8_t Reg)
{
  uint8_t buffer[2];

  write(fd, &Reg, 1);
  read(fd, buffer, 2);

  return((int32_t) ((int32_t)buffer[0] << 8) | (int32_t)buffer[1]);
}

uint8_t Read_Byte(uint8_t Reg)
{
  uint8_t buffer;

  write(fd, &Reg, 1);
  read(fd, &buffer, 1);

  return(buffer);
}

// Address BMP180: 0x77
int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, "/dev/i2c-1");
  char *device = new char[strlen(text) + 2];
  strcpy(device,text);
  uint8_t Address = get_param_int(argc, params, 1, 0x77);

  fd = open(device, O_RDWR);
  if (fd < 0)
  {
    v_setstr(retval, "Error opening I2C bus");
    return(0);
  }

  int16_t rc = ioctl(fd, I2C_SLAVE, Address);
  if (rc < 0)
  {
    v_setstr(retval, "Error opening BMP180");
    close(fd);
    return(0);
  }

  // Get chip id
  uint8_t buffer[2];
  buffer[0] = 0xD0;
  write(fd, buffer, 1);
  read(fd, buffer, 1);
  if(buffer[0] != 85)
  {
    v_setstr(retval, "Error Chip ID does not match");
    close(fd);
    return(0);
  }

  // Read calibration values

  ac1 = Read_2Bytes(0xAA);
  ac2 = Read_2Bytes(0xAC);
  ac3 = Read_2Bytes(0xAE);
  ac4 = Read_2Bytes(0xB0);
  ac5 = Read_2Bytes(0xB2);
  ac6 = Read_2Bytes(0xB4);
  b1  = Read_2Bytes(0xB6);
  b2  = Read_2Bytes(0xB8);
  mb  = Read_2Bytes(0xBA);
  mc  = Read_2Bytes(0xBC);
  md  = Read_2Bytes(0xBE);

  return(1);
}

int CMD_StartMeasurement(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t buffer[3];
  int32_t UncompTemp;
  float Temperature;
  uint32_t UncompPres;
  uint32_t Pressure;
  int32_t x1, x2, x3, b3, b5, b6;
  uint32_t b4, b7;

  // Read uncompensated temperature
  buffer[0] = 0xF4;
  buffer[1] = 0x2E;
  write(fd, buffer, 2);
  usleep(5000);
  UncompTemp = (int32_t)Read_Byte(0xF6) << 8 | (int32_t)Read_Byte(0xF7);

  // Read uncompensated pressure
  buffer[0] = 0xF4;
  buffer[1] = 0x34 + (Oversampling << 6);
  write(fd, buffer, 2);
  usleep((2 + (3 << Oversampling)) * 1000);
  buffer[0] = 0xF6;
  write(fd, buffer, 1);
  read(fd, buffer, 3);
  UncompPres = ((((uint32_t)buffer[0] << 16) + ((uint32_t)buffer[1] << 8) + buffer[0]) >> (8 - Oversampling));

  // Calculate true temperature
  x1 = ((UncompTemp - ac6) * ac5) >> 15;
  x2 = (mc << 11) / (x1 + md);
  b5 = x1 + x2;
  Temperature = ((b5 + 8) >> 4) / 10.0f;

  // Calculate true pressure
  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6) >> 12) >> 11;
  x2 = (ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((int32_t)ac1) * 4 + x3) << Oversampling) + 2) >> 2;
  x1 = (ac3 * b6) >> 13;
  x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
  b7 = ((uint32_t)(UncompPres - b3) * (50000 >> Oversampling));
  if (b7 < 0x80000000)
    Pressure = (b7 << 1) / b4;
  else
    Pressure = (b7 / b4) << 1;
  x1 = (Pressure >> 8) * (Pressure >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * Pressure) >> 16;
  Pressure += (x1 + x2 + 3791) >> 4;

  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;

  v_toarray1(retval, 2);
  v_setreal(v_elem(retval, 0), Temperature);
  v_setreal(v_elem(retval, 1), (float) Pressure);

  return 1;
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  close(fd);
  return(1);
}

void Close(void)
{
  close(fd);
}
