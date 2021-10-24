#include <string.h>

#include "client.h"
#include "database_operations.h"

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

  client_t expected_data;
  expected_data.number = 1;
  strncpy(expected_data.name, "Dima", MAX_LEN_NAME);
  strncpy(expected_data.surname, "Programmer", MAX_LEN_SURNAME);
  strncpy(expected_data.addres, "Moskow", MAX_LEN_ADRESS);
  strncpy(expected_data.telnumber, "1488", MAX_LEN_TEL_NUMBER);
  expected_data.indebtedness = 100;
  expected_data.credit_limit = 205;
  expected_data.cash_payments = 101;

  FILE *f = fopen(filename, "w+");
  write_client(&expected_data, f);
  rewind(f);

  client_t got_data;
  read_client(&got_data, f);

  if (compare_clients(&got_data, &expected_data) == 0)
    printf("test_write_client: SUCCESS\n");
  fclose(f);
}

void test_update_database() {
  const char *filename_rec = "test_rec.txt";

  client_t expected_data;
  expected_data.number = 1;
  strncpy(expected_data.name, "Dima", MAX_LEN_NAME);
  strncpy(expected_data.surname, "Programmer", MAX_LEN_SURNAME);
  strncpy(expected_data.addres, "Moskow", MAX_LEN_ADRESS);
  strncpy(expected_data.telnumber, "1488", MAX_LEN_TEL_NUMBER);
  expected_data.indebtedness = 100;
  expected_data.credit_limit = 227;
  expected_data.cash_payments = 101;

  client_t client_data;
  client_data.number = 1;
  strncpy(client_data.name, "Dima", MAX_LEN_NAME);
  strncpy(client_data.surname, "Programmer", MAX_LEN_SURNAME);
  strncpy(client_data.addres, "Moskow", MAX_LEN_ADRESS);
  strncpy(client_data.telnumber, "1488", MAX_LEN_TEL_NUMBER);
  client_data.indebtedness = 100;
  client_data.credit_limit = 205;
  client_data.cash_payments = 101;

  const char *filename_trans = "test_transf.txt";
  client_t transfer;
  transfer.number = 1;
  transfer.cash_payments = 22;

  FILE *f_rec = fopen(filename_rec, "w+");
  write_client(&client_data, f_rec);
  rewind(f_rec);

  FILE *f_transf = fopen(filename_trans, "w+");
  write_transaction(&transfer, f_transf);
  rewind(f_transf);

  const char *filename_blackrec = "test_blackrecord.txt";
  FILE *f_blackrecord = fopen(filename_blackrec, "w+");
  client_t rec_buf, transfer_buf;
  update_database(f_rec, f_transf, f_blackrecord, &rec_buf, &transfer_buf);
  rewind(f_blackrecord);

  client_t got_data;
  read_client(&got_data, f_blackrecord);

  if (compare_clients(&got_data, &expected_data) == 0)
    printf("test_update_database: SUCCESS\n");

  fclose(f_transf);
  fclose(f_rec);
  fclose(f_blackrecord);
}

int main() {
  test_write_client();
  test_update_database();
}
