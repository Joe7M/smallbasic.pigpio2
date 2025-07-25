//This file is part of the SmallBasicPIGPIO plugin to
//use gpio pins on a Raspberry pi with SmallBASIC.
//MIT licence
//Joerg Siebenmorgen, 2025

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>
#include <stdint.h>
#include <cstdio>

#include <cstdlib>
#include <unistd.h>



#include "spi.h"

int32_t SPI_Open(char *device)
{
	int32_t fd = open(device, O_RDWR);
  return(fd);
}

int32_t SPI_SetSpeed(int32_t fd, uint32_t Speed)
{
  return(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &Speed));
}

int32_t SPI_Close(int32_t fd)
{
  close(fd);
  return 0;
}

int32_t SPI_SetLSBFirst(int32_t fd)
{
  uint32_t LSBFirst = 1;
  return(ioctl(fd, SPI_IOC_WR_LSB_FIRST, &LSBFirst));
}

int32_t SPI_SetMSBFirst(int32_t fd)
{
  uint32_t LSBFirst = 0;
  return(ioctl(fd, SPI_IOC_WR_LSB_FIRST, &LSBFirst));
}

int32_t SPI_SetBitsPerWord(int32_t fd, uint32_t BitsPerWord)
{
  return(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &BitsPerWord));
}

int32_t SPI_SetMode(int32_t fd, uint32_t Mode)
{
  return(ioctl(fd, SPI_IOC_WR_MODE32, &Mode));
}

int32_t SPI_Write(int32_t fd, uint8_t *WriteBuffer, uint32_t Bytes, uint32_t PageSize)
{
  uint8_t* pWriteBuffer = WriteBuffer;

  uint32_t FullMessages_Write = Bytes / PageSize;
  struct spi_ioc_transfer xfer[1] = {0};

  for(uint32_t ii = 0; ii < FullMessages_Write; ii++)
  {
    xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
    xfer[0].rx_buf = (unsigned long)NULL;
    xfer[0].delay_usecs = 0;
    xfer[0].len = PageSize;
    pWriteBuffer += PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0) return(1);
  }
  if(Bytes % PageSize)
  {
    xfer[0].tx_buf = (unsigned long)(pWriteBuffer);
    xfer[0].rx_buf = (unsigned long)NULL;
    xfer[0].delay_usecs = 0;
    xfer[0].len = Bytes % PageSize;
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &xfer) < 0) return(1);
  }

  return(0);
}
