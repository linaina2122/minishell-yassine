/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linaina <linaina@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:15:05 by yfawzi            #+#    #+#             */
/*   Updated: 2023/08/27 23:15:06 by linaina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(char **command)
{
	int		i;
	int		j;
	int		k;

	i = 1;
	j = 1;
	k = 0;
	if (!command[1])
		printf("\n");
	while (command[i])
	{
		if (command[i][0] == '-')
		{
			while (command[i][j] == 'n')
				j++;
			if (command[i][j] != 'n' && command[i][j] != '\0')
				break ;
			k = 1;
		}
		if (command[i][0] != '-')
			break ;
		j = 0;
		i++;
	}
	if (k == 0)
		i = 1;
	if (command[i])
	{
		if (k > 0)
		{
			while (command[i])
			{
				if (command[i + 1])
					printf("%s ", command[i++]);
				else
					printf("%s", command[i++]);
			}
		}
		else
		{
			while (command[i])
			{
				if (command[i + 1])
					printf("%s ", command[i++]);
				else
					printf("%s", command[i++]);
			}
			printf("\n");
		}
	}
}

t_env	*look_for_pwd(void)
{
	t_env	*tmp;

	tmp = glo.env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "PWD=") > 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (0);
}

void	look_for_old(void)
{
	t_env	*tmp;
	t_env	*tmp1;

	tmp = glo.env;
	tmp1 = glo.env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "OLDPWD=") > 0)
		{
			free(tmp->value);
			tmp1 = look_for_pwd();
			tmp->value = ft_strdup(tmp1->value);
		}
		tmp = tmp->next;
	}
}

void	look_for_new(void)
{
	t_env	*tmp;

	tmp = glo.env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "PWD=") > 0)
		{
			free(tmp->value);
			tmp->value = 0;
			break ;
		}
		tmp = tmp->next;
	}
}
void	cd(char **command)
{
	int		val;
	char	*str;
	char	*str1;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!command[1])
		return ;
	val = chdir(command[1]);
	if (val < 0)
	{
		str = getcwd(0, 0);
		i = ft_strlen(str);
		j = ft_strlen(command[1]);
		str1 = malloc(i + j + 1);
		str1 = ft_strcpy(str1, str);
		str1[i] = '/';
		str1[i + 1] = '\0';
		str1 = ft_strjoin(str1, command[1]);
		val = chdir(str1);
		free(str1);
		if (val < 0)
			perror("cd");
	}
}

void	pwd(void)
{
	char	*str;

	str = getcwd(0, 0);
	printf("%s\n", str);
	free(str);
}

void	ft_exit(void)
{
	ft_putendl_fd("gggggggsdagdash", 2);
	exit(0);
}

void	env(void)
{
	t_env	*tmp;

	tmp = glo.env;
	while (tmp)
	{
		printf("%s", tmp->name);
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
}

char	*add_eq(char *str)
{
	char	*ret;
	int		i;

	i = 0;
	ret = malloc(ft_strlen(str) + 2);
	while (str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '=';
	ret[i + 1] = 0;
	free(str);
	return (ret);
}

void ft_unset(char	**str)
{
	t_env	*tmp;
	t_env	*tmp1;
	int		i;
	int		j;
	int		k;

	i = 1;
	j = 0;
	k = 0;
	while (str[i])
	{
		str[i] = add_eq(str[i]);
		i++;
	}
	i = 1;
	while (str[i])
	{
		tmp = glo.env;
		tmp1 = glo.env;
		while (tmp1)
		{
			if (ft_strlen(str[i]) == ft_strlen(tmp1->name))
			{
				if (ft_strcmp(str[i], tmp1->name) == 1)
				{
					glo.env = ret_new_env(k);
					break ;
				}
			}
			if (j > 0)
				tmp = tmp->next;
			tmp1 = tmp1->next;
			j++;
			k++;
		}
		i++;
	}
}

int	check_for_export(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (-1);
}

void	add_new_var(char *str0, char *str1)
{
	t_env	*last;
	t_env	*new_elem;

	new_elem = malloc(sizeof(t_env));
	last = glo.env;
	while (last->next)
	{
		last = last->next;
	}
	new_elem->name = ft_strdup(str0);
	if (!str1)
		new_elem->value = ft_strdup("\n");
	else
		new_elem->value = ft_strdup(str1);
	new_elem->next = NULL;
	last->next = new_elem;
}

void	export(char **str)
{
	t_env	*tmp;
	char	**hol;
	int		i;
	int		j;

	j = 0;
	if (!str[1])
	{
		tmp = glo.env;
		while (tmp)
		{
			printf("declare -x ");
			printf("%s", tmp->name);
			printf("%s\n", tmp->value);
			tmp = tmp->next;
		}
		return ;
	}
	while (str[j])
	{
		if (check_for_export(str[j]) < 0)
			return ;
		i = 0;
		tmp = glo.env;
		hol = ft_split(str[j], '=');
		hol[0] = ft_strjoin(hol[0], "=");
		while (tmp)
		{
			if (ft_strlen(tmp->name) == ft_strlen(hol[0]))
			{
				if (ft_strcmp(hol[0], tmp->name) == 1)
					break ;
			}
			tmp = tmp->next;
		}
		if (tmp)
		{
			free(tmp->value);
			tmp->value = ft_strdup(hol[1])	;
			while (hol[i])
				free(hol[i++]);
			free(hol);
			return ;
		}
		add_new_var(hol[0], hol[1]);
		while (hol[i])
			free(hol[i++]);
		free(hol);
		j++;
	}
}