/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:25:44 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/26 05:12:51 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	valid_cmd(char **cmd, char **env)
{
	char	**cmd_files;
	int		i;

	if (!cmd)
		return (1);
	cmd_files = split_path(cmd, env);
	if (!cmd_files)
		return (1);
	i = 0;
	while (cmd_files[i])
	{
		if (access(cmd_files[i], X_OK) == 0)
		{
			free_tab(cmd_files);
			return (0);
		}
		i++;
	}
	free_tab(cmd_files);
	return (1);
}

pid_t	process_cmd(t_shell *shell)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		free_all(shell, NULL);
		ft_putstr_fd("pipe error\n", 2);
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		free_all(shell, NULL);
		ft_putstr_fd("fork error\n", 2);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		redirect_child(shell, pipefd);
	if (shell->prev_fd != -1)
		close(shell->prev_fd);
	close(pipefd[1]);
	shell->prev_fd = pipefd[0];
	return (pid);
}

int	redirect_bis(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->cmd[i] != NULL && ft_strcmp(shell->cmd[i], "|"))
	{
		if (!ft_strcmp(shell->cmd[i], "<"))
		{
			shell->in_count++;
			if (redirect_input(shell, i) != 0)
				break ;
		}
		else if (!ft_strcmp(shell->cmd[i], ">") || (!ft_strcmp(shell->cmd[i],
					">>")))
		{
			shell->out_count++;
			if (redirect_output(shell, i) != 0)
				break ;
		}
		i++;
	}
	if ((shell->cmd[i] == NULL || !ft_strcmp(shell->cmd[i], "|")))
		return (process_cmd(shell));
	return (1);
}

int	process_redirect(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->cmd[i] != NULL && ft_strcmp(shell->cmd[i], "|"))
	{
		if (!ft_strcmp(shell->cmd[i], "<<"))
		{
			if (redirect_input(shell, i) == 130)
				return (130);
			shell->in_count++;
		}
		i++;
	}
	if (shell->size < 3)
		return (0);
	return (redirect_bis(shell));
}

int	redirect(t_shell *shell)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (shell->cmd[i] && shell->cmd[i + 1])
	{
		if (!ft_strcmp(shell->cmd[i], "|") && !ft_strcmp(shell->cmd[i + 1],
				"|"))
		{
			ft_putstr_fd("Minishell: syntax  error near unexepected token |\n",
				2);
			return (2);
		}
		i++;
	}
	shell->pipe_count = count_pipe(shell->cmd);
	shell->size = tab_size(shell->cmd);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		redirect_loop(shell);
	return (wait_ignore(pid));
}
