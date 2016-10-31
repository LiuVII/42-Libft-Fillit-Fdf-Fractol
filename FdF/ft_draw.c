/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 22:56:04 by mfilipch          #+#    #+#             */
/*   Updated: 2016/10/18 22:56:06 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// int		get_color(float zmin, float zmax, float zmean, float z)
// {
// 	int		r;
// 	int		g;
// 	int		b;

// 	g = (z > zmean) ? (int)((z - zmean) / (zmax - zmean) * 255) % 256 : 0;
// 	(z < zmean) ? (g = (int)((z - zmean) / (zmin - zmean) * 255) % 256) : 0;
// 	r = (z > zmean) ? 0 : g;
// 	b = (z < zmean) ? 0 : g;
// 	return (0x00FFFFFF - (r << 16) - (g << 8) - b);
// }

int		get_color(t_data *d, float z)
{
	int		c1;
	int		c2;
	float	ratio;

	c1 = d->cmin;
	c2 = d->cmax;
	if (d->cmean >= 0 && z >= d->zmean)
	{
		c1 = d->cmean;
		ratio = (z - d->zmean) / (d->zmax - d->zmean);
	}
	else if (d->cmean >= 0 && z < d->zmean)
	{
		c2 = d->cmean;
		ratio = (z - d->zmin) / (d->zmean - d->zmin);
	}
	else
	{
		c1 = d->cmin;
		ratio = (z - d->zmin) / (d->zmax - d->zmin);
	}
	z = (int)((((int)c2 >> 16) % 256 - ((int)c1 >> 16) % 256) * ratio + ((int)c1 >> 16) % 256) << 16;
	z += (int)((((int)c2 >> 8) % 256 - ((int)c1 >> 8) % 256) * ratio + ((int)c1 >> 8) % 256) << 8;
	z += (int)((((int)c2) % 256 - ((int)c1) % 256) * ratio + ((int)c1) % 256);
	return (z);
}

int		set_color(t_3d p1, t_3d p2, t_3d p, int flag)
{
	float	ratio;

	ratio = 0;
	if (flag == 1)
		ratio = (p.x - p1.x) / (p2.x - p1.x);
	else if (flag == 2)
		ratio = (p.y - p1.y) / (p2.y - p1.y);
	p.z = (int)((((int)p2.z >> 16) % 256 - ((int)p1.z >> 16) % 256) * ratio + ((int)p1.z >> 16) % 256) << 16;
	p.z += (int)((((int)p2.z >> 8) % 256 - ((int)p1.z >> 8) % 256) * ratio + ((int)p1.z >> 8) % 256) << 8;
	p.z += (int)((((int)p2.z) % 256 - ((int)p1.z) % 256) * ratio + ((int)p1.z) % 256);
	return (p.z);
}

void	ft_draw_pixel(t_data *d, int x, int y, int c)
{
	int		i;
	long	mem;

	if (x < 0 || y < 0 || x > XS - 10 || y > YS - 10)
		return ;
	i = -1;
	mem = d->bpp / 8 * x + d->ls * y;
	while (i++ < d->bpp / 8 - 1)
	{
		d->img_p0[mem + i] = c % 256;
		c = c / 256;
	}
}

void	ft_dline(t_data *d, t_3d p1, t_3d p2)
{
	t_3d	p;
	t_2d	a;
	t_2d	b;
	float	dist;

	if ((p1.x > XS && p2.x > XS) || (p1.x < 0 && p2.x < 0) ||
		(p1.y > YS && p2.y > YS) || (p1.y < 0 && p2.y < 0))
		return ;
	p.x = p1.x;
	p.y = p1.y;
	dist = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	if (ABS(p2.x - p1.x) >= ABS(p2.y - p1.y) && ABS(p1.x - p2.x) / dist >= 0.01)
	{
		a.x = (p2.y - p1.y) / (p2.x - p1.x);
		b.x = (p1.y * p2.x - p2.y * p1.x) / (p2.x - p1.x);
		while ((p2.x - (p.x += (p2.x - p1.x) / dist)) * SIGN(p2.x - p1.x) >= 0)
			ft_draw_pixel(d, p.x, a.x * p.x + b.x, set_color(p1, p2, p, 1));
	}
	else if (ABS(p1.y - p2.y) / dist >= 0.01)
	{
		a.y = (p2.x - p1.x) / (p2.y - p1.y);
		b.y = (p1.x * p2.y - p2.x * p1.y) / (p2.y - p1.y);
		while ((p2.y - (p.y += (p2.y - p1.y) / dist)) * SIGN(p2.y - p1.y) >= 0)
			ft_draw_pixel(d, a.y * p.y + b.y, p.y, set_color(p1, p2, p, 2));
	}
}

int		ft_drawit(t_data *d)
{
	int		i;
	int		j;
	int		k;
	t_3d	p;

	d->img_p = mlx_new_image(d->mlx, XS, YS);
	d->img_p0 = mlx_get_data_addr(d->img_p, &(d->bpp), &(d->ls), &(d->endian));
	i = -1;
	while (++i < d->img_size.z && (j = -1))
		while (++j < d->img_size.y && (k = -1))
			while (++k < d->img_size.x)
			{
				p = ft_tr(d, d->img[i][j][k]);
				(i > 0) ? ft_dline(d, p, ft_tr(d, d->img[i - 1][j][k])) : 0;
				(j > 0) ? ft_dline(d, p, ft_tr(d, d->img[i][j - 1][k])) : 0;
				(k > 0) ? ft_dline(d, p, ft_tr(d, d->img[i][j][k - 1])) : 0;
				if (k == 0 && j == 0 && i == 0)
					ft_draw_pixel(d, p.x, p.y, set_color(p, p, p, 0));
			}
	mlx_clear_window(d->mlx, d->win);
	mlx_put_image_to_window(d->mlx, d->win, d->img_p, 0, 0);
	mlx_destroy_image(d->mlx, d->img_p);
	return (0);
}
