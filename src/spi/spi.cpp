// This file is part of the SmallBasicPIGPIO plugin to
// use gpio pins on a Raspberry pi with SmallBASIC.
// MIT licence
// Joerg Siebenmorgen, 2024
//
// SDL version	 : sudo sbasicg -m/home/pi/SmallBasicGPIO/bin/ -n led.bas
// Console version: sudo sbasic -m /home/pi/SmallBasicGPIO/bin/ led.bas

// https://www.kernel.org/doc/Documentation/spi/spidev
// https://github.com/azorg/spi/blob/master/spi.c

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

#include <linux/spi/spidev.h>
#include "spi.h"
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>

int32_t fd;
struct spi_ioc_transfer trx;
uint32_t PageSize = 4096;
uint32_t Delay = 0;

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, "/dev/spidev0.0");
  char *device = new char[strlen(text) + 2];
  strcpy(device,text);
  PageSize = get_param_int(argc, params, 1, 4096);

  fd = open(device, O_RDWR);
  if (fd < 0)
  {
    v_setstr(retval, "Error opening SPI device");
    return(0);
  }

  uint32_t MaxSpeed = 1000000;
  if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &MaxSpeed) < 0)
  {
    v_setstr(retval, "Error setting SPI speed to 1 MHz");
    close(fd);
    return(0);
  }

  uint32_t LSBFirst = 0;
  if(ioctl(fd, SPI_IOC_WR_LSB_FIRST, &LSBFirst) != 0)
  {
    v_setstr(retval, "Error setting MSB first");
    close(fd);
    return(0);
  }

  uint32_t BitsPerWord = 8;
  if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &BitsPerWord) != 0)
  {
    v_setstr(retval, "Error writing bits per word");
    close(fd);
    return(0);
  }

  uint32_t Mode = SPI_MODE_0;
  if(ioctl(fd, SPI_IOC_WR_MODE32, &Mode) != 0)
  {
    v_setstr(retval, "Error writing SPI mode");
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
  uint32_t Mode = get_param_int(argc, params, 0, 0);

  switch(Mode)
  {
    case 1:
      Mode = SPI_MODE_1;
      break;
    case 2:
      Mode = SPI_MODE_2;
      break;
    case 3:
      Mode = SPI_MODE_3;
      break;
    default:
      Mode = SPI_MODE_0;
  }

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

  return(1);
}

int CMD_SetBitsPerWord(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t BitsPerWord = get_param_int(argc, params, 0, 8);

  if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &BitsPerWord) != 0)
  {
    v_setstr(retval, "Error writing bits per word");
    close(fd);
    return(0);
  }

  return(1);
}

int CMD_GetBitsPerWord(int argc, slib_par_t *params, var_t *retval)
{
	uint32_t BitsPerWord;
  if(ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &BitsPerWord) != 0)
  {
    v_setstr(retval, "Error reading bits per word");
    close(fd);
    return(0);
  }

	v_setint(retval, BitsPerWord);

  return(1);
}

int CMD_SetLSBFirst(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t LSBFirst = get_param_int(argc, params, 0, 1);

  if(ioctl(fd, SPI_IOC_WR_LSB_FIRST, &LSBFirst) != 0)
  {
    v_setstr(retval, "Error setting LSB first");
    close(fd);
    return(0);
  }

  return(1);
}

int CMD_GetLSBFirst(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t LSBFirst;

  if(ioctl(fd, SPI_IOC_RD_LSB_FIRST, &LSBFirst) != 0)
  {
    v_setstr(retval, "Error reading LSB first");
    close(fd);
    return(0);
  }

	v_setint(retval, LSBFirst);

  return(1);
}

