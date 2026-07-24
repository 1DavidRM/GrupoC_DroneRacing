#pragma once
#ifdef __cplusplus
extern "C" {
    #endif
    void rc_espnow_init(void);
    void get_rc_command(double *out);
    #ifdef __cplusplus
}
#endif