#include "client.h"

#include <stdio.h>
#include <stdlib.h>

void print_field_info() {
  printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
         "1 number account: ", "2 client name: ", "3 Surname: ",
         "4 Addres client: ", "5 client Telnum: ", "6 client indebtedness: ",
         "7 client credit limit: ", "8 client cash payments: ");
}

int read_client(client_t *client, FILE *f) {
  if (fscanf(f, "%12d%11s%11s%16s%20s%12lf%12lf%12lf", &client->number,
             client->name, client->surname, client->addres, client->telnumber,
             &client->indebtedness, &client->credit_limit,
             &client->cash_payments) != COUNT_FIELDS_CLIENT)
    return ERR_READ;

  return EXIT_SUCCESS;
}

void write_client(client_t *client, FILE *f) {
  fprintf(f, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", client->number,
          client->name, client->surname, client->addres, client->telnumber,
          client->indebtedness, client->credit_limit, client->cash_payments);
}

int read_transaction(client_t *transfer, FILE *f) {
  if (fscanf(f, "%d %lf", &transfer->number, &transfer->cash_payments) !=
      COUNT_FIELDS_TRANSFER) {
    return ERR_READ;
  }
  return EXIT_SUCCESS;
}
void write_transaction(client_t *transfer, FILE *f) {
  fprintf(f, "%-2d %4.2lf \n", transfer->number, transfer->cash_payments);
}
