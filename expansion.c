/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:22:14 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/20 12:56:03 by lmenoni          ###   ########.fr       */
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

char	*get_before_var(char *s)
{
	int	i;
	int	j;
	char	*r;
	
	i = 0;
	j = 0;
	if (*s == '\0')
		return (NULL);
	while (s[i] != '\0')
	{
		if (s[i] == '$' && s[i + 1] != '\0' && !isspace(s[i + 1]))
			break ;
		i++;
	}
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
		if (**s == '$' && *(*s + 1) != '\0' && !isspace(*(*s + 1)))
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
	if (!var && *s == '\0')
		return (free(buff), temp);
	while (1)
	{
		
		if (!var && !buff && *s == '\0')
			return (free(temp), r);
		r = ft_merge(r, buff);
		r = ft_merge(r, var);
		buff = get_before_var(s);
		var = get_var(&s, data);
	}
}
