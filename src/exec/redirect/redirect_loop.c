/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:43:41 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/26 05:11:24 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	check_next_pipe(t_shell *shell)
{
	char	**cmd;
	int		i;

	i = 0;
	cmd = ft_calloc(5000, sizeof(char *));
	while (shell->cmd && shell->cmd[i] && ft_strcmp(shell->cmd[i], "|"))
	{
		cmd[i] = ft_strdup(shell->cmd[i]);
		i++;
	}
	cmd[i] = NULL;
	if (cmd && cmd[0] && ((ft_tabtabchr(cmd, "<<") || ft_tabchr(cmd, '<'))))
		dup2(shell->save_in, STDIN_FILENO);
	dup2(shell->save_out, STDOUT_FILENO);
	free_tab(cmd);
}

static void	redirect_exit(t_shell *shell)
{
	close(shell->save_in);
	close(shell->save_out);
	free_all(shell, NULL);
	exit(130);
}

void	redirect_loop(t_shell *shell)
{
	pid_t	*wait_pid;
	int		exit_code;
	int		i;

	i = 0;
	shell->prev_fd = -1;
	wait_pid = malloc(sizeof(pid_t) * (shell->pipe_count + 1));
	shell->save_in = dup(STDIN_FILENO);
	while (shell->pipe_count != -1)
	{
		shell->out_count = 0;
		if (shell->in_count == 0 && shell->out_count != 0)
			shell->save_in = dup(STDIN_FILENO);
		shell->in_count = 0;
		shell->save_out = dup(STDOUT_FILENO);
		wait_pid[i] = process_redirect(shell);
		if (wait_pid[i] == 130)
			redirect_exit(shell);
		shell->cmd = next_pipe(shell->cmd);
		check_next_pipe(shell);
		shell->pipe_count--;
		close(shell->save_out);
		close(shell->save_in);
		i++;
	}
	exit_code = 0;
	while (i != -1)
	{
		exit_code = wait_ignore(wait_pid[i]);
		i--;
	}
	free_all(shell, NULL);
	exit(exit_code);
}

void	do_redirect(t_shell *shell, int pipefd[2])
{
	if (shell->prev_fd != -1 && shell->in_count == 0)
	{
		dup2(shell->prev_fd, STDIN_FILENO);
		close(shell->prev_fd);
	}
	if (shell->pipe_count != 0 && shell->out_count == 0)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	redirect_child(t_shell *shell, int pipefd[2])
{
	char	**cmd;

	do_redirect(shell, pipefd);
	cmd = get_flag(shell);
	if (is_builtin(shell)->check == 0)
	{
		free_tab(shell->cmd);
		shell->cmd = cmd;
		exec_builtin(shell->cmd, shell);
		free_all(shell, NULL);
		exit(0);
	}
	if (valid_cmd(cmd, shell->env) == 1)
		not_valid_cmd(shell, cmd);
	binary_path(cmd, shell);
	exec_path(cmd, shell);
}
