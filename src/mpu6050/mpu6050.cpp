/*
 * This file is part of the SmallBasicPIGPIO plugin to
 * use gpio pins on a Raspberry pi with SmallBASIC.
 * MIT license
 * Joerg Siebenmorgen, 2024
 *
 * "https://github.com/tockn/MPU6050_tockn" was very helpful
 * to get the sensor working
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
#include "mpu6050.h"

int fd;
float AccelerationLSBSensitivity = 16384;
float GyroscopeLSBSensitivity = 65.5;

int CMD_Open(int argc, slib_par_t *params, var_t *retval)
{
  const char *text = get_param_str(argc, params, 0, "/dev/i2c-0");
  char *device = new char[strlen(text) + 2];
  strcpy(device,text);
  uint8_t Address = get_param_int(argc, params, 1, 0x68);

  fd = open(device, O_RDWR);
  if (fd < 0)
  {
    v_setstr(retval, "Error opening I2C bus");
    return(0);
  }

  int16_t rc = ioctl(fd, I2C_SLAVE, Address);
  if (rc < 0)
  {
    v_setstr(retval, "Error opening MPU6050");
    close(fd);
    return(0);
  }

  uint8_t buffer[2];

  buffer[0] = 0x75;
  write(fd, buffer, 1);
  read(fd, buffer, 1);

  if(buffer[0] != 0x68 && buffer[0] != 0x69)
  {
    v_setstr(retval, "Error MPU6050 WHO_AM_I value wrong");
    close(fd);
    return(0);
  }

  // Set SMPLRT_DIV to 0
  // Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
  buffer[0] = 0x19;
  buffer[1] = 0;
  write(fd, buffer, 2);

  // MPU config
  // external Frame Synchronization disabled
  buffer[0] = 0x1A;
  buffer[1] = 0;
  write(fd, buffer, 2);

  // Gyro config: 500°/s
  buffer[0] = 0x1B;
  buffer[1] = 0x8;
  write(fd, buffer, 2);
  GyroscopeLSBSensitivity = 65.5;

  // Accel config: 2g
  buffer[0] = 0x1C;
  buffer[1] = 0x0;
  write(fd, buffer, 2);
  AccelerationLSBSensitivity = 16384;

  // Turn on
  buffer[0] = 0x6B;
  buffer[1] = 0x01;
  write(fd, buffer, 2);

  return(1);
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


int CMD_GetAcceleration(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t buffer[6];
  buffer[0] = 0x3B;
  write(fd, buffer, 1);
  read(fd, buffer, 6);

  float AccX = ((float)  int16_t((buffer[0] << 8) | buffer[1])) / AccelerationLSBSensitivity;
  float AccY = ((float)  int16_t((buffer[2] << 8) | buffer[3])) / AccelerationLSBSensitivity;
  float AccZ = ((float)  int16_t((buffer[4] << 8) | buffer[5])) / AccelerationLSBSensitivity;

  v_toarray1(retval, 3);
  v_setreal(v_elem(retval, 0), AccX);
  v_setreal(v_elem(retval, 1), AccY);
  v_setreal(v_elem(retval, 2), AccZ);

  return 1;
}

int CMD_GetGyroscope(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t buffer[6];
  buffer[0] = 0x43;
  write(fd, buffer, 1);
  read(fd, buffer, 6);

  float GyroX = ((float) int16_t((buffer[0] << 8) | buffer[1])) / GyroscopeLSBSensitivity;
  float GyroY = ((float) int16_t((buffer[2] << 8) | buffer[3])) / GyroscopeLSBSensitivity;
  float GyroZ = ((float) int16_t((buffer[4] << 8) | buffer[5])) / GyroscopeLSBSensitivity;

  v_toarray1(retval, 3);
  v_setreal(v_elem(retval, 0), GyroX);
  v_setreal(v_elem(retval, 1), GyroY);
  v_setreal(v_elem(retval, 2), GyroZ);

  return 1;
}

int CMD_GetTemperature(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t buffer[2];
  buffer[0] = 0x41;
  write(fd, buffer, 1);
  read(fd, buffer, 2);

  float Temp = ((float) int16_t((buffer[0] << 8) | buffer[1])) / 340.0 + 36.53;

  v_setreal(retval, Temp);

  return 1;
}

/**
 * Gyroscope Configuration
 *
 * mpu6050.GyroscopeConfig(range)
 * 
 * Sets the full scale range of the Gyroscope. Range can have the
 * values 250, 500, 1000, or 2000 in °/s.
 */
int CMD_GyroscopeConfig(int argc, slib_par_t *params, var_t *retval)
{
  uint16_t Setting = get_param_int(argc, params, 0, 250);

  switch(Setting)
  {
    case 250:
      Setting = 0x00;
      GyroscopeLSBSensitivity = 131.0;
      break;
    case 500:
      Setting = 0x08;
      GyroscopeLSBSensitivity = 65.5;
      break;
    case 1000:
      Setting = 0x10;
      GyroscopeLSBSensitivity = 32.8;
      break;
    case 2000:
      Setting = 0x18;
      GyroscopeLSBSensitivity = 16.4;
      break;
    default:
      Setting = 0x00;
  }

  uint8_t buffer[2];
  buffer[0] = 0x1B;
  buffer[1] = Setting;
  write(fd, buffer, 2);

  return 1;
}

/**
 * Accelerometer Configuration
 *
 * mpu6050.AccelerationConfig(range)
 * 
 * Sets the full scale range of the Accelerometer. Range can have the
 * values 2, 4, 8, or 16, which corresponds to the following g-force
 * 
 * 2 ->  2g
 * 4 ->  4g
 * 8 ->  8g
 * 16 -> 16g
 */
int CMD_AccelerationConfig(int argc, slib_par_t *params, var_t *retval)
{
  uint8_t Setting = get_param_int(argc, params, 0, 2);

  switch(Setting)
  {
    case 2:
      Setting = 0x00;
      AccelerationLSBSensitivity = 16384;
      break;
    case 4:
      Setting = 0x08;
      AccelerationLSBSensitivity = 8192;
      break;
    case 8:
      Setting = 0x10;
      AccelerationLSBSensitivity = 4096;
      break;
    case 16:
      Setting = 0x18;
      AccelerationLSBSensitivity = 2048;
      break;
    default:
      Setting = 0x00;
  }

  uint8_t buffer[2];
  buffer[0] = 0x1C;
  buffer[1] = Setting;
  write(fd, buffer, 2);

  return 1;
}