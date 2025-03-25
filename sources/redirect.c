/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:39:28 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/25 22:57:17 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//static char stump[1] = "\0";

int redirect_out(char **file_name, t_minishell *m)
{
	int fd;
	if (file_name[1] != NULL)
	{
		printf("minishell: %s: ambiguous redirect", file_name[0]);
		return (1);
	}
	fd = open(file_name[0], O_CLOEXEC	| O_CREAT	| O_WRONLY	| O_TRUNC,
							S_IWUSR		| S_IRUSR	| S_IRGRP	| S_IROTH);
	if (fd == -1)
		; // @TODO: open failure
	return(store_write_fd(fd, m));
}

int redirect_append(char **file_name, t_minishell *m)
{
	int fd;
	if (file_name[1] != NULL)
	{
		printf("minishell: %s: ambiguous redirect", file_name[0]);
		return (1);
	}
	fd = open(file_name[0], O_CLOEXEC	| O_CREAT	| O_WRONLY	| O_APPEND,
							S_IWUSR		| S_IRUSR	| S_IRGRP	| S_IROTH);
	if (fd == -1)
		return (1); // @TODO: open failure
	return(store_write_fd(fd, m));
}

int redirect_in(char **file_name, t_minishell *m)
{
	int fd;
	if (file_name[1] != NULL)
	{
		printf("minishell: %s: ambiguous redirect", file_name[0]);
		return (1);
	}
	fd = open(file_name[0], O_CLOEXEC | O_RDONLY);
	if (fd == -1)
		return (1); // @TODO: open failure
	return(store_read_fd(fd, m));
}