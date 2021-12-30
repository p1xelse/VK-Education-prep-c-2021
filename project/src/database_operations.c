#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "transfer.h"

void db_write_clients(FILE *db_clients, client_t *client) {
  print_field_info();

  while (read_client(client, stdin) == EXIT_SUCCESS) {
    write_client(client, db_clients);
  }
}

void db_write_transfers(FILE *db_transfers, transfer_t *transfer) {
  printf("%s\n%s\n", "1 Number account: ", "2 Client cash payments: ");

  while (read_transfer(transfer, stdin) == EXIT_SUCCESS) {
    write_transfer(transfer, db_transfers);
  }
}

void db_update(FILE *db_clients, FILE *db_transfers, FILE *blackrecord,
                     client_t *client_data, transfer_t *transfer) {
  while (read_client(client_data, db_clients) == EXIT_SUCCESS) {
    while (read_transfer(transfer, db_transfers) == EXIT_SUCCESS) {
      if (client_data->number == transfer->number &&
          transfer->cash_payments != 0)
        client_data->credit_limit += transfer->cash_payments;
    }
    write_client(client_data, blackrecord);
    rewind(db_transfers);
  }
}
