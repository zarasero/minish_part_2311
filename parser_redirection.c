/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:21:35 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/22 15:21:38 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	free_lexer_str(t_lexer *current)
{
	if (current->str)
	{
		free(current->str);
		current->str = NULL;
	}
	free(current);
	current = NULL;
}

static void	remove_lexer_list(t_lexer **lexer, t_lexer *current, t_lexer *prev)
{
	if (prev)
		prev->next = current->next;
	else
		*lexer = current->next;
	if (current->next)
		current->next->prev = prev;
}

void	delete_lexer(t_lexer **lexer, int index)
{
	t_lexer	*current;
	t_lexer	*prev;

	if (!lexer || !*lexer)
		return ;
	current = *lexer;
	prev = NULL;
	while (current && current->index != index)
	{
		prev = current;
		current = current->next;
	}
	if (!current)
		return ;
	remove_lexer_list(lexer, current, prev);
	free_lexer_str(current);
}

int	add_new_redir(t_lexer *current, t_lexer **lexer, t_pars_mini *pars_mini)
{
	t_lexer	*new;
	int		index1;
	int		index2;

	if (!current || !current->next)
		return (1);
	new = lexer_create(ft_strdup(current->next->str), current->token,
			pars_mini->num_redirections);
	if (!new)
		return (1);
	lexer_add_back(&pars_mini->redirections, new);
	index1 = current->index;
	index2 = current->next->index;
	delete_lexer(lexer, index1);
	delete_lexer(lexer, index2);
	pars_mini->num_redirections++;
	return (0);
}

int	separe_redirections(t_lexer **lexer, t_pars_mini *pars_mini, int count_pipe)
{
	t_lexer	*current;
	int		i;

	current = *lexer;
	i = 0;
	if (current && current->token == 1)
	{
		current = current->next;
		i++;
	}
	while (current && current->token != 1)
	{
		if (current->token > 1)
		{
			if (add_new_redir(current, lexer, pars_mini))
				return (1);
			if (*lexer == NULL)
				break ;
			current = *lexer;
			if (current->token == 1 && current->next && i > 0)
				current = current->next;
		}
		else if (current->token == 0 && current->next)
			current = current->next;
		else
			break ;
	}
	return (0);
}
