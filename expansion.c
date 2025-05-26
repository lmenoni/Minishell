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

bool	double_quoted(char *s, char *to_check)
{
	bool	in_quote;

	in_quote = false;
	while (s != to_check)
	{
		if (*s == '"')
			in_quote = !in_quote;
		s++;
	}
	return (in_quote);
}

bool	single_quoted(char *s, char *to_check)
{
	bool	in_quote;

	in_quote = false;
	while (s != to_check)
	{
		if (*s == '\'')
			in_quote = !in_quote;
		s++;
	}
	return (in_quote);
}

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

char	*get_before_var(char *s, bool expand)
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
		if (s[i] == '$' && s[i + 1] != '\0' && !isspace(s[i + 1]) && expand)
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
// get beforevar va modificata in modo che non si fermi a variabili tra single quote e rimuova le quote se non siamo in heredoc


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
//get var va modificata in modo che non prenda in considerazione variabili tra single quote se non stiamo facendo here doc

char *expand_dollar(char *s, t_data *data, bool expand)
{
	char	*r;
	char	*var;
	char	*temp;
	char	*buff;

	r = NULL;
	temp = s;
	buff = get_before_var(s, expand);
	var = get_var(&s, data);
	if (!var && *s == '\0')
		return (free(buff), temp);
	while (1)
	{
		
		if (!var && !buff && *s == '\0')
			return (free(temp), r);
		r = ft_merge(r, buff);
		r = ft_merge(r, var);
		buff = get_before_var(s, expand);
		var = get_var(&s, data);
	}
}
// espansione $?, da gestire una volta scritta la esecuzione
// in caso di here doc non vanno tolte le quote e espande anche variabili tra singel quote
/*idea di tokenizzare anche le i dollari se e solo se fuori da quote di qualsiasi tip
crearre un nuovo puntatore attached nei nodi token, se il doken $ è direttammente collegato a una stringa attached diventera lultimo token creato
se invece viene creato un nodo strinng e trova qualcosa direttamente attaccato e l'ultimo token creato è doollaro attached diventer quel token
tutto questo solo se lultimo nodo aggiunto non è here doc
una volta eseguito here doc e fatto il syntax parsing espandiamo
i token string vengono sollevati dalle quote salvandosi però se ci sono single quotes(essendo che i $ fuori dalle quote sono in token diversi adesso se ci sono delle single quotes sicuramente la varriabile eè tra esse e non va espansa)
non espandiamo la variabile
nei token $ invece eseguiamo l'espansione 
tokeniziamo in caso siano piu parole separate da spazi
e poi controlliamo
1 se il primo carattere è ' ' e ha uun attached non bisogna joinare con il prev  e se il prev-->prev è un redirect in tal caso è amb_redi
2 se ci sono degli spazi in fondo non va fatto il join con il next(qui next->attached si setta a NULL) anche qui bisogna controllare se c'è un amb_redi
una volta fatti i join del caso i token creati vengono infilati tra i token e possiamo continuare*/