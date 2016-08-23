#include "cll1p_token.h"

static int cll1p_err()
{
  perror("cll1p: ");
  return (0);
}

static void *cll1p_err_ptr()
{
  perror("cll1p: ");
  return (0);
}

static cll1p_list *push_token(cll1p_list *list, char *token, char *value, int id)
{
    cll1p_list *node = malloc(sizeof(*node));

    if (!node)
      return (cll1p_err_ptr());
    strcpy(node->token.token, token);
    node->token.value = value;
    node->next = list;
    return (node);
}

static int cll1p_is_number(char *stream, int i, cll1p_list **list)
{
  char *substr;
  int  start;

  start = i;
  while (isdigit(stream[i]))
    ++i;
  if (!stream[i] || isspace(stream[i]))
  {
    if (!(substr = strndup(&stream[start], i - start)))
      return (cll1p_err());
    if (!(*list = push_token(*list, CLL1P_NUMBER, substr, 1)))
      return (cll1p_err());
    return (i);
  }
  return (-1);
}

static int cll1p_is_string(char *stream, int i, cll1p_list **list)
{
  int start;
  char *substr;

  start = i;
  while (!isspace(stream[i]))
    ++i;
  if (i != start)
    {
      if (!(substr = strndup(&stream[start], i - start)))
        return (cll1p_err());
      if (!(*list = push_token(*list, CLL1P_STRING, substr, 2)))
        return (cll1p_err());
      return (i);
    }
  return (-1);
}

static int cll1p_is_space(char *stream, int i, cll1p_list **list)
{
  int start;
  char *substr;

  start = i;
  while (isspace(stream[i]))
    ++i;
  if (i != start)
    {
      if (!(substr = strndup(&stream[start], i - start)))
        return (cll1p_err());
      if (!(*list = push_token(*list, CLL1P_SPACE, substr, 3)))
        return (cll1p_err());
      return (i);
    }
  return (-1);
}

static void cll1p_init_checks(int (*checks[3])(char *, int, cll1p_list **list), int number_as_string)
{
  checks[0] = cll1p_is_space;
  if (number_as_string)
  {
    checks[1] = cll1p_is_number;
    checks[2] = cll1p_is_string;
  }
  else
  {
    checks[1] = cll1p_is_string;
    checks[2] = 0;
  }
  checks[3] = 0;
}

static cll1p_token cll1p_convert_list_to_array(cll1p_list *list, int length)
{
  cll1p_token tokens = malloc(sizeof(t_cll1p_token) * (length + 1));
  int i = 0;

  if (!tokens)
    return (cll1p_err_ptr());
  while (i < length)
  {
    memcpy(&tokens[length - i - 1], &list->token, sizeof(t_cll1p_token));
    list = list->next;
    ++i;
  }
  strcpy(tokens[length].token, CLL1P_UNDEF);
  return (tokens);
}

static void cll1p_free_list(cll1p_list *list)
{
    if (list->next)
      cll1p_free_list(list->next);
    free(list);
}

cll1p_token cll1p_tokenize(char *stream, int number_as_string)
{
  int j;
  int i = 0;
  int ret;
  int s_length = strlen(stream);
  int length = 0;
  int (*checks[3])(char *, int, cll1p_list **list);
  cll1p_list *list = 0;
  cll1p_token tokens;

  cll1p_init_checks(checks, number_as_string);
  while (i < s_length)
  {
    j = 0;
    while (checks[j])
    {
      ret = checks[j](stream, i, &list);
      if (ret != -1)
      {
        i = ret;
        ++length;
        break ;
      }
      ++j;
    }
    if (!checks[j])
      {
        fprintf(stderr, "Invalid token found\n");
        return (0);
      }
  }
  tokens = cll1p_convert_list_to_array(list, length);
  cll1p_free_list(list);
  return (tokens);
}
