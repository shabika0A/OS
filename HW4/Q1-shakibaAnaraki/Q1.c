#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_CUSTOMERS 5 // number of customers
#define NUM_TRANSACTION 6 // number of transactions/threads
#define INIT_STOCK 100 // the initial value for each person stock

// type transaction. you should pass an object of this type to the function arguments while creating thread
typedef struct
{
    double amount;
    int customer_id;
} transaction_t;

double safeBox = 1000.0; // shared variable for all customers

///////// Start your code here ///////////

/* 1. define an array to store the stock of all customers
   2. define the necessary mutexes for each customer account and safe box */

typedef struct
{
    double amount;

} account_t;

account_t *accounts;
pthread_mutex_t *customer_mutexes;
pthread_mutex_t safeBox_mutex;

void *withdraw(void *arg)
{
    transaction_t *transaction = (transaction_t *)arg;
    int customer_id = transaction->customer_id;
    double transaction_amount = transaction->amount;
    pthread_mutex_lock(&customer_mutexes[customer_id]);
    pthread_mutex_lock(&safeBox_mutex);
    double first_money = accounts[customer_id].amount;
    if((first_money > 0 &&first_money+safeBox<transaction_amount) || (first_money<0 && safeBox<transaction_amount)){
        printf("Customer %d withdrow %.2f is not possible.",
           customer_id,transaction_amount);
    }else{
    accounts[customer_id].amount -= transaction_amount;
    double taken_from_safeBox = 0;

    if (accounts[customer_id].amount < 0)
    {
        
        if(first_money>0){
            safeBox -= transaction_amount-first_money ;
        }else{
            safeBox -= transaction_amount ;
        }
        
        taken_from_safeBox = transaction_amount-first_money ;
        
    }
    printf("Customer %d withdrew %.2f$. from account:%.2f  and %.2f from safe-box. New balance: %.2f\tSafe-box value: %.2f\n",
           customer_id,transaction_amount,transaction_amount- taken_from_safeBox, taken_from_safeBox,
           accounts[customer_id].amount, safeBox);
    pthread_mutex_unlock(&customer_mutexes[customer_id]);
    pthread_mutex_unlock(&safeBox_mutex);
    }
    return NULL;
}

void *deposit(void *arg)
{
    transaction_t *transaction = (transaction_t *)arg;
    int customer_id = transaction->customer_id;
    double transaction_amount = transaction->amount;

    pthread_mutex_lock(&customer_mutexes[customer_id]);
    pthread_mutex_lock(&safeBox_mutex);
    double first_money = accounts[customer_id].amount;
    accounts[customer_id].amount += transaction_amount;
    double added_to_safeBox = 0;

    if (accounts[customer_id].amount > 0 && first_money < 0)
    {
        safeBox += 0 - first_money;
        added_to_safeBox = 0 - first_money;
        
    }

    printf("Customer %d stock was charged by %.2f. New balance: %.2f\tSafe-box value: %.2f\n",
           customer_id, transaction_amount, accounts[customer_id].amount, safeBox);

    pthread_mutex_unlock(&customer_mutexes[customer_id]);
    pthread_mutex_unlock(&safeBox_mutex);
    return NULL;
}

int main()
{
    srand(time(NULL));

    accounts = (account_t *)malloc(sizeof(account_t) * NUM_CUSTOMERS);
    customer_mutexes = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * NUM_CUSTOMERS);

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        accounts[i].amount = INIT_STOCK;
        pthread_mutex_init(&customer_mutexes[i], NULL);
    }

    pthread_mutex_init(&safeBox_mutex, NULL);

    pthread_t threads[NUM_TRANSACTION];

    for (int i = 0; i < NUM_TRANSACTION; ++i)
    {
        transaction_t *transaction = (transaction_t *)malloc(sizeof(transaction_t));
        transaction->amount = 20 *(5*i+1) ; 
        transaction->customer_id = rand() % NUM_CUSTOMERS;

        if (rand() % 2 == 0)
        {
            pthread_create(&threads[i], NULL, withdraw, (void *)transaction);
        }
        else
        {
            pthread_create(&threads[i], NULL, deposit, (void *)transaction);
        }
    }

    for (int i = 0; i < NUM_TRANSACTION; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    free(accounts);
    free(customer_mutexes);
    pthread_mutex_destroy(&safeBox_mutex);

    return 0;
}


