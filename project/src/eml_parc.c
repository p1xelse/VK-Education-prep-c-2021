#define _GNU_SOURCE

#include "eml_parc.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_SPACE 1
#define NOT_SPACE 0
#define BUF_NUM_SIZE 5

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

  while ((getline(&line, &len, f)) != -1) {
    if (line[0] == ' ') {
      if (strchr(line, '\r')) line[strchr(line, '\r') - line] = '\0';

      if (strchr(line, '\n')) line[strchr(line, '\n') - line] = '\0';

      while (line[1] == ' ') memmove(&line[1], &line[2], strlen(line));
      elem->data =
          realloc(elem->data, strlen(elem->data) + 1 + strlen(line) + 1);
      strncat(elem->data, line, strlen(line));
    } else {
      fseek(f, pos, SEEK_SET);
      break;
    }
  }
  free(line);
}

void to_w(char *s, elem_t *elem, FILE *f) {
  if (strchr(s, '\r')) s[strchr(s, '\r') - s] = '\0';

  if (strchr(s, '\n')) s[strchr(s, '\n') - s] = '\0';

  elem->data = strdup(&s[strlen("To: ")]);

  long pos = ftell(f);
  char *line = NULL;
  size_t len = 0;

  while ((getline(&line, &len, f)) != -1) {
    if (line[0] == ' ') {
      pos = ftell(f);
      while (line[1] == ' ') memmove(&line[1], &line[2], strlen(line) - 1);
      elem->data =
          realloc(elem->data, strlen(elem->data) + 1 + strlen(line) + 1);
      strncat(elem->data, line, strlen(line));
    } else {
      fseek(f, pos, SEEK_SET);
      break;
    }
  }
  free(line);
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
  int pos = ftell(f);
  while ((getline(&line, &len, f)) != -1) {
    if (!isempty(line)) {
      free(line);
      return 0;
    }
  }

  free(line);

  fseek(f, pos, SEEK_SET);

  return 1;
}

int is_end_del(char *s, char *del) {
  char *p = strstr(s, del);
  if (p[strlen(del)] == '-') return 1;

  return 0;
}

int get_count_bound(FILE *f, char *s) {
  if (s[0] == '"') {
    memmove(&s[0], &s[1], strlen(s));
    s[strchr(s, '"') - s] = '\0';
  }

  if (s[0] == '=') memmove(&s[0], &s[1], strlen(s));

  if (strchr(s, '\n')) s[strchr(s, '\n') - s] = '\0';

  if (strchr(s, '\r')) s[strchr(s, '\r') - s] = '\0';

  del_space_end(s);
  del_space_beg(s);

  char *line = NULL;
  size_t len = 0;
  int count = 0;
  int count_bound = 0;

  while ((getline(&line, &len, f)) != -1) {
    if (strstr(line, s) && !is_end_del(line, s)) {
      count_bound++;
      // printf("line = %s\n", s);
      if (!all_empty_after(f)) count++;
    }
  }

  free(line);

  return count;
}

