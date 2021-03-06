/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 19:08:50 by vdefilip          #+#    #+#             */
/*   Updated: 2014/05/23 19:08:50 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strcat(char *s1, const char *s2)
{
	char	*str;

	str = s1;
	while (*s1 != 0)
		s1++;
	while (*s2 != 0)
		*s1++ = *s2++;
	*s1 = '\0';
	return (str);
}
