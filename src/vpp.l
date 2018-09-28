%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <assert.h>
#include "vpp_compiler_directive_lookup.h"
#include "vpp.h"


#define log(type, x...) 				\
	do {						\
		if (e->input_file) {			\
			fprintf(stderr,			\
				"%s:%u: " type ": ",	\
				e->input_file->fname,	\
				e->input_file->lineno);	\
		}					\
		fprintf(stderr,	x);			\
	} while (0)

#define msg(x...)	do { fprintf(stderr, x); } while (0)
#define warn(e, x...)	do { log("warning", x); } while (0)
#define err(e, x...)	do { log("error", x); } while (0)
#define fatal(x...)	do { fprintf(stderr, x); abort(); } while (0)

#define emit_write(e, x, len)					\
	do {							\
		if (ifdef_state_allows_processing(e))		\
			wb_write(&e->write_buffer, x, len);	\
	} while (0)

#define emit(e, x)	do { emit_write(e, x, strlen(x)); } while (0)

#if !defined(_GNU_SOURCE)
	static char * __fake_strndup (const char *s, size_t n)
	{
		char *d;
		int i;
		for (i=0; i<n; i++) {
			if (s[i] == '\0') {
				n = i;
				break;
			}
		}
		d = malloc(n + 1);
		memcpy(d, s, n);
		d[n] = '\0';
		return d;
	}
	#undef strndup
	#define strndup(s,n) __fake_strndup(s,n)
#endif

struct write_buffer {
	size_t size;
	size_t len;
	char *buf;
};

static int wb_write (struct write_buffer *wb, char *p, size_t len)
{
	assert(p != NULL);
	if (len > wb->size - wb->len) {
		size_t new_size = wb->size + len + 8192;
		if ((wb->buf = realloc(wb->buf, new_size)) == NULL)
			fatal("failed allocating %u bytes for parse buffer!\n", (unsigned) new_size);
		wb->size = new_size;
	}
	assert(wb->buf != NULL || len == 0);
	assert(wb->len + len <= wb->size);
	memcpy(wb->buf + wb->len, p, len);
	wb->len += len;
	return 1;
}

struct input_file {
	FILE *f;
	char *fname;
	unsigned lineno;
	struct input_file *next;
};

struct definition {
	char *name;
	int n_args;
	char **args;
	char *text;
};

struct scanner_extra {
	struct write_buffer write_buffer;
	int n_include_path;
	const char **include_path;
	struct input_file *input_file;
	void *definition_tree;
	unsigned char *ifdef_state_stack;
	unsigned ifdef_state_stack_size;
};


#define YY_EXTRA_TYPE  struct scanner_extra*

enum ifdef_state {
	ifdef_false = 0,
	ifdef_true,
	ifdef_false_else,
	ifdef_true_else
};

static void ifdef_state_push (struct scanner_extra *e, enum ifdef_state state)
{
	unsigned char *tmp = realloc(e->ifdef_state_stack, ++e->ifdef_state_stack_size);
	if (!tmp)
		fatal("failed to ralloc() ifdef_state_stack (to %u bytes)!\n", e->ifdef_state_stack_size);
	e->ifdef_state_stack = tmp;
	e->ifdef_state_stack[e->ifdef_state_stack_size-1] = state;
}

static int ifdef_state_pop (struct scanner_extra *e)
{
	if (e->ifdef_state_stack_size > 0) {
		e->ifdef_state_stack = realloc(e->ifdef_state_stack, --e->ifdef_state_stack_size);
		return 0;
	}
	return -1;
}

static int ifdef_state_allows_processing (struct scanner_extra *e)
{
	int i;
	for (i=0; i<e->ifdef_state_stack_size; i++) {
		enum ifdef_state s = e->ifdef_state_stack[i];
		if (s == ifdef_false || s == ifdef_true_else)
			return 0;
	}
	return 1;
}

static void emit_newline (struct scanner_extra *e)
{
	emit_write(e, "\n", 1);
	e->input_file->lineno++;
}

static void emit_line_directive (struct scanner_extra *e, int level)
{
	char s [64];
	snprintf(s, sizeof(s), "`line %u \"", e->input_file->lineno);
	emit(e, s);
	emit(e, e->input_file->fname);
	snprintf(s, sizeof(s), "\" %d\n", level);
	emit(e, s);
}

static int is_compiler_directive (const char *s)
{
	return compiler_directive_lookup(s, strlen(s)) ? 1 : 0;
}

