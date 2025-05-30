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
		if (ft_strncmp(temp->e, var, ft_strlen(var)) == 0)
			return(&temp->e[i + 1]);
		temp = temp->next;
	}
	return (NULL);
}

void	while_not_var(char *s, char *t, int *i, bool expand)
{
	while (t[*i] != '\0')
	{
		if (t[*i] == '$'
			&& (!is_space(t[*i + 1]) && t[*i + 1] != '\0' && t[*i + 1] != '"' && t[*i + 1] != '\'')
			&& (is_quoted(s, &t[*i]) != 1 || expand))
			break ;
		if (t[*i] == '$' && is_quoted(s, &t[*i]) == 0 && (t[*i + 1] == '"' || t[*i + 1] == '\''))
			break ;
		*i = *i + 1;
	}
}

char	*get_expanded_var(char *s, int *idx, t_data *data)
{
	int	i;
	char	*name;
	char	*r;

	if (s[0] == '\0')
		return (NULL);
	i = 1;
	while (s[i] != '\0' && s[i] != '$' && s[i] != '"' && s[i] != '\'' && !is_space(s[i]))
		i++;
	name  = ft_buffjoin(ft_strndup(&s[1], (i - 1)), "=");
	r = ft_strdup(check_env(data, name));
	free(name);
	*idx = *idx + i;
	return (r);
}

char	*expand_dollar(char *s, t_data *data, bool expand)
{
	int	i;
	char	*t;
	char	*r;

	i = 0;
	t = s;
	r = NULL;
	while (t[i] != '\0')
	{
		while_not_var(s, t, &i, expand);
		if (!r)
			r = ft_strndup(t, i);
		else
			r = ft_merge(r, ft_strndup(t, i));
		r = ft_merge(r, get_expanded_var(&t[i], &i, data));
		t += i;
		i = 0;
	}
	free(s);
	return (r);
}

// espansione $?, da gestire una volta scritta la esecuzione
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