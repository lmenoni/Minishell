#include "libft.h"

void    skip_spaces(char *s, int *i)
{
    while (s[*i] != '\0' && is_space(s[*i]))
    {
        *i = *i + 1;
    }
}
