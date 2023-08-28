/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linaina <linaina@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 22:40:34 by linaina           #+#    #+#             */
/*   Updated: 2023/08/29 00:38:56 by linaina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int final_cmd_len(char  **str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i][0] == '<' || str[i][0] == '>')
            return (i);
        i++;
    }
    return (i);
}

void    final_cmd()
{
    t_args  *tmp;
    char    **ret;
    int     len;
    int     i;

    tmp = glo.args;
    while (tmp)
    {
        i = 0;
        len = final_cmd_len(tmp->command);
        ret = malloc((len + 1) * sizeof(char *));
        while (len--)
        {
            ret[i] = ft_strdup(tmp->command[i]);
            i++;
        }
        ret[i] = 0;
        i = 0;
        while (tmp->command[i])
            free(tmp->command[i++]);
        free(tmp->command);
        tmp->command = ret;
        tmp = tmp->next;
    }
}

t_env   *look_for_path(void)
{
    t_env   *tmp;

    tmp = glo.env;
    while (tmp)
    {
        if (ft_strcmp(tmp->name, "PATH=") > 0)
            return (tmp);
        tmp = tmp->next;
    }
    return (0);
}

int	ft_size_env(void)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = glo.env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**lst_to_env(void)
{
	t_env	*tmp;
	char	**ret;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	tmp = glo.env;
	ret = malloc((ft_size_env() + 1) * sizeof(char *));
	while (tmp)
	{
		key = ft_strdup(tmp->name);
		value = (tmp->value);
		ret[i] = ft_strjoin(key, value);
		free(value);
		i++;
		tmp = tmp->next;
	}
	ret[i] = NULL;
	i = 0;
	return (ret);
}

int check_for_builtins(char **arg)
{

    if (ft_strcmp(arg[0], "echo") == 1)
        echo(arg);
    else if (ft_strcmp(arg[0], "cd") == 1)
        cd(arg);
    else if (ft_strcmp(arg[0], "pwd") == 1)
        pwd();
    else if (ft_strcmp(arg[0], "exit") == 1)
        ft_exit();
    else if (ft_strcmp(arg[0], "env") == 1)
        env();
    else if (ft_strcmp(arg[0], "unset") == 1)
        ft_unset(arg);
    else if (ft_strcmp(arg[0], "export") == 1)
        export(arg);
    else
        return(0);
    return (1);
}

int check_for_builtin(char **arg)
{
    if (ft_strcmp(arg[0], "cd") == 1)
        cd(arg);
    else if (ft_strcmp(arg[0], "exit") == 1)
        ft_exit();
    else if (ft_strcmp(arg[0], "unset") == 1)
        ft_unset(arg);
    else if (ft_strcmp(arg[0], "export") == 1)
        export(arg);
    else
        return(0);
    return (1);
}

void    execution()
{
    t_args  *args;
    t_env   *env;
    char    **splitenv;
    int     i;
    int     pid;
    int     save[2];
    int     fd[2];

    args = glo.args;
    save[1] = dup(1);
    save[0] = dup(0);
    while (args)
    {
        if (check_for_builtin(args->command) > 0)
            args = args->next;
        else
        {
            if (args->command[0])
            {
                pipe(fd);
                pid = fork();
                if (pid == 0)
                {
                    if (args->filein < 0 || args->fileout < 0)
                        exit(-1);
                    if(args->next)
                    {
                        dup2(fd[1], 1);
                        close(fd[1]);
                    }
                    close(fd[0]);
                    if (args->filein > 0)
                        dup2(args->filein, 0);
                    if (args->fileout > 1)
                        dup2(args->fileout, 1);
                    if (check_for_builtins(args->command) > 0)
                    {
                        exit(1);
                    }
                    env = look_for_path();
                    if (!env)
                    {
                        printf("Error Deleted PATH\n");
                        exit(-1);
                    }
                    if(ft_strchr(args->command[0], '/'))
                        {
                            if(access(args->command[0], X_OK) >= 0)
                            {
                                execve(args->command[0], args->command, lst_to_env());
                                perror("error\n");
                                exit(-1);
                            }
                            else
                            {
                                printf("ERROR PATH NOT FOUND\n");
                                close(args->fileout);
                                dup2(1, save[1]);
                                exit(-1);
                            }   
                        }
                    else
                    {
                    i = 0;
                    splitenv = ft_split(env->value, ':');
                    while (splitenv[i])
                    {
                        splitenv[i] = ft_strjoin(splitenv[i], "/");    
                        splitenv[i] = ft_strjoin(splitenv[i], args->command[0]);
                        if (access(splitenv[i], X_OK) >= 0)
                            break;
                        i++;   
                    }
                    if (!splitenv[i])
                    {
                        printf("ERROR PATH NOT FOUND\n");
                        close(args->fileout);
                        dup2(1, save[1]);
                        exit(-1);
                    }
                    
                    execve(splitenv[i], args->command, lst_to_env());
                    perror("Error: ");
                    exit(-1);
                    }
                }
            }
            args = args->next;
            dup2(fd[0], 0);
            close(fd[1]);
            close(fd[0]);
        }
    }
    int status;
   while(1)
   {
    int i = waitpid(-1, &status, 0);
    if(i == -1)
        break;
   }
    dup2(save[0], 0);
    close(save[0]);
}