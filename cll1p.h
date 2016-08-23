#ifndef CLL1P_H_
# define CLL1P_H_

# define CLL1P_TOKEN_LENGTH 64
/*
  Some useful token used by cll1p
  */
# define CLL1P_EXPR "cll1p_expr"
# define CLL1P_EPSILON "cll1p_epsilon"
# define CLL1P_UNDEF "cll1p_undef"


/*
  table idx for expr*/
# define CLL1P_EXPR_IDX 0


# include <string.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_cll1p_token
{
  char                token[CLL1P_TOKEN_LENGTH];
  char                *value;
}             t_cll1p_token;

typedef t_cll1p_token* cll1p_token;

typedef struct s_cll1p_grammar
{
  char                    token[CLL1P_TOKEN_LENGTH];
  int                    id;
}              t_cll1p_grammar;

// internal use only
typedef struct s_cll1p_list
{
  t_cll1p_token       token;
  struct s_cll1p_list *next;
}               cll1p_list;

typedef cll1p_token** cll1p_table;
typedef t_cll1p_grammar* cll1p_grammar;

/*
  create ll table
  number_max_tokens is the max size of possible derived tokens + 1
  Never forget that CLL1P_EXPR is always at 0 0
  The returned table is filled by CLL1P_UNDEF
  */
cll1p_table cll1p_create_table(unsigned int x, unsigned int y, unsigned int number_max_tokens);
/*
  free all the table
  */
void        cll1p_delete_table(cll1p_table table);

/*
  you must set all tokens by providing a char * array of all tokens
  to get a binded table id for each
  */
cll1p_grammar cll1p_create_grammar(char **tokens);
/*
  get table id from a token
  */
int           cll1p_get_grammar_id(cll1p_grammar grammar, char *token);
/*
  free grammar array
  */
void          cll1p_delete_grammar(cll1p_grammar grammar);

/*
  parse input stream token
  return 1 on success
  retrun 0 on fail
  fx is a function pointer which is apply when token match with another
  data is a custom variable passed to fx
  */

int         cll1p_parse(cll1p_token tokens, cll1p_table table, cll1p_grammar grammar, void (*fx)(void *data, cll1p_token token), void *data);

# endif /* !CLLIP_H_ */
