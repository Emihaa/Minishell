/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:47:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/11 21:35:27 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
void	heredoc_write(t_minishell *minishell, int write_fd, char *delimiter)
{
	const int delimiter_len = ft_strlen(delimiter) + 1; // maybe problem
	char *line;

	while (1)
	{
		line = readline("> ");
		minishell->line_counter += 1;
		if (line == NULL)
		{
			perror("readline error"); // @TODO: error cheking
			if (errno == EXIT_SUCCESS)
			{
				printf("minishell: warning: here-document at line %i \
delimited by end-of-file (wanted `%s')\n", minishell->line_counter, delimiter);
			}
			break ;
		}
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			break ;
		if (write(write_fd, line, ft_strlen(line)) == -1)
			perror("write line"); // error cheking
		if (write(write_fd, "\n", 1) == -1)
			perror("write '\\n'"); // error cheking
		free(line);
	}
	free(line);
	close(write_fd);
}

static inline
uint8_t	num_len(uint32_t num)
{
	uint8_t i;

	i = 0;
	while (num > 0)
	{
		num = num / 10;
		i++;
	}
	return (i);
}
// @TODO: change to name to /tmp/...
// we need some kind of global heredoc count to check if it is under 17
static
char	*create_temp_file_name(void)
{
	static uint32_t heredoc_num = 1; // this should probably be included into the minishell struct and passed here
									// also should be reset whenever starting a new command reading loop
	static char name_buf[30] = HEREDOC_TEMP_NAME; // len is 15
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

// change to name to /tmp
static
int	create_heredoc_fds(int fds[2])
{
	const char *file_name = create_temp_file_name();

	printf("%s\n", file_name);
	if (file_name == NULL)
		perror("filename is NULL"); // @TODO: error cheking
	fds[1] = open(file_name, O_EXCL | O_CREAT | O_CLOEXEC | O_WRONLY, S_IWUSR | S_IRUSR); 
	if (fds[1] == -1)
	{
		perror("heredoc failure: write"); // @TODO: error shit
		if (errno == EEXIST)
			return (create_heredoc_fds(fds));
		return (1); // @TODO: more error stuff
	}
	fds[0] = open(file_name, O_CLOEXEC | O_RDONLY);
	if (fds[0] == -1)
	{
		perror("heredoc failure: read"); // @TODO: error shit
	}
	if (unlink(file_name) == -1)
		return (1); // @TODO: more error stuff
	return (0);
}

int heredoc(t_minishell *minishell, char *delimiter)
{
	int fds[2];
	int errval; // delete

	errval = create_heredoc_fds(fds);
	printf("heredoc_fds r_val: %i\n" , errval); // delete
	heredoc_write(minishell, fds[WRITE], delimiter);
	store_redirects(&fds[READ], NULL, minishell);
	return (errval); // maybe error value
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


