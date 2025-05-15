/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:11:32 by lmenoni           #+#    #+#             */
/*   Updated: 2024/11/30 14:11:43 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printpointer(unsigned long p)
{
	char	*h;
	int		count;

	count = 0;
	if (p == 0)
	{
		write (1, "(nil)", 5);
		return (3);
	}
	h = "0123456789abcdef";
	if (p < 16)
		write (1, "0x", 2);
	if (p >= 16)
		count += ft_printpointer((p / 16));
	write (1, &h[p % 16], 1);
	return (count + 1);
}
