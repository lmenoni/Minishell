/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexa_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:36:14 by lmenoni           #+#    #+#             */
/*   Updated: 2024/11/30 13:36:16 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printhexa(unsigned int n)
{
	char	*h;
	int		count;

	count = 0;
	h = "0123456789abcdef";
	if (n >= 16)
		count += ft_printhexa(n / 16);
	write (1, &h[n % 16], 1);
	return (count + 1);
}
