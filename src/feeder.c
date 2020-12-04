#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include "utils.h"
#include "error_handling.h"
#include "shared_memory.h"
#include "feeder_utils.h"
#include "semaphore_utils.h"


int rd_sem_id, wrt_sem_id;

int main(int argc, char *argv[]) {

    if (argc != 5) {
        usage(argv[0]);
    }

    srand(((unsigned)time(NULL)));

    size_t *integer_array = NULL;
    cmd_args args = {};

    parse_args(argc, argv, &args);

    integer_array = malloc(args.array_size * sizeof(size_t));
    if (!integer_array) 
        handle_error("Error: Could not allocate enough memory for the integer_array\n");

    for (size_t i = 0; i != args.array_size; ++i) {
        integer_array[i] = rand();
    }

    int shm_id;
    int status;
    pid_t pid, wpid;

    /* Create shared memory */
    shm_t *shared_memory = shared_memory_create((key_t) IPC_PRIVATE);

    /* Initialize semaphores */
    sem_init(&rd_sem_id, args.n_processes);
    sem_init(&wrt_sem_id, 1);
    /* Set values to the semaphores */
    sem_set_value(rd_sem_id, args.n_processes, 0);
    sem_set_value(wrt_sem_id, 1, 0);


    for (size_t peer = 0; peer != args.n_processes; ++peer) {
        pid = fork();

        /* child process */
        if (pid == 0) {

            size_t *integer_array_copy = NULL;
            double running_average = 0.0;
            
            integer_array_copy = malloc(args.array_size * sizeof(size_t));
            if (!integer_array_copy) {
                handle_error("Error: malloc()");
            }
            memset(integer_array_copy, 0, sizeof(size_t) * args.array_size);

            for (size_t j = 0; j != args.array_size; ++j) {
                read_number_from_shm(shared_memory, integer_array_copy, j, peer, &running_average);
            }

            if (peer == args.n_processes - 1) {
                running_average /= (double) args.array_size;
                printf("Last process %d has running average %lf\n", getpid(), running_average);
                create_log(integer_array_copy, args.array_size, running_average);
            }

            free(integer_array);
            free(integer_array_copy);
            exit(EXIT_SUCCESS);
        }
        else if (pid < 0 ) {
            handle_error("Error: fork()");
        }
    }
    
    for (size_t i = 0; i != args.array_size; ++i) {
       write_number_to_shm(shared_memory, integer_array[i], args.n_processes);
    }

    while((wpid = wait(&status)) > 0);


    /* remove the semaphores */
    sem_destroy(wrt_sem_id);
    sem_destroy(rd_sem_id);
    /* deallocate shared memory */
    shm_id = shared_memory_dettach(shared_memory);
    shared_memory_deallocate(shm_id);
    /* free the array of integers */
    free(integer_array);

    return EXIT_SUCCESS;
}