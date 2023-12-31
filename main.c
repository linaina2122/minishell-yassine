/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linaina <linaina@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:50:30 by yfawzi            #+#    #+#             */
/*   Updated: 2023/08/29 01:02:46 by linaina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_glo	glo;

int	check_for_var(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			return (1);
		i++;
	}
	return (0);
}

int	find_name_len(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (ft_isalnum(str[i]) > 0)
		{
			j++;
			i++;
		}
		if (j > 0)
			return (j);
		i++;
	}
	return (0);
}

char	*find_name(char *str)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	ret = malloc(find_name_len(str) + 1);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			while (ft_isalnum(str[i]) > 0)
			{
				ret[j++] = str[i++];
			}
			ret[j] = 0;
			return (ret);
		}
		i++;
	}
	return (0);
}

int	num_of_vars(char *str)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '$')
			{
				while (str[i] == '$')
					i++;
				while (str[i] && str[i] != '$')
					i++;
			}
			if (str[i] == '$' && str[i + 1])
				ret++;
		}
		i++;
	}
	return (ret);
}

int	pre_varlen(char *str)
{
	int	i;
	int	ret;

	if (str[0] == '\0')
		return (0);
	ret = 0;
	i = 0;
	i++;
	if (str[i] == ' ' || str[i] == '$')
	{
		while (str[i] == ' ')
			i++;
		if (str[i] == '$')
			return (-1);
	}
	i = 0;
	while (str[i] && str[i] != '$')
	{
		ret++;
		i++;
	}
	return (ret);
}

char	*ret_var(t_env *env, char *name)
{
	t_env	*tmp;

	tmp = env;
	name = ft_strjoin(name, "=");
	while (tmp)
	{
		if (ft_strlen(tmp->name) == ft_strlen(name))
		{
			if (ft_strncmp(tmp->name, name, ft_strlen(name)) == 0)
			{
				free(name);
				return (tmp->value);
			}
		}
		tmp = tmp->next;
	}
	free(name);
	return (0);
}

char	*check_var(char *str)
{
	int		i;
	int		k;
	char	*hol;
	char	*ret;

	i = 0;
	k = 0;
	k = pre_varlen(str);
	if ((size_t)k == ft_strlen(str) || k == -1)
		return (str);
	ret = ft_calloc(k + 1, k + 1);
	while (str[i])
	{
		hol = ft_substr(str, i, k);
		ret = ft_strjoin(ret, hol);
		free(hol);
		if (str[i] == '$' && str[i + 1] == '?')
		{
			hol = ft_itoa(glo.exit_status);
			ret = ft_strjoin(ret, hol);
			i+=2;
			free(hol);
		}
		else
		{
			if (str[i] == '$')
			{
				hol = ft_substr(ret_var(glo.env, find_name(str + i)), 0, ft_strlen(ret_var(glo.env, find_name(str))));
				ret = ft_strjoin(ret, hol);
				free(hol);
				i += find_name_len(str + i) + 1;
			}
			i += k;
			if (str[i] && str[i] == '$' && str[i + 1])
			{
				ret = ft_strjoin(ret, ret_var(glo.env, find_name(str)));
				i += find_name_len(str + i);
				i++;
				if (str[i])
					k = pre_varlen(str + i);
			}
			if (str[i])
			{
				if (k == 0 && str[i] != '$')
					k++;
				hol = ft_substr(str, i, k);
				ret = ft_strjoin(ret, hol);
				i += k;
				k--;
				free(hol);
			}
		}
	}
	free(str);
	return (ret);
}

t_args	*ret_com(char *str)
{
	t_args	*ret_args;
	t_args	*tmp;
	char	**args;
	int		i;
	int		check_for_double;

	ret_args = 0;
	if (check_quotes(str) < 0 || check_for_brackets(str) < 0)
		return (0);
	hidden_symbols(str);
	str = cleaned_spaces(str);
	if (check_for_space_error(str) < 0)
		return (0);
	if (check_for_pipes(str) < 0 || check_for_empty_pipe(str) < 0)
		return (0);
	check_for_double = check_if_valid(str);
	if (check_for_double >=  0)
	{
		if (check_for_double == 0)
			return (NULL); 
		str = fixed_pipes(str, check_for_double);
	}
	args = ft_split(str, '|');
	i = 0;
	while (args[i])
	{
		args[i] = ft_clean(args[i]);
		i++;
	}
	i = 0;
	while (args[i])
	{
		ft_lstadd_back(&ret_args, ft_lstnew_args(args[i]));
		i++;
	}
	ret_red(ret_args);
	tmp = ret_args;
	while (tmp)
	{
		i  = 0;
		while (tmp->command[i])
		{
			return_symbol(tmp->command[i]);
			tmp->command[i] = check_var(tmp->command[i]);
			i++;
		}
		tmp = tmp->next;
	}
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
	free(str);
	return (ret_args);
}

void	ft_printer(t_args *arg)
{
	int		i;
	int		hol;
	t_args	*args;

	i = 0;
	args = arg;
	while (args)
	{
		i = 0;
		while (args->command[i])
			printf("%s\n", args->command[i++]);
		i = 0;
		printf("num of red == %d\n", args->red[0][0]);
		hol = args->red[0][0];
		while (i < hol)
		{
			printf("red[%d] index == %d is: %d\n", i, args->red[2][i], args->red[1][i]);
			i++;
		}
		printf("-------------\n");
		args = args->next;
	}
}

void	hadnle_signle(int n)
{
	(void) n;
	write(1, "\n", 1);
	rl_replace_line ("", 0);
	rl_on_new_line();
	rl_redisplay();
	glo.exit_status = 1;
}

int main(int arc, char **arv, char **enva)
{
	t_env	*envar;
	t_args	*args;
	char	*line;
	(void)arc;
	(void)arv;
	envar = ret_env(enva);
	glo.env = envar;
	while (1)
	{
		signal (SIGINT, hadnle_signle);
		signal(SIGQUIT, SIG_IGN);
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			exit(1);
		}
		add_history(line);  
		args = ret_com(line);
		glo.args = args;
		ret_fd();
		final_cmd();
		// ft_printer(glo.args);
		execution();
		close_fd();
		ft_free(args);
	}
	free_list(envar);
}