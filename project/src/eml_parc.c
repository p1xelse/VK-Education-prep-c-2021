#define _GNU_SOURCE

#include "eml_parc.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_SPACE 1
#define NOT_SPACE 0

typedef enum {
  L_FROM,
  L_CTYPE,
  L_TO,
  L_DATE,
  L_ERR,
  L_COUNT,
} lexem_t;

typedef enum {
  S_BEGIN,
  S_FROM_W,
  S_CTYPE_W,
  S_TO_W,
  S_DATE_W,
  S_END,
  S_ERR,
  S_COUNT,
} state_t;

typedef struct {
  lexem_t type;
  char *data;
} elem_t;

void del_space_end(char *s) {
  for (size_t i = strlen(s) - 1; i > 0; i--)
    if (isspace(s[i]))
      s[i] = '\0';
    else
      break;
}

void del_space_beg(char *s) {
  int count = 0;

  for (size_t i = 0; i < strlen(s); i--)
    if (isspace(s[i]))
      count++;
    else
      break;

  memmove(s, s + count, strlen(s) - count + 1);
}

void from_w(char *s, elem_t *elem, FILE *f) {
  if (strchr(s, '\r')) s[strchr(s, '\r') - s] = '\0';

  if (strchr(s, '\n')) s[strchr(s, '\n') - s] = '\0';

  elem->data = strdup(&s[strlen("From:")]);
  del_space_beg(elem->data);
  long pos = ftell(f);
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, f)) != -1) {
    // printf("line = %s\n", line);
    if (line[0] == ' ') {
      if (strchr(line, '\r')) line[strchr(line, '\r') - line] = '\0';

      if (strchr(line, '\n')) line[strchr(line, '\n') - line] = '\0';

      while (line[1] == ' ') memmove(&line[1], &line[2], strlen(line));
      // printf("elem1 = %s\n", elem->data);
      elem->data =
          realloc(elem->data, strlen(elem->data) + 1 + strlen(line) + 1);
      strcat(elem->data, line);
      // printf("elem2 = %s\n", elem->data);
    } else {
      fseek(f, pos, SEEK_SET);
      break;
    }
  }
}

void to_w(char *s, elem_t *elem, FILE *f) {
  if (strchr(s, '\r')) s[strchr(s, '\r') - s] = '\0';

  if (strchr(s, '\n')) s[strchr(s, '\n') - s] = '\0';

  elem->data = strdup(&s[strlen("To: ")]);

  long pos = ftell(f);
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, f)) != -1) {
    if (line[0] == ' ') {
      pos = ftell(f);
      while (line[1] == ' ') memmove(&line[1], &line[2], strlen(line) - 1);
      elem->data =
          realloc(elem->data, strlen(elem->data) + 1 + strlen(line) + 1);
      strcat(elem->data, line);
    } else {
      fseek(f, pos, SEEK_SET);
      break;
    }
  }
}

int isempty(const char *s) {
  while (*s) {
    if (!isspace(*s)) return 0;
    s++;
  }
  return 1;
}

int all_empty_after(FILE *f) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int pos = ftell(f);
  while ((read = getline(&line, &len, f)) != -1) {
    // printf("empty %s\n", line);
    if (!isempty(line)) return 0;
  }

  fseek(f, pos, SEEK_SET);

  return 1;
}

int is_end_del(char *s, char *del) {
  char *p = strstr(s, del);
  if (p[strlen(del)] == '-') return 1;

  return 0;
}

int get_count_bound(FILE *f, char *s) {
  // long pos = ftell(f);
  if (s[0] == '"') {
    memmove(&s[0], &s[1], strlen(s));
    s[strchr(s, '"') - s] = '\0';
  }

  if (s[0] == '=') memmove(&s[0], &s[1], strlen(s));

  if (strchr(s, '\n')) s[strchr(s, '\n') - s] = '\0';

  if (strchr(s, '\r')) s[strchr(s, '\r') - s] = '\0';

  del_space_end(s);
  del_space_beg(s);
  // printf("bound = %s len = %ld\n", s, strlen(s));

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int count = 0;
  int count_bound = 0;

  while ((read = getline(&line, &len, f)) != -1) {
    if (strstr(line, s) && !is_end_del(line, s)) {
      count_bound++;
      // printf("line = %s\n", s);
      if (!all_empty_after(f)) count++;
    }
  }

  // printf("count = %d\n", count);
  // fseek(f, pos, SEEK_SET);
  return count;
}

