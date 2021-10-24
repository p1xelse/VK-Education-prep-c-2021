#ifndef PROJECT_INCLUDE_CLIENT_H_
#define PROJECT_INCLUDE_CLIENT_H_

#include <stdio.h>

#define MAX_LEN_NAME 20
#define MAX_LEN_SURNAME 20
#define MAX_LEN_ADRESS 30
#define MAX_LEN_TEL_NUMBER 15
#define COUNT_FIELDS_CLIENT 8
#define COUNT_FIELDS_TRANSFER 2

#define ERR_READ 10

typedef struct {
  int number;
  char name[MAX_LEN_NAME];
  char surname[MAX_LEN_SURNAME];
  char addres[MAX_LEN_ADRESS];
  char telnumber[MAX_LEN_TEL_NUMBER];
  double indebtedness;
  double credit_limit;
  double cash_payments;
} client_t;

void print_field_info();
void write_client(client_t *client, FILE *f);
int read_client(client_t *client, FILE *f);
int read_transaction(client_t *transfer, FILE *f);
void write_transaction(client_t *transfer, FILE *f);

#endif  // PROJECT_INCLUDE_CLIENT_H_
