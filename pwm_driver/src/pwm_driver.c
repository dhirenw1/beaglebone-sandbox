/*
 * ARM Processor - PWM Duty Cycle Controller
 * Controls PRU0 PWM duty cycle via shared memory
 */

#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// PRU0 DRAM physical address
#define PRU0_DRAM_BASE 0x4A300000
#define PRU0_DRAM_SIZE 0x2000  // 8KB

// Shared memory structure (must match PRU code)
struct shared_mem {
    volatile uint32_t duty_cycle_percent;
};

uint8_t keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
    usleep(100000);
}

int main(int argc, char *argv[]) {
    int mem_fd;
    void *pru_mem;
    volatile struct shared_mem *shared;
    uint32_t duty_cycle;\

    signal(SIGINT, intHandler);
    
    // Parse command line argument
    if (argc < 2) {
        printf("Usage: %s <duty_cycle_percent>\n", argv[0]);
        printf("   or: %s sweep  (sweeps from 0-100%%)\n", argv[0]);
        return 1;
    }
    
    // Open /dev/mem
    mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd < 0) {
        perror("Failed to open /dev/mem");
        printf("Try running with sudo\n");
        return 1;
    }
    
    // Map PRU0 DRAM to user space
    pru_mem = mmap(NULL, PRU0_DRAM_SIZE, PROT_READ | PROT_WRITE, 
                   MAP_SHARED, mem_fd, PRU0_DRAM_BASE);
    
    if (pru_mem == MAP_FAILED) {
        perror("Failed to mmap PRU memory");
        close(mem_fd);
        return 1;
    }
    
    // Get pointer to shared memory structure
    shared = (volatile struct shared_mem *)pru_mem;
    
    // Check if we're doing a sweep
    if (strcmp(argv[1], "sweep") == 0) {
        printf("Sweeping duty cycle from 0%% to 100%% and back...\n");
        printf("Press Ctrl+C to stop\n\n");
        
        while (keepRunning) {
            // Sweep up
            for (duty_cycle = 0; duty_cycle <= 100 && keepRunning; duty_cycle += 5) {
                shared->duty_cycle_percent = duty_cycle;
                printf("\rDuty Cycle: %3u%%", duty_cycle);
                fflush(stdout);
                usleep(100000);  // 100ms delay
            }
            
            // Sweep down
            for (duty_cycle = 100; duty_cycle > 0 && keepRunning; duty_cycle -= 5) {
                shared->duty_cycle_percent = duty_cycle;
                printf("\rDuty Cycle: %3u%%", duty_cycle);
                fflush(stdout);
                usleep(100000);  // 100ms delay
            }
        }
        shared->duty_cycle_percent = 0;
    } else {
        // Set specific duty cycle
        duty_cycle = atoi(argv[1]);
        
        if (duty_cycle > 100) {
            printf("Duty cycle must be 0-100\n");
            munmap(pru_mem, PRU0_DRAM_SIZE);
            close(mem_fd);
            return 1;
        }
        
        shared->duty_cycle_percent = duty_cycle;
        printf("Set duty cycle to %u%%\n", duty_cycle);
    }
    
    // Cleanup
    munmap(pru_mem, PRU0_DRAM_SIZE);
    close(mem_fd);
    
    return 0;
}