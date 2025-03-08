/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:47:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/08 19:01:52 by ltaalas          ###   ########.fr       */
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




void unused_heredoc(char *delim)
{
	const int delim_len = ft_strlen(delim);
	char *line;

	while (1)
	{
		line = readline("> ");
			if (ft_strncmp(line, delim, delim_len) == 0)
				break ;
		free(line);
	}
	free(line);

}


void heredoc_write(t_minishell *minishell, int write_fd, char *delimiter)
{
	const int delimiter_len = ft_strlen(delimiter) + 1; // maybe problem
	char *line;

	while (1)
	{
		line = readline("> ");
		minishell->line_counter += 1;
		if (line == NULL)
		{
			perror(NULL); // @TODO: error cheking
			if (errno == 0)
			{
				printf("minishell: warning: here-document at line %i \
				delimited by end-of-file (wanted `%s')\n", minishell->line_counter, delimiter);
			}
			break ;
		}
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			break ;
		if (write(write_fd, line, ft_strlen(line)) == -1)
			perror(NULL);
		if (write(write_fd, "\n", 1) == -1)
			perror(NULL);
		free(line);
	}
	free(line);
	close(write_fd);
}

uint32_t num_len(uint32_t num)
{
	uint32_t i;

	i = 0;
	while (num > 0)
	{
		num = num / 10;
		i++;
	}
	printf("%i\n", i);
	return (i);
}
#define HEREDOC_TEMP_NAME "./heredoc_temp"
#define NAME_BASE_LEN sizeof(HEREDOC_TEMP_NAME) - 1
int a = NAME_BASE_LEN;
// we need some kind of global heredoc count to check if it is under 17
char *create_temp_file_name(void)
{
	static uint32_t heredoc_num = 1;
	static char name_buf[20] = HEREDOC_TEMP_NAME; // len is 15
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

// change to /tmp
void create_heredoc_fds(int fds[2])
{
	const char *file_name = create_temp_file_name();;

	printf("%s\n", file_name);
	if (file_name == NULL)
		perror(NULL); // @TODO: error cheking
	fds[1] = open(file_name, O_EXCL | O_CREAT | O_CLOEXEC | O_RDWR, S_IWUSR | S_IRUSR); 
	if (fds[1] == -1)
	{
		perror("heredoc failure: write"); // @TODO: error shit
		if (errno == EEXIST)
			create_heredoc_fds(fds);
		return ;
	}
	fds[0] = open(file_name, O_CLOEXEC | O_RDONLY);
	if (fds[0] == -1)
	{
		perror("heredoc failure: read"); // @TODO: error shit
	}
	unlink(file_name);
}

//	Testing stuff
/*
cc heredoc.c -l readline ../libs/libft/build/libft.a ../libs/lt_alloc/build/lt_alloc.a -o heredoc
*/

int main(int argc, char *argv[], char *envp[])
{
	t_minishell minishell;
	minishell.line_counter = 0;
	(void)argc;
	(void)argv;
	int wstatus;
	pid_t heredoc_pid;
	//read_loop(envp);
	minishell.stdin_copy = dup(STDERR_FILENO);
	printf("%i\n", getpid());
	int fds[2];
	create_heredoc_fds(fds);
	heredoc_write(&minishell, fds[1], "DELIM");
	dup2(fds[0], STDIN_FILENO);
	close(fds[0]);
	pid_t pid = fork();
	if (pid == 0)
	{
		close(minishell.stdin_copy);
		char *cat_argv[3] = {[0] = "cat", "-e", [2] = NULL};
		printf("to cat \t%i\n", fds[0]);
		execve("/bin/cat", cat_argv, envp);
		exit(1);
	}
	dup2(minishell.stdin_copy, STDIN_FILENO);
	close(minishell.stdin_copy);
	pid = waitpid(pid, &wstatus, 0);
	printf("%s\t%i\n", strerror(WEXITSTATUS(wstatus)), wstatus);
	printf("exit\n");
    return (0);
}