int CMD_SetDelay(int argc, slib_par_t *params, var_t *retval)
{
  Delay = get_param_int(argc, params, 0, 0);
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
  // Write parameters
  if(is_param_array(argc, params, 0))
  {
    var_p_t WriteArray = params[0].var_p;  //Get array

    if(v_maxdim(WriteArray) > 1)
    {
      v_setstr(retval, "ReadWrite requires an 1D-array");
      close(fd);
      return 0;
    }

    uint32_t WriteBytes  = v_ubound(WriteArray, 0) - v_lbound(WriteArray, 0) + 1;
    uint8_t* WriteBuffer = new uint8_t[WriteBytes];
    uint8_t* pWriteBuffer = WriteBuffer;

    for(uint32_t ii = 0; ii < WriteBytes; ii++)
    {
      WriteBuffer[ii] = get_array_elem_num(WriteArray, ii);
    }

    // Setup SPI read and write messages
    // Only PageSize bytes can be read and written at once.
    // Read and write data in chunks of PageSize.
    // To be on the save side only one message per transfer.

    uint32_t FullMessages_Write = WriteBytes / PageSize;
    struct spi_ioc_transfer xfer[1] = {0};

    // Write messages
    for(uint32_t ii = 0; ii < FullMessages_Write; ii++)
    {
      xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
      xfer[0].rx_buf = (unsigned long)NULL;
      xfer[0].delay_usecs = Delay;
      xfer[0].len = PageSize;
      pWriteBuffer += PageSize;
      if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
      {
        v_setstr(retval, "Error transmitting SPI array data");
        close(fd);
        delete[] WriteBuffer;
        return(0);
      }
    }
    if(WriteBytes % PageSize)
    {
      xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
      xfer[0].rx_buf = (unsigned long)NULL;
      xfer[0].delay_usecs = Delay;
      xfer[0].len = WriteBytes % PageSize;
      if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
      {
        v_setstr(retval, "Error transmitting SPI array data");
        close(fd);
        delete[] WriteBuffer;
        return(0);
      }
    }
    delete[] WriteBuffer;
  }
  else
  {
    uint32_t WriteByte = get_param_int(argc, params, 0, 1);
    struct spi_ioc_transfer xfer[1] = {0};
    xfer[0].tx_buf = (unsigned long)(&WriteByte);
    xfer[0].rx_buf = (unsigned long)NULL;
    xfer[0].delay_usecs = Delay;
    xfer[0].len = 1;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      return(0);
    }
  }

  return(1);
}

int CMD_WriteReg(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t Reg = get_param_int(argc, params, 0, 0);

  // Write parameters
  if(is_param_array(argc, params, 1))
  {
    var_p_t WriteArray = params[1].var_p;  //Get array

    if(v_maxdim(WriteArray) > 1)
    {
      v_setstr(retval, "ReadWrite requires an 1D-array");
      close(fd);
      return 0;
    }

    uint32_t WriteBytes  = v_ubound(WriteArray, 0) - v_lbound(WriteArray, 0) + 1 + 1;
    uint8_t* WriteBuffer = new uint8_t[WriteBytes];
    uint8_t* pWriteBuffer = WriteBuffer;

    WriteBuffer[0] = Reg;

    for(uint32_t ii = 1; ii < WriteBytes; ii++)
    {
      WriteBuffer[ii] = get_array_elem_num(WriteArray, ii - 1);
    }

    uint32_t FullMessages_Write = WriteBytes / PageSize;
    struct spi_ioc_transfer xfer[1] = {0};

    // Write messages
    for(uint32_t ii = 0; ii < FullMessages_Write; ii++)
    {
      xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
      xfer[0].rx_buf = (unsigned long)NULL;
      xfer[0].delay_usecs = Delay;
      xfer[0].len = PageSize;
      pWriteBuffer += PageSize;
      if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    	{
      	v_setstr(retval, "Error transmitting SPI data");
      	close(fd);
      	delete[] WriteBuffer;
      	return(0);
    	}
    }
    if(WriteBytes % PageSize)
    {
      xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
      xfer[0].rx_buf = (unsigned long)NULL;
      xfer[0].delay_usecs = Delay;
      xfer[0].len = WriteBytes % PageSize;
      if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    	{
      	v_setstr(retval, "Error transmitting SPI data");
      	close(fd);
      	delete[] WriteBuffer;
      	return(0);
    	}
    }
    delete[] WriteBuffer;
  }
  else
  {
    uint8_t WriteByte = get_param_int(argc, params, 1, 0);
    struct spi_ioc_transfer xfer[1] = {0};

    uint8_t* WriteBuffer = new uint8_t[2];

    WriteBuffer[0] = Reg;
    WriteBuffer[1] = WriteByte;

    xfer[0].tx_buf = (unsigned long)(WriteBuffer);
    xfer[0].rx_buf = (unsigned long)NULL;
    xfer[0].delay_usecs = Delay;
    xfer[0].len = 2;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] WriteBuffer;
      return(0);
    }
  delete[] WriteBuffer;
  }

  return(1);
}

