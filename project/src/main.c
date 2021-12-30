#include <stdio.h>

#include "client.h"
#include "transfer.h"
#include "database_operations.h"

#define FILENAME_REC "record.dat"
#define FILENAME_TRANS "transaction.dat"
#define FILENAME_BL_RECORD "blackrecord.dat"

int main(void) {
  setbuf(stdin, NULL);
  int choice = 0;

  FILE *db_clients, *db_transfers, *blackrecord;
  client_t client_data, transfer;

  printf("%s",
         "please enter action\n1 enter data client:\n2 enter data "
         "transaction:\n3 update base\n\n");

  while (scanf("%d", &choice) != -1) {
    switch (choice) {
      case 1:

        db_clients = fopen(FILENAME_REC, "w");

        if (db_clients == NULL) {
          puts("Not acess");
        } else {
          db_write_clients(db_clients, &client_data);
          printf("Success!\n");
          fclose(db_clients);
          choice = 0;
        }

        break;

      case 2:

        db_transfers = fopen(FILENAME_TRANS, "w");

        if (db_transfers == NULL) {
          puts("Not acess");
        } else {
          db_write_transfers(db_transfers, &transfer);
          printf("Success!");
          fclose(db_transfers);
        }

        break;

      case 3:
        db_clients = fopen(FILENAME_REC, "r+");
        db_transfers = fopen(FILENAME_TRANS, "r+");
        blackrecord = fopen(FILENAME_BL_RECORD, "r+");


        if (db_clients == NULL || db_transfers == NULL || blackrecord == NULL) {
          if (db_clients != NULL)
            fclose(db_clients);
          if (db_transfers != NULL)
            fclose(db_transfers);
          if (blackrecord != NULL)
            fclose(blackrecord);
          puts("exit");
        } else {
          db_update(db_clients, db_transfers, blackrecord, &client_data, &transfer);
          fclose(db_clients);
          fclose(db_transfers);
          fclose(blackrecord);
          printf("Success!");
        }

        break;
      default:
        puts("error");
        break;
    }

    printf("%s",
           "please enter action\n1 enter data client:\n2 enter data "
           "transaction:\n3 update base\n");
  }
  return 0;
}
