#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define NUM_OPERATIONS 100000
#define SHARED_MEM_NAME "/shared_account_balance"
#define INITIAL_BALANCE 1000.0

typedef struct {
    double balance;
    int error; 
} OperationResult;

void check_operation(double old_balance, double amount, double new_balance, const char* operation) {
    if ((operation[0] == 'd' && new_balance != old_balance + amount) ||
        (operation[0] == 'w' && new_balance != old_balance - amount)) {
        printf("Error in %s: expected %.2f, got %.2f\n", operation, 
            operation[0] == 'd' ? old_balance + amount : old_balance - amount, new_balance);
        exit(1);
    }
}

int main() {
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(double));
    double* account_balance = (double*) mmap(0, sizeof(double), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    *account_balance = INITIAL_BALANCE;

    pid_t pid1 = fork();

    if (pid1 == 0) {
        for (int i = 0; i < NUM_OPERATIONS; i++) {
            double old_balance = *account_balance;
            double amount = 1.0;
            *account_balance += amount;
            double new_balance = *account_balance;

            check_operation(old_balance, amount, new_balance, "deposit");

            usleep(1);
        }
        munmap(account_balance, sizeof(double));
        exit(0);
    } else {
        pid_t pid2 = fork();

        if (pid2 == 0) {
            for (int i = 0; i < NUM_OPERATIONS; i++) {
                double old_balance = *account_balance;
                double amount = 1.0;
                *account_balance -= amount;
                double new_balance = *account_balance;

                check_operation(old_balance, amount, new_balance, "withdraw");

  
                usleep(1);
            }
            munmap(account_balance, sizeof(double));
            exit(0);
        }
    }

    wait(NULL);
    wait(NULL);

    printf("Final account balance: %.2f\n", *account_balance);

    munmap(account_balance, sizeof(double));
    shm_unlink(SHARED_MEM_NAME);

    return 0;
}