static int add_include_path (struct scanner_extra *e, const char *path)
{
	void *tmp = realloc(e->include_path, (e->n_include_path+1) * sizeof(e->include_path[0]));
	if (!tmp)
		fatal("memory allocation for include path list failed!\n");
	e->include_path = tmp;
	e->include_path[e->n_include_path] = path;
	e->n_include_path++;
	return 0;
}

static FILE* push_input_file (struct scanner_extra *e, const char *new_fname)
{
	struct input_file *i = calloc(1, sizeof(*i));
	if (!i)
		fatal("memory allocation for input file state failed!\n");
	if (new_fname) {
		int j;
		for (j=-1; j<e->n_include_path; j++) {
			if (j == -1) {
				i->fname = strdup(new_fname);
			} else {
				int len1 = strlen(e->include_path[j]);
				int len2 = strlen(new_fname);
				i->fname = malloc(len1 + len2 + 2);
				memcpy(i->fname, e->include_path[j], len1);
				i->fname[len1] = '/';
				memcpy(&i->fname[len1 + 1], new_fname, len2);
				i->fname[len1 + 1 + len2] = '\0';
			}
			if (!i->fname)
				fatal("memory allocation for path name failed!\n");
			i->f = fopen(i->fname, "r");
			if (i->f == NULL) {
				free(i->fname);
			} else {
				i->lineno = 1;
				i->next = e->input_file;
				e->input_file = i;
				emit_line_directive(e, 1);
				return i->f;
			}
		}
		err(e, "Failed opening file '%s'\n", new_fname);
		free(i);
	}
	return NULL;
}

static void pop_input_file (struct scanner_extra *e)
{
	struct input_file *i = e->input_file;
	if (i) {
		e->input_file = i->next;
		if (i->f)
			fclose(i->f);
		if (i->fname)
			free(i->fname);
		free(i);
		if (e->input_file)
			emit_line_directive(e, 2);
	}
}

/* replace the char from pos with given str `seed'
static char* replace_string(char* src, int pos, char* seed)
{
    char *destination=src;
    int idx=0;
    assert(pos < strlen(src));
    while(idx < strlen(seed))
    {
      destination[pos+idx] = seed[idx];
      idx++;
    }
    return destination;
}*/

static void strip_backslash_line_continuation (char *s)
{
	char *src = s;
	char *dst = s;
	while (src[0] != '\0') {
		if (src[0] == '\\' && (src[1] == '\n' || (src[1] == '\r' && src[2] == '\n')))
			src++;
		dst[0] = src[0];
		src++;
		dst++;
	}
	dst[0] = '\0';
}

static char *ws_stripped_string (char *s)
{
	char *comment_start;
	unsigned len;
	while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r')
		s++;
	if ((comment_start = strstr(s, "//")) != NULL)
		len = comment_start - s - 1;
	else
		len = strlen(s);
	while (s[len-1] == ' ' || s[len-1] == '\t' || s[len-1] == '\n' || s[len-1] == '\r')
		len--;
	s[len] = '\0';
	return s;
}

static int identifier_length (const char *i)
{
	size_t n = 0;
	if (i[0] == '\\') {
		n = strcspn(i, " \t\n");
	} else {
		if (i[0] == '_' || (i[0] >= 'a' && i[0] <= 'z') || (i[0] >= 'A' && i[0] <= 'Z'))
			n = 1;
		while (i[n] == '_' || (i[n] >= 'a' && i[n] <= 'z') || (i[n] >= 'A' && i[n] <= 'Z') || (i[n] >= '0' && i[n] <= '9') || i[n] == '$')
			n++;
	}
	return n;
}

static size_t count_char (const char *s, char c, size_t limit)
{
	size_t n = 0;
	size_t i;
	for (i=0; i<limit; i++) {
		if (s[i] == c)
			n++;
	}
	return n;
}

