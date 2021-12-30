#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "client.h"
#include "database_operations.h"
#include "transfer.h"

int compare_clients(client_t *cl_1, client_t *cl_2) {
  int rc = 0;
  rc = rc || (cl_1->number != cl_2->number);
  rc = rc || (cl_1->indebtedness != cl_2->indebtedness);
  rc = rc || (cl_1->credit_limit != cl_2->credit_limit);
  rc = rc || (cl_1->cash_payments != cl_2->cash_payments);
  rc = rc || (strcmp(cl_1->name, cl_1->name));
  rc = rc || (strcmp(cl_1->surname, cl_1->surname));
  rc = rc || (strcmp(cl_1->addres, cl_1->addres));
  rc = rc || (strcmp(cl_1->telnumber, cl_1->telnumber));

  return rc;
}

void test_write_client() {
  const char *filename = "test_write.txt";

  client_t expected_data = {.number = 1,
                            .name = "Dima",
                            .surname = "Programmer",
                            .addres = "Moskow",
                            .telnumber = "1488",
                            .indebtedness = 100,
                            .credit_limit = 205,
                            .cash_payments = 101};

  FILE *f = fopen(filename, "w+");
  if (f == NULL) {
    printf("Error open file during test");
    return;
  }
  write_client(&expected_data, f);
  rewind(f);

  client_t got_data;
  if (read_client(&got_data, f) != EXIT_SUCCESS) {
    printf("Error read client during test");
    return;
  }

  if (compare_clients(&got_data, &expected_data) == 0)
    printf("test_write_client: SUCCESS\n");
  fclose(f);
}

void test_update_database() {
  const char *filename_rec = "test_rec.txt";

  client_t expected_data = {.number = 1,
                            .name = "Dima",
                            .surname = "Programmer",
                            .addres = "Moskow",
                            .telnumber = "1488",
                            .indebtedness = 100,
                            .credit_limit = 227,
                            .cash_payments = 101};

  client_t client_data = expected_data;
  client_data.credit_limit = 205;

  const char *filename_trans = "test_transf.txt";
  transfer_t transfer;
  transfer.number = 1;
  transfer.cash_payments =
      expected_data.credit_limit - client_data.credit_limit;

  FILE *f_rec = fopen(filename_rec, "w+");

  if (f_rec == NULL) {
    printf("Error open file during test");
    return;
  }

  write_client(&client_data, f_rec);
  rewind(f_rec);

  FILE *f_transf = fopen(filename_trans, "w+");
  if (f_transf == NULL) {
    printf("Error open file during test");
    fclose(f_rec);
    return;
  }
  write_transfer(&transfer, f_transf);
  rewind(f_transf);

  const char *filename_blackrec = "test_blackrecord.txt";
  FILE *f_blackrecord = fopen(filename_blackrec, "w+");
  if (f_blackrecord == NULL) {
    printf("Error open file during test");
    fclose(f_rec);
    fclose(f_transf);
    return;
  }
  client_t rec_buf, transfer_buf;
  db_update(f_rec, f_transf, f_blackrecord, &rec_buf, &transfer_buf);
  rewind(f_blackrecord);

  client_t got_data;
  if (read_client(&got_data, f_blackrecord) != EXIT_SUCCESS) {
    printf("Error read client during test");
    fclose(f_rec);
    fclose(f_transf);
    return;
  }

  int res_cmp = compare_clients(&got_data, &expected_data);
  assert(res_cmp == 0);
  if (res_cmp == 0)
    printf("test_update_database: SUCCESS\n");

  fclose(f_transf);
  fclose(f_rec);
  fclose(f_blackrecord);
}

int main() {
  test_write_client();
  test_update_database();
}
