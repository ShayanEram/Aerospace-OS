#include "model.h"
#include "types.h"
#include "process.h"
#include "scheduler.h"

#include "task1.h"
#include "task2.h"

extern void task1(void);
extern void task2(void);

#define NUMBER_OF_PARTITIONS 2

int main() 
{
    system_t sys = {
        .major_frame_ns = 100000000ULL,
        .num_partitions = 2
    };

    partition_t parts[NUMBER_OF_PARTITIONS];
    partition_init(&parts[0], "Partition1", 50000000ULL, 0ULL);
    partition_init(&parts[1], "Partition2", 50000000ULL, 50000000ULL);

    process_t procs0_0[1] = {
        {
            .name = "Process1",
            .period_ns = 100000000ULL,
            .timecap_ns = 20000000ULL,
            .priority = 10,
            .entry_point = task1
        }
    };
    process_t procs1_0[1] = {
        {
            .name = "Process1",
            .period_ns = 100000000ULL,
            .timecap_ns = 40000000ULL,
            .priority = 9,
            .entry_point = task2
        }
    };

    parts[0].procs = procs0_0;
    parts[0].num_processes = 1;
    parts[0].id = 0;
    parts[1].procs = procs1_0;
    parts[1].num_processes = 1;
    parts[1].id = 1;
    sys.parts = parts;

    for (int i = 0; i < sys.num_partitions; ++i){
        for (int j = 0; j < sys.parts[i].num_processes; ++j){
            start_process(&sys.parts[i].procs[j], &sys.parts[i]);
        }
    }

    run_scheduler(&sys);
    return 0;
}
