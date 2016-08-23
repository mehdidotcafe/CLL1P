/*
  default cll1p tokenize
  you sould create your own
  Only three tokens are avaibles :
    -> cll1p_string
    -> cll1p_space
    -> cll1p_number
  */

#ifndef CLL1P_TOKEN_
# define CLL1P_TOKEN_

# define CLL1P_STRING "cll1p_string"
# define CLL1P_SPACE "cll1p_space"
# define CLL1P_NUMBER "cll1p_number"

# include <ctype.h>
# include "cll1p.h"

/**
  Tokensizer function
  number_as_string is a boolean
  if number_as_string == 0 number will be tokenize in string
  **/

cll1p_token cll1p_tokenize(char *stream, int number_as_string);

#endif /* !CLL1P_TOKEN_ */
