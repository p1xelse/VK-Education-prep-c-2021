#ifndef PROJECT_INCLUDE_CLIENT_H_
#define PROJECT_INCLUDE_CLIENT_H_

#include <stdio.h>

enum {
  NUMBER_MAX_LEN = 12,
  NAME_MAX_LEN = 11,
  SURNAME_MAX_LEN = 11,
  ADDRES_MAX_LEN = 16,
  TEL_NUMBER_MAX_LEN = 20,
  DOBLE_VALUE_MAX_LEN = 12
};

#define COUNT_FIELDS_CLIENT 8
#define ERR_READ_CLIENT 11

typedef struct {
  int number;
  char name[NAME_MAX_LEN];
  char surname[SURNAME_MAX_LEN];
  char addres[ADDRES_MAX_LEN];
  char telnumber[TEL_NUMBER_MAX_LEN];
  double indebtedness;
  double credit_limit;
  double cash_payments;
} client_t;

void print_field_info();
void write_client(const client_t *client, FILE *f);
int read_client(client_t *client, FILE *f);

#endif  // PROJECT_INCLUDE_CLIENT_H_
