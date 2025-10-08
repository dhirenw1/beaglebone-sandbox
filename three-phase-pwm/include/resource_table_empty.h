#ifndef _RSC_TABLE_PRU_H_
#define _RSC_TABLE_PRU_H_

#include <stddef.h>
#include <rsc_types.h>

struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1];
} resourceTable __attribute__((section(".resource_table"))) = {
    {
        1,         /* Resource table version */
        0,         /* number of entries */
        {0, 0},    /* reserved */
    },
};

#endif
