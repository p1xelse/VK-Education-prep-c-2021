#include "client.h"

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 128

void print_field_info() {
  puts(
      "1 number account: \n"
      "2 client name: \n"
      "3 Surname: \n"
      "4 Addres client: \n"
      "5 client Telnum: \n"
      "6 client indebtedness: \n"
      "7 client credit limit: \n"
      "8 client cash payments: \n");
}

int read_client(client_t *client, FILE *f) {
  if (client == NULL || f == NULL) return ERR_READ_CLIENT;

  char format_string[BUF_SIZE];
  snprintf(format_string, BUF_SIZE, "%%%dd%%%ds%%%ds%%%ds%%%ds%%%dlf%%%dlf%%%dlf",
          NUMBER_MAX_LEN, NAME_MAX_LEN, SURNAME_MAX_LEN, ADDRES_MAX_LEN,
          TEL_NUMBER_MAX_LEN, DOBLE_VALUE_MAX_LEN, DOBLE_VALUE_MAX_LEN,
          DOBLE_VALUE_MAX_LEN);
  if (fscanf(f, format_string, &client->number, client->name, client->surname,
             client->addres, client->telnumber, &client->indebtedness,
             &client->credit_limit,
             &client->cash_payments) != COUNT_FIELDS_CLIENT)
    return ERR_READ_CLIENT;

  return EXIT_SUCCESS;
}

void write_client(const client_t *client, FILE *f) {
  char format_string[BUF_SIZE];
  snprintf(format_string, BUF_SIZE, "%%-%dd%%-%ds%%-%ds%%-%ds%%%ds%%%d.2f%%%d.2f%%%d.2f\n",
          NUMBER_MAX_LEN, NAME_MAX_LEN, SURNAME_MAX_LEN, ADDRES_MAX_LEN,
          TEL_NUMBER_MAX_LEN, DOBLE_VALUE_MAX_LEN, DOBLE_VALUE_MAX_LEN,
          DOBLE_VALUE_MAX_LEN);
  fprintf(f, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", client->number,
          client->name, client->surname, client->addres, client->telnumber,
          client->indebtedness, client->credit_limit, client->cash_payments);
}
