/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 07:42:28 by throbert          #+#    #+#             */
/*   Updated: 2025/02/26 08:39:38 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	contain_wildcard(char *current_token)
{
	if (ft_strchr(current_token, '*'))
		return (1);
	return (0);
}

static int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

char	**append_to_result(char **res, char **app)
{
	char	**new;
	int		len_result;
	int		len_append;
	int		i;
	
	len_result = ft_tablen(res);
	len_append = ft_tablen(app);
	new = malloc(sizeof(char *) * (len_result + len_append + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < len_result && !ft_strchr(res[i], '*'))
	{
		new[i] = ft_strdup(res[i]);
		i++;
	}
	len_append = 0;
	while (app[len_append])
	{
		new[i] = ft_strdup(app[len_append]);
		i++;
		len_append++;
	}
	new[i] = NULL;
	return (new);
}

char	**fill_new_cmd(char *token_to_expand, char **token_cmd)
{
	char			**cmd_wildcard;
	struct dirent	*entry;
	int				size;
	int				j;
	DIR				*curr_dir;
	char			**result;
	int				bool_append;

	bool_append = 0;
	result = token_cmd;
	size = 0;
	j = 0;
	cmd_wildcard = NULL;
	if ((ft_strchr(token_to_expand, '"') || ft_strchr(token_to_expand, '\'')))
	{
	    char *dup_tok;
	
	    dup_tok = ft_strdup(token_to_expand);
	    dup_tok = remove_quotes(dup_tok);
	    cmd_wildcard = malloc(sizeof(char *) * 2);
	    if (!cmd_wildcard)
	    {
	        free(dup_tok);
	        return (NULL);
	    }
		cmd_wildcard[0] = malloc(sizeof(char) * (ft_strlen(dup_tok) + 1));
	    cmd_wildcard[0] = dup_tok;
	    cmd_wildcard[1] = NULL;
	    return (append_to_result(result, cmd_wildcard));
	}
	curr_dir = opendir(".");
	while ((entry = readdir(curr_dir)))
	{
		if (match_pattern(entry->d_name, token_to_expand))
			size++;
	}
	closedir(curr_dir);
	cmd_wildcard = malloc(sizeof(char *) * (size + 1));
	if (!cmd_wildcard)
		return (NULL);
	j = 0;
	curr_dir = opendir(".");
	while ((entry = readdir(curr_dir)))
	{
		if (match_pattern(entry->d_name, token_to_expand))
		{
			bool_append ++;
			cmd_wildcard[j] = ft_strdup(entry->d_name);
			j++;
		}
	}
	closedir(curr_dir);
	cmd_wildcard[j] = NULL;
	if (bool_append)
		result = append_to_result(result, cmd_wildcard);
	return (result);
}

char	**expand_wildcard(char **token_cmd)
{
	char	**cmd_wildcard_to_append;
	int		i;

	i = 0;
	cmd_wildcard_to_append = NULL;
	while (token_cmd[i])
	{
		if (contain_wildcard(token_cmd[i]))
		{
			cmd_wildcard_to_append = fill_new_cmd(token_cmd[i], token_cmd);
		}
		i++;
	}
	if (!cmd_wildcard_to_append)
		return (token_cmd);
	while (cmd_wildcard_to_append[i])
		i++;
	cmd_wildcard_to_append[i] = NULL;
	return (cmd_wildcard_to_append);
}
