/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bghandri <bghandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 04:32:12 by bghandri          #+#    #+#             */
/*   Updated: 2023/07/24 00:36:16 by bghandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SIZE_PATH 4096
# define FIRST 0
# define DERNIER 1
# define INTER 2
# define FORK 355
# define CHILD 1000

extern int					g_code_exit;

enum						e_character_type
{
	ALPHANUM = 0,
	REDIRECT = 1,
	RED_IN = 2,
	RED_OUT = 3,
	PIPE = 4,
	ESPACE = 5,
	RED_D_IN = 6,
	RED_D_OUT = 7,
	QUOTE_S = 8,
	QUOTE_D = 9,
	LAST = 10
};

enum						e_token_type
{
	COMMANDE = 1,
	T_PIPE = 2,
	ARG = 3,
	INFILE = 4,
	OUTFILE = 5,
	REDIRECT_OUT = 6,
	REDIRECT_IN = 7,
	REDIRECT_D_OUT = 8,
	REDIRECT_D_IN = 9,
	LIMITEUR = 10,
	OPTION = 11,
};

enum
{
	SUCCESS = 0,
	ERROR = 1,
	MISUSE = 13,
	CANTEXEC = 126,
	NOTFOUND = 127,
	CSIGINT = 130,
	CMD_FOUND = 200,
	ERROR_PIPE = 201,
	ERROR_REDIRECT = 202,
	ERROR_PIPE2 = 203,
	ERROR_QUOTE_S = 204,
	ERROR_QUOTE_D = 205,
	ERROR_BACKSLASH = 206,
	OVERFLOW = 207,
	ERROR_REDIRECT2 = 208,
};

// test de ce a quoi vont les structures

// infos sur tout le token (a completer)
typedef struct token_info
{
	enum e_token_type		type;
}							t_token_info;

// chaque charactere est un character
typedef struct s_character
{
	char					value;
	enum e_character_type	type;
	struct s_character		*next;
}							t_character;

// chaque token est un ensemble de charactere du meme type
typedef struct token
{
	int						token_index;
	char					*value;
	t_token_info			*info;
	struct token			*next;
	struct token			*prev;
}							t_token;
typedef struct commande
{
	char					**cmd;
	struct commande			*next;
}							t_commande;

typedef struct s_global_exec
{
	char					**export;
}							t_global_exec;

typedef struct s_global_parsing
{
	int						*info_args;
	char					**args;
	t_token					*tokens;
	t_commande				*commande;
	char					*line;
	t_global_exec			*exec;
	char					***env;
}							t_global_parsing;

typedef struct s_split
{
	int						count;
	char					**result;
	char					**clean_result;
	int						space_count;
	int						i;
	int						len;
	int						in_quotes;
	char					type_inquote;
	int						start;
	int						len_str_to_add;
}							t_split;

typedef struct s_var_add_to_export
{
	char					**new_tab;
	int						db_tablen;
	char					*name_copy;
	char					*name_s_equal;
	char					*current_name;
	int						found;
}							t_var_add_to_export;

typedef struct s_expand_join
{
	char					*new_args;
	int						i;
	int						j;
	bool					expande_in;
}							t_expand_join;

typedef struct s_importe_expande
{
	char					*tmp;
	char					*new_args;
	int						info;
}							t_importe_expande;

typedef struct s_expande_var
{
	int						i;
	char					*new_args;
	int						*new_type_args;
	int						*nb_args;
}							t_expande_var;

typedef struct s_clean_null_var
{
	int						nb_new_args;
	char					**new_args;
	int						i;
	int						j;
}							t_clean_null_var;

typedef struct s_bachslash
{
	int						i;
	int						j;
	int						inquote;
	int						max;
	char					*result;
}							t_bachslash;

typedef struct exec
{
	char					*infile;
	char					*outfile;
	int						fd_infile;
	int						fd_outfile;
	int						fd_in_last_pipe;
	int						fd_heredoc;
	char					*limiteur;
	int						pos;
	int						nb_cmd;
	int						i;
	int						path_input;
	char					**path;
	char					*path_cmd;
	t_global_parsing		*g_parsing;
	t_token					*tokens;
}							t_exec;

// debug
void						verifyString(const char *str);
void						ft_print_token_debug(t_global_parsing *g_parsing,
								t_token *head);

// main
char						**ft_get_export(char **env);
void						minishell_loop(char ***env, t_global_exec *g_exec,
								t_global_parsing *g_parsing);
void						int_handler(int sig);
void						quit_handler(int sig);
int							ft_is_error_parsing(t_global_parsing *g_parsing,
								int nb_args);
int							ft_is_error_par_part2(t_global_parsing *g_parsing);
char						**ft_db_array_dup(char **db_array);
char						*ft_add_quotes_export(char *str);
void						ft_add_quote_exp(const char *str, int equal_found,
								char *new_str);
int							ft_custom_error(char **args);
void						ft_print_error_redirect(char **args);
int							ft_general_error(t_global_parsing *g_parsing);
void						ft_do_exec(char ***env, t_global_parsing *g_parsing,
								int nb_args);
void						ft_do_parsing(char ***env,
								t_global_parsing *g_parsing, int *nb_args);
void						ft_get_exit_code_lstat(char *const *cmd,
								struct stat *path_stat);
void						ft_check_error_exec(char **cmd);
int							ft_is_error_full_cmd(char *const *cmd,
								char *full_cmd);
void						gestion_pipe2(char ***env,
								t_global_parsing **g_parsing, int *nb_args);
void						gestion_unclosed_quote(char ***env,
								t_global_parsing **g_parsing, int *nb_args);
void						ft_free_g_parsing(t_global_parsing *g_parsing);
void						ft_set_index_for_exec(t_token **tokens);
int							only_misuse(char *str, char *invalid_char);
int							ft_check_full_cmd(char *cmd);
void						ft_check_not_found(char **cmd);
void						ft_init_global_parsing(t_global_parsing *g_parsing);

// DOSSIER EXEC
char						**ft_lexeur(char *line);
char						**ft_db_array_dup(char **db_array);
char						*get_name(char *arg);

// Dossier BUILTIN

// ------------------------ CD ------------------------
int							builtin_cd(char **args, char ***env);
void						ft_cd_tild(char **const *env, char *home);
void						ft_cd_tiret(char **const *env, char *prev_dir);
void						ft_change_env_after_cd(char ***env,
								char *target_dir);
void						ft_move_to_directory(char ***env, char *home,
								char *prev_dir, char *target_dir);
void						ft_init_cd_var(char **const *env, char **home,
								char **prev_dir);
int							ft_gestion_cd_error2(char **args, char *home,
								char *prev_dir);
int							ft_gestion_cd_error1(char *home, char *prev_dir,
								const char *target_dir);
char						*ft_getenv(char *name, char **env);
char						*ft_replace_env(char *name, char *value);
int							ft_setenv(char *name, char *value, char ***env);
int							ft_is_db_array(char *name, char **array);

// ------------------------ ECHO ------------------------
int							ft_print_echo_arg(char **args, int i,
								int print_newline);
int							ft_print_fullarg(int j, char *full_arg, char *tmp);

// ------------------------ ENV ------------------------
void						builtin_env(char **args, char **env);
// ------------------------ EXIT ------------------------
void						builtin_exit(char **args, t_global_parsing **g_pars,
								t_exec *info);
int							ft_gestion_exit_error(char *const *args,
								int nb_args, long long int nb_check,
								t_global_parsing **g_pars);
void						ft_do_exit(char *const *args,
								t_global_parsing *const *g_pars, t_exec *exec);
void						ft_check_atoll(char **str, int *negative);
long long int				ft_check_overflow(const char *str,
								long long int res, int negative);
// ------------------------ EXPORT ------------------------
void						ft_add_and_sort(char ***env, t_global_exec **g_exec,
								char *name, char *value);
void						ft_free_export_var(char *name, char *value,
								char *full_clean_str);
void						ft_do_export(char *const *args, char ***env,
								t_global_exec **g_exec);
int							ft_get_name(char *full_clean_str, int *i,
								char **name);
void						ft_remove_qt_value(const char *value,
								char *new_value, int i, int j);
char						**ft_get_new_tab(char **tab, char *str, int i,
								int db_tablen);
char						**ft_get_new_tab_export(char **tab, char *str,
								int i, int db_tablen);
int							ft_get_len_name(const char *arg, int i);
int							ft_get_len_solo(const char *long_name_cpy,
								int len_solo);
void						ft_init_vars(char **tab, char *name, int *i,
								t_var_add_to_export *vars);
int							ft_get_index_to_export(char *const *tab, char *name,
								int i, t_var_add_to_export *vars);
int							ft_no_egal_export(char *const *tab, char *str,
								int i);
void						ft_add_and_free_export(char *name,
								t_global_exec **g_exec, char *value_for_export,
								char *env_to_add_export);
void						ft_set_quotes_if_nul(char *name,
								char **value_for_export,
								char **env_to_add_export);
char						*ft_remove_egal_name(char *const *name);
int							ft_get_name(char *full_clean_str, int *i,
								char **name);
char						*ft_remove_egal_name(char *const *name);
void						ft_free_export_var(char *name, char *value,
								char *full_clean_str);
void						ft_add_and_sort(char ***env, t_global_exec **g_exec,
								char *name, char *value);
int							ft_is_alnum(char c);
void						ft_add_to_export(t_global_exec **g_exec,
								char *new_env, char *name);
char						*ft_clean_quote_str(char *value);
void						ft_remove_qt_value(const char *value,
								char *new_value, int i, int j);
void						ft_do_export(char *const *args, char ***env,
								t_global_exec **g_exec);
void						builtin_export(char **args, char ***env,
								t_global_exec **g_exec);
int							ft_get_name(char *full_clean_str, int *i,
								char **name);
char						*ft_remove_egal_name(char *const *name);
void						ft_free_export_var(char *name, char *value,
								char *full_clean_str);
void						ft_add_and_sort(char ***env, t_global_exec **g_exec,
								char *name, char *value);
int							ft_is_alnum(char c);
bool						ft_check_name(char *name);
int							ft_check_equal_error(char **args, int nb_args);
int							ft_check_no_args(char **args,
								t_global_exec **g_exec);
int							ft_check_solo_invalid_arg(char *args);
int							ft_check_export_err(char **args,
								t_global_exec **g_exec, int nb_args);
void						ft_add_env(char *name, char *value, char ***env,
								t_global_exec **g_exec);
void						ft_set_quotes_if_nul(char *name,
								char **value_for_export,
								char **env_to_add_export);
void						ft_add_and_free_export(char *name,
								t_global_exec **g_exec, char *value_for_export,
								char *env_to_add_export);
void						ft_sort_env(char ***env);
int							ft_is_alpha(char c);
char						*get_value_for_export(char *value);
char						*get_current_name(char *long_name);
int							ft_get_len_solo(const char *long_name_cpy,
								int len_solo);
char						**ft_add_to_db_tab_export(char **tab, char *str,
								char *name);
int							ft_no_egal_export(char *const *tab, char *str,
								int i);
int							ft_db_tablen(char **tab);
char						*get_name(char *arg);
int							ft_get_len_name(const char *arg, int i);
char						*get_value(char *arg);
char						*ft_get_env_to_add(char *name, char *value);
int							ft_get_index_to_export(char *const *tab, char *name,
								int i, t_var_add_to_export *vars);
void						ft_init_vars(char **tab, char *name, int *i,
								t_var_add_to_export *vars);
char						**ft_get_new_tab_export(char **tab, char *str,
								int i, int db_tablen);
char						**ft_add_to_db_tab(char **tab, char *str,
								char *name);
char						**ft_get_new_tab(char **tab, char *str, int i,
								int db_tablen);
// ------------------------ PWD ------------------------
void						ft_is_error_unset(const char *name);
void						ft_merge_rest(t_token **token_head,
								t_character **current_char);
char						*append_value(char *str, char c);
void						builtin_echo(char **args);
void						builtin_export(char **args, char ***env,
								t_global_exec **g_exec);
void						builtin_unset(char **args, char ***env,
								t_global_exec **g_exec);
void						builtin_pwd(char **args);
int							ft_check_solo_invalid_arg(char *args);
bool						ft_check_name(char *name);

// Dossier PARSING
bool						ft_gestion_bs(int nb_args, char *const *args, int i,
								int len_last_arg);
int							ft_is_impair_bs(int nb_bs);
bool						error_quote(int *type_args, int nb_args,
								char **args);
char						*get_prev_arg(char **arg, int i);
char						*get_next_arg(char **arg, int i);
bool						prev_next_error(char **args, int i);
bool						ft_is_file(char *str);
bool						ft_is_solo_expand(char *str);
bool						prev_next_redi_error(char **args, int i,
								int *type_args, int nb_args);
bool						error_expand(int type, char *next, char **env);
bool						error_size_or_spe_redi(int *type_args, int nb_args,
								char **args, char **env);
bool						error_pipe(int *type_args, int nb_args,
								char **args);

char						*expand_exit_code(char *expande);
char						*traslate_expand(char *arg_ct, char *ext_exp,
								int *i, char **env);
int							ft_do_gestion_expand(int **type_args,
								t_global_parsing **g_pars, char **env,
								t_expande_var *v);
void						ft_gestion_dollar_expand(int *const *type_args,
								const int *nb_args,
								t_global_parsing *const *g_pars,
								t_expande_var *v);
void						ft_gestion_digit_expand(
								t_global_parsing *const *g_pars,
								t_expande_var *v);
char						*replace_expande(char *args, int *i, char **env,
								int *info);
char						*importe_expande(char *args, char **env);
bool						have_expande(char *args);
int							ft_nb_args_out_null(char **old_arg,
								int size_db_arr);
char						**ft_clean_null_db_array(char **old_arg,
								int *size_db_arr);
int							ft_size_of_expende(char *expande);
char						*get_expende_detect(int size_of_expende,
								char *expande);
char						*is_sp_expand(char *expand_search);
char						*give_env_expand(char *expande_search, int size,
								char **env);
int							join_expand(char *expande, int *j, int s_extract,
								char *new_args);
bool						back_slach(char *args, int i);
void						ft_init_var_clean_null(char **old_arg,
								const int *size_db_arr, t_clean_null_var *var);
bool						solo_dolard(char *str);
bool						is_limiteur(int *type_args, int i);
char						*exit_code_expande(char *args, int exit);
void						expande(int **type_args, int *nb_args,
								t_global_parsing **g_pars, char **env);
char						*join_and_rp_args(char *args_con, char *expande,
								int s_extract, int *info);
int							ft_go_next_arg(const char *args_con,
								t_expand_join *s_ex_join);
void						ft_fill_new_args(const char *args_con,
								const int *info, t_expand_join *s_ex_join);
int							ft_init_s_ex_join(char *args_con,
								const char *expande, int s_extract,
								t_expand_join *s_ex_join);
char						*ft_malloc_expand(const char *args_con,
								const char *expande, int s_extract,
								char *new_args);

char						*remove_backslash(char *str);
void						ft_get_result_bs(const char *str, char *result,
								int *i, int *j);
int							is_backslash(char *str);
void						ft_gestion_backslash(int **type_args, int *nb_args,
								t_global_parsing **g_pars);
bool						fist_error(int *type_args);
bool						error_grammaticale(int *type_args, int nb_args);
int							count_nb_space(int *type_args, int nb_args);
void						b_zero_last_args(char **last_args, int nb_new_args,
								int i);
char						**kick_args_space(char **new_args, int *type_args,
								int *nb_args);

char						*ft_db_array_join(char **strings, int count);
void						ft_get_result_db_join(char **strings, int count,
								char *result, int len);
void						change_cmd_list(t_commande *list_commande);
void						ft_change_cmds(t_commande *list_commande, int *i);
t_commande					*cmd_complete(t_token *token);
void						init_struc_cmd(t_commande *commande);
int							nb_pipe(t_token *info);
void						creat_cmd_list(t_commande *cmd, int nb_node);
char						*ft_join_cmd(char *s1, char *s2);
void						b_zero_for_cmd_join(char **cmd_join, int nb_cdm,
								int i);
char						**give_cmd_join(t_token *token, int nb_cdm);
int							ft_malloc_cmd_join(int nb_cdm, char ***cmd_join,
								int i);
void						ft_do_join_cmd(char **cmd_join, int i,
								t_token *tok_sch, int *is_first);
char						*ft_strncpy(char *dest, char *src, unsigned int n);
void						ft_free_db_by_len(char **array, int len);
int							count_spaces(char *str, int len);
char						**allocate_result_array(int space_count);
void						ft_do_split_custom(char *str, t_split *s);
void						ft_is_in_quote_split(const char *str, t_split *s);
int							ft_init_result(char *str, t_split *s);
void						ft_init_s(char *str, t_split *s);
char						**ft_custom_split(char *str);
void						ft_add_last_str_split(char *str, t_split *s);
void						add_cmd_to_list_commande(t_commande *list_commande,
								char **cmd_join);
void						free_list_commande(t_commande *commande);
int							ft_db_arr_len(char **arr);
int							ft_get_type2(char *const *line_split, int i,
								int *type);
int							*ft_get_info_args2(char **line_split,
								int *give_nb_args);

char						**ft_parsing(int *nb_args,
								t_global_parsing **g_pars, char ***env);
int							*ft_get_info_args(char **line_split,
								int *give_nb_token);
char						**join_inter_space(char **args, int *type_args,
								int *nb_args);
char						**kick_quote(int *type_args, int nb_args,
								char **args);
t_token						*ft_get_tokens_with_infos(char **args, int nb_args);
int							ft_isprint(int chara);
int							ft_is_outfile(t_token *token);
int							ft_is_infile(t_token *token);
t_token						*addtoken(t_token *head, char *value);
int							ft_get_infos_by_pos(t_token *token);
char						*copy_cont(char *str);
char						*copy_sans_quote(char *args);
char						*copy_sans_quote(char *args);

// Dossier UTILS
void						ft_complete(char const *s, char *str, int *i);
char						*ft_strjoin(char *s1, char *s2);
int							ft_strlen(char *str);
char						**ft_split(char *str, char charset);
char						*ft_strdup(char *src);
int							ft_strncmp(char *s1, char *s2, unsigned int n);
char						*ft_strcpy(char *dest, char *src);
char						*ft_strndup(const char *s1, size_t n);
void						*ft_memcpy(void *dst, const void *src, size_t n);
int							ft_is_upper(char c);
char						*ft_strchr(const char *s, int c);
int							ft_strcmp(char *s1, char *s2);
size_t						ft_strlcpy(char *dst, const char *src, size_t size);
char						*ft_strtrim(char *s1, char *set);
void						free_db_array(char **array);
void						free_list_tokens(t_token *tokens);
void						free_list_commande(t_commande *commande);
int							ft_db_tablen(char **tab);
void						ft_free_g_parsing(t_global_parsing *g_parsing);
char						*ft_itoa(int num);
char						*ft_strcat(char *s1, const char *s2);
int							ft_is_digit(char c);
char						*ft_substr(char *s, unsigned int start, size_t len);
char						*ft_strstr(char *str, char *to_find);
int							is_only_space(char *str);
int							ft_db_arr_len(char **arr);
void						ft_bzero(void *s, size_t n);
int							ft_fprintf(int fd, const char *format, ...);

// Dossier LEXEUR
t_token						*merge_characters(t_character *head);
t_token						*ft_handle_quotes(t_token *tokenHead,
								t_character **current_charPtr);
char						*append_value(char *str, char c);
t_character					*ft_parse_string(const char *input);
void						get_current_charptr(t_character **current_charPtr,
								t_character *next_char);

// Dossier error_args
bool						search_error_args(int *type_args, int *nb_args,
								char **args, char **env);
bool						error_no_only_type(char *str);
bool						error_size(char *args, int size_max);
void						give_sp_args_redi(char *args, int *type_args,
								int pos);
bool						quote_is_open(char *args);

// Dossier EXPAND
char						*exit_code_expande(char *args, int exit);
char						*importe_expande(char *args, char **env);
void						expande(int **type_args, int *nb_args,
								t_global_parsing **g_pars, char **env);
bool						back_slach(char *args, int i);
char						*join_and_rp_args(char *args_con, char *expande,
								int size_extract, int *info);
char						*give_env_expand(char *expande_search, int size,
								char **env);
char						*give_env_expand(char *expande_search, int size,
								char **env);
char						*is_sp_expand(char *expand_search);
char						*get_expende_detect(int size_of_expende,
								char *expande);
int							ft_size_of_expende(char *expande);
bool						solo_dolard(char *str);
char						**ft_clean_null_db_array(char **old_arg,
								int *size_db_arr);

// Dossier INFOS_TOKENS
int							ft_is_command(t_token *token);
int							ft_is_argument(char *value);
int							ft_is_option(char *value);
int							is_env(t_token *token);
t_commande					*cmd_complete(t_token *token);
char						*ft_join_cmd(char *s1, char *s2);
t_token						*ft_verif_cmd(t_token **tokenHead);
t_token						*create_token(t_token *head, char *value,
								int index);
t_token_info				*add_infos_to_token(char *value, t_token *token,
								int *is_commande_in_pipe);
t_token						*ft_save_next_tokens(t_token *current);
void						ft_free_value_splited(char **value_splited);
void						ft_free_paths(char **path_splited, char *pathCopy);
void						ft_free_paths2(char **path_splited, char *pathCopy,
								char *fullPath);
int							nb_pipe(t_token *info);

// Dossier exec
void						init_exec(t_exec *exec);
int							nb_token_whis_index(t_token *tokens, int index);
t_token						*creat_info_token_list(t_token *tokens,
								int nb_node);
int							copy_cont_token(t_token *dest, t_token *src);
t_token						*dup_info(t_token *info_token, t_token *tokens,
								int index);
t_token						*get_info_token(t_token *tokens, int index);
bool						is_token_redi_in(t_token *token);
void						new_infile(t_exec *exec, t_token *token);
void						write_in_heredoc(int fd_heredoc, char *line,
								int size_line);
void						quit_heredoc(t_exec *exec, char *line);
void						heredoc_loop(t_exec *exec, int exit_code,
								char *line, char ***env);
void						start_heredoc(t_exec *exec, int exit_code);
void						destruction_infile(t_exec *exec);
void						dectruction_heredoc(t_exec *exec);
int							wait_heredoc(int info, t_exec *exec, int tmp_error,
								pid_t pid);
int							new_heredoc(t_exec *exec, t_token *token);
int							set_new_infile(t_exec *exec, t_token *tokens);
int							gestion_infile(t_token *tokens, t_exec *exec);
void						set_new_outfile(t_exec *exec, t_token *token);
bool						is_token_redi_out(t_token *token);
void						gestion_outfile(t_token *tokens, t_exec *exec);
char						*for_test(char *test, char *argv);
int							find_path(char **path, char *cmd, t_exec *info);
int							add_one(int *is_bultin);
int							ft_bultins_fork(char **cmd, char ***env,
								t_exec *info);
int							add_zero(int *info_return);
int							is_bultins_not_fork(char **cmd, char ***env,
								t_exec *info, int pos);
void						null_cmd(t_exec *info);
void						fail_execve(t_exec *info, char **cmd, char *path,
								t_token *token);
int							exec_cmd(t_exec *info, char ***env, char **cmd,
								t_token *tokens);
void						close_for_first(int *pipefd, t_exec *info);
int							creat_pipe_and_file(t_exec *info, int *pipefd);
int							dup_in_fork_first_inter(t_exec *info, int *pipefd);
int							first(char **cmd, t_exec *info, char ***env,
								t_token *tokens);
void						close_inter(int *pipefd, t_exec *info);
int							gestion_file_inter(t_exec *info, int *pipefd);
int							inter(char **cmd, t_exec *info, char ***env,
								t_token *tokens);
int							gestion_file_last(t_exec *info);
int							close_last(t_exec *info);
int							dup_in_fork_last_solo(t_exec *info);
int							last(char **cmd, t_exec *info, char ***env,
								t_token *tokens);
int							start_exec(char **cdm, t_exec *info, char ***env,
								t_token *tokens);
void						close_for_solo_and_free(t_exec *info);
int							file_solo(t_exec *info);
int							solo_exec(char **cmd, t_exec *info, char ***env,
								t_token *tokens);
int							start_exec_one(t_token *tokens, char **cmd,
								t_exec *exec, char ***env);
int							start_exec_mult(t_token *tokens, char **cmd,
								t_exec *exec, char ***env);
int							get_path(t_exec *exec, char **envp);
void						free_name_file(t_exec *exec);
void						ft_free_g_parsing_total(
								t_global_parsing *g_parsing);
void						free_path(t_exec *exec);
int							exec_one(t_token *tokens, t_commande *cmd,
								char ***env, t_exec *exec);
int							exec_multi(t_token *tokens, t_commande *cmd,
								char ***env, t_exec *exec);
void						exec(t_token *tokens, t_commande *cmd, char ***env,
								t_global_parsing **g_pars);

#endif
