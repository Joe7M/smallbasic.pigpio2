// This file is part of the SmallBasicPIGPIO plugin to
// use gpio pins on a Raspberry pi with SmallBASIC.
// MIT licence
// Joerg Siebenmorgen, 2024
//
// SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
// Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

// https://www.kernel.org/doc/Documentation/spi/spidev

#include "config.h"

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <unistd.h>
#include <sys/time.h>

#include "var.h"
#include "module.h"
#include "param.h"

#include "spi.h"
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>

int32_t fd;
struct spi_ioc_transfer trx;
uint32_t MaxSpeed;

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, "/dev/spidev0.0");
  char *device = new char[strlen(text) + 2];
  strcpy(device,text);

  fd = open(device, O_RDWR);
  if (fd < 0)
  {
    v_setstr(retval, "Error opening SPI device");
    return(0);
  }

  if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &MaxSpeed) < 0)
  {
    v_setstr(retval, "Error reading SPI maximum speed");
    close(fd);
    return(0);
  }

  return(1);
}


int CMD_GetMode(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t buffer;

  if (ioctl(fd, SPI_IOC_RD_MODE32, &buffer) < 0)
  {
    v_setstr(retval, "Error reading SPI mode");
    close(fd);
    return(0);
  }

  v_setint(retval, buffer);

  return(1);
}

int CMD_SetMode(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t NewMode = get_param_int(argc, params, 0, 0);
  uint32_t Mode;

  switch(NewMode)
  {
    case 1:
      NewMode = SPI_MODE_1;
      break;
    case 2:
      NewMode = SPI_MODE_2;
      break;
    case 3:
      NewMode = SPI_MODE_3;
      break;
    default:
      NewMode = SPI_MODE_0;
  }

  if(ioctl(fd, SPI_IOC_RD_MODE32, &Mode) < 0)
  {
    v_setstr(retval, "Error reading SPI mode");
    close(fd);
    return(0);
  }

  Mode |= NewMode;

  if(ioctl(fd, SPI_IOC_WR_MODE32, &Mode) != 0)
  {
    v_setstr(retval, "Error writing SPI mode");
    close(fd);
    return(0);
  }

  return(1);
}

int CMD_GetMaxSpeed(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t Speed;

  if(ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &Speed) < 0)
  {
    v_setstr(retval, "Error reading SPI maximum speed");
    close(fd);
    return(0);
  }

  v_setint(retval, Speed);

  return(1);
}


int CMD_SetMaxSpeed(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t Speed = get_param_int(argc, params, 0, 1000000); // Hz

  if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &Speed) != 0)
  {
    v_setstr(retval, "Error writing SPI max speed");
    close(fd);
    return(0);
  }

  MaxSpeed = Speed;

  return(1);
}


int Close(void)
{
  close(fd);
  return(1);
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  close(fd);
  return(1);
}

int CMD_Write(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t Speed = get_param_int(argc, params, 1, MaxSpeed); // Hz

  if(is_param_array(argc, params, 0))
  {
    var_p_t array = params[0].var_p;  //Get array

    if(v_maxdim(array) > 1)
    {
      v_setstr(retval, "Write requires an 1D-array");
      return 0;
    }

    uint32_t bytes  = v_ubound(array, 0) - v_lbound(array, 0) + 1;
    uint8_t* buffer = new uint8_t[bytes];

    for(uint32_t ii = 0; ii < bytes; ii++)
    {
      buffer[ii] = get_array_elem_num(array, ii);
    }

    trx.tx_buf = (unsigned long)buffer;
    trx.rx_buf = (unsigned long)NULL;
    trx.bits_per_word = 8;
    trx.speed_hz = Speed;
    trx.delay_usecs = 0;
    trx.len = bytes;

    if(ioctl(fd, SPI_IOC_MESSAGE(1), &trx) < 0)
	  {
		  v_setstr(retval, "Error transmitting spi data");
		  return(0);
	  }
  }
  else
  {
    uint8_t byte = get_param_int(argc, params, 0, 0);

    trx.tx_buf = (unsigned long)&byte;
    trx.rx_buf = (unsigned long)NULL;
    trx.bits_per_word = 8;
    trx.speed_hz = Speed;
    trx.delay_usecs = 0;
    trx.len = 1;

    if(ioctl(fd, SPI_IOC_MESSAGE(1), &trx) < 0)
	  {
		  v_setstr(retval, "Error transmitting spi data");
		  return(0);
	  }
  }

  return(1);
}

int CMD_Read(int argc, slib_par_t *params, var_t *retval)
{
/*
  uint8_t  id    = get_param_int(argc, params, 0, 0);
  uint32_t bytes = get_param_int(argc, params, 1, 1);
  uint8_t fd  = I2C_FD_List[id];

  uint8_t* buffer = new uint8_t[bytes];
  uint8_t result = 1;

  int32_t len = read(fd, buffer, bytes);
  if(len < 0)
  {
    v_setstr(retval, "Error while reading from device.");
    result = 0;
  }
  else if(len == 0)
  {
    v_setint(retval, 0);
  }
  else if(len == 1)
  {
    v_setint(retval, buffer[0]);
  }
  else
  {
    v_toarray1(retval, len);
    for(int32_t ii = 0; ii < len; ii++)
    {
      v_setint(v_elem(retval, ii), buffer[ii]);
    }
  }

  delete[] buffer;
  return(result);
*/
  return(1);
}
