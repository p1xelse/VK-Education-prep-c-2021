#include <stdio.h>

#include "client.h"
#include "database_operations.h"

#define FILENAME_REC "record.dat"
#define FILENAME_TRANS "transaction.dat"
#define FILENAME_BL_RECORD "blackrecord.dat"

int main(void) {
  setbuf(stdin, NULL);
  int choice = 0;

  FILE *ptr, *ptr_2, *blackrecord;
  client_t client_data, transfer;

  printf("%s",
         "please enter action\n1 enter data client:\n2 enter data "
         "transaction:\n3 update base\n\n");

  while (scanf("%d", &choice) != -1) {
    switch (choice) {
      case 1:
        ptr = fopen(FILENAME_REC, "r+");

        if (ptr == NULL) {
          puts("Not acess");
        } else {
          clients_data_write(ptr, &client_data);
          printf("Success!\n");
          fclose(ptr);
          choice = 0;
        }

        break;

      case 2:
        ptr = fopen(FILENAME_TRANS, "r+");

        if (ptr == NULL) {
          puts("Not acess");
        } else {
          transactions_data_write(ptr, &transfer);
          printf("Success!");
          fclose(ptr);
        }

        break;

      case 3:
        ptr = fopen(FILENAME_REC, "r+");
        ptr_2 = fopen(FILENAME_TRANS, "r+");
        blackrecord = fopen(FILENAME_BL_RECORD, "r+");

        if (ptr == NULL || ptr_2 == NULL || blackrecord == NULL) {
          puts("exit");
        } else {
          update_database(ptr, ptr_2, blackrecord, &client_data, &transfer);
          fclose(ptr);
          fclose(ptr_2);
          fclose(blackrecord);
          printf("Success!");
        }

        break;
      default:
        puts("error");
        break;
    }

    // printf("%s",
    //        "please enter action\n1 enter data client:\n2 enter data "
    //        "transaction:\n3 update base\n4 exit\n");
  }
  return 0;
}