void ctype_f(char *s, FILE *f, elem_t *elem) {
  int flag_multipart = 0;
  if (strstr(s, "multipart") || strstr(s, "MULTIPART")) flag_multipart = 1;

  long pos = ftell(f);
  char *line = NULL;
  size_t len = 0;

  while (flag_multipart == 0 && (getline(&line, &len, f)) != -1) {
    if (line[0] == ' ') {
      pos = ftell(f);
      if (strstr(line, "multipart") || strstr(line, "MULTIPART"))
        flag_multipart = 1;
    } else {
      fseek(f, pos, SEEK_SET);
      break;
    }
  }
  free(line);

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
    if (count != 0) {
      char buf[BUF_NUM_SIZE];
      snprintf(buf, sizeof(buf), "%d", count);
      elem->data = strdup(buf);
    } else {
      elem->data = strdup("0");
    }

    fseek(f, pos, SEEK_SET);

    return;
  }

  line = NULL;
  len = 0;

  while (flag_multipart == 1 && (getline(&line, &len, f)) != -1) {
    if (isspace(line[0]) && (flag_bound == 0)) {
      pos = ftell(f);
      char *p = strstr(line, "boundary");

      if (p == NULL) p = strstr(line, "BOUNDARY");

      if (p != NULL && (*(p - 1) != ';') && !isspace(*(p - 1))) {
        elem->data = strdup("1");
        free(line);
        return;
      }

      if (p) {
        flag_bound = 1;
        count = get_count_bound(f, p + (strlen("boundary=")));
        fseek(f, pos, SEEK_SET);
        if (count != 0) {
          char buf[BUF_NUM_SIZE];
          snprintf(buf, sizeof(buf), "%d", count);
          elem->data = strdup(buf);
        } else {
          elem->data = strdup("0");
        }
      }
    } else {
      fseek(f, pos, SEEK_SET);
      break;
    }
  }
  free(line);

  // printf("elem.data = %s\ncount = %d\n", elem->data, count);

  if (flag_multipart == 0) elem->data = strdup("1");
}

void date_w(char *s, elem_t *elem, FILE *f) {
  if (strchr(s, '\r')) s[strchr(s, '\r') - s] = '\0';

  if (strchr(s, '\n')) s[strchr(s, '\n') - s] = '\0';

  elem->data = strdup(&s[strlen("Date: ")]);

  long pos = ftell(f);
  char *line = NULL;
  size_t len = 0;

  while ((getline(&line, &len, f)) != -1) {
    if (line[0] == ' ') {
      while (line[1] == ' ') memmove(&line[1], &line[2], strlen(line) - 1);
      elem->data =
          realloc(elem->data, strlen(elem->data) + 1 + strlen(line) + 1);
      strncat(elem->data, line, strlen(line));
    } else {
      break;
    }
  }
  free(line);

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
    // L_DATE
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
  elem_t out_arr[4] = {
      {L_ERR, NULL}, {L_ERR, NULL}, {L_ERR, NULL}, {L_ERR, NULL}};

  while ((getline(&line, &len, f)) != -1) {
    if (out_arr[0].type != L_ERR && out_arr[1].type != L_ERR &&
        out_arr[2].type != L_ERR && out_arr[3].type != L_ERR)
      break;

    elem_t elem = {L_ERR, NULL};
    int res = str_eml_parse(line, &elem, f, out_arr);

    if (res != EXIT_FAILURE && elem.data != NULL) {
      if (strchr(elem.data, '\r'))
        elem.data[strchr(elem.data, '\r') - elem.data] = '\0';
      if (strchr(elem.data, '\n'))
        elem.data[strchr(elem.data, '\n') - elem.data] = '\0';
      if (out_arr[elem.type].type == L_ERR) {
        // out_arr[elem.type].type = elem.type;
        // free(out_arr[elem.type].data);
        // out_arr[elem.type].data = strdup(elem.data);
        if (elem.type == L_FROM) {
          out_arr[L_FROM].type = elem.type;
          out_arr[L_FROM].data = strdup(elem.data);
        }
        if (elem.type == L_TO) {
          out_arr[L_TO].type = elem.type;
          out_arr[L_TO].data = strdup(elem.data);
        }
        if (elem.type == L_DATE) {
          out_arr[L_DATE].type = elem.type;
          out_arr[L_DATE].data = strdup(elem.data);
        }
        if (elem.type == L_CTYPE) {
          out_arr[L_CTYPE].type = elem.type;
          out_arr[L_CTYPE].data = strdup(elem.data);
        }
      }
    }

    free(elem.data);
  }
  free(line);

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

  free(out_arr[L_CTYPE].data);
  free(out_arr[L_DATE].data);
  free(out_arr[L_FROM].data);
  free(out_arr[L_TO].data);
}
