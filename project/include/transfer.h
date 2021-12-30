#ifndef PROJECT_INCLUDE_TRANSFER_H_
#define PROJECT_INCLUDE_TRANSFER_H_

#include <stdio.h>

#define COUNT_FIELDS_TRANSFER 2

#define ERR_READ_TRANSFER 10

typedef struct {
  int number;
  double cash_payments;
} transfer_t;


int read_transfer(transfer_t *transfer, FILE *f);
void write_transfer(transfer_t *transfer, FILE *f);

#endif  // PROJECT_INCLUDE_TRANSFER_H_