static char * scan_identifier (char *s, char **end)
{
	if (!s)
		return NULL;
	if (s[0] == '\\') {
		if (end)
			*end = s + strcspn(s, " \t\n");
		return s;
	}
	for (; *s != '\0'; s++) {
		if (*s == '_' || (*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z'))
			break;
	}
	if (*s == '\0')
		return NULL;
	if (end) {
		char *e = s;
		for (; *e != '\0'; e++) {
			if (*e == '_')
				continue;
			if ((*e >= 'a' && *e <= 'z') || (*e >= 'A' && *e <= 'Z'))
				continue;
			if (*e >= '0' && *e <= '9')
				continue;
			if (*e == '$')
				continue;
			break;
		}
		*end = e;
	}
	return s;
}

static char * scan_argument (char *s, char **end)
{
	size_t parenl = 0;
	size_t parenr = 0;
	char *e = s;
	while (*e != '\0') {
		if (*e == ',' || *e == ')') {
			if (parenl == parenr) {
				*end = e;
				return s;
			}
		}
		if (*e == '(')
			parenl++;
		if (*e == ')')
			parenr++;
		e++;
	}
	return NULL;
}

static int defstring_compare (const char *i1, const char *i2)
{
	size_t n1 = identifier_length(i1);
	size_t n2 = identifier_length(i2);
	size_t n = (n1 > n2) ? n1 : n2;
	return strncmp(i1, i2, n);
}

static int def_compare (const void *d1, const void *d2)
{
	const char *i1 = ((const struct definition *) d1)->name;
	const char *i2 = ((const struct definition *) d2)->name;
	return defstring_compare(i1, i2);
}

static int def_compare_match_any (const void *d1, const void *d2)
{
	return 0;
}

static void def_print (const void *ptr, VISIT order, int level)
{
	const struct definition *d = *(const struct definition **) ptr;
	if (order == postorder || order == leaf) {
		int i;
		msg("def: '%s', args ", d->name);
		for (i=0; i<d->n_args; i++)
			msg("'%s', ", d->args[i]);
		msg("text '%s'\n", d->text);
	}
}

static struct definition* def_find (struct scanner_extra *e, char *defname)
{
	struct definition key = { .name = defname };
	void *ptr = tfind(&key, &e->definition_tree, def_compare);
	return ptr ? *(struct definition**) ptr : NULL;
}

static void def_free (struct scanner_extra *e, struct definition *d)
{
	char *dname = d->name;
	int dn_args = d->n_args;
	char **dargs = d->args;
	char *dtext = d->text;
	tdelete(d, &e->definition_tree, def_compare);
	if (dname)
		free(dname);
	if (dargs) {
		int i;
		for (i=0; i<dn_args; i++)
			free(dargs[i]);
		free(dargs);
	}
	if (dtext)
		free(dtext);
	free(d);
}

static void def_remove (struct scanner_extra *e, char *defname)
{
	struct definition *d = def_find(e, defname);
	if (d) {
		def_free(e, d);
	} else {
		warn(e, "trying to `undef '%s', which is not defined at this point!\n", defname);
		warn(e, "----- list of active definitions: -------\n");
		twalk(e->definition_tree, def_print);
		warn(e, "-----------------------------------------\n");
	}
}

static void def_free_all (struct scanner_extra *e)
{
	for (;;) {
		void *ptr = tfind(NULL, &e->definition_tree, def_compare_match_any);
		if (ptr == NULL)
			break;
		else
			def_free(e, *(struct definition**) ptr);
	}
}

static void def_add (struct scanner_extra *e, char *defstring)
{
	char *defname = scan_identifier(defstring, &defstring);
	size_t defname_len = defstring - defname;
	struct definition *d = calloc(1, sizeof(*d));
	if (defstring[0] == '(') {
		while (defstring[0] != ')') {
			char *defarg = scan_identifier(defstring, &defstring);
			if (defarg != NULL) {
				d->args = realloc(d->args, (d->n_args + 1) * sizeof(d->args[0]));
				d->args[d->n_args++] = strndup(defarg, defstring-defarg);
			}
		}
		defstring++;
	}
	if (defstring[0] != '\0') {
		while (defstring[0] == ' ' || defstring[0] == '\t')
			defstring++;
	}
	d->name = strndup(defname, defname_len);
	d->text = strdup(defstring);
	if (is_compiler_directive(defname))
		warn(e, "re-`define compiler directive `%s\n", defname);
	if (tfind(d, &e->definition_tree, def_compare)) {
		warn(e, "re-`define previously defined `%s\n", defname);
		def_remove(e, defname);
	}
	tsearch(d, &e->definition_tree, def_compare);
}

static int def_eval (struct scanner_extra *e, char *defstring, char **endp)
{
	struct definition *d;
	if (defstring[0] != '`' || !(d = def_find(e, defstring + 1))) {
		emit(e, defstring);
		if (endp)
			*endp = defstring + strlen(defstring) + 1;
		return 0;
	} else {
		struct write_buffer wb = { 0 };
		char *text, *i_start, *i_end;
		char *arg [d->n_args];
		char *s;
		int i;
		scan_identifier(defstring + 1, &s);
		for (i=0; i<d->n_args; i++) {
			if (i == 0) {
				s = strchr(s, '(');
				*s = '\0';
			}
			arg[i] = scan_argument(s + 1, &s);
			*s = '\0';
			if (arg[i] == NULL) {
				err(e, "macro `%s should have exactly %d args!\n", d->name, d->n_args);
				return -1;
			}
		}
		if (d->n_args > 0)
			s++;
		if (endp)
			*endp = s;
		for (text=d->text; *text!='\0'; ) {
			i_start = scan_identifier(text, &i_end);
			if (i_start) {
				for (i=0; i<d->n_args; i++) {
					if (strncmp(i_start, d->args[i], i_end - i_start) == 0) {
						char *a = ws_stripped_string(arg[i]);
						wb_write(&wb, text, i_start - text);
						wb_write(&wb, a, strlen(a));
						break;
					}
				}
				if (i == d->n_args)
					wb_write(&wb, text, i_end - text);
				text = i_end;
			} else {
				wb_write(&wb, text, strlen(text));
				break;
			}
		}
		wb_write(&wb, "\0", 1);
		s = wb.buf;
		while (s && *s != '\0') {
			char *s1 = strchr(s, '`');
			if (!s1 || *s1 == '\0') {
				emit(e, s);
				break;
			}
			emit_write(e, s, s1 - s);
			s = s1;
			if (s && s != '\0') {
				char *end;
				if (def_eval(e, s, &end) < 0) {
					free(wb.buf);
					return -1;
				}	
				s = end;
			}
		}
		free(wb.buf);
	}
	return 0;
}

%}

%option reentrant
%option noyywrap
%option nounput
%option 8bit

%x c_style_comment

SIMPLE_IDENTIFIER	[_a-zA-Z][_a-zA-Z0-9\$]*
ESCAPED_IDENTIFIER	\\[^ \t\n]*
IDENTIFIER		({SIMPLE_IDENTIFIER}|{ESCAPED_IDENTIFIER})

%%
<c_style_comment>{
	"*/"		BEGIN(INITIAL);
	[^*\n]+
	"*"
	\n		emit_newline(yyget_extra(yyscanner));
}

<INITIAL>{
	"/*"		BEGIN(c_style_comment);

	"//"[^\n]*\n	emit_newline(yyget_extra(yyscanner));

	"`include"[ \t]*\"[^\"]+\" {
		struct scanner_extra *e = yyget_extra(yyscanner);
		char *fname = strchr(yytext, '\"') + 1;
		fname[strlen(fname)-1] = '\0';
		yyin = push_input_file(e, fname);
		if (yyin) {
			yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE, yyscanner), yyscanner);
		} else {
			yyterminate();
		}
	}

	"`line"[ \t]+[^\n]+\n {
		struct scanner_extra *e = yyget_extra(yyscanner);
		struct input_file *i = e->input_file;
		char *s = yytext + 6;
		i->lineno = strtol(s, &s, 10);
		while (s[0] == ' ' || s[0] == '\t')
			s++;
		if (s[0] == '\"') {
			char *fname_start = ++s;
			while (s[0] != '\0' && s[0] != '\"' && s[0] != '\n')
				s++;
			if (s[0] != '"') {
				err(e, "no terminating \"-char at end of file name in %s:%d\n", i->fname, i->lineno);
				yyterminate();
			}
			if (i->fname)
				free(i->fname);
			i->fname = strndup(fname_start, s - fname_start);
		}
		emit(e, yytext);
	}

	"`define"([^\n](\\\r?\n)?)*+\n {
		struct scanner_extra *e = yyget_extra(yyscanner);
		if (ifdef_state_allows_processing(e)) {
			char *s;
			s = ws_stripped_string(yytext + strlen("`define"));
			strip_backslash_line_continuation(s);
			def_add(e, s);
		}
		emit_newline(e);
	}

        /*No need, using -l option with make_dparser can support Longest Match
          to avoid mis-parsing 'a&&b' to 'a & (&b)'
	"&&"([^&](\\\r?\n)?)*+\n {
		struct scanner_extra *e = yyget_extra(yyscanner);
		char *s;
		s = replace_string(yytext, 0, "##");
		strip_backslash_line_continuation(s);
		emit(e, yytext);
	}
        */

	"`undef"[ \t]+[^ \t\n]+ {
		struct scanner_extra *e = yyget_extra(yyscanner);
		if (ifdef_state_allows_processing(e))
			def_remove(e, ws_stripped_string(yytext + strlen("`undef")));
	}

	"`ifdef"[ \t]+[^ \t\n]+ {
		struct scanner_extra *e = yyget_extra(yyscanner);
		struct definition *d = def_find(e, ws_stripped_string(yytext + strlen("`ifdef")));
		ifdef_state_push(e, (d != NULL) ? ifdef_true : ifdef_false);
	}

	"`ifndef"[ \t]+[^ \t\n]+ {
		struct scanner_extra *e = yyget_extra(yyscanner);
		struct definition *d = def_find(e, ws_stripped_string(yytext + strlen("`ifndef")));
		ifdef_state_push(e, (d == NULL) ? ifdef_true : ifdef_false);
	}

	"`else" {
		struct scanner_extra *e = yyget_extra(yyscanner);
		if (e->ifdef_state_stack_size > 0) {
			enum ifdef_state s = e->ifdef_state_stack[e->ifdef_state_stack_size-1];
			if (s == ifdef_true)
				e->ifdef_state_stack[e->ifdef_state_stack_size-1] = ifdef_true_else;
			else if (s == ifdef_false)
				e->ifdef_state_stack[e->ifdef_state_stack_size-1] = ifdef_false_else;
		} else {
			err(e, "`else directive without `ifdef or `ifndef!\n");
			yyterminate();
		}
	}

	"`elsif"[ \t]+[^ \t\n]+ {
		struct scanner_extra *e = yyget_extra(yyscanner);
		struct definition *d = def_find(e, ws_stripped_string(yytext + strlen("`elsif")));
		if (e->ifdef_state_stack_size > 0) {
			enum ifdef_state s = e->ifdef_state_stack[e->ifdef_state_stack_size-1];
			if (s == ifdef_true) {
				e->ifdef_state_stack[e->ifdef_state_stack_size-1] = ifdef_true_else;
			} else {
				s = (d != NULL) ? ifdef_true : ifdef_false;
				e->ifdef_state_stack[e->ifdef_state_stack_size-1] = s;
			}
		} else {
			err(e, "`elsif directive without `ifdef or `ifndef!\n");
			yyterminate();
		}
	}

	"`endif" {
		struct scanner_extra *e = yyget_extra(yyscanner);
		if (ifdef_state_pop(e) != 0) {
			err(e, "unbalanced `endif!\n");
			yyterminate();
		}
	}

	"`"{IDENTIFIER}([ \t\n]*\([^\)]*\))? {
		struct scanner_extra *e = yyget_extra(yyscanner);
		struct write_buffer macro_wb = { 0 };
		wb_write(&macro_wb, yytext, yyleng);
		size_t parenl = count_char(macro_wb.buf, '(', macro_wb.len);
		size_t parenr = count_char(macro_wb.buf, ')', macro_wb.len);
		while (parenl != parenr) {
			int inp = input(yyscanner);
			char c = inp;
			if (inp == EOF)
				break;
			if (c == '(')
				parenl++;
			if (c == ')')
				parenr++;
			wb_write(&macro_wb, &c, 1);
		}
		if (parenl == parenr) {
			wb_write(&macro_wb, "\0", 1);
			if (def_eval(e, macro_wb.buf, NULL) < 0) {
				free(macro_wb.buf);
				yyterminate();
			}
		} else {
			err(e, "unbalanced parentheses and unexpected end of file after macro!\n");
			free(macro_wb.buf);
			yyterminate();
		}
		free(macro_wb.buf);
	}
}

