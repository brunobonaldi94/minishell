/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbonaldi <bbonaldi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 20:20:01 by bbonaldi          #+#    #+#             */
/*   Updated: 2022/11/14 15:28:36 by bbonaldi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	ft_execute_tree(t_ms *ms, t_executor *exec_tree);

void	ft_exec_cmds(t_ms *ms, t_executor *exec_tree)
{
	t_list		*pid_list;
	pid_t		*pid;
	char		**envp;

	if (!exec_tree)
		return ;
	ft_build_cmds(exec_tree->cmds, ms->env.path);
	ft_init_pipes(ms, exec_tree);
	pid = (pid_t *)malloc(sizeof(pid_t));
	*pid = fork();
	if (*pid == ERROR_CODE_FUNCTION)
		exit(1);//implementar error handler
	if (*pid == CHILD_PROCESS)
	{
		envp = ft_rebuild_envp(ms->env.var);
		ft_handle_pipes(ms, exec_tree);
		ft_set_redirection_fds(exec_tree);
		if (!exec_tree->cmds->cmd)
		{
			//implementar error handling
			ft_free_matrix((void ***)&(envp));
			exit(1);
			return ;
		}
		execve(exec_tree->cmds->cmd, exec_tree->cmds->argv, envp);
		ft_free_matrix((void ***)&(envp));
	}
	else
	{
		pid_list = ft_lstnew(pid);
		ft_lstadd_back(&ms->pids, pid_list);
	}
}

void	ft_execute_pipe(t_ms *ms, t_executor *exec_tree)
{
	ms->should_pipe = TRUE;
	ft_execute_tree(ms, exec_tree->left);
	ft_execute_tree(ms, exec_tree->right);
}

void	ft_execute_tree(t_ms *ms, t_executor *exec_tree)
{
	if (!exec_tree)
		return ;
	if (exec_tree->operator &&
		ft_strncmp(exec_tree->operator, PIPE, ft_strlen(PIPE)) == 0)
		ft_execute_pipe(ms, exec_tree);
	else
		ft_exec_cmds(ms, exec_tree);
}

void	ft_execute(t_ms *ms)
{
	t_list			*pids;

	ft_dup_stdin_out(ms);
	ft_execute_tree(ms, ms->executor);
	ft_restore_stdin_out(ms);
	ft_close_pipe_fds(ms->fd_pipe);
	pids = ms->pids;
	while (pids)
	{
		waitpid((*(pid_t *)pids->content), &ms->exit_code, 0);
		if (WIFEXITED(ms->exit_code))
			ms->exit_code = WEXITSTATUS(ms->exit_code);
		pids = pids->next;
	}
	return ;
}