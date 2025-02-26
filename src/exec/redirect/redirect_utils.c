/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 01:54:43 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/26 04:50:05 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_pipe(char **cmd)
{
	int	pipe_count;
	int	i;

	i = 0;
	pipe_count = 0;
	while (cmd[i] != NULL)
	{
		if (!ft_strcmp(cmd[i], "|"))
			pipe_count++;
		i++;
	}
	return (pipe_count);
}

int	is_redirect(char *cmd)
{
	if (!ft_strcmp(cmd, "<") || !ft_strcmp(cmd, ">") || !ft_strcmp(cmd, ">>")
		|| !ft_strcmp(cmd, "<<"))
	{
		return (1);
	}
	return (0);
}

void	not_valid_cmd(t_shell *shell, char **cmd)
{
	if (cmd && cmd[0])
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	free_all(shell, cmd);
	exit(127);
}
