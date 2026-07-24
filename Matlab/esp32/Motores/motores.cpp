#include "driver/mcpwm_prelude.h"
#include "motores_wrapper.h"

#define PWM_FREQ_HZ   10000        // 10 kHz, ajusta si necesitas otro
#define DEADTIME_NS   500          // 500 ns de tiempo muerto, típico para MOSFETs rápidos
#define TIMER_RES_HZ  10000000     // 10 MHz de resolución del timer (100 ns por tick)

typedef struct {
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t oper;
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t genA;
    mcpwm_gen_handle_t genB;
    uint32_t period_ticks;
} motor_pwm_t;

static motor_pwm_t motores[4];

// Configura un par timer+operador+2 generadores con dead-time, para UN motor
static void motor_setup(int idx, int group_id, int gpio_high, int gpio_low) {
    uint32_t period = TIMER_RES_HZ / PWM_FREQ_HZ;
    motores[idx].period_ticks = period;

    mcpwm_timer_config_t timer_config = {
        .group_id = group_id,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = TIMER_RES_HZ,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
        .period_ticks = period,
    };
    mcpwm_new_timer(&timer_config, &motores[idx].timer);

    mcpwm_operator_config_t oper_config = { .group_id = group_id };
    mcpwm_new_operator(&oper_config, &motores[idx].oper);
    mcpwm_operator_connect_timer(motores[idx].oper, motores[idx].timer);

    mcpwm_comparator_config_t cmp_config = { .flags = { .update_cmp_on_tez = true } };
    mcpwm_new_comparator(motores[idx].oper, &cmp_config, &motores[idx].comparator);
    mcpwm_comparator_set_compare_value(motores[idx].comparator, 0); // arranca en 0%

    mcpwm_generator_config_t genA_config = { .gen_gpio_num = gpio_high };
    mcpwm_new_generator(motores[idx].oper, &genA_config, &motores[idx].genA);
    mcpwm_generator_config_t genB_config = { .gen_gpio_num = gpio_low };
    mcpwm_new_generator(motores[idx].oper, &genB_config, &motores[idx].genB);

    // Generador A (high-side): sube en el inicio del periodo, baja al llegar al compare
    mcpwm_generator_set_action_on_timer_event(motores[idx].genA,
                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    mcpwm_generator_set_action_on_compare_event(motores[idx].genA,
                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, motores[idx].comparator, MCPWM_GEN_ACTION_LOW));

    // Generador B (low-side): arranca igual, se invierte con dead-time abajo
    mcpwm_generator_set_action_on_timer_event(motores[idx].genB,
                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    mcpwm_generator_set_action_on_compare_event(motores[idx].genB,
                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, motores[idx].comparator, MCPWM_GEN_ACTION_LOW));

    // Dead-time: retrasa el flanco de subida del low-side, y lo invierte (complementario real)
    mcpwm_dead_time_config_t dt_config = {
        .posedge_delay_ticks = (uint32_t)(DEADTIME_NS / (1e9 / TIMER_RES_HZ)),
        .flags = { .invert_output = true },
    };
    mcpwm_generator_set_dead_time(motores[idx].genA, motores[idx].genB, &dt_config);

    mcpwm_timer_enable(motores[idx].timer);
    mcpwm_timer_start_stop(motores[idx].timer, MCPWM_TIMER_START_NO_STOP);
}

void motores_init(void) {
    // Motor 1, 2, 3 -> Grupo 0 (MCPWM0), un timer/operador cada uno
    motor_setup(0, 0, 25, /*LOW1_AQUI*/ 33);
    motor_setup(1, 0, 26, /*LOW2_AQUI*/ 32);
    motor_setup(2, 0, 27, /*LOW3_AQUI*/ 4);
    // Motor 4 -> Grupo 1 (MCPWM1)
    motor_setup(3, 1, 14, /*LOW4_AQUI*/ 2);
}

void motores_set_duty(const double *f) {
    for (int i = 0; i < 4; i++) {
        double pct = f[i];
        if (pct < 0) pct = 0;
        if (pct > 100) pct = 100;
        uint32_t compare_value = (uint32_t)((pct / 100.0) * motores[i].period_ticks);
        mcpwm_comparator_set_compare_value(motores[i].comparator, compare_value);
    }
}