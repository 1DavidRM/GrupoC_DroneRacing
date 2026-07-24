#pragma once
#ifdef __cplusplus
extern "C" {
    #endif
    void imu_init(void);
    void imu_read(double *out); // out[0..2]=accel XYZ, out[3..5]=gyro XYZ
    #ifdef __cplusplus
}
#endif