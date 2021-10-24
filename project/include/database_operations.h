#ifndef PROJECT_INCLUDE_DATABASE_OPERATIONS_H_
#define PROJECT_INCLUDE_DATABASE_OPERATIONS_H_

#include <stdio.h>

#include "client.h"

void clients_data_write(FILE *ofPTR, client_t *client);
void transactions_data_write(FILE *ofPTR, client_t *transfer);
void update_database(FILE *ofPTR, FILE *ofPTR_2, FILE *blackrecord,
                     client_t *client_data, client_t *transfer);

#endif  // PROJECT_INCLUDE_DATABASE_OPERATIONS_H_
