/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:09:46 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/13 12:09:48 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	count_cmd(t_lexer *lexer)
{
	int	res;

	res = 0;
	while (lexer && lexer->token != PIPE)
	{
		++res;
		lexer = lexer->next;
	}
	return (res);
}

static void	process_redirections(t_lexer **tmp, t_pars_mini *pars_mini,
	int count_pipe)
{
	pars_mini->redirections = NULL;
	pars_mini->num_redirections = 0;
	if (separe_redirections(tmp, pars_mini, count_pipe))
	{
		printf("Error detaching redirections\n");
	}
}

static void	advance_to_next_pipe(t_lexer **tmp, int *count_pipe)
{
	while (*tmp && (*tmp)->token != PIPE)
		*tmp = (*tmp)->next;
}

void	parser_part(int count_pipe, t_lexer **lexer_list, t_shell *shell)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = *lexer_list;
	count_pipe++;
	shell->pars_mini = malloc(sizeof(t_pars_mini));
	if (!shell->pars_mini)
	{
		printf("Memory allocation failed for pars_mini\n");
		return ;
	}
	while (count_pipe > 0 && tmp)
	{
		process_redirections(&tmp, shell->pars_mini, count_pipe);
		if (!i++)
			*lexer_list = tmp;
		else
			tmp = tmp->next;
		if (!create_and_add_command(tmp, shell->pars_mini, &shell->commands))
			break ;
		if (!tmp || --count_pipe <= 0)
			break ;
		advance_to_next_pipe(&tmp, &count_pipe);
	}
}
