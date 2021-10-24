#include <stdio.h>
#include <stdlib.h>

#include "client.h"

void clients_data_write(FILE *out_file, client_t *client) {
  print_field_info();

  while (read_client(client, stdin) == EXIT_SUCCESS) {
    write_client(client, out_file);
  }
}

void transactions_data_write(FILE *out_file, client_t *transfer) {
  printf("%s\n%s\n", "1 Number account: ", "2 Client cash payments: ");

  while (read_transaction(transfer, stdin) == EXIT_SUCCESS) {
    write_transaction(transfer, out_file);
  }
}

void update_database(FILE *client_base, FILE *trans_base, FILE *blackrecord,
                     client_t *client_data, client_t *transfer) {
  while (read_client(client_data, client_base) == EXIT_SUCCESS) {
    while (read_transaction(transfer, trans_base) == EXIT_SUCCESS) {
      if (client_data->number == transfer->number &&
          transfer->cash_payments != 0)
        client_data->credit_limit += transfer->cash_payments;
    }
    write_client(client_data, blackrecord);
    rewind(trans_base);
  }
}
