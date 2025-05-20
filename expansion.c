/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:22:14 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/19 17:38:04 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *check_env(t_data *data, char *var)
{
	t_env *temp;
	int	i;
	
	i = 0;
	temp = data->env_data;
	while (temp)
	{
		i = 0;
		while (temp->e[i] != '\0' && temp->e[i] != '=')
			i++;
		if (ft_strncmp(temp->e, var, i) == 0)
			return(&temp->e[i + 1]);
		temp = temp->next;
	}
	return (NULL);
}

// char	*get_expanded_var(char *var, t_data *data)
// {
// 	char	*r;

// 	r = NULL;
// 	r = ft_strdup(check_env(data, var));
// 	return (r);
// }
char	*get_before_var(char *s)
{
	int	i;
	int	j;
	char	*r;
	
	i = 0;
	j = 0;
	if (*s == '\0')
		return (NULL);
	while (s[i] != '\0' && (s[i] != '$' && s[i + 1] != '\0'
		&& s[i + 1] != ' '))
		i++;
	r = malloc((i + 1) * sizeof(char));
	while (j < i)
	{
		r[j] = s[j];
		j++;
	}
	r[j] = '\0';
	return (r);
}


char	*get_var(char **s, t_data *data)
{
	int	i;
	char	*r;
	char	*name;

	i = 0;
	r = NULL;
	while (**s != '\0')
	{
		if (**s == '$' && *(*s + 1) != '\0' && *(*s + 1) != ' ')
		{
			(*s)++;
			while ((*s)[i] != '\0' && !isspace((*s)[i])
				&& (*s)[i] != '"' && (*s)[i] != '\'')
				i++;
			name = ft_strndup(*s, i);
			r = ft_strdup(check_env(data, name));
			while (**s != '\0' && !isspace(**s)
				&& **s != '"' && **s != '\'' )
				(*s)++;
			return (free(name), r);
		}
		(*s)++;
	}
	return (r);
}

char *expand_dollar(char *s, t_data *data)
{
	char	*r;
	char	*var;
	char	*temp;
	char	*buff;

	r = NULL;
	temp = s;
	buff = get_before_var(s);
	var = get_var(&s, data);
	if (!var)
		return (free(buff), s);
	while (1)
	{
		
		if (!var && !buff)
			return (free(temp), r);
		ft_printf("BUFF IS %s\n", buff);
		ft_printf("VAR IS %s\n", var);
		r = ft_merge(r, buff);
		r = ft_merge(r, var);
		buff = get_before_var(s);
		var = get_var(&s, data);
	}
}
