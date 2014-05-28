/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnspace.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 19:08:49 by vdefilip          #+#    #+#             */
/*   Updated: 2014/05/23 19:08:49 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

void	ft_putnspace(size_t n)
{
	while (n--)
		ft_putchar(' ');
}
