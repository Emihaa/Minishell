/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:39:28 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/26 19:36:03 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//static char stump[1] = "\0";


// @TODO: restructure error messages
// we might want to do some kind of printf thing with a buffer
static inline
void	ambigous_redirect(char *file_name)
{
	ft_putstr_fd("minishell: ", 2);
	if (file_name != NULL)
		ft_putstr_fd(file_name, 2);
	ft_putendl_fd(": ambigous redirect", 2);
}

static inline
void	open_failure(char *file_name)
{
	ft_putstr_fd("minishell: ", 2);
	perror(file_name);
}

int redirect_out(char **file_name, t_minishell *m)
{
	int fd;

	if (file_name[1] != NULL)
	{
		ambigous_redirect(file_name[0]);
		return (1);
	}
	fd = open(file_name[0], O_CLOEXEC	| O_CREAT	| O_WRONLY	| O_TRUNC,
							S_IWUSR		| S_IRUSR	| S_IRGRP	| S_IROTH);
	if (fd == -1)
	{
		open_failure(file_name[0]);
		return (1);
	}
	store_write_fd(fd, m);
	return(0);
}

int redirect_append(char **file_name, t_minishell *m)
{
	int	fd;

	if (file_name[1] != NULL)
	{
		ambigous_redirect(file_name[0]);
		return (1);
	}
	fd = open(file_name[0], O_CLOEXEC	| O_CREAT	| O_WRONLY	| O_APPEND,
							S_IWUSR		| S_IRUSR	| S_IRGRP	| S_IROTH);
	if (fd == -1)
	{
		open_failure(file_name[0]);
		return (1);
	}
	store_write_fd(fd, m);
	return(0);
}

int	redirect_in(char **file_name, t_minishell *m)
{
	int	fd;

	if (file_name[1] != NULL)
	{
		ambigous_redirect(file_name[0]);
		return (1);
	}
	fd = open(file_name[0], O_CLOEXEC | O_RDONLY);
	if (fd == -1)
	{
		open_failure(file_name[0]);
		return (1);
	}
	store_read_fd(fd, m);
	return(0);
}