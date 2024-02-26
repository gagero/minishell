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
	if (**str && **str != '"' && **str != '\'')
		while (*end && *end != '|' && *end != '>' && *end != '<' && *end != '\n')
			end++;
	else if (**str == '"')
		while (*end && *end != '"' && *end != '\n')
			end++;
	else if (**str == '\'')
		while (*end && *end != '\'' && *end != '\n')
			end++;
	return (ft_substr(beg, 0, end - beg));
}

// string made invalid here
void	*trim(void *void_node)
{
	char	*trimmed;
	char	*end;
	t_type	*node;

	node = void_node;
	if ((intptr_t)(node->word.word) > (intptr_t)4)
	{
		trimmed = ft_strdup(node->word.word);
		end = trimmed + ft_strlen(trimmed) - 1;
		while (ft_isspace(*trimmed))
			trimmed++;
		while (ft_isspace(*end) && end > trimmed)
			end--;
		trimmed[end - trimmed + 1] = 0;
		node->word.word = trimmed;
	}
	return (node);
}

t_type	*tokenize(const char *text)
{
	t_type	*ret;

	ret = malloc(sizeof(t_type));
	if (!ret)
		return (NULL);
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
	{
		ret->word.is_quoted = false;
		ret->word.word = extract_word(&text);
	}
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

static int next_special(char *text)
{
	long pipe;
	long left;
	long right;
	int min;

	pipe = ft_strchr(text, '|') - text;
	left = ft_strchr(text, '<') - text;
	right = ft_strchr(text, '>') - text;
	if (pipe < left && pipe < right)
		min = pipe;
	else if (left < pipe && left < right)
		min = left;
	else if (right < pipe && right < left)
		min = right;
	else
		min = 0;
	if (min <= 0)
		return (ft_strchr(text, '\0') - text);
	return (min);
}

// TODO: test
t_list	*lexer(char *text, char **last_environ)
{
	t_list 			*ret;
	unsigned int	len;
	t_list			*last;

	len = 0;
	env_assign(&text, &last_environ);
	ret = NULL;
	while (text && *text)
	{
		ft_lstadd_back(&ret, ft_lstnew(tokenize(text)));
		if (((t_type *)ft_lstlast(ret)->content)->redir == APPEND || ((t_type *)ft_lstlast(ret)->content)->redir == HEREDOC)
			text += 2;
		else if (((t_type *)ft_lstlast(ret)->content)->redir == OUTPUT || ((t_type *)ft_lstlast(ret)->content)->redir == INPUT || ((t_type *)ft_lstlast(ret)->content)->redir == PIPE)
			text += 1;
		else
			text += next_special(text) /* + 1 */;
		last = (t_list *)ft_lstindex(ret, len - 1);
		if (((uintptr_t)((t_type *)last->content)) > (uintptr_t)4 && ft_strrchr(((t_type *)last->content)->word.word, '"') == ((t_type *)last->content)->word.word)
			quote_prompt(&ret, len - 1, true);
		if (((uintptr_t)((t_type *)last->content)) > (uintptr_t)4 && ft_strrchr(((t_type *)last->content)->word.word, '\'') == ((t_type *)last->content)->word.word)
			quote_prompt(&ret, len - 1, false);
		if (((t_type *)last->content)->redir == PIPE && ((t_type *)last->prev->content)->redir == PIPE)
			pipe_prompt(&ret, len - 2);
		len++;
	}
	if (ret)
		ret = ft_lstmap(ret, trim, free);
	return (ret);
}
