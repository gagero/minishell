#include "libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static unsigned int size2d(char **arr)
{
	unsigned int	i;

	i = 0;
	while(arr[i])
		i++;
	return (i);
}

// TODO: test
static char	*substitute_into_string(char *string, char *to_replace, char *replace_with)
{
	char	**components;
	char	*beg_ptr;
	char	*end_ptr;
	int		i;
	char	*ret;

	components = malloc(ft_strlen(string) * sizeof(char *));
	ret = malloc(ft_strlen(string) + (ft_strlen(to_replace) * (size2d(components) / 2)));
	if (!components || !ret)
		return (free(components), free(ret), NULL);
	*ret = 0;
	beg_ptr = string;
	end_ptr = ft_strnstr(string, to_replace, ft_strlen(string));
	i = 0;
	while (end_ptr)
	{
		components[i] = ft_substr(string, beg_ptr - string, end_ptr - string /* - 1*/);
		if (!components[i++])
			return (NULL);
		beg_ptr += ft_strlen(to_replace) + (end_ptr - string /* - 1*/);
		end_ptr = ft_strnstr(string, to_replace, ft_strlen(string));
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

// filter for envvars
char	*substitute(char *buf)
{
	char	*dollar_ptr;
	char	*word_end;
	char	*name;

	dollar_ptr = ft_strchr(buf, '$');
	word_end = ft_strchr(dollar_ptr, ' ');
	if (!word_end)
		word_end = dollar_ptr + ft_strlen(dollar_ptr);
	name = ft_substr(dollar_ptr, 0, word_end - dollar_ptr);
	if (!name)
		return (NULL);
	if (strcmp(name, "$?") != 0)
		dollar_ptr = substitute_into_string(buf, name, getenv(name));
	else
		dollar_ptr = substitute_into_string(buf, name, ft_itoa(g_last_proc_code));
	free(name);
	return (dollar_ptr);
}
