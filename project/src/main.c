#include <stdio.h>

#include "utils.h"

#define FILENAME "transaction.dat"

struct masterRecord {
  int Number;
  char Name[20];
  char Surname[20];
  char addres[30];
  char TelNumber[15];
  double indebtedness;
  double credit_limit;
  double cash_payments;
};

typedef struct masterRecord Data;

void master_write(FILE *ofPTR, Data Client);
void transaction_write(FILE *ofPTR, Data transfer);
void black_record(FILE *ofPTR, FILE *ofPTR_2, FILE *blackrecord,
                 Data client_data, Data transfer);

int main(void) {
  int choice = 0;

  FILE *ptr, *ptr_2, *blackrecord;
  Data client_data, transfer;
  printf("%s",
         "please enter action\n1 enter data client:\n2 enter data "
         "transaction:\n3 update base\n");

  while (scanf("%d", &choice) != -1) {
    switch (choice) {
      case 1:
        ptr = fopen("record.dat", "r+");

        if (ptr == NULL) {
          puts("Not acess");
        } else {
          masterWrite(ptr, client_data);
          fclose(ptr);
        }

        break;

      case 2:
        ptr = fopen(FILENAME, "r+");

        if (ptr == NULL) {
          puts("Not acess");
        } else {
          transactionWrite(ptr, transfer);
          fclose(ptr);
        }

        break;
      case 3:
        ptr = fopen("record.dat", "r");
        ptr_2 = fopen("transaction.dat", "r");
        blackrecord = fopen("blackrecord.dat", "w");

        if (ptr == NULL || ptr_2 == NULL || blackrecord == NULL) {
          puts("exit");
        } else {
          blackRecord(ptr, ptr_2, blackrecord, client_data, transfer);
          fclose(ptr);
          fclose(ptr_2);
          fclose(blackrecord);
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

void master_write(FILE *ofPTR, Data Client) {
  printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
         "1 Number account: ", "2 Client name: ", "3 Surname: ",
         "4 Addres client: ", "5 Client Telnum: ", "6 Client indebtedness: ",
         "7 Client credit limit: ", "8 Client cash payments: ");

  while (scanf("%12d%11s%11s%16s%20s%12lf%12lf%12lf", &Client.Number,
               Client.Name, Client.Surname, Client.addres, Client.TelNumber,
               &Client.indebtedness, &Client.credit_limit,
               &Client.cash_payments) != -1) {
    fprintf(ofPTR, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
            Client.Number, Client.Name, Client.Surname, Client.addres,
            Client.TelNumber, Client.indebtedness, Client.credit_limit,
            Client.cash_payments);
  }
}

void transaction_write(FILE *ofPTR, Data transfer) {
  printf("%s\n%s\n", "1 Number account: ", "2 Client cash payments: ");
  while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) != -1) {
    fprintf(ofPTR, "%-3d%-6.2f\n", transfer.Number, transfer.cash_payments);
    printf("%s\n%s\n", "1 Number account:", "2 Client cash payments: ");
  }
}
void black_record(FILE *ofPTR, FILE *ofPTR_2, FILE *blackrecord,
                 Data client_data, Data transfer) {
  while (fscanf(ofPTR, "%12d%11s%11s%16s%20s%12lf%12lf%12lf",
                &client_data.Number, client_data.Name, client_data.Surname,
                client_data.addres, client_data.TelNumber,
                &client_data.indebtedness, &client_data.credit_limit,
                &client_data.cash_payments) != -1) {
    while (fscanf(ofPTR_2, "%d %lf", &transfer.Number,
                  &transfer.cash_payments) != -1) {
      if (client_data.Number == transfer.Number && transfer.cash_payments != 0)
        client_data.credit_limit += transfer.cash_payments;
    }
    fprintf(blackrecord, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
            client_data.Number, client_data.Name, client_data.Surname,
            client_data.addres, client_data.TelNumber, client_data.indebtedness,
            client_data.credit_limit, client_data.cash_payments);
    rewind(ofPTR_2);
  }
}