// int is_false_multipart(FILE *f) {
//   long pos = ftell(f);
//   char *line = NULL;
//   size_t len = 0;
//   ssize_t read;
//   int prev = NOT_SPACE;
//   int k = 0;
//   while ((read = getline(&line, &len, f)) != -1) {
//     if (!isempty(line) && prev == IS_SPACE) {
//       // printf("%d %s", k, line);
//       return 0;
//     }
//     if (isempty(line)) prev = IS_SPACE;

//     k++;
//   }

//   fseek(f, pos, SEEK_SET);

//   return 1;
// }

void ctype_f(char *s, FILE *f, elem_t *elem) {
  // printf("s = %s\n", s);
  int flag_multipart = 0;
  if (strstr(s, "multipart") || strstr(s, "MULTIPART")) flag_multipart = 1;

  long pos = ftell(f);
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while (flag_multipart == 0 && (read = getline(&line, &len, f)) != -1) {
    if (line[0] == ' ') {
      pos = ftell(f);
      if (strstr(line, "multipart") || strstr(line, "MULTIPART"))
        flag_multipart = 1;
    } else {
      fseek(f, pos, SEEK_SET);
      break;
    }
  }

  fseek(f, pos, SEEK_SET);

  int flag_bound = 0;
  char *z = strstr(s, "boundary");

  if (z == NULL) z = strstr(s, "BOUNDARY");

  if (z != NULL && (*(z - 1) != ';') && !isspace(*(z - 1))) {
    elem->data = strdup("1");
    return;
  }

  if (z) flag_bound = 1;

  int count;

  if (flag_bound) {
    count = get_count_bound(f, z + (strlen("boundary=")));
    // printf("count = %d\n", count);
    if (count != 0)
      sprintf(elem->data, "%d", count);
    else {
      elem->data = strdup("0");
    }

    fseek(f, pos, SEEK_SET);

    return;
  }

  // printf("flag = %d", flag_multipart);
  while (flag_multipart == 1 && (read = getline(&line, &len, f)) != -1) {
    if (isspace(line[0]) && (flag_bound == 0)) {
      pos = ftell(f);
      char *p = strstr(line, "boundary");

      if (p == NULL) p = strstr(line, "BOUNDARY");

      if (p != NULL && (*(p - 1) != ';') && !isspace(*(p - 1))) {
        elem->data = strdup("1");
        return;
      }

      if (p) {
        flag_bound = 1;
        count = get_count_bound(f, p + (strlen("boundary=")));
        fseek(f, pos, SEEK_SET);
        if (count != 0)
          sprintf(elem->data, "%d", count);
        else {
          elem->data = "0";
        }
      }
    } else {
      fseek(f, pos, SEEK_SET);
      break;
    }
  }

  // printf("elem.data = %s\ncount = %d\n", elem->data, count);

  if (flag_multipart == 0) elem->data = "1";
}

void date_w(char *s, elem_t *elem, FILE *f) {
  if (strchr(s, '\r')) s[strchr(s, '\r') - s] = '\0';

  if (strchr(s, '\n')) s[strchr(s, '\n') - s] = '\0';

  elem->data = strdup(&s[strlen("Date: ")]);

  long pos = ftell(f);
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, f)) != -1) {
    if (line[0] == ' ') {
      while (line[1] == ' ') memmove(&line[1], &line[2], strlen(line) - 1);
      elem->data =
          realloc(elem->data, strlen(elem->data) + 1 + strlen(line) + 1);
      strcat(elem->data, line);
    } else
      break;
  }

  fseek(f, pos, SEEK_SET);
}

// int count_parts(FILE *f, )

typedef void (*action_str_t)(char *s, elem_t *elem, FILE *f);
typedef void (*action_f_t)(char *s, FILE *f, elem_t *elem);

