/*
 * PRU0 - 3-Phase PWM Generator for BLDC FOC
 * Generates three 120-degree phase-shifted PWM signals at 20kHz
 * Uses PRU IEP timer for accurate timing
 */

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <pru_iep.h>
#include "../include/resource_table_empty.h"

// PRU runs at 200MHz = 5ns per cycle
#define PRU_CLOCK_HZ 200000000

// PWM parameters
#define PWM_FREQ_HZ 20000
#define PWM_PERIOD_CYCLES (PRU_CLOCK_HZ / PWM_FREQ_HZ)  // 10,000 cycles = 50us

// Initial duty cycle (50%)
#define DUTY_CYCLE_PERCENT 25
#define ON_TIME_CYCLES ((PWM_PERIOD_CYCLES * DUTY_CYCLE_PERCENT) / 100)  // 5000 cycles

// Phase shifts (120 degrees each)
#define PHASE_SHIFT_CYCLES (PWM_PERIOD_CYCLES / 3)  // 3333 cycles

// Pin definitions - using working pin combination
// P8_12 = pr1_pru0_pru_r30_14 (Phase A)
// P8_11 = pr1_pru0_pru_r30_15 (Phase B)  
// P9_30 = pr1_pru0_pru_r30_2 (Phase C)
#define PHASE_A_PIN (1 << 14)
#define PHASE_B_PIN (1 << 15)
#define PHASE_C_PIN (1 << 2)

volatile register uint32_t __R30;  // Output register
volatile register uint32_t __R31;  // Input register

// Helper function to check if timer is within a pulse
static inline uint8_t is_pulse_active(uint32_t timer, uint32_t start, uint32_t duration, uint32_t period) {
    uint32_t end = (start + duration);
    
    // Check if pulse wraps around the period boundary
    if (end <= period) {
        // Normal case: no wrap-around
        return (timer >= start && timer < end);
    } else {
        // Wrap-around case: pulse continues from start to period end, then 0 to (end-period)
        end = end - period;
        return (timer >= start || timer < end);
    }
}

void main(void) {
    uint32_t timer;
    uint32_t tmp_reg = 0;
    
    // Clear all output pins
    __R30 = 0;
    
    // Enable IEP timer
    CT_IEP.TMR_GLB_CFG = 0x11;  // Enable timer, default increment
    CT_IEP.TMR_CNT = 0;         // Reset counter
    
    while (1) {
        // Read timer value
        timer = CT_IEP.TMR_CNT;

        // Temp output before writing to __R30 register
        tmp_reg = 0;
        
        // Control Phase A (starts at 0 degrees)
        if (is_pulse_active(timer, 0, ON_TIME_CYCLES, PWM_PERIOD_CYCLES)) {
            tmp_reg |= PHASE_A_PIN;
        }
        
        // Control Phase B (starts at 120 degrees)
        if (is_pulse_active(timer, PHASE_SHIFT_CYCLES, ON_TIME_CYCLES, PWM_PERIOD_CYCLES)) {
            tmp_reg |= PHASE_B_PIN;
        }
        
        // Control Phase C (starts at 240 degrees)
        if (is_pulse_active(timer, 2 * PHASE_SHIFT_CYCLES, ON_TIME_CYCLES, PWM_PERIOD_CYCLES)) {
            tmp_reg |= PHASE_C_PIN;
        }

        __R30 = tmp_reg;
        
        // Reset at end of period
        if (timer >= PWM_PERIOD_CYCLES) {
            CT_IEP.TMR_CNT = 0;
        }
    }
}
