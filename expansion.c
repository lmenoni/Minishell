/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:22:14 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/17 18:20:46 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool    there_is_expand(char *s)
{
    int i;

    i = 0;
    while (s[i] != '\0')
    {
        if (s[i] == '$' && s[i + 1] != '\0' && s[i + 1] != ' ')
            return (true);
        i++;
    }
    return (false);
}

char    *expand_dollar(char *s)
{
    char    *r;
    char    *var;
    char    *temp;

    r = NULL;
    var = //funzione che prende la variabile (get_var(&s), data) // get_var: ritorna la variabile da cercare in env e sposta il pintatore di s al carattere dopo;
    temp = NULL;
    if (!var)
        return (s);
    while (1)
    {
        var = //funzione che prende la variabile (get_var(&s), data) // get_var: ritorna la variabile da cercare in env e sposta il pintatore di s al carattere dopo;
        temp = //funzione che prende la stringa fino alla variabile trovata ($...) o eol;
        if (!var && !temp)
            return (r);
        r = ft_buffjoin(r, temp);
        r = ft_buffjoin(r, var);
        free(var);
        free(temp);
        var = NULL;
        var = NULL;
    }
    /*
    se c'e allochi while lo spazio fino al dollaro in r e copi
    espandi il dollaro  fai buffjoin su r
    ricontrolli se sulla rimanente stringa c'e qualcosa da espandere
    se ce salvi la stringa fino al dollaro  e fai join r
    espandi e fai join su r
    inito il while ritorni r*/
}
