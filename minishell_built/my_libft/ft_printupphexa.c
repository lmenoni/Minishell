/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putupphexa_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:05:34 by lmenoni           #+#    #+#             */
/*   Updated: 2024/11/30 14:05:38 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printupphexa(unsigned int n)
{
	char	*h;
	int		count;

	count = 0;
	h = "0123456789ABCDEF";
	if (n >= 16)
		count += ft_printupphexa(n / 16);
	write (1, &h[n % 16], 1);
	return (count + 1);
}
