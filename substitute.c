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
		if (!components[i++]) // FIXME: components segfaults
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

static int wait_for_proc(int *last_code)
{
	int	i;

	i = 0;
	while (g_running_processes[i])
		i++;
	if (!i && last_code)
		return (*last_code);
	waitpid(g_running_processes[i], &i, 0);
	g_running_processes[i] = 0;
	return (i);
}

static char *copy_skip(const char *const str, const char *const to_skip)
{
	char *ret;
	int	i;
	int	j;

	ret = malloc(ft_strlen(str) - 1);
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (i < ft_strlen(str))
	{
		if (str + j == to_skip)
		{
			j++;
			continue ;
		}
		ret[i] = str[j];
		j++;
		i++;
	}
	return (ret);
}

t_type *substitute(t_type *buf, int *last_code)
{
	char		*dollar_ptr;
	char		*name;
	char		*replacement;
	t_type	*ret;

	ret = malloc(sizeof(t_type));
	if (!ret)
		return (NULL);
	if (buf->redir <= 4)
	{
		ret->redir = buf->redir;
		return (ret);
	}
	ret->word.quote_state = buf->word.quote_state;
	dollar_ptr = ft_strchr(buf->word.word, '$');
	if (!dollar_ptr)
	{
		ret->word.word = buf->word.word;
		return (ret);
	}
	name = ft_substr(dollar_ptr, 1, buf->word.word + ft_strlen(buf->word.word) - dollar_ptr);
	if (!name)
		return (free(ret), NULL);
	buf->word.word = copy_skip(buf->word.word, dollar_ptr);
	if (!buf->word.word)
		return (NULL);
	if (ft_strncmp(name, "?", 1) == 0)
		dollar_ptr = substitute_into_string(buf->word.word, name, ft_itoa(wait_for_proc(last_code)));
	else
	{
		replacement = getenv(name);
		if (!replacement)
			replacement = ft_strdup("\n");
		dollar_ptr = substitute_into_string(buf->word.word, name, replacement);
		free(replacement);
	}
	free(name);
	ret->word.word = dollar_ptr;
	return (ret);
}
