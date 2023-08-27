/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linaina <linaina@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 23:03:35 by linaina           #+#    #+#             */
/*   Updated: 2023/08/28 00:25:10 by linaina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env *ret_new_env(int i)
{
    t_env   *new;
    t_env   *tmp;
    t_env   *tmp1;
    t_env   *hol1;
    char    *hol;
    int     j;

    j = 0;
    tmp1 = glo.env;
    while(tmp1)
    {
        if (j != i)
        {
            if (!new)
            {
                hol = 0;
                hol = ft_strjoin(hol, tmp1->name);
                hol = ft_strjoin(hol, tmp1->value);
                new = ft_lstnew(hol);
                tmp = new;
                new = new->next;
                free(hol);
            }
            else
            {
                hol = 0;
                hol = ft_strjoin(hol, tmp1->name);
                hol = ft_strjoin(hol, tmp1->value);
                new = ft_lstnew(hol);
                new = new->next;
                free(hol);
            }
        }
        hol1 = tmp1;
        tmp1 = tmp1->next;
        free(hol1->name);
        free(hol1->value);
        free(hol1);
    }
    return (tmp);
}