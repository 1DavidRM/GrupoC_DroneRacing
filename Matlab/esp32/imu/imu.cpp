#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "imu_wrapper.h"

static Adafruit_MPU6050 mpu;

void imu_init(void) {
    Wire.begin(); // SDA=21, SCL=22 default en la mayoría de dev boards ESP32
    mpu.begin();
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
}

void imu_read(double *out) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    out[0] = a.acceleration.x;
    out[1] = a.acceleration.y;
    out[2] = a.acceleration.z;
    out[3] = g.gyro.x;
    out[4] = g.gyro.y;
    out[5] = g.gyro.z;
}