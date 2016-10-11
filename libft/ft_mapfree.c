/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mapfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/30 13:00:41 by mfilipch          #+#    #+#             */
/*   Updated: 2016/09/30 13:00:42 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_mapfree(char ***map)
{
	int		i;

	i = -1;
	if (!map || !*map)
		return ;
	while ((*map)[++i])
	{
		free((*map)[i]);
		(*map)[i] = NULL;
	}
	free(*map);
	*map = NULL;
}
