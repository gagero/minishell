#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// TODO: test
static char	*substitute_into_string(char *string, char *to_replace, char *replace_with)
{
	char	**components;
	char	*beg_ptr;
	char	*end_ptr;
	int		i;
	char	*ret;

	components = malloc((ft_strlen(string) + 1) * sizeof(char *));
	ret = malloc(ft_strlen(string) - ft_strlen(to_replace) + ft_strlen(replace_with) + 1); // TODO: double check math
	if (!components || !ret)
		return (free(components), free(ret), NULL);
	*ret = 0;
	beg_ptr = string;
	end_ptr = ft_strnstr(beg_ptr, to_replace, ft_strlen(string));
	i = 0;
	while (end_ptr)
	{
		components[i] = ft_substr(string, beg_ptr - string, end_ptr - string /* - 1*/);
		if (!components[i++])
			return (NULL);
		beg_ptr += ft_strlen(to_replace) + (end_ptr - string /* - 1*/);
		end_ptr = ft_strnstr(beg_ptr, to_replace, ft_strlen(string));
	}
	components[i] = NULL;
	i = 0;
	while (components[i])
	{
		ret = ft_strjoin(ret, components[i++]);
		ret = ft_strjoin(ret, replace_with);
	}
	return (free(string), free2d(components, true), ret);
}

int wait_for_proc(void)
{
	int	i;

	i = 0;
	while (!g_running_processes[i])
		i++;
	waitpid(g_running_processes[i], &i, 0);
	return (i);
}

t_type *substitute(const t_type *buf)
{
	char		*dollar_ptr;
	char		*name;
	char		*replacement;
	t_type	*ret;

	ret = malloc(sizeof(t_type));
	ret->word.is_quoted = buf->word.is_quoted;
	if (buf->redir <= 4)
	{
		ret->redir = buf->redir;
		return (ret);
	}
	dollar_ptr = ft_strchr(buf->word.word, '$');
	if (!dollar_ptr)
		{
			ret->word.word = buf->word.word;
			return (ret);
		}
	name = ft_substr(dollar_ptr, 1, buf->word.word + ft_strlen(buf->word.word) - dollar_ptr);
	if (!name)
		return (NULL);
	replacement = getenv(name);
	if (!replacement)
		replacement = ft_strdup("\n");
	if (strcmp(name, "$?") != 0)
		dollar_ptr = substitute_into_string(buf->word.word, name, replacement);
	else
		dollar_ptr = substitute_into_string(buf->word.word, name, ft_itoa(g_running_processes[wait_for_proc()]));
	free(name);
	free(replacement);
	ret->word.word = dollar_ptr;
	return (ret);
}
