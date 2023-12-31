/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linaina <linaina@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 00:52:47 by yfawzi            #+#    #+#             */
/*   Updated: 2023/08/28 23:52:48 by linaina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_valid(char *str)
{
	int	i;
	int	pipnum;

	i = 0;
	pipnum = 0;
	while (str[i])
	{
		if (str[i] == '|')
		{
			while (str[i + 1] == ' ' || str[i + 1] == '\t')
				i++;	
			if (str[i + 1] && str[i + 1] == '|')
				return (pipnum + 1);
			pipnum++;
		}
		i++;
	}
	return (-1);
}

int	cleaned_len(char *str)
{
	int	i;
	int	ret;
	int	hol;

	ret = 0;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			hol = first_quote(str + i, str[i]);
			while (str[i] && hol)
			{
				i++;
				ret++;
				hol--;
			}
		}
		if (str[i] == ' ' || str[i] == '\t')
		{
			if (str[i + 1] == ' ' || str[i] == '\t')
			{
				while (str[i] == ' ' || str[i] == '\t')
					i++;
				ret++;
			}
		}
		ret++;
		i++;
	}
	return (ret);
}

char	*ft_clean(char *str)
{
	int		i;
	int		j;
	char	*ret;
	int		hol;

	i = 0;
	j = 0;
	ret = malloc(cleaned_len(str) + 1);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			hol = first_quote(str + i, str[i]);
			while (str[i] && hol)
			{
				ret[j] = str[i];
				i++;
				j++;
				hol--;
			}
		}
		if (str[i] == ' ' && str[i] == '\t')
		{
			while (str[i + 1] == ' ' || str[i + 1] == '\t')
				i++;
		}
		ret[j] = str[i];
		i++;
		j++;
	}
	ret[j] = 0;
	free(str);
	return (ret);
}

int	ft_spaces_len(char	*str)
{
	int		i;
	int		ret;
	char	hol;

	ret = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			hol = str[i++];
			while (str[i] && str[i] != hol)
			{
				ret++;
				i++;
			}
			ret++;
		}
		if (str[i] == ' ' || str[i] == '\t')
		{
			while (str[i + 1] && (str[i + 1] == ' ' || str[i + 1] == '\t'))
				i++;
		}
		if (str[i])
			i++;
		ret++;
	}
	if (str)
	{
		if (i > 0)
		{
			if (str[i - 1] == ' ' || str[i - 1] == '\t')
				ret--;
		}
	}
	return (ret);
}

char	*cleaned_spaces(char *str)
{
	int		i;
	int		j;
	int		k;
	char	*ret;

	i = 0;
	j = 0;
	k = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	ret = malloc(ft_spaces_len(str) + 1);
	while (str[i])
	{
		if (!str[i + 1]  && (str[i] == ' ' || str[i] == '\t'))
				break ;
		if (str[i] == '\'' || str[i] == '"')
		{
			k = quote_len(str, i) + 2;
			while (k--)
				ret[j++] = str[i++];
		}
		if (str[i] == ' ' || str[i] == '\t')
		{
			while (str[i + 1] && (str[i + 1] == ' ' || str[i + 1] == '\t'))
				i++;
			if ((str[i] == ' ' || str[i] == '\t') && !str[i + 1])
				i++;
		}
		if (str[i])
			ret[j++] = str[i++];
	}
	ret[j] = 0;
	free(str);
	return (ret);
}