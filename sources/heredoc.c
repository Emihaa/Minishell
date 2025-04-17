/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:47:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/18 00:39:18 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


//@TODO: heredocuments have to be opened before any forking happens

/*

ltaalas@c1r3p1:~/projects/minishell/sources$ << DELIM
>
bash: warning: here-document at line 1 delimited by end-of-file (wanted `DELIM')
ltaalas@c1r3p1:~/projects/minishell/sources$
ltaalas@c1r3p1:~/projects/minishell/sources$
ltaalas@c1r3p1:~/projects/minishell/sources$
ltaalas@c1r3p1:~/projects/minishell/sources$
ltaalas@c1r3p1:~/projects/minishell/sources$
ltaalas@c1r3p1:~/projects/minishell/sources$
ltaalas@c1r3p1:~/projects/minishell/sources$ << DELIM
>
bash: warning: here-document at line 8 delimited by end-of-file (wanted `DELIM')
ltaalas@c1r3p1:~/projects/minishell/sources$ echo $?
0
ltaalas@c1r3p1:~/projects/minishell/sources$ << DELIM | ls
>
bash: warning: here-document at line 10 delimited by end-of-file (wanted `DELIM')
 a.out	   heredoc.c	      'heredoc copy 3.c'  'heredoc copy.c'   lexer.c	   old_lexer_testing_shit   test.txt
 heredoc  'heredoc copy 2.c'  'heredoc copy 4.c'   lexer	     minishell.c   test			    tree.c
ltaalas@c1r3p1:~/projects/minishell/sources$ echo $?
0
ltaalas@c1r3p1:~/projects/minishell/sources$ << DELIM | >> >
bash: syntax error near unexpected token `>'
>
bash: warning: here-document at line 12 delimited by end-of-file (wanted `DELIM')
ltaalas@c1r3p1:~/projects/minishell/sources$ echo $?
2
ltaalas@c1r3p1:~/projects/minishell/sources$ << DELIM
> asd
> DELIM
ltaalas@c1r3p1:~/projects/minishell/sources$ echo $?
0
ltaalas@c1r3p1:~/projects/minishell/sources$ << DELIM
> asd
> asd
> asd
> asd
>
bash: warning: here-document at line 18 delimited by end-of-file (wanted `DELIM')
ltaalas@c1r3p1:~/projects/minishell/sources$

*/


static
int	heredoc_no_expansion(t_minishell *minishell, int write_fd, char *delimiter)
{
	const int	delimiter_len = ft_strlen(delimiter) + 1;
	char		*line;
	int			read_rval;

	while (1)
	{
		read_rval = heredoc_read(minishell, &line, delimiter);
		if (read_rval != 0)
			break ;
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			break ;
		if (put_str_nl(write_fd, line) == -1)
			break ; // @TODO: maybe error cheking
		free(line);
	}
	free(line);
	if (close(write_fd) == -1)
		syscall_failure(minishell);
	return (read_rval);
}

static
int write_env_variable(char *str, const uint32_t start, int fd, t_minishell *m)
{
	uint32_t		len;
	char			*env_var;

	if (is_valid_var_start(str[start + 1]) == false)
	{
		if (str[start + 1] == '?')
		{
			ft_putnbr_fd(m->exit_status, fd);
			return (1);
		}
		write(fd, &str[start], 1);
		return (0);
	}
	len = 0;
	env_var = get_env_var_value(str, get_key_len(str, ft_strlen(str)));
	(void)put_str(fd, env_var);
	return (len);
}

static
void heredoc_with_expansion_write_loop(t_minishell *m, int write_fd, char *line)
{
	const uint32_t	line_len = ft_strlen(line);
	uint32_t		i;
	uint32_t		len;

	i = 0;
	while (i < line_len)
	{
		len = 0;
		while (line[i + len] != '\0' && line[i + len] != '$')
			len += 1;
		if (write_bytes(write_fd, line + i, len) == -1)
			break ; // @TODO: maybe error cheking
		if (line[i + len] == '$')
			len += write_env_variable(line, i + len, write_fd, m);
		i += len + 1;
	}
}

// this is going to be a "simple" prototype that uses getenv
// will be replaced with our own env version
// there will be no quote removal inside heredoc
static
int	heredoc_with_expansion(t_minishell *minishell, int write_fd, char *delimiter)
{
	const int	delimiter_len = ft_strlen(delimiter) + 1; // maybe problem
	char		*line;
	int			read_rval;

	while (1)
	{
		read_rval = heredoc_read(minishell, &line, delimiter);
		if (read_rval != 0)
			break ;
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			break ;
		heredoc_with_expansion_write_loop(minishell, write_fd, line);
		if (put_char(write_fd, '\n'))
			break ; // @TODO: error cheking
		free(line);
	}
	free(line);
	if (close(write_fd) == -1)
		syscall_failure(minishell);
	return (read_rval);
}

int heredoc(t_arena *arena, t_minishell *minishell, t_token *data)
{
	int				fds[2];
	char			*delimiter;
	uint32_t		new_size;
	int				read_rval;

	if (create_heredoc_fds(fds) == -1)
		syscall_failure(minishell);
	delimiter = arena_alloc(arena, sizeof(char) * data->string_len + 1);
	new_size = set_quote_removed_string(delimiter, data);
	if (new_size < data->string_len)
		read_rval = heredoc_no_expansion(minishell, fds[WRITE], delimiter);
	else
		read_rval = heredoc_with_expansion(minishell, fds[WRITE], delimiter);
	minishell->heredoc_fds[minishell->heredoc_count] = fds[READ];
	minishell->heredoc_count += 1;
	if (read_rval == -2)
		return (-2);
	return (fds[READ]);
}
