// This file is part of the SmallBasicPIGPIO plugin to
// use gpio pins on a Raspberry pi with SmallBASIC.
// MIT licence
// Joerg Siebenmorgen, 2024
//
// SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
// Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

// https://www.kernel.org/doc/Documentation/i2c/dev-interface

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

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}
#include <sys/ioctl.h>
#include <fcntl.h>


std::vector<uint8_t> I2C_FD_List;

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t Address = get_param_int(argc, params, 0, 0);

  const char *text = get_param_str(argc, params, 1, "/dev/i2c-1");
  char *device = new char[strlen(text) + 2];
  strcpy(device,text);

  int16_t fd;
  int16_t rc;

  fd = open(device, O_RDWR);
  if (fd < 0)
  {
    v_setstr(retval, "Error opening I2C bus");
    return(0);
  }

  rc = ioctl(fd, I2C_SLAVE, Address);
  if (rc < 0)
  {
    v_setstr(retval, "Error opening I2C device");
    close(fd);
    return(0);
  }

  I2C_FD_List.push_back(fd);
  v_setint(retval, I2C_FD_List.size() - 1);
  return(1);
}

int Close(void)
{
  for(const uint8_t& fd : I2C_FD_List)
  {
    close(fd);
  }
  return(1);
}

int CMD_Close(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  close(I2C_FD_List[id]);
  return(1);
}

int CMD_Write(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint8_t fd = I2C_FD_List[id];

  if(is_param_array(argc, params, 1))
  {
    var_p_t array = params[1].var_p;  //Get array

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

    if(write(fd, buffer, bytes) < 0)
    {
      v_setstr(retval, "Error while writing to device.");
      return(0);
    }
  }
  else
  {
    uint8_t byte = get_param_int(argc, params, 1, 0);
    if(write(fd, &byte, 1) < 0)
    {
      v_setstr(retval, "Error while writing to device.");
      return(0);
    }
  }

  return(1);
}

int CMD_Write_Reg(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id  = get_param_int(argc, params, 0, 0);
  uint8_t Reg = get_param_int(argc, params, 1, 0);
  uint8_t fd  = I2C_FD_List[id];

  if(is_param_array(argc, params, 2))
  {
    var_p_t array = params[2].var_p;  //Get array

    if(v_maxdim(array) > 1)
    {
      v_setstr(retval, "Write requires an 1D-array");
      return 0;
    }

    uint32_t bytes  = v_ubound(array, 0) - v_lbound(array, 0) + 1;
    uint8_t* buffer = new uint8_t[bytes + 1];

    buffer[0] = Reg;
    for(uint32_t ii = 0; ii < bytes; ii++)
    {
      buffer[ii + 1] = get_array_elem_num(array, ii);
    }

    if(write(fd, buffer, bytes + 1) < 0)
    {
      v_setstr(retval, "Error while writing to device.");
      return(0);
    }
  }
  else
  {
    uint8_t byte = get_param_int(argc, params, 2, 0);
    uint8_t* buffer = new uint8_t[2];
    buffer[0] = Reg;
    buffer[1] = byte;
    if(write(fd, buffer, 2) < 0)
    {
      v_setstr(retval, "Error while writing to device.");
      return(0);
    }
  }

  return(1);
}


int CMD_Read(int argc, slib_par_t *params, var_t *retval)
{
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
}


int CMD_Read_Reg(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t  id    = get_param_int(argc, params, 0, 0);
  uint8_t  Reg   = get_param_int(argc, params, 1, 0);
  uint32_t bytes = get_param_int(argc, params, 2, 1);

  uint8_t  fd     = I2C_FD_List[id];
  uint8_t* buffer = new uint8_t[bytes];
  uint8_t  result = 1;

  int32_t len = write(fd, &Reg, 1);

  if(len < 0)
  {
    v_setstr(retval, "Error while writing to device.");
    result = 0;
  }

  len = read(fd, buffer, bytes);
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
}

int CMD_Smbus_ReadReg_Byte(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id  = get_param_int(argc, params, 0, 0);
  uint8_t Reg = get_param_int(argc, params, 1, 0);

  uint8_t fd  = I2C_FD_List[id];
  int32_t res;

  res = i2c_smbus_read_byte_data(fd, Reg);
  if (res < 0)
  {
    v_setstr(retval, "Error while reading device.");
    return(0);
  }

  v_setint(retval, res);
  return(1);
}

int CMD_Smbus_ReadReg_Word(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id  = get_param_int(argc, params, 0, 0);
  uint8_t Reg = get_param_int(argc, params, 1, 0);

  uint8_t fd  = I2C_FD_List[id];
  int32_t res;

  res = i2c_smbus_read_word_data(fd, Reg);
  if (res < 0)
  {
    v_setstr(retval, "Error while reading device.");
    return(0);
  }

  v_setint(retval, res);
  return(1);
}

int CMD_Smbus_Read_Byte(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id  = get_param_int(argc, params, 0, 0);

  uint8_t fd  = I2C_FD_List[id];
  int32_t res;

  res = i2c_smbus_read_byte(fd);
  if (res < 0)
  {
    v_setstr(retval, "Error while reading device.");
    return(0);
  }

  v_setint(retval, res);
  return(1);
}

int CMD_Smbus_Write_Byte(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id = get_param_int(argc, params, 0, 0);
  uint8_t fd = I2C_FD_List[id];

  uint8_t byte = get_param_int(argc, params, 1, 0);
  if(i2c_smbus_write_byte(fd, byte) < 0)
  {
    v_setstr(retval, "Error while writing to device.");
    return(0);
  }

  return(1);
}

int CMD_Smbus_Write_Word(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t id  = get_param_int(argc, params, 0, 0);
  uint8_t Reg = get_param_int(argc, params, 1, 0);
  uint16_t word = get_param_int(argc, params, 2, 0);
  uint8_t fd  = I2C_FD_List[id];

  if(i2c_smbus_write_word_data(fd, Reg, word) < 0)
  {
    v_setstr(retval, "Error while writing to device.");
    return(0);
  }

  return(1);
}