int CMD_Read(int argc, slib_par_t *params, var_t *retval)
{
  uint32_t Bytes = get_param_int(argc, params, 0, 1);

  uint8_t* buffer = new uint8_t[Bytes];
  uint8_t* buffer1 = buffer;

  // only PageSize bytes can be read at once.
  // Read data in chunks of PageSize

  uint32_t NumberSPIFullMessages = Bytes / PageSize;

  struct spi_ioc_transfer xfer[1] = {0};

  for(uint32_t ii = 0; ii < NumberSPIFullMessages; ii++)
  {
    xfer[0].tx_buf = (unsigned long)NULL;
    xfer[0].rx_buf = (unsigned long)(buffer1);
    xfer[0].delay_usecs = Delay;
    xfer[0].len = PageSize;
    buffer1 += PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] buffer;
      return(0);
    }
  }

  if(Bytes % PageSize)
  {
    xfer[0].tx_buf = (unsigned long)NULL;
    xfer[0].rx_buf = (unsigned long)(buffer1);
    xfer[0].delay_usecs = Delay;
    xfer[0].len = Bytes % PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] buffer;
      return(0);
    }
  }

  if(Bytes > 1)
  {
    v_toarray1(retval, Bytes);
    for(uint32_t ii = 0; ii < Bytes; ii++)
    {
      v_setint(v_elem(retval, ii), buffer[ii]);
    }
  }
  else
  {
    v_setint(retval, buffer[0]);
  }

  delete[] buffer;

  return(1);
}

int CMD_ReadReg(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t  Address = get_param_int(argc, params, 0, 0);
  uint32_t Bytes = get_param_int(argc, params, 1, 1);

  uint8_t* buffer = new uint8_t[Bytes];
  uint8_t* buffer1 = buffer;

  // only PageSize bytes can be read at once.
  // Read data in chunks of PageSize

  uint32_t NumberSPIFullMessages = Bytes / PageSize;
  struct spi_ioc_transfer xfer[1] = {0};

  xfer[0].tx_buf = (unsigned long)&Address;
  xfer[0].rx_buf = (unsigned long)NULL;
  xfer[0].delay_usecs = Delay;
  xfer[0].len = 1;
  if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
  {
    v_setstr(retval, "Error transmitting SPI data");
    close(fd);
    delete[] buffer;
    return(0);
  }

  for(uint32_t ii = 0; ii < NumberSPIFullMessages; ii++)
  {
    xfer[0].tx_buf = (unsigned long)NULL;
    xfer[0].rx_buf = (unsigned long)(buffer1);
    xfer[0].delay_usecs = Delay;
    xfer[0].len = PageSize;
    buffer1 += PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] buffer;
      return(0);
    }
  }

  if(Bytes % PageSize)
  {
    xfer[0].tx_buf = (unsigned long)NULL;
    xfer[0].rx_buf = (unsigned long)(buffer1);
    xfer[0].delay_usecs = Delay;
    xfer[0].len = Bytes % PageSize;
  }

  if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
  {
    v_setstr(retval, "Error transmitting spi data");
    close(fd);
    delete[] buffer;
    return(0);
  }

  if(Bytes > 1)
  {
    v_toarray1(retval, Bytes);
    for(uint32_t ii = 0; ii < Bytes; ii++)
    {
      v_setint(v_elem(retval, ii), buffer[ii]);
    }
  }
  else
  {
    v_setint(retval, buffer[0]);
  }

  delete[] buffer;

  return(1);
}

int CMD_ReadWrite(int argc, slib_par_t *params, var_t *retval)
{
  // Write parameters
  if(!is_param_array(argc, params, 0))
  {
    v_setstr(retval, "ReadWrite requires an array with data to write");
    close(fd);
    return 0;
  }

  var_p_t WriteArray = params[0].var_p;  //Get array

  if(v_maxdim(WriteArray) > 1)
  {
    v_setstr(retval, "ReadWrite requires an 1D-array");
    close(fd);
    return 0;
  }

  uint32_t WriteBytes  = v_ubound(WriteArray, 0) - v_lbound(WriteArray, 0) + 1;
  uint8_t* WriteBuffer = new uint8_t[WriteBytes];
  uint8_t* pWriteBuffer = WriteBuffer;

  for(uint32_t ii = 0; ii < WriteBytes; ii++)
  {
    WriteBuffer[ii] = get_array_elem_num(WriteArray, ii);
  }

  // Read parameters
  uint32_t ReadBytes = get_param_int(argc, params, 1, 1);
  uint8_t* ReadBuffer = new uint8_t[ReadBytes];
  uint8_t* pReadBuffer = ReadBuffer;

  // Setup SPI read and write messages
  //   Only PageSize bytes can be read/write at once.
  //   Read/write data in chunks of PageSize

  uint32_t FullMessages_Write = WriteBytes / PageSize;
  uint32_t FullMessages_Read  = ReadBytes / PageSize;

  struct spi_ioc_transfer xfer[1] = {0};

  // Write messages
  for(uint32_t ii = 0; ii < FullMessages_Write; ii++)
  {
    xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
    xfer[0].rx_buf = (unsigned long)NULL;
    xfer[0].delay_usecs = Delay;
    xfer[0].len = PageSize;
    pWriteBuffer += PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] ReadBuffer;
      delete[] WriteBuffer;
      return(0);
    }
  }
  if(WriteBytes % PageSize)
  {
    xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
    xfer[0].rx_buf = (unsigned long)NULL;
    xfer[0].delay_usecs = Delay;
    xfer[0].len = WriteBytes % PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] ReadBuffer;
      delete[] WriteBuffer;
      return(0);
    }
  }
  // Read messages
  for(uint32_t ii = 0; ii < FullMessages_Read; ii++)
  {
    xfer[0].tx_buf = (unsigned long)NULL;
    xfer[0].rx_buf = (unsigned long)(pReadBuffer);
    xfer[0].delay_usecs = Delay;
    xfer[0].len = PageSize;
    pWriteBuffer += PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] ReadBuffer;
      delete[] WriteBuffer;
      return(0);
    }
  }
  if(ReadBytes  % PageSize)
  {
    xfer[0].tx_buf = (unsigned long)NULL;
    xfer[0].rx_buf = (unsigned long)(pReadBuffer);
    xfer[0].delay_usecs = Delay;
    xfer[0].len = ReadBytes % PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] ReadBuffer;
      delete[] WriteBuffer;
      return(0);
    }
  }

  v_toarray1(retval, ReadBytes);
  for(uint32_t ii = 0; ii < ReadBytes; ii++)
  {
    v_setint(v_elem(retval, ii), ReadBuffer[ii]);
  }

  delete[] ReadBuffer;
  delete[] WriteBuffer;

  return(1);
}


int CMD_ReadWriteParallel(int argc, slib_par_t *params, var_t *retval)
{
  // Write parameters
  if(!is_param_array(argc, params, 0))
  {
    v_setstr(retval, "ReadWrite requires an array with data to write");
    close(fd);
    return 0;
  }

  var_p_t WriteArray = params[0].var_p;  //Get array

  if(v_maxdim(WriteArray) > 1)
  {
    v_setstr(retval, "ReadWrite requires an 1D-array");
    close(fd);
    return 0;
  }

  uint32_t Bytes  = v_ubound(WriteArray, 0) - v_lbound(WriteArray, 0) + 1;
  uint8_t* WriteBuffer = new uint8_t[Bytes];
  uint8_t* pWriteBuffer = WriteBuffer;

  for(uint32_t ii = 0; ii < Bytes; ii++)
  {
    WriteBuffer[ii] = get_array_elem_num(WriteArray, ii);
  }

  // Read parameters
  uint8_t* ReadBuffer = new uint8_t[Bytes];
  uint8_t* pReadBuffer = ReadBuffer;

  // Setup SPI read and write messages
  //   Only PageSize bytes can be read/write at once.
  //   Read/write data in chunks of PageSize

  uint32_t FullMessages = Bytes / PageSize;
  struct spi_ioc_transfer xfer[1] = {0};

  // Messages
  for(uint32_t ii = 0; ii < FullMessages; ii++)
  {
    xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
    xfer[0].rx_buf = (unsigned long)(pReadBuffer);
    xfer[0].delay_usecs = Delay;
    xfer[0].len = PageSize;
    pWriteBuffer += PageSize;
    pReadBuffer += PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] ReadBuffer;
      delete[] WriteBuffer;
      return(0);
    }
  }
  if(Bytes % PageSize)
  {
    xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
    xfer[0].rx_buf = (unsigned long)(pReadBuffer);
    xfer[0].delay_usecs = Delay;
    xfer[0].len = Bytes % PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0)
    {
      v_setstr(retval, "Error transmitting SPI data");
      close(fd);
      delete[] ReadBuffer;
      delete[] WriteBuffer;
      return(0);
    }
  }

  v_toarray1(retval, Bytes);
  for(uint32_t ii = 0; ii < Bytes; ii++)
  {
    v_setint(v_elem(retval, ii), ReadBuffer[ii]);
  }

  delete[] ReadBuffer;
  delete[] WriteBuffer;

  return(1);
}
