#ifndef PROJECT_INCLUDE_DATABASE_OPERATIONS_H_
#define PROJECT_INCLUDE_DATABASE_OPERATIONS_H_

#include <stdio.h>

#include "client.h"

void db_write_clients(FILE *db_clients, client_t *client);
void db_write_transfers(FILE *db_transfers, client_t *transfer);
void db_update(FILE *db_clients, FILE *db_transfers, FILE *blackrecord,
                     client_t *client_data, client_t *transfer);

#endif  // PROJECT_INCLUDE_DATABASE_OPERATIONS_H_
