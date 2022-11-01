/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harndt <humberto.arndt@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 21:56:49 by bbonaldi          #+#    #+#             */
/*   Updated: 2022/10/31 21:49:40 by harndt           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_prompt(t_ms *ms)
{
	while (TRUE)
	{
		ft_init_ms(ms);
		// set_signals();
		ms->buffer = readline(BASH_START);
		if (ms->buffer == NULL) 
			return (SUCCESS_CODE);
		ms->buffer_start = ms->buffer;
		add_history(ms->buffer);
		if (ft_strncmp(ms->buffer,"quit", strlen("quit")) == 0)
		{
			rl_clear_history();
			ft_free_ms(ms);
			exit(SUCCESS_CODE);
		}
		ft_tokenizer(ms);
		ft_print_tokens_list(ms->tokens);
		printf( "%s\n", ms->buffer_start);
		ft_free_ms(ms);
	}
	return (SUCCESS_CODE);
}