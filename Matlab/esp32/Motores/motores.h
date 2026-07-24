#pragma once
#ifdef __cplusplus
extern "C" {
    #endif
    void motores_init(void);
    void motores_set_duty(const double *f); // f[0]=F1, f[1]=F2, f[2]=F3, f[3]=F4 (0-100%)
    #ifdef __cplusplus
}
#endif