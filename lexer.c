#include <stdint.h>
#include <stdlib.h>
#include "libft/libft.h"
#include "lexer.h"
#include "builtin.h"

char	*extract_word(const char **str)
{
	const char	*end;
	const char	*beg;

	end = *str;
	beg = *str;
	if (**str != '"' && **str != '\'')
		while (*end != '|' && *end != '>' && *end != '<')
			end++;
	else if (**str == '"')
		while (*end != '"')
			end++;
	else if (**str == '\'')
		while (*end != '\'')
			end++;
	*str = end - 1;
	return (ft_substr(beg, 0, end - beg /* - 1 */));
}

static char	*trim(t_list *node)
{
	char	*trimmed;
	char	*end;

	trimmed = NULL;
	if ((uintptr_t)((t_type *)node->content)->word.word > (uintptr_t)4)
	{
		trimmed = ft_strdup(((t_type *)node->content)->word.word);
		end = trimmed + (ft_strlen(trimmed) - 1);
		while (*trimmed == ' ')
			trimmed++;
		while (*end == ' ')
			end--;
	}
	if (trimmed)
		trimmed[end - trimmed] = 0;
	return (trimmed);
}

t_type	*tokenize(const char	*text)
{
	t_type	*ret;

	ret = malloc(sizeof(t_type));
	if (*text == '|')
		ret->redir = PIPE;
	else if (*text == '>' && *(text + 1) == '>')
		ret->redir = APPEND;
	else if (*text == '>' && *(text + 1) != '>')
		ret->redir = OUTPUT;
	else if (*text == '<' && *(text + 1) == '<')
		ret->redir = HEREDOC;
	else if (*text == '<' && *(text + 1) != '<')
		ret->redir = INPUT;
	else
		ret->word.word = extract_word(&text);
	return (ret);
}

static void	env_assign(char **text, char ***last_environ)
{
	char	*end;
	char	*eq;
	char	**env;
	bool	is_export;

	eq = ft_strchr(*text, '=');
	if (eq)
	{
		is_export = (ft_strncmp(*text, "export ", 7) == 0);
		if (is_export)
			*text += 6;
		end = *text;
		while (*end != ' ')
			end++;
		eq = ft_substr(*text, 0, end - *text);
		env = ft_split(eq, '=');
		putenviron(env[0], env[1], is_export, last_environ);
		*text = end + 1;
	}
}

// TODO: test
t_list	*lexer(char *text, char **last_environ)
{
	t_list 			*ret;
	unsigned int	len;
	t_list			*last;

	len = 0;
	env_assign(&text, &last_environ);
	while (text)
	{
		ft_lstadd_back(&ret, ft_lstnew(tokenize(text)));
		if (((t_type *)ft_lstlast(ret)->content)->redir == APPEND || ((t_type *)ft_lstlast(ret)->content)->redir == HEREDOC)
			text += 2;
		else
			text += 1;
		last = (t_list *)ft_lstindex(ret, len - 1);
		if (((uintptr_t)((t_type *)last->content)->word.word) > (uintptr_t)4 && ft_strrchr(((t_type *)last->content)->word.word, '"') == ((t_type *)last->content)->word.word)
			quote_prompt(&ret, len - 1, true);
		if (((uintptr_t)((t_type *)last->content)->word.word) > (uintptr_t)4 && ft_strrchr(((t_type *)last->content)->word.word, '\'') == ((t_type *)last->content)->word.word)
			quote_prompt(&ret, len - 1, false);
		if (((t_type *)last->content)->redir == PIPE && ((t_type *)last->prev->content)->redir == PIPE)
			pipe_prompt(&ret, len - 2);
		len++;
	}
	ft_lstmap(ret, (void *(*)(void *))trim, free);
	return (ret);
}
