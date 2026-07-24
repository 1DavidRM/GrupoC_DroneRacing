#include "imu_wrapper.h"

void imu_init(void) {
    // No hace nada en simulación de PC
}

void imu_read(double *out) {
    out[0] = 0.0;  out[1] = 0.0;  out[2] = 9.81; // reposo: gravedad completa en Z
    out[3] = 0.0;  out[4] = 0.0;  out[5] = 0.0;  // sin rotación
}