/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:35:18 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/13 12:35:21 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

//# include "../libft/libft.h"
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <sys/wait.h>
# include <errno.h>
# include <pthread.h>
# include <sys/time.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <ctype.h>

extern int	g_global_exit;

typedef enum s_tokens
{
	WORD,
	PIPE,
	IN,//<
	HEREDOC,// <<
	OUT,//>
	APPEND,//>>
}	t_tokens;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	int				index;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_simple_cmds
{
	char					**str;
	int						num_redirections;
	char					*hd_file_name;
	t_lexer					*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

typedef struct s_pars_mini
{
	t_lexer			*lexer_list;
	t_lexer			*redirections;
	int				num_redirections;
	char			**str;
	struct s_shell	*shell;
}	t_pars_mini;

typedef struct s_shell
{
	char			*input_line;
	int				count_pipe;
	t_lexer			*lexer_list;
	t_simple_cmds	*pars;
	t_pars_mini		*pars_mini;
	char			**envp;
	t_simple_cmds	*commands;
}	t_shell;

typedef struct s_temp
{
	int			i;
	int			j;
}	t_temp;

void			ft_free_lex(t_shell *shell);
void			ft_free_pars(t_shell *shell);
void			free_pars(t_simple_cmds *pars);
void			free_lexer_list(t_lexer **lexer);
void			free_redirections(t_lexer *redirection);
void			free_simple_cmds_list(t_simple_cmds *commands);
void			free_command(char **cmd);
char			*ft_strjoin_free(char *s1, char *s2);
void			ft_free_tous(t_shell *shell);

int				ft_minicheck(char *line);
int				find_matching_quote(char *line, int start, char quote);
int				check_quotes(char *line);
int				ft_check_errors(t_lexer *list);

t_lexer			*lexer_create(char *value, t_tokens token, int index);
void			lexer_add_back(t_lexer **list, t_lexer *new_token);
void			ft_create_lexer_list(char *value, t_tokens type, int index,
					t_lexer **list);
int				ft_read_word(char *line);
int				ft_read_word_quote(char *line, char quote);
int				ft_read_token_1(char *line, t_lexer **list, int index);
void			ft_read_token(char *line, t_lexer **list, t_shell *shell);
char			*process_quoted_or_unquoted(char **line, int *total_len);

int				special_tokens(char *line, t_lexer **list, int *index,
					t_shell *shell);
int				create_token(char *value, int type, t_lexer **list, int *index);
int				ft_process1(char **line);
char			*ft_process2(char *result, char *temp);

void			print_tokens(t_lexer *list);
void			print_simple_cmds(t_simple_cmds *cmds);
int				ft_ifspace(char line);
size_t			ft_strlen(const char *s);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_substr(const char *str, size_t start, size_t len);
char			*ft_strdup(const char *s);
char			*ft_itoa(int n);

void			ft_init_shell(t_shell *shell, char **envp);
void			ft_start_loop(char **envp);

void			delete_lexer(t_lexer **lexer, int index);
int				add_new_redir(t_lexer *current, t_lexer **lexer,
					t_pars_mini *pars_mini);
int				separe_redirections(t_lexer **lexer, t_pars_mini *pars_mini,
					int count_pipe);
int				count_cmd(t_lexer *lexer);
char			**create_command(t_lexer *lexer);
t_simple_cmds	*create_and_add_command(t_lexer *tmp,
					t_pars_mini *pars_mini, t_simple_cmds **commands);
t_simple_cmds	*new_simple_cmd(t_lexer *lexer, t_pars_mini *pars_mini);
t_simple_cmds	*last_simple_cmd(t_simple_cmds *list);
void			add_new_simple_cmd(t_simple_cmds **list, t_simple_cmds *new);
void			parser_part(int count_pipe, t_lexer **lexer_list,
					t_shell *shell);

void			expand_part(t_shell *shell);
char			*process_str(const char *input, t_shell *shell);
char			*allocate_result(int len);
int				ft_trouve_len(const char *input, char **envp);
char			*get_env_value(const char *var_name, char **envp);
void			handle_dollar(t_shell *shell, const char *str,
					t_temp *temp, char *result);
void			handle_quote(const char *input, int *i, char *quote, int *len);
void			han_quote1(const char *input, char *quote,
					t_temp *temp, char *result);
void			handle_variable(const char *input, int *i, int *len,
					char **envp);
void			handle_env_variable(const char *input, int *i,
					int *len, char **envp);
void			han_env1(t_shell *shell, const char *str, t_temp *temp,
					char *result);
void			handle_exit_status(char *result, t_temp *temp);
void			ft_signal_ctr_d(void);
void			ft_signal_ctr_c_hd(int status);
void			ft_signal_ctr_c(int status);
void			signals_heredoc(void);
void			signals(void);

#endif