typedef struct {
  state_t state;
  action_str_t action;
  action_f_t f_act;
} rule_t;

rule_t syntax[S_COUNT][L_COUNT] = {
    //		            L_FROM, 		    L_CTYPE, 	        L_TO,
    //L_DATE
    /*S_BEGIN*/ {{S_END, from_w, NULL},
                 {S_END, NULL, ctype_f},
                 {S_END, to_w, NULL},
                 {S_END, date_w, NULL}},
    /*S_FROM_W*/
    {{S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL}},
    /*S_CTYPE_W*/
    {{S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL}},
    /*S_TO_W*/
    {{S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL}},
    /*S_DATE_W*/
    {{S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL},
     {S_ERR, NULL, NULL}}};

lexem_t get_lexem(const char *s) {
  // printf("%s\n", s);
  if (!strncmp(s, "From:", strlen("From:"))) return L_FROM;
  if (!strncmp(s, "To:", strlen("To:"))) return L_TO;
  if (!strncmp(s, "Date:", strlen("Date:"))) return L_DATE;
  if (!strncmp(s, "Content-Type:", strlen("Content-Type:"))) return L_CTYPE;

  return L_ERR;
}

int str_eml_parse(char *s, elem_t *elem, FILE *f, elem_t *out_arr) {
  state_t state = S_BEGIN;

  lexem_t lexem = get_lexem(s);
  //   printf("%d", lexem);

  if (lexem == L_ERR) return EXIT_FAILURE;

  if (lexem == out_arr[0].type || lexem == out_arr[1].type ||
      lexem == out_arr[2].type || lexem == out_arr[3].type)
    return EXIT_FAILURE;

  elem->type = lexem;
  rule_t rule = syntax[state][lexem];

  if (rule.f_act != NULL) rule.f_act(s, f, elem);
  if (rule.action != NULL) rule.action(s, elem, f);

  return EXIT_SUCCESS;
}

void file_eml_parse(FILE *f) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  elem_t out_arr[4] = {
      {L_ERR, NULL}, {L_ERR, NULL}, {L_ERR, NULL}, {L_ERR, NULL}};
  elem_t elem;

  while ((read = getline(&line, &len, f)) != -1) {
    // printf("lne = %s\n", line);
    int res = str_eml_parse(line, &elem, f, out_arr);

    if (res != EXIT_FAILURE) {
      if (strchr(elem.data, '\r'))
        elem.data[strchr(elem.data, '\r') - elem.data] = '\0';
      if (strchr(elem.data, '\n'))
        elem.data[strchr(elem.data, '\n') - elem.data] = '\0';
      if (out_arr[elem.type].type == L_ERR) {
        out_arr[elem.type].type = elem.type;
        out_arr[elem.type].data = strdup(elem.data);
        // printf("%d %s\n\n", elem.type, elem.data);
        // printf("%d %s\n\n", out_arr[elem.type].type,
        // out_arr[elem.type].data);
      }

      // printf("%d %s\n", elem.type,  out_arr[elem.type].data);
    }
    if (out_arr[0].type != L_ERR && out_arr[1].type != L_ERR &&
        out_arr[2].type != L_ERR && out_arr[3].type != L_ERR)
      break;
  }
  // printf("%d %s\n\n", out_arr[2].type, out_arr[2].data);
  if (out_arr[L_FROM].data == NULL)
    printf("|");
  else
    printf("%s|", out_arr[L_FROM].data);

  if (out_arr[L_TO].data == NULL)
    printf("|");
  else
    printf("%s|", out_arr[L_TO].data);

  if (out_arr[L_DATE].data == NULL)
    printf("|");
  else
    printf("%s|", out_arr[L_DATE].data);

  if (out_arr[L_CTYPE].data == NULL)
    printf("1");
  else
    printf("%s", out_arr[L_CTYPE].data);

  // printf("%s|%s|%s|%s\n", out_arr[L_FROM].data, out_arr[L_TO].data,
  // out_arr[L_DATE].data, out_arr[L_CTYPE].data);
}
