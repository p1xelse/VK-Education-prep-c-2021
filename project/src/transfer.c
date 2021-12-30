#include <stdio.h>
#include <stdlib.h>

#include "transfer.h"

int read_transfer(transfer_t *transfer, FILE *f) {
  if (fscanf(f, "%d %lf", &transfer->number, &transfer->cash_payments) !=
      COUNT_FIELDS_TRANSFER) {
    return ERR_READ_TRANSFER;
  }
  return EXIT_SUCCESS;
}
void write_transfer(transfer_t *transfer, FILE *f) {
  fprintf(f, "%-2d %4.2lf \n", transfer->number, transfer->cash_payments);
}
