/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:47:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/05 00:03:30 by ehaanpaa         ###   ########.fr       */
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

extern int g_global_int;


static inline
void print_eof_error(t_minishell *m, char *delimiter)
{
	FILE *temp;
	
	temp = stdout;
	stdout = stderr;
	printf(EOF_ERROR, m->line_counter, delimiter);
	stdout = temp;
}

static
int heredoc_read(t_minishell *minishell, char **line, char *delimiter)
{
	// signal(SIGINT, SIG_DFL);
	*line = readline("> ");
	printf("global: %i\n", g_global_int);
	if (*line == NULL)
	{
		print_eof_error(minishell, delimiter); // should this be on stderror?
		return (-1);
	}
	minishell->line_counter += 1;
	return (0);
}

static
void	heredoc_write_no_expansion(t_minishell *minishell, int write_fd, char *delimiter)
{
	const int delimiter_len = ft_strlen(delimiter) + 1; // maybe problem
	char *line;

	while (1)
	{
		if (heredoc_read(minishell, &line, delimiter))
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
}

int write_env_variable(char *string, const uint32_t start, int fd, t_minishell *m)
{
	uint32_t		len;
	char			*env_var;

	if (is_valid_var_start(string[start + 1]) == false)
	{
		if (string[start + 1] == '?')
		{
			ft_putnbr_fd(m->exit_status, fd);
			return (1);
		}
		write(fd, &string[start], 1);
		return (0);
	}
	len = 0;
	env_var = find_env_var(&string[start + 1], ft_strlen(&string[start + 1]), &len, m->envp);
	put_str(fd, env_var);
	return (len);
}

// this is going to be a "simple" prototype that uses getenv
// will be replaced with our own env version
// there will be no quote removal inside heredoc
static
void	heredoc_write_with_expansion(t_minishell *minishell, int write_fd, char *delimiter)
{
	const int delimiter_len = ft_strlen(delimiter) + 1; // maybe problem
	char *line;
	uint32_t len;
	uint32_t i;
	uint32_t line_len;

	while (1)
	{
		if (heredoc_read(minishell, &line, delimiter))
			break ;
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			break ;
		line_len = ft_strlen(line);
		i = 0;
		while (i < line_len)
		{
			len = 0;
			while (line[i + len] != '\0' && line[i + len] != '$')
				len += 1;
			if (write_bytes(write_fd, line + i, len) == -1)
				break ; // @TODO: maybe error cheking
			if (line[i + len] == '$')
				len += write_env_variable(line, i + len, write_fd, minishell);
			i += len + 1;
		}
		if (put_char(write_fd, '\n'))
			break ; // @TODO: error cheking
		free(line);
	}
	free(line);
	if (close(write_fd) == -1)
		syscall_failure(minishell);
}

// @TODO: change to name to /tmp/...
// we need some kind of global heredoc count to check if it is under 17
static
char	*create_temp_file_name(uint32_t heredoc_num)
{
	//static uint32_t heredoc_num = 1; // this should probably be included into the minishell struct and passed here
									// also should be reset whenever starting a new command reading loop
	static char name_buf[30] = HEREDOC_TEMP_NAME;
	uint32_t num_temp;
	uint8_t i;

	num_temp = heredoc_num;
	i = num_len(heredoc_num);
	name_buf[NAME_BASE_LEN + i] = '\0';
	while(i--)
	{
		name_buf[NAME_BASE_LEN + i] = (num_temp % 10) + '0';
		num_temp = num_temp / 10;
	}
	heredoc_num += 1;
	return(name_buf);
}

// @TODO: change to name to /tmp
// @TODO: might want to make different error return values
static
int	create_heredoc_fds(int fds[2])
{
	static uint32_t heredoc_num = 1;
	const char *file_name;
	int return_val;

	return_val = 0;
	heredoc_num = 1;
	while (1)
	{
		file_name = create_temp_file_name(heredoc_num++);
		fds[1] = open(file_name, O_EXCL | O_CREAT | O_CLOEXEC | O_WRONLY, S_IWUSR | S_IRUSR); 
		if (fds[1] != -1)
			break ;
		if (errno == EEXIST)
			continue ;
		return (-1); // @TODO: more error stuff
	}	
	fds[0] = open(file_name, O_CLOEXEC | O_RDONLY);
	if (fds[0] == -1)
		return_val = -1;
	if (unlink(file_name) == -1)
		return_val = -1; // @TODO: more error stuff
	return (return_val);
}

int heredoc(t_minishell *minishell, t_token *data)
{
	int fds[2];
	// int errval = 0; // delete
	char *delimiter;
	uint32_t new_size;

	if (create_heredoc_fds(fds) == -1)
		syscall_failure(minishell);
	delimiter = arena_alloc(&minishell->node_arena, sizeof(char) * data->string_len + 1); 
	new_size = set_quote_removed_string(delimiter, data);
	arena_unalloc(&minishell->node_arena, (data->string_len + 1) - new_size);
	if (new_size < data->string_len)
		heredoc_write_no_expansion(minishell, fds[WRITE], delimiter);
	else
		heredoc_write_with_expansion(minishell, fds[WRITE], delimiter);
	minishell->heredoc_fds[minishell->heredoc_count] = fds[READ];
	minishell->heredoc_count += 1;
	return (fds[READ]); // maybe error value
}



//	Testing stuff
/*
cc heredoc.c -l readline ../libs/libft/build/libft.a ../libs/lt_alloc/build/lt_alloc.a -o heredoc -g
*/



// int main(int argc, char *argv[], char *envp[])
// {
// 	pid_t pids[3] = {0};
// 	t_minishell minishell;
// 	init_minishell(&minishell);
// 	(void)argc;
// 	(void)argv;
// 	int wstatus;
// 	//read_loop(envp);
// 	printf("%i\n", getpid());
// 	while (minishell.command_count < 2)
// 	{
// 		heredoc(&minishell, "DELIM");
// 		pid_t pid = fork();
// 		if (pid == 0)
// 		{
// 			apply_redirect(&minishell);
// 			char *cat_argv[3] = {[0] = "cat", "-e", [2] = NULL};
// 			printf("fd to cat \t%i\n", 17);
// 			if (execve("/bin/cat", cat_argv, envp) == -1)
// 				perror("execve fail");
// 			exit(1);
// 		}
// 		reset_redirect(&minishell);
// 		pids[minishell.command_count] = pid;
// 		minishell.command_count += 1;
// 	}
// 	int exit_status = wait_for_sub_processes(&minishell, pids);
// 	printf("last printf %s\t%i\n", strerror(exit_status), exit_status);
// 	printf("exit\n");
//     return (0);
// }


