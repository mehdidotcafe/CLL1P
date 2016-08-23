#include "cll1p.h"

cll1p_table   cll1p_create_table(unsigned int x, unsigned int y, unsigned int mxt)
{
  cll1p_table table;
  int         i_x;
  int         i_y;
  int         i_mxt;
  int         i;

  i = 0;
  // add "cll1p_undef" at the end
  ++mxt;
  table = malloc(sizeof(void *) * (x + 1));
  if (!table)
    {
      perror("cll1p: ");
      return (0);
    }
  table[x] = 0;
  for (i_x = 0; i_x < x; i_x++)
  {
    table[i_x] = malloc(sizeof(void *) * (y + 1));
    table[i_x][y] = 0;
    for (i_y = 0; i_y < y; i_y++)
    {
      table[i_x][i_y] = malloc(sizeof(t_cll1p_token) * (mxt + 1));
      for (i_mxt = 0; i_mxt < mxt; i_mxt++)
      {
        strcpy(table[i_x][i_y][i_mxt].token, CLL1P_UNDEF);
        table[i_x][i_y][i_mxt].value = 0;
        ++i;
      }
    }
  }
  return (table);
}

void cll1p_delete_table(cll1p_table table)
{
  int i_x;
  int i_y;

  i_x = 0;
  while (table[i_x])
    {
      i_y = 0;
      while (table[i_y])
      {
        free(table[i_x][i_y]);
        ++i_y;
      }
      free(table[i_x]);
      ++i_x;
    }
  free(table);
}

static cll1p_list *pop_first_token(cll1p_list *stack)
{
  cll1p_list *next;

  next = stack->next;
  free(stack);
  return (next);
}

static cll1p_list *push_first_token(cll1p_list *stack, cll1p_token token)
{
  cll1p_list *node = malloc(sizeof(*node));

  if (!node)
    return (0);
  memcpy(&node->token, token, sizeof(*token));
  node->next = stack;
  return (node);
}

static cll1p_list *push_table_tokens_to_stack(cll1p_list *stack, cll1p_token tokens)
{
  int i;

  i = 0;
  while (strcmp(tokens[i].token, CLL1P_UNDEF))
    ++i;
  while (--i >= 0)
    stack = push_first_token(stack, &tokens[i]);
  return (stack);
}

static cll1p_list *cll1p_init_stack()
{
  cll1p_list *stack = malloc(sizeof(*stack));

  if (!stack)
    {
      perror("cll1p: ");
      return (0);
    }
  strcpy(stack->token.token, CLL1P_EXPR);
  stack->token.value = 0;
  stack->next = 0;
  return (stack);
}

void cll1p_free_stack(cll1p_list *stack)
{
  if (stack->next)
    cll1p_free_stack(stack->next);
  free(stack);
}

int cll1p_parse(cll1p_token tokens, cll1p_table table, cll1p_grammar grammar, void (*fx)(void *data, cll1p_token token), void *data)
{
  cll1p_list *stack;
  cll1p_token stack_token;
  cll1p_token curr_token;
  unsigned int i;
  unsigned int curr_token_id;
  unsigned int stack_token_id;

  i = 0;
  if (!(stack = cll1p_init_stack()))
    return (0);
  while (strcmp(tokens[i].token, CLL1P_UNDEF))
  {
    if (!stack)
      return (0);
    curr_token = &tokens[i];
    stack_token = &stack->token;
    stack_token_id = cll1p_get_grammar_id(grammar, stack_token->token);
    curr_token_id = cll1p_get_grammar_id(grammar, curr_token->token);
    if (!strcmp(curr_token->token, stack_token->token))
      {
        if (fx)
          fx(data, curr_token);
        stack = pop_first_token(stack);
        ++i;
      }
    else if (table[stack_token_id][curr_token_id] && strcmp(table[stack_token_id][curr_token_id][0].token, CLL1P_UNDEF) != 0)
      {
        stack = pop_first_token(stack);
        if (strcmp(table[stack_token_id][curr_token_id][0].token, CLL1P_EPSILON) != 0)
          if (!(stack = push_table_tokens_to_stack(stack, table[stack_token_id][curr_token_id])))
            return (0);
      }
    else
      return (0);
  }
  if (stack)
    {
      cll1p_free_stack(stack);
      return (0);
    }
  return (1);
}

cll1p_grammar cll1p_create_grammar(char **tokens)
{
  int i = 0;
  cll1p_grammar grammar;

  while (tokens[i])
    ++i;
  ++i;
  if (!(grammar = malloc(sizeof(t_cll1p_grammar) * i)))
    {
      perror("cll1p: ");
      return (0);
    }
  i = 0;
  while (tokens[i])
  {
    strcpy(grammar[i].token, tokens[i]);
    grammar[i].id = i;
    ++i;
  }
  grammar[i].id = -1;
  return (grammar);
}

int cll1p_get_grammar_id(cll1p_grammar grammar, char *token)
{
  int i = 0;

  while (grammar[i].id != -1)
  {
    if (!strcmp(grammar[i].token, token))
      return ((int)grammar[i].id);
    ++i;
  }
  return (-1);
}


void cll1p_delete_grammar(cll1p_grammar grammar)
{
  free(grammar);
}