<<EOF>> {
	struct scanner_extra *e = yyget_extra(yyscanner);
	yypop_buffer_state(yyscanner);
	pop_input_file(e);
	if (!YY_CURRENT_BUFFER)
		yyterminate();
}

\n	{ emit_newline(yyget_extra(yyscanner)); }
.	{ emit(yyget_extra(yyscanner), yytext); }
%%


int vpp_preprocess_to_parsebuffer (char **parsebuffer, const char *input_file, const char **include_path_list)
{
	struct scanner_extra e = { { 0 } };
	yyscan_t scanner;
	FILE *in_file;
	if (input_file == NULL)
		fatal("vpp_preprocess_to_parsebuffer() called with NULL input_file\n");
	if (include_path_list) {
		while (*include_path_list != NULL) {
			add_include_path(&e, *include_path_list);
			include_path_list++;
		}
	}
	yylex_init(&scanner);
	yyset_extra(&e, scanner);
	if ((in_file = push_input_file(&e, input_file)) != NULL) {
		yyset_in(in_file, scanner);
		yylex(scanner);
	}
	yylex_destroy(scanner);
	def_free_all(&e);
	if (e.include_path)
		free(e.include_path);
	wb_write(&e.write_buffer, "\0", 1);
	*parsebuffer = e.write_buffer.buf;
	return e.write_buffer.len - 1;
}

