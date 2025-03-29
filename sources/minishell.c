/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/29 01:48:06 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>

#include "../includes/minishell.h" // fix maybe

void builtin_env(char **envp)
{
	int i;

	i = 0;
	if (envp == NULL)
		return ;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
// @TODO: we need to figure out how we want to do file redirections for builtins when run in the main process (parent)
// one way would be to not redirect anything but just write into the fd that minishell->fds[WRITE] points to
void builtin_echo(char *argv[])
{
	bool newline;
	int i;

	if (argv[1] == NULL)
	{
		write(1, "\n", 1);
		return ;
	}
	newline = !(0 || !ft_strncmp(argv[1], "-n", 3));
	i = !newline + 1;
	while (1)
	{
		printf("%s", argv[i]);
			i++;
		if (argv[i] == NULL)
			break ;
		printf(" ");
	}
	if (newline == true)
		printf("\n");
}


// watch the youtube video that had something about path name shortening
// it also had some stuff about arenas and macros for c. was about 13mins long
// will need that path name shortening for this cd probably
// void builtin_cd(char **argv, char **env)
// {
// 	int i;
// 	const char *home = find_env_var("HOME", 4, i, env);
// 	char *curpath;
// 	char *directory;


// 	i = 0;
// 	while (argv[i] != NULL)
// 		i++;
// 	if (i > 1)
// 	{
// 		ft_putendl_fd("minishell: cd: too many arguments", 2);
// 		return (1);
// 	}
// 	directory = argv[0];
// 	if (argv[0] == NULL)
// 	{
// 		if (home == NULL)
// 		{

// 			ft_putendl_fd("minishell: cd: HOME not set", 2);
// 			return(1);
// 		}
// 		directory = home;
// 	}
// 	if (directory[0] == '/') // step 3
// 	{
// 		curpath = directory;
		
// 	}
// 	(directory[0] == '.')
// 	chdir()
// }

//pwd command
//propably need file dupping (dup2)
int print_working_directory(void)
{
	char current_path[PATH_MAX];
	getcwd(current_path, PATH_MAX);
	printf("%s\n", current_path);
	return (BUILTIN_PWD);
}

void export(void)
{
	char *s = "vegenakki";
	printf("%s\n", s);

	//should return something?
}

void print_expansion(char *line)
{	
	//line++;
	printf("%s\n", getenv(line));	
}

char *get_token_name(t_token *token)
{
	if (token->type == PIPE)
		return("|");
	if (token->type == REDIRECT_IN)
		return("<");
	if (token->type == REDIRECT_OUT)
		return(">");
	if (token->type == REDIRECT_APPEND)
		return(">>");
	if (token->type == HERE_DOCUMENT)
		return("<<");
	if (token->type == WORD)
		return("word");
	if (token->type == END_OF_LINE)
		return ("newline");
	return ("ERROR");
}

void print_token(t_token *token)
{
	printf("token number: %i\ttoken name: %s\ttoken string: %.*s\n",
			token->type, get_token_name(token), (int)token->string_len, token->u_data.string);
}

int	create_and_store_pipe(t_minishell *m, int8_t *side)
{
	if (*side == WRITE || *side == -1)
	{
		if (pipe(m->pipe) == -1)
			syscall_failure(m); //@TODO: error cheking
		store_write_fd(m->pipe[WRITE], m);
		m->pipe[WRITE] = -1;
		*side = READ;
		return (0);
	}
	else if (*side == READ)
	{
		store_read_fd(m->pipe[READ], m);
		m->pipe[READ] = -1;
		*side = WRITE;
		return (0);
	}
	return (0);
}
// in the future this will actually need the argv inside the tree node
// rename probably

static inline
void close_pipe(t_minishell *m)
{
	if (m->pipe[WRITE] != -1)
		if (close(m->pipe[WRITE]))
			syscall_failure(m);
	if (m->pipe[READ] != -1)
		if (close(m->pipe[READ]))
			syscall_failure(m);
}

void builtin_exit(char **argv, t_minishell *m)
{
	(void)argv;
	minishell_cleanup(m);
	if (m->pipe_side != -1)
		write(2, "exit\n", 5);
	exit(m->exit_status);
}

t_builtin check_for_builtin(char *command)
{
	if (ft_strncmp(command, "exit", 5) == 0)
		return(BUILTIN_EXIT);
	if (ft_strncmp(command, "echo", 5) == 0)
		return(BUILTIN_ECHO);
	if (ft_strncmp(command, "cd", 3) == 0)
		return(BUILTIN_CD);
	if (ft_strncmp(command, "pwd", 4) == 0)
		return(BUILTIN_PWD);
	if (ft_strncmp(command, "env", 4) == 0)
		return(BUILTIN_ENV);
	if (ft_strncmp(command, "unset", 5) == 0)
		return(BUILTIN_UNSET);
	if (ft_strncmp(command, "export", 5) == 0)
		return(BUILTIN_EXPORT);
	return (BUILTIN_FALSE);
}

int	execute_builtin(t_minishell *m, char **argv, t_builtin command)
{
	if (command == BUILTIN_EXIT)
		builtin_exit(argv, m);
	if (command == BUILTIN_ECHO)
		builtin_echo(argv); // @TODO: add command
	if (command == BUILTIN_CD)
		; // @TODO: add command
	if (command == BUILTIN_PWD)
		return (print_working_directory()); // @TODO: add command
	if (command == BUILTIN_ENV)
		; // @TODO: add command
	if (command == BUILTIN_UNSET)
		; // @TODO: add command
	if (command == BUILTIN_EXPORT)
		; // @TODO: add command
	return (0);
}

void command_not_found(t_minishell *m, char *cmd)
{
	stdout = stderr;
	printf("minishell: %s: command not found\n", cmd);
	m->exit_status = 127;
	builtin_exit(NULL, m);
}


char *get_cmd_with_path(t_arena *a, t_minishell *m, char *cmd)
{
	const uint32_t cmd_str_len = ft_strlen(cmd);
	char *path;
	uint32_t i;
	char *cmd_with_path;
	
	i = 0;
	path = find_env_var("PATH", 4, &i, m->envp);
	if (path == NULL || *path == '\0')
		return (cmd);
	while (path[i] != '\0')
	{
		i = 0;
		while (path[i] != ':' && path[i] != '\0')
			i++;
		cmd_with_path = arena_alloc(a, sizeof(char) * (i + cmd_str_len + 2));
		ft_memmove(cmd_with_path, path, i);
		cmd_with_path[i] = '/';
		ft_memmove(&cmd_with_path[i + 1], cmd, cmd_str_len);
		if (access(cmd_with_path, F_OK) == 0)
			return (cmd_with_path);
		arena_unalloc(a, sizeof(char) * (i + cmd_str_len + 2));
		path += i + (path[i] == ':');
	}
	command_not_found(m, cmd);
	return (NULL);
}

void run_command(t_minishell *m, char **argv)
{
	char *cmd_with_path;

	cmd_with_path = argv[0];
	if (ft_strchr(cmd_with_path, '/') == NULL)
		cmd_with_path = get_cmd_with_path(&m->node_arena, m, argv[0]); // replace with proper command finding function
	if (execve(cmd_with_path, argv, m->envp) == -1) // just have execve catch most error values
		perror("execve fail");
	printf("errno: %i\n", errno);
	m->exit_status = 1;
	if (errno == ENOENT)
		m->exit_status = 127;
	if (errno == EACCES)
		m->exit_status = 126;
	perror(argv[0]);
	builtin_exit(NULL, m);
}

void close_heredocs(t_minishell *m)
{
	uint32_t i;

	i = 0;
	while(i < m->heredoc_count)
	{
		if (m->heredoc_fds[i] != -1)
		{
			if (close(m->heredoc_fds[i]) == -1)
				syscall_failure(m);
		}
		i++;
	}
}

pid_t	execute_subprocess(t_minishell *m, char **argv, t_builtin builtin)
{
	pid_t	pid;

	for (int i = 0; argv[i] != NULL; ++i)
	{
		printf("i: %i\n", i);
		printf("argv[%i]: %s\n", i, argv[i]);
	}
	pid = fork();
	if (pid == (pid_t)(-1))
		; // @TODO: error cheking
	if (pid == 0)
	{
		apply_redirect(m);
		close_pipe(m);
		if (builtin != BUILTIN_FALSE)
		{
			execute_builtin(m, argv, builtin);
			exit(m->exit_status);
		}
		run_command(m, argv);
	}
	m->command_count += 1;
	return (pid);
}

void execute_command(t_minishell *m, char **argv, int status)
{
	pid_t pid;
	t_builtin builtin_type;

	if (argv == NULL)
		return ;
	if (status != 0)
	{
		pid = fork();
		if (pid == -1)
			; // @TODO: error cheking
		if (pid == 0)
			error_exit(m, status);
		m->command_count += 1;
		m->last_pid = pid;
		return ;
	}
	builtin_type = check_for_builtin(argv[0]);
	if (m->pipe_side == -1 && builtin_type != BUILTIN_FALSE)
	{
		execute_builtin(m, argv, builtin_type);
		return ;
	}
	m->last_pid = execute_subprocess(m, argv, builtin_type);
}

void	wait_for_sub_processes(t_minishell *minishell)
{
	uint32_t	i;
	int			wstatus;
	pid_t		pid;

	i = 0;
	printf("command count: %o\n", minishell->command_count);
	printf("last_pid = %i\n", minishell->last_pid);
	while (i < minishell->command_count)
	{
		pid = wait(&wstatus);
		if (pid == (pid_t)(-1))
			perror("wait issue");
		if (pid == minishell->last_pid)
		{
			printf("pid == %i\n", pid);
			printf("exit_status before = %i\n", minishell->exit_status);
			minishell->exit_status = WEXITSTATUS(wstatus);
			printf("exit_status after = %i\n", minishell->exit_status);

		}
		i++;
	}
}

void store_heredoc(t_minishell *m, int fd)
{
	uint32_t i;

	i = 0;
	store_read_fd(fd, m);
	while (i < m->heredoc_count)
	{
		if (m->heredoc_fds[i] == fd)
		{
			m->heredoc_fds[i] = -1;
			break ;
		}
		i++;
	}
}

int minishell_exec_loop(t_minishell *m, t_node *tree)
{
	t_node *current_head;
	int status;
	
	m->pipe_side = -1;
	status = 0;
	while (tree)
	{
		current_head = tree;
		if (m->pipe_side == READ)
			status = create_and_store_pipe(m, &m->pipe_side);
		if (tree->token.type == PIPE)
			status = create_and_store_pipe(m, &m->pipe_side);
		while (tree)
		{
			if(tree->token.type > 0) // maybe temp stuff
				store_heredoc(m, tree->token.type); // delimiter will still have quotes removed
			else if (tree->token.type == REDIRECT_OUT)
				status = redirect_out(tree->token.u_data.argv, m);
			else if (tree->token.type == REDIRECT_IN)
				status = redirect_in(tree->token.u_data.argv, m);
			else if (tree->token.type == REDIRECT_APPEND)
				status = redirect_append(tree->token.u_data.argv, m);
			else if (tree->token.type == WORD)
			{
				execute_command(m, tree->token.u_data.argv, status);
				break ;
			}
			tree = tree->left;	
		}
		reset_redirect(m);
		tree = current_head->right;
	}
	printf("arena_size at end of exec loop: %lu\n", m->node_arena.size);
	return (42);
}

void read_loop(t_minishell *m)
{
	t_node *tree;
	uint32_t i;

	while (1)
	{
		i = 0;
		m->command_count = 0;
		m->heredoc_count = 0;
		m->line = readline("minishell> ");
		if (m->line == NULL)
			break ;
		m->line_counter += 1;
		while (is_space(m->line[i]))
			i++;
		if (m->line[i] == '\0')
			continue ;
		add_history(m->line); // bash would add a line with only spaces to the history. I dont think that makes any sense so we'll look at it later
		tree = parser(&m->node_arena, &m->line[i]);
		if (tree != NULL)
			minishell_exec_loop(m, tree);
		wait_for_sub_processes(m);
		free(m->line);
		m->line = NULL;
		arena_reset(&m->node_arena);
	}
}

void minishell_cleanup(t_minishell *minishell)
{
	arena_delete(&minishell->node_arena);
	arena_delete(&minishell->scratch_arena);
	arena_delete(&minishell->env_arena);
	free(minishell->line);
}
// set default values for the minishell struct
// the struct is going to work as a kind of storage for globally needef alues
// we might want to pre allocate the arenas that will be used here to make cleanup easier
void init_minishell(t_minishell *minishell, char **envp)
{
	static int heredoc_fds_arr[16] = {0};

	minishell->node_arena = arena_new(DEFAULT_ARENA_CAPACITY);
	if (minishell->node_arena.data == NULL)
		; //@TODO: error cheking
	minishell->env_arena = arena_new(ARG_MAX);
	if (minishell->env_arena.data == NULL)
		; //@TODO: error cheking
	minishell->scratch_arena = arena_new(1024);
	if (minishell->scratch_arena.data == NULL)
		; //@TODO: error cheking
	minishell->line = NULL;
	minishell->command_count = 0;
	minishell->line_counter = 0;
	minishell->exit_status = 0;
	minishell->heredoc_fds = heredoc_fds_arr;
	minishell->heredoc_count = 0;
	minishell->envp = envp; // need to be our own env etc.. bla bla bal
	minishell->redir_fds[READ] = STDIN_FILENO;
	minishell->redir_fds[WRITE] = STDOUT_FILENO;
	minishell->pipe[READ] = -1;
	minishell->pipe[WRITE] = -1;
	minishell->last_pid = 0; 
	minishell->pids = NULL;
	get_minishell(minishell); // this will probably not be used so remeber to take care of it
}

int main(int argc, char *argv[], char *envp[])
{
	t_minishell minishell;

	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);
	read_loop(&minishell);
	minishell_cleanup(&minishell);
	printf("exit\n");
    return (0);
}
