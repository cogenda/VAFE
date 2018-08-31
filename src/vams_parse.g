/**
 *  Verilog-AMS Grammar Specification for dparser http://dparser.sf.net/
 *
 *   cf. Verilog-AMS Language Reference Manual. Analog & Mixed-Signal Extensions to Verilog HDL
 *       Version 2.4, May 30, 2014
 *	 http://www.accellera.org/downloads/standards/v-ams/VAMS-LRM-2-4.pdf
 *
 *       Version 2.3, August 4, 2008
 *	 http://www.accellera.org/downloads/standards/v-ams/VAMS-LRM-2-4.pdf
 *       http://www.verilog.org/verilog-ams/htmlpages/public-docs/lrm/2.3/VAMS-LRM-2-3.pdf
 *       http://www.verilog.org/verilog-ams/htmlpages/public-docs/
 *       http://www.eda.org/verilog-ams/htmlpages/lit.html
 *
 *  Better use a wide screen when viewing or editing this file. No 80-char limit.
 */

{
	#include <ctype.h>
	#include <errno.h>
	#include <fcntl.h>
	#include <math.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/uio.h>
	#include <unistd.h>

	#define XVPI_ERROR_STATE vpiCompile
	#include "xvpi.h"
	#include "vams_keyword_lookup.h"
	#include "vams_parse.h"

	struct vams_source_parser_globals {
		PLI_INT32 cell_instance;
		PLI_INT32 time_unit;
		PLI_INT32 time_precision;
		double default_decay_time;
		PLI_INT32 default_nettype;
		PLI_INT32 unconnected_drive;
	};

	/* need to be defined before #include <dparse.h> */
	#define D_ParseNode_User struct xvpi_object *
	#define D_ParseNode_Globals struct vams_source_parser_globals
	#include <dparse.h>
	#include <dparse_tree.h>

	/* let's steal some bits from dparser/d/util.c */
	extern void d_fail(char *str, ...);
	extern char * dup_str (char *s, char *e);
	extern int d_debug_level;
	extern int d_verbose_level;

	D_ParserTables parser_tables_gram;

	static char *last_pathname_to_free;
	static int Wobsolete = 0;

	#define obsolete(loc, since_version, feature, LRM_reference)					\
	do {												\
		if (Wobsolete)										\
			warn("%s:%d: Language feature '%s' is obsolete since version %s (cf. %s).\n",	\
				loc.pathname, loc.line, feature, since_version, LRM_reference);		\
	} while (0)


	#define MAX_LINE_LENGTH 12  /* must be at least 4 */
	#define INDENT_SPACES 2

	static char * change_newline2space (char *s)
	{
		char *ss = s;
		while (*ss++) {
			if (*ss == '\n')
				*ss = ' ';
		}
		if (strlen(s) > MAX_LINE_LENGTH) {
			s[MAX_LINE_LENGTH-3] = '.';
			s[MAX_LINE_LENGTH-2] = '.';
			s[MAX_LINE_LENGTH-1] = '.';
			s[MAX_LINE_LENGTH] = '\0';
		}
		return s;
	}

	static void print_parsetree_node (int depth, char *name, char *value, void *client_data)
	{
		FILE *f = client_data;
		fprintf(f, "%*s", depth * INDENT_SPACES, "");
		fprintf(f, "%s  %s.\n", name, change_newline2space(value));
	}

	/* The Verilog/AMS grammar is highly ambiguous. If there are multiple choices available, simply pick the first one. */
	static D_ParseNode * ambiguity_fn (D_Parser *pp, int n, D_ParseNode **v)
	{
		return v[0];
	}

/*	static void syntax_error_report_fn (struct D_Parser *ap)
	{
		Parser *p = (Parser *) ap;
		char *fn = d_dup_pathname_str(p->user.loc.pathname);
		char *after = 0;
		ZNode *z = p->snode_hash.last_all ? p->snode_hash.last_all->zns.v[0] : 0;
		while (z && z->pn->parse_node.start_loc.s == z->pn->parse_node.end)
			z = (z->sns.v && z->sns.v[0]->zns.v) ? z->sns.v[0]->zns.v[0] : 0;
		if (z && z->pn->parse_node.start_loc.s != z->pn->parse_node.end) 
			after = dup_str(z->pn->parse_node.start_loc.s, z->pn->parse_node.end);
		if (after)
			fprintf(stderr, "%s:%d: syntax error after '%s'\n", fn, p->user.loc.line, after);
		else
			fprintf(stderr, "%s:%d: syntax error\n", fn, p->user.loc.line);
		if (after)
			free(after);
		free(fn);
	}
*/
	static void reset_globals (struct vams_source_parser_globals *g)
	{
		g->cell_instance = 0;
		g->time_unit = 0;
		g->time_precision = 0;
		g->default_decay_time = 0.0;
		g->default_nettype = vpiNone;
		g->unconnected_drive = vpiHighZ;
	}

	int vams_parse (char *parsebuf, size_t parsebuf_len, int debug, int tree_print)
	{
		struct vams_source_parser_globals globals;
		D_ParseNode *pn;
		int ret = 1;

		xvpi_object_get_child_or_create(NULL, vpiCallback, vpiIterator);
		xvpi_object_get_child_or_create(NULL, vpiDiscipline, vpiIterator);
		xvpi_object_get_child_or_create(NULL, vpiModule, vpiIterator);
		xvpi_object_get_child_or_create(NULL, vpiNature, vpiIterator);
		xvpi_object_get_child_or_create(NULL, vpiTimeQueue, vpiIterator);
		xvpi_object_get_child_or_create(NULL, vpiUdpDefn, vpiIterator);
		xvpi_object_get_child_or_create(NULL, vpiUserSystf, vpiIterator);

		if (debug) {
			d_debug_level = 10;
			d_verbose_level = 10;
		}

		D_Parser *p = new_D_Parser(&parser_tables_gram, sizeof(D_ParseNode_User));

		p->initial_globals = &globals;
		p->loc.pathname = last_pathname_to_free = NULL;
		p->ambiguity_fn = ambiguity_fn;
		p->save_parse_tree = 1;

		reset_globals(p->initial_globals);

		pn = dparse(p, parsebuf, parsebuf_len);

		if (p->syntax_errors) {
			ret = 0;
		} else if (tree_print && pn != NULL) {
			FILE *tree_file = fopen("parsetree", "w");
			if (!tree_file) {
				err("failed opening '%s': %s\n", "parsetree", strerror(errno));
				ret = 0;
			} else {
				print_parsetree(parser_tables_gram, pn, print_parsetree_node, (void *) tree_file);
				fclose(tree_file);
			}
			msg("parsetree written to file 'parsetree' in current directory.\n");
		}
                //loh add my elabrate test func
                //xvpi_recurs_elabrate(pn, "parameter_declaration");
		if (pn != NULL)
			free_D_ParseNode(p, pn);

		free_D_Parser(p);
		free(last_pathname_to_free);
		return ret;
	}

	static int current_language = VAMS_2_4;

	static int begin_keywords (const char *s, const char *e)
	{
		static const struct { char *name; int id; } languages [] = {
			{ "1364-1995", IEEE_1364_1995 },
			{ "1364-2001", IEEE_1364_2001 },
			{ "1364-2005", IEEE_1364_2005 },
			{ "VA-1.0", VA_1_0 },
			{ "VAMS-1.0", VA_1_0 },
			{ "VAMS-1.3", VAMS_1_3 },
			{ "VAMS-2.0", VAMS_2_0 },
			{ "VAMS-2.1", VAMS_2_1 },
			{ "VAMS-2.2", VAMS_2_2 },
			{ "VAMS-2.3", VAMS_2_3 },
			{ "VAMS-2.4", VAMS_2_4 },
		};
		int i, len = e-s-2;
		s++;
		if (current_language != VAMS_2_4)
			d_fail("nested `begin_keywords/`end_keywords blocks are not allowed.\n");
		for (i=0; i<sizeof(languages)/sizeof(languages[0]); i++) {
			if (strncmp(languages[i].name, s, len) == 0) {
				current_language = languages[i].id;
				return 0;
			}
		}
		return -1;
	}

	static void end_keywords (void)
	{
		current_language = VAMS_2_4;
	}

	static int is_keyword (const char *s, const char *e, int language)
	{
		const struct vams_keyword *k = vams_keyword_lookup(s, e-s);
		if (k && (k->languages & language))
			return 1;
		return 0;
	}

	/** scan integer line number and filename string after `line compiler directive **/
	static int scan_lineno_filename (d_loc_t *loc, unsigned char *op_assoc, int *op_priority)
	{
		long next_line = strtol(loc->s, &loc->s, 10);
		while (loc->s[0] == ' ' || loc->s[0] == '\t')
			loc->s++;
		if (loc->s[0] == '"') {
			char *fname_start = ++(loc->s);
			while (loc->s[0] != '\0' && loc->s[0] != '"' && loc->s[0] != '\n')
				loc->s++;
			if (loc->s[0] != '"')
				d_fail("no terminating \"-char at end of file name in %s:%d\n", loc->pathname, loc->line);
			if (loc->pathname)
				free(loc->pathname);
			loc->pathname = last_pathname_to_free = dup_str(fname_start, loc->s);
			loc->s++;
		}
		loc->line = next_line;
		loc->previous_col = loc->col = strtol(loc->s, &loc->s, 10);
		while (loc->s[0] != '\0' && loc->s[0] != '\n')
			loc->s++;
		if (loc->s[0] == '\n')
			loc->s++;
		return 1;
	}

	static void xvpi_obj_add_scope_props (struct xvpi_object *obj)
	{
		xvpi_object_add_child(obj, xvpi_object_new(vpiBranch, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiInternalScope, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiMemory, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiModule, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiModuleArray, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiNamedEvent, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiNamedEventArray, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiNet, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiNetArray, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiParameter, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiReg, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiRegArray, vpiIterator));
		xvpi_object_add_child(obj, xvpi_object_new(vpiVariables, vpiIterator));
		/* This temporary iterator collects generate-statements for evaluation in elaboration phase. */
		if (obj->type == vpiModule)
			xvpi_object_add_child(obj, xvpi_object_new(vpiStmt, vpiIterator));
	}

	static void xvpi_object_rename_child (struct xvpi_object *obj, PLI_INT32 old_name, PLI_INT32 new_name)
	{
		struct xvpi_object *child = xvpi_object_get_child(obj, old_name, vpiUndefined);
		if (child != NULL)
			child->name = new_name;
	}

	#define parsenode_rename_childs(pn, old_name, new_name) parsenode_rename_childs_f(&(pn), old_name, new_name)
	static void parsenode_rename_childs_f(struct D_ParseNode *pn, PLI_INT32 old_name, PLI_INT32 new_name)
	{
		int i;
		for (i=0; i<d_get_number_of_children(pn); i++) {
			struct D_ParseNode *child = d_get_child(pn, i);
			if (child != NULL) {
				struct xvpi_object *child_obj = child->user;
				if (child_obj != NULL && child_obj->name == old_name)
					child_obj->name = new_name;
			}
		}
	}

	#define parsenode_attach_childs_to_xvpi_object(pn, obj) parsenode_attach_childs_to_xvpi_object_f(&(pn), obj)
	#define parsenode_attach_childs_to_xvpi_object_f(pn, obj) parsenode_attach_renamed_childs_to_xvpi_object_f(pn, obj, vpiUndefined)
	#define parsenode_attach_renamed_childs_to_xvpi_object(pn, obj, name) parsenode_attach_renamed_childs_to_xvpi_object_f(&(pn), obj, name)
	static void parsenode_attach_renamed_childs_to_xvpi_object_f (struct D_ParseNode *pn, struct xvpi_object *obj, PLI_INT32 name)
	{
		if (pn == NULL)
			return;
		if (pn->user != NULL && pn->user != obj) {
			struct xvpi_object *pn_obj = pn->user;
			if (name != vpiUndefined)
				pn_obj->name = name;
			xvpi_object_add_child(obj, pn_obj);
		} else {
			int i;
			for (i=0; i<d_get_number_of_children(pn); i++) {
				struct D_ParseNode *child = d_get_child(pn, i);
				if (child != NULL)
					parsenode_attach_renamed_childs_to_xvpi_object_f(child, obj, name);
			}
		}
	}

	static void parsenode_attach_childs_to_list_pn (struct D_ParseNode *list_pn, struct D_ParseNode *pn)
	{
		int i;
		for (i=0; i<d_get_number_of_children(list_pn); i++) {
			struct D_ParseNode *child = d_get_child(list_pn, i);
			if (child->user)
				parsenode_attach_childs_to_xvpi_object_f(pn, child->user);
			parsenode_attach_childs_to_list_pn(child, pn);
		}
	}

	#define parsenode_update_list(pn, list_pn, parser) parsenode_update_list_f(&(pn), &(list_pn), parser)
	static void parsenode_update_list_f (struct D_ParseNode *pn, struct D_ParseNode *list_pn, struct D_Parser *parser)
	{
		int i;
		for (i=0; i<d_get_number_of_children(pn); i++) {
			struct D_ParseNode *child = d_get_child(pn, i);
			if (child && child != list_pn) {
				parsenode_attach_childs_to_list_pn(list_pn, child);
				free_D_ParseTreeBelow(parser, child);
				child->user = NULL;
			}
		}
	}

	#define parsenode_to_xvpi_object_string(pn, name) parsenode_to_xvpi_object_string_f(&(pn), name)
	static struct xvpi_object * parsenode_to_xvpi_object_string_f (struct D_ParseNode *pn, PLI_INT32 name)
	{
		if (pn != NULL) {
			char *s = pn->start_loc.s;
			int len = pn->end - s;
			return xvpi_object_new(name, xvpiStrProp, s, len);
		}
		return NULL;
	}

	#define parsenode_to_double(pn) parsenode_to_double_f(&(pn))
	static double parsenode_to_double_f (struct D_ParseNode *pn)
	{
		int len = pn->end - pn->start_loc.s;
		char buf [len+1];
		char *suffix;
		double val;
		int i, j;
		for (i=0, j=0; i<len; i++) {
			if (pn->start_loc.s[i] != '_')
				buf[j++] = pn->start_loc.s[i];
		}
		buf[j] = '\0';
		errno = 0;
		val = strtod(buf, &suffix);
		if (val == -HUGE_VAL || val == HUGE_VAL)
			d_fail("Error parsing real number: number too big!\n");
		if (errno != 0)
			d_fail("Error parsing real number!\n");
		switch (*suffix) {
		case 'T': return (val * 1e+12);
		case 'G': return (val * 1e+9);
		case 'M': return (val * 1e+6);
		case 'k': /* fall-through */
		case 'K': return (val * 1e+3);
		case 'm': return (val * 1e-3);
		case 'u': return (val * 1e-6);
		case 'n': return (val * 1e-9);
		case 'p': return (val * 1e-12);
		case 'f': return (val * 1e-15);
		case 'a': return (val * 1e-18);
		default: ;
		}
		return val;
	}

	#define parsenode_find_userdata(pn) parsenode_find_userdata_by_type_f(&(pn), vpiUndefined)
	#define parsenode_find_userdata_f(pn) parsenode_find_userdata_by_type_f(pn, vpiUndefined)
	#define parsenode_find_userdata_by_type(pn, type) parsenode_find_userdata_by_type_f(&(pn), type)
	static struct xvpi_object * parsenode_find_userdata_by_type_f (struct D_ParseNode *pn, PLI_INT32 type)
	{
		if (pn != NULL) {
			int i;
			if (pn->user != NULL) {
				struct xvpi_object *obj = pn->user;
				if (type == vpiUndefined || obj->type == type)
					return pn->user;
			}
			for (i=0; i<d_get_number_of_children(pn); i++) {
				struct xvpi_object *obj = parsenode_find_userdata_by_type_f(d_get_child(pn, i), type);
				if (obj != NULL && (type == vpiUndefined || obj->type == type))
					return obj;
			}
		}
		return NULL;
	}

	#define parsenode_find_by_name(pn, name, depth) parsenode_find_by_name_f(&(pn), name, depth)
	static struct D_ParseNode * parsenode_find_by_name_f (struct D_ParseNode *pn, const char *name, int depth)
	{
		int i;
		for (i=0; i<d_get_number_of_children(pn); i++) {
			struct D_ParseNode *child = d_get_child(pn, i);
			if (child) {
				if (strcmp(name,  parser_tables_gram.symbols[child->symbol].name) == 0) {
					return child;
				} else if (depth > 0) {
					if ((child = parsenode_find_by_name_f(child, name, depth-1)) != NULL)
						return child;
				}
			}
		}
		return NULL;
	}

	#define parsenode_find_obj_and_child(obj_pn, child_pn) parsenode_find_obj_and_child_f(&(obj_pn), &(child_pn))
	static struct xvpi_object * parsenode_find_obj_and_child_f (struct D_ParseNode *obj_pn, struct D_ParseNode *child_pn)
	{
		struct xvpi_object *obj = parsenode_find_userdata_f(obj_pn);
		if (obj == NULL)
			fatal("parsenode_find_obj_and_child_f(): could not find userdata ptr!\n");
		parsenode_attach_childs_to_xvpi_object_f(child_pn, obj);
		return obj;
	}

	#define parsenode_xvpi_object_new(parsenode, name, type) parsenode_xvpi_object_new_f(&(parsenode), name, type, #type)
	static struct xvpi_object * parsenode_xvpi_object_new_f (struct D_ParseNode *pn, PLI_INT32 name, PLI_INT32 type, const char *type_string)
	{
		struct xvpi_object *obj = xvpi_object_new(name, type);
		xvpi_object_add_child(obj, xvpi_object_new(vpiType, xvpiIntProp, type));
		xvpi_object_add_child(obj, xvpi_object_new(vpiType, xvpiStrProp, type_string, -1));
		xvpi_object_add_child(obj, xvpi_object_new(vpiLineNo, xvpiIntProp, pn->start_loc.line));
		xvpi_object_add_child(obj, xvpi_object_new(vpiFile, xvpiStrProp, pn->start_loc.pathname, -1));
		xvpi_object_add_child(obj, parsenode_to_xvpi_object_string_f(pn, vpiDecompile));
		xvpi_object_add_child(obj, xvpi_object_new(vpiAttribute, vpiIterator));
		switch (type) {
		case vpiNamedEvent:
		case vpiNet:
		case vpiReg:
		case vpiVariables:
			xvpi_object_add_child(obj, xvpi_object_new(xvpiDimension, vpiIterator));
		default: ; /* no need to add dimensions for other object types */
		}
		switch (type) {
		case vpiModule:
		case vpiBegin:
		case vpiNamedBegin:
		case vpiNamedFork:
		case vpiFork:
		case vpiCase:
		case vpiFor:
		case vpiIf:
		case vpiIfElse:
		case vpiAnalogFunction:
		case vpiFunction:
		case vpiTask:
			xvpi_obj_add_scope_props(obj);
		default: ; /* no need to add scope properties for other object types */
		}
		return obj;
	}

	#define parsenode_to_xvpi_object_ref(pn, name) parsenode_to_xvpi_object_ref_f(&(pn), name)
	static struct xvpi_object * parsenode_to_xvpi_object_ref_f (struct D_ParseNode *pn, PLI_INT32 name)
	{
		if (pn != NULL) {
			struct xvpi_object *ref = parsenode_xvpi_object_new_f(pn, name, xvpiReference, "xvpiReference");
			xvpi_object_add_child(ref, parsenode_to_xvpi_object_string_f(pn, vpiName));
			return ref;
		}
		return NULL;
	}

	#define parsenode_to_constant(pn, const_type) parsenode_to_constant_f(&(pn), const_type)
	static struct xvpi_object * parsenode_to_constant_f (struct D_ParseNode *pn, PLI_INT32 const_type)
	{
		struct xvpi_object *op = parsenode_xvpi_object_new(*pn, vpiExpr, vpiConstant);
		xvpi_object_add_child(op, xvpi_object_new(vpiConstType, xvpiIntProp, const_type));
		xvpi_object_add_child(op, parsenode_to_xvpi_object_string_f(pn, xvpiValue));
		return op;
	}

	#define parsenode_to_stmt_group(pn, type) parsenode_to_stmt_group_f(&(pn), type, 2)
	static struct xvpi_object * parsenode_to_stmt_group_f (struct D_ParseNode *pn, PLI_INT32 type, int depth)
	{
		struct D_ParseNode *block_id = parsenode_find_by_name_f(pn, "block_identifier", depth);
		struct xvpi_object *stmt_group;
		const char *type_string = NULL;

		if (block_id == NULL)
			block_id = parsenode_find_by_name_f(pn, "analog_block_identifier", depth);

		if (block_id == NULL)
			block_id = parsenode_find_by_name_f(pn, "generate_block_identifier", depth);

		if (block_id != NULL) {
			if (type == vpiBegin)
				type = vpiNamedBegin;
			if (type == vpiFork)
				type = vpiNamedFork;
		}

		switch (type) {
		case vpiBegin:
			type_string = "vpiBegin";
			break;
		case vpiNamedBegin:
			type_string = "vpiNamedBegin";
			break;
		case vpiFork:
			type_string = "vpiFork";
			break;
		case vpiNamedFork:
			type_string = "vpiNamedFork";
			break;
		default:
			fatal("unkown type in %s!\n", __FUNCTION__);
		}

		stmt_group = parsenode_xvpi_object_new_f(pn, vpiStmt, type, type_string);

		if (block_id != NULL)
			xvpi_object_add_child(stmt_group, parsenode_to_xvpi_object_string_f(block_id, vpiName));

		xvpi_object_add_child(stmt_group, xvpi_object_new(vpiStmt, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, stmt_group);

		return stmt_group;
	} ;

	#define parsenode_xvpi_object_new_process(pn, type) parsenode_xvpi_object_new_process_f(&(pn), type)
	static struct xvpi_object * parsenode_xvpi_object_new_process_f (struct D_ParseNode *pn, PLI_INT32 type)
	{
		const char *type_string = (type == vpiInitial) ? "vpiInitial" : "vpiAlways";
		struct xvpi_object *process = parsenode_xvpi_object_new_f(pn, vpiProcess, type, type_string);
		parsenode_attach_childs_to_xvpi_object_f(pn, process);
		return process;
	}

	#define parsenode_to_operation(pn, op_type) parsenode_to_operation_f(&(pn), op_type)
	static struct xvpi_object * parsenode_to_operation_f (struct D_ParseNode *pn, PLI_INT32 op_type)
	{
		struct xvpi_object *op = parsenode_xvpi_object_new_f(pn, vpiExpr, vpiOperation, "vpiOperation");
		if (op_type != vpiUndefined)
			xvpi_object_add_child(op, xvpi_object_new(vpiOpType, xvpiIntProp, op_type));
		xvpi_object_add_child(op, xvpi_object_new(vpiExpr, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, op);
		xvpi_object_rename_child(op, vpiExpr, vpiOperand);
		return op;
	}

	#define parsenode_to_obj_1prop(pn, name, type, name1, n1) parsenode_to_obj_1prop_f(&(pn), name, type, #type, name1, &(n1))
	static struct xvpi_object * parsenode_to_obj_1prop_f (struct D_ParseNode *pn, PLI_INT32 name, PLI_INT32 type, const char *type_string,
							      PLI_INT32 name1, struct D_ParseNode *n1)
	{
		struct xvpi_object *obj = parsenode_xvpi_object_new_f(pn, name, type, type_string);
		struct xvpi_object *prop1 = parsenode_find_userdata_f(n1);
		if (prop1) {
			prop1->name = name1;
			xvpi_object_add_child(obj, prop1);
		}
		return obj;
	}

	#define parsenode_to_obj_2props(pn, name, type, name1, n1, name2, n2) parsenode_to_obj_2props_f(&(pn), name, type, #type, name1, &(n1), name2, &(n2))
	static struct xvpi_object * parsenode_to_obj_2props_f (struct D_ParseNode *pn, PLI_INT32 name, PLI_INT32 type, const char *type_string,
							       PLI_INT32 name1, struct D_ParseNode *n1,
							       PLI_INT32 name2, struct D_ParseNode *n2)
	{
		struct xvpi_object *obj = parsenode_to_obj_1prop_f(pn, name, type, type_string, name1, n1);
		struct xvpi_object *prop2 = parsenode_find_userdata_f(n2);
		if (prop2) {
			prop2->name = name2;
			xvpi_object_add_child(obj, prop2);
		}
		return obj;
	}

	#define parsenode_to_obj_3props(pn, name, type, name1, n1, name2, n2, name3, n3) parsenode_to_obj_3props_f(&(pn), name, type, #type, name1, &(n1), name2, &(n2), name3, &(n3))
	static struct xvpi_object * parsenode_to_obj_3props_f (struct D_ParseNode *pn, PLI_INT32 name, PLI_INT32 type, const char *type_string,
							       PLI_INT32 name1, struct D_ParseNode *n1,
							       PLI_INT32 name2, struct D_ParseNode *n2,
							       PLI_INT32 name3, struct D_ParseNode *n3)
	{
		struct xvpi_object *obj = parsenode_to_obj_2props_f(pn, name, type, type_string, name1, n1, name2, n2);
		struct xvpi_object *prop3 = parsenode_find_userdata_f(n3);
		if (prop3) {
			prop3->name = name3;
			xvpi_object_add_child(obj, prop3);
		}
		return obj;
	}

	#define parsenode_to_obj_4props(pn, name, type, name1, n1, name2, n2, name3, n3, name4, n4) parsenode_to_obj_4props_f(&(pn), name, type, #type, name1, &(n1), name2, &(n2), name3, &(n3), name4, &(n4))
	static struct xvpi_object * parsenode_to_obj_4props_f (struct D_ParseNode *pn, PLI_INT32 name, PLI_INT32 type, const char *type_string,
							       PLI_INT32 name1, struct D_ParseNode *n1,
							       PLI_INT32 name2, struct D_ParseNode *n2,
							       PLI_INT32 name3, struct D_ParseNode *n3,
							       PLI_INT32 name4, struct D_ParseNode *n4)
	{
		struct xvpi_object *obj = parsenode_to_obj_3props_f(pn, name, type, type_string, name1, n1, name2, n2, name3, n3);
		struct xvpi_object *prop4 = parsenode_find_userdata_f(n4);
		if (prop4) {
			prop4->name = name4;
			xvpi_object_add_child(obj, prop4);
		}
		return obj;
	}

	#define parsenode_to_case_stmt(pn, case_type) parsenode_to_case_stmt_f(&(pn), case_type)
	static struct xvpi_object * parsenode_to_case_stmt_f (struct D_ParseNode *pn, PLI_INT32 case_type)
	{
		struct xvpi_object *stmt = parsenode_xvpi_object_new_f(pn, vpiStmt, vpiCase, "vpiCase");
		xvpi_object_add_child(stmt, xvpi_object_new(vpiCaseType, xvpiIntProp, case_type));
		xvpi_object_add_child(stmt, xvpi_object_new(vpiCaseItem, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, stmt);
		xvpi_object_rename_child(stmt, vpiExpr, vpiCondition);
		return stmt;
	}

	#define parsenode_to_case_item(pn) parsenode_to_case_item_f(&(pn))
	static struct xvpi_object * parsenode_to_case_item_f (struct D_ParseNode *pn)
	{
		struct xvpi_object *stmt = parsenode_xvpi_object_new_f(pn, vpiCaseItem, vpiCaseItem, "vpiCaseItem");
		xvpi_object_add_child(stmt, xvpi_object_new(vpiExpr, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, stmt);
		return stmt;
	}

	#define parsenode_to_module(pn, defname_pn, name_pn) parsenode_to_module_f(&(pn), &(defname_pn), &(name_pn))
	static struct xvpi_object * parsenode_to_module_f (struct D_ParseNode *pn, struct D_ParseNode *defname_pn, struct D_ParseNode *name_pn)
	{
		struct xvpi_object *module = parsenode_xvpi_object_new_f(pn, vpiModule, vpiModule, "vpiModule");
		xvpi_object_add_child(module, parsenode_to_xvpi_object_string_f(defname_pn, vpiDefName));
		xvpi_object_add_child(module, parsenode_to_xvpi_object_string_f(name_pn, vpiName));
		xvpi_object_add_child(module, xvpi_object_new(vpiCellInstance, xvpiBoolProp, pn->globals->cell_instance));
		xvpi_object_add_child(module, xvpi_object_new(vpiDefDecayTime, xvpiIntProp, (PLI_INT32) pn->globals->default_decay_time));
		xvpi_object_add_child(module, xvpi_object_new(vpiDefDecayTime, xvpiRealProp, pn->globals->default_decay_time));
		xvpi_object_add_child(module, xvpi_object_new(vpiDefNetType, xvpiIntProp, pn->globals->default_nettype));
		xvpi_object_add_child(module, xvpi_object_new(vpiDefLineNo, xvpiIntProp, pn->start_loc.line));
		xvpi_object_add_child(module, xvpi_object_new(vpiDefFile, xvpiStrProp, pn->start_loc.pathname, -1));
		//xvpi_object_add_child(module, xvpi_object_new(vpiDefDelayMode, xvpiIntProp, 0));
		xvpi_object_add_child(module, xvpi_object_new(vpiProtected, xvpiBoolProp, 0));
		xvpi_object_add_child(module, xvpi_object_new(vpiTimePrecision, xvpiIntProp, pn->globals->time_precision));
		xvpi_object_add_child(module, xvpi_object_new(vpiTimeUnit, xvpiIntProp, pn->globals->time_unit));
		xvpi_object_add_child(module, xvpi_object_new(vpiTopModule, xvpiBoolProp, 1));
		xvpi_object_add_child(module, xvpi_object_new(vpiUnconnDrive, xvpiIntProp, pn->globals->unconnected_drive));
		//xvpi_object_add_child(module, xvpi_object_new(vpiConfig, xvpiStrProp, "", -1));
		//xvpi_object_add_child(module, xvpi_object_new(vpiLibrary, xvpiStrProp, "", -1));
		//xvpi_object_add_child(module, xvpi_object_new(vpiCell, xvpiStrProp, "", -1));
		xvpi_object_add_child(module, xvpi_object_new(vpiPort, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiIODecl, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiSpecParam, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiParamAssign, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiDefParam, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiPrimitive, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiPrimitiveArray, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiContAssign, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiProcess, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiModPath, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiTchk, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiNode, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiTask, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiFunction, vpiIterator));
		xvpi_object_add_child(module, xvpi_object_new(vpiAnalogFunction, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, module);
		return module;
	}
	#define parsenode_to_module_instance(pn, defname_pn, param_pn, instance_pn) parsenode_to_module_instance_f(&(pn), &(defname_pn), &(param_pn), &(instance_pn))
	static struct xvpi_object * parsenode_to_module_instance_f (struct D_ParseNode *pn, struct D_ParseNode *defname_pn, struct D_ParseNode *param_pn, struct D_ParseNode *instance_pn)
	{
		struct xvpi_object *module_instance = parsenode_xvpi_object_new_f(pn, vpiModule, vpiModule, "vpiModule");
		xvpi_object_add_child(module_instance, parsenode_to_xvpi_object_string_f(defname_pn, vpiDefName));
		xvpi_object_add_child(module_instance, xvpi_object_new(vpiIODecl, vpiIterator));
		xvpi_object_add_child(module_instance, xvpi_object_new(vpiParamAssign, vpiIterator));
		xvpi_object_add_child(module_instance, xvpi_object_new(vpiPort, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(param_pn, module_instance);
		parsenode_attach_childs_to_xvpi_object_f(instance_pn, module_instance);
		return module_instance;
	}

	#define parsenode_to_task_or_function(pn, type) parsenode_to_task_or_function_f(&(pn), type, #type)
	static struct xvpi_object * parsenode_to_task_or_function_f (struct D_ParseNode *pn, PLI_INT32 type, const char *type_string)
	{
		struct xvpi_object *obj = parsenode_xvpi_object_new_f(pn, type, type, type_string);
		xvpi_object_add_child(obj, xvpi_object_new(vpiIODecl, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, obj);
		return obj;
	}

	static void set_nettype (struct xvpi_object *net, PLI_INT32 net_type)
	{
		const char *net_type_string = NULL;
		switch (net_type) {
		case vpiGround:	 net_type_string = "vpiGround"; break;
		case vpiNone:	 net_type_string = "vpiNone"; break;
		case vpiSupply0: net_type_string = "vpiSupply0"; break;
		case vpiSupply1: net_type_string = "vpiSupply1"; break;
		case vpiTri:	 net_type_string = "vpiTri"; break;
		case vpiTriAnd:	 net_type_string = "vpiTriAnd"; break;
		case vpiTriOr:	 net_type_string = "vpiTriOr"; break;
		case vpiTri0:	 net_type_string = "vpiTri0"; break;
		case vpiTri1:	 net_type_string = "vpiTri1"; break;
		case vpiUwire:	 net_type_string = "vpiUwire"; break;
		case vpiWand:	 net_type_string = "vpiWand"; break;
		case vpiWire:	 net_type_string = "vpiWire"; break;
		case vpiWor:	 net_type_string = "vpiWor"; break;
		case vpiWreal:	 net_type_string = "vpiWreal"; break;
		}
		if (net_type_string != NULL) {
			xvpi_object_add_child(net, xvpi_object_new(vpiNetType, xvpiIntProp, net_type));
			xvpi_object_add_child(net, xvpi_object_new(vpiNetType, xvpiStrProp, net_type_string, -1));
		}
	}

	#define parsenode_to_nettype(pn) parsenode_to_nettype_f(&(pn))
	static PLI_INT32 parsenode_to_nettype_f (struct D_ParseNode *pn)
	{
		if (pn->end - pn->start_loc.s >= 3) {
			/* Perfect hash function, gperf would do fine, but is overkill here. dparser ensures restricted input:
			 *  supply0 | supply1 | tri | triand | trior | tri0 | tri1 | uwire | wire | wand | wor | none
			 */
			switch (pn->start_loc.s[0]) {
			case 'g': return vpiGround;
			case 'n': return vpiNone;
			case 's':
				switch (pn->start_loc.s[6]) {
				case '0': return vpiSupply0;
				case '1': return vpiSupply1;
				}
			case 't':
				if (pn->end - pn->start_loc.s == 3)
					return vpiTri;
				switch (pn->start_loc.s[3]) {
				case 'a': return vpiTriAnd;
				case 'o': return vpiTriOr;
				case '0': return vpiTri0;
				case '1': return vpiTri1;
				}
			case 'u': return vpiUwire;
			case 'w':
				switch (pn->start_loc.s[1]) {
				case 'a': return vpiWand;
				case 'i': return vpiWire;
				case 'o': return vpiWor;
				case 'r': return vpiWreal;
				}
			}
		}
		fatal("memory corruption!\n");
		return vpiNone;
	}

	#define parsenode_to_net(pn) parsenode_to_net_f(&(pn))
	static struct xvpi_object * parsenode_to_net_f (struct D_ParseNode *pn)
	{
		struct xvpi_object *net = parsenode_xvpi_object_new_f(pn, vpiNet, vpiNet, "vpiNet");
		xvpi_object_add_child(net, xvpi_object_new(vpiContAssign, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiPrimTerm, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiPathTerm, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiTchkTerm, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiDriver, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiLocalDriver, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiLoad, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiLocalLoad, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiPortInst, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiBit, vpiIterator));
		xvpi_object_add_child(net, xvpi_object_new(vpiRange, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, net);
		return net;
	}

	static void parsenode_update_net_f (struct xvpi_object *net, PLI_INT32 net_type, struct xvpi_object *discipline, struct xvpi_object *is_signed,
					    struct xvpi_object *vectored, struct xvpi_object *scalared, struct xvpi_object *delay3, struct xvpi_object *range)
	{
		xvpi_object_add_child(net, discipline);
		xvpi_object_add_child(net, is_signed);
		xvpi_object_add_child(net, vectored);
		xvpi_object_add_child(net, scalared);
		xvpi_object_add_child(net, delay3);
		if (range != NULL) {
			xvpi_object_add_child(net, xvpi_object_get_child(range, vpiLeftRange, vpiUndefined));
			xvpi_object_add_child(net, xvpi_object_get_child(range, vpiRightRange, vpiUndefined));
		}
		set_nettype(net, net_type);
	}

	#define parsenode_update_nets(pn, net_type, parser) parsenode_update_nets_f(&(pn), net_type, parser)
	static void parsenode_update_nets_f (struct D_ParseNode *pn, PLI_INT32 net_type, struct D_Parser *parser)
	{
		struct D_ParseNode *discipline_pn = parsenode_find_by_name_f(pn, "discipline_identifier", 2);
		struct xvpi_object *discipline = parsenode_to_xvpi_object_ref_f(discipline_pn, vpiDiscipline);
		struct D_ParseNode *is_signed_pn = parsenode_find_by_name_f(pn, "signed", 2);
		struct xvpi_object *is_signed = xvpi_object_new(vpiSigned, xvpiBoolProp, (is_signed_pn == NULL) ? 0 : 1);
		struct D_ParseNode *vectored_pn = parsenode_find_by_name_f(pn, "vectored", 3);
		struct xvpi_object *vectored = xvpi_object_new(vpiExplicitVectored, xvpiBoolProp, (vectored_pn == NULL) ? 0 : 1);
		struct D_ParseNode *scalared_pn = parsenode_find_by_name_f(pn, "scalared", 3);
		struct xvpi_object *scalared = xvpi_object_new(vpiExplicitScalared, xvpiBoolProp, (scalared_pn == NULL) ? 0 : 1);
		struct D_ParseNode *delay3_pn = parsenode_find_by_name_f(pn, "delay3", 2);
		struct xvpi_object *delay3 = (delay3_pn == NULL) ? NULL : delay3_pn->user;
		struct D_ParseNode *range_pn = parsenode_find_by_name_f(pn, "range", 2);
		struct xvpi_object *range = (range_pn == NULL) ? NULL : range_pn->user;
		// TODO: nyi: charge strength, drive strength
		struct D_ParseNode *net_list_pn = d_get_child(pn, d_get_number_of_children(pn) - 2);
		struct xvpi_object *net;
		int i;
		net = parsenode_find_userdata_f(d_get_child(net_list_pn, 0));
		parsenode_update_net_f(net, net_type, discipline, is_signed, vectored, scalared, delay3, range);
		net_list_pn = d_get_child(net_list_pn, 1);
		for (i=0; i<d_get_number_of_children(net_list_pn); i++) {
			net = parsenode_find_userdata_f(d_get_child(net_list_pn, i));
			parsenode_update_net_f(net, net_type, discipline, is_signed, vectored, scalared, delay3, range);
		}
		if (range_pn != NULL) {
			range_pn->user = NULL;
			free_D_ParseTreeBelow(parser, range_pn);
		}
		if (range != NULL)
			xvpi_object_unref(range);
		if (delay3_pn != NULL) {
			delay3_pn->user = NULL;
			free_D_ParseTreeBelow(parser, delay3_pn);
		}
	}

	static void update_var_to_reg (struct D_ParseNode *reg_pn, struct xvpi_object *is_signed, struct xvpi_object *discipline, struct xvpi_object *range)
	{
		struct xvpi_object *reg = reg_pn->user;
		reg->name = vpiReg;
		reg->type = vpiReg;
		xvpi_object_remove_child(reg, xvpi_object_get_child(reg, vpiType, xvpiStrProp));
		xvpi_object_add_child(reg, xvpi_object_new(vpiType, xvpiStrProp, "vpiReg", -1));
		xvpi_object_add_child(reg, xvpi_object_new(vpiContAssign, vpiIterator));
		xvpi_object_add_child(reg, xvpi_object_new(vpiPrimTerm, vpiIterator));
		xvpi_object_add_child(reg, xvpi_object_new(vpiDriver, vpiIterator));
		xvpi_object_add_child(reg, xvpi_object_new(vpiLoad, vpiIterator));
		xvpi_object_add_child(reg, xvpi_object_new(vpiPortInst, vpiIterator));
		xvpi_object_add_child(reg, xvpi_object_new(vpiBit, vpiIterator));
		/* We treat regs initially as reg arrays. A second pass will fix it up and wrap arrays in vpiRegArray object. */
		xvpi_object_add_child(reg, xvpi_object_new(vpiRange, vpiIterator));
		xvpi_object_add_child(reg, is_signed);
		if (discipline != NULL)
			xvpi_object_add_child(reg, discipline);
		if (range != NULL) {
			xvpi_object_add_child(reg, xvpi_object_get_child(range, vpiLeftRange, vpiUndefined));
			xvpi_object_add_child(reg, xvpi_object_get_child(range, vpiRightRange, vpiUndefined));
		}
	}

	#define parsenode_update_vars_to_regs(pn, vars_pn, parser) parsenode_update_vars_to_regs_f(&(pn), &(vars_pn), parser)
	static void parsenode_update_vars_to_regs_f (struct D_ParseNode *pn, struct D_ParseNode *vars_pn, struct D_Parser *parser)
	{
		struct D_ParseNode *discipline_pn = parsenode_find_by_name_f(pn, "discipline_identifier", 2);
		struct xvpi_object *discipline = parsenode_to_xvpi_object_ref_f(discipline_pn, vpiDiscipline);
		struct D_ParseNode *is_signed_pn = parsenode_find_by_name_f(pn, "signed", 2);
		struct xvpi_object *is_signed = xvpi_object_new(vpiSigned, xvpiBoolProp, (is_signed_pn == NULL) ? 0 : 1);
		struct D_ParseNode *range_pn = parsenode_find_by_name_f(pn, "range", 2);
		struct xvpi_object *range = (range_pn == NULL) ? NULL : range_pn->user;
		int i;
		update_var_to_reg(d_get_child(vars_pn, 0), is_signed, discipline, range);
		vars_pn = d_get_child(vars_pn, 1);
		for (i=0; i<d_get_number_of_children(vars_pn); i++)
			update_var_to_reg(d_get_child(d_get_child(vars_pn, i), 1), is_signed, discipline, range);
		if (range_pn != NULL) {
			range_pn->user = NULL;
			free_D_ParseTreeBelow(parser, range_pn);
		}
		if (range != NULL)
			xvpi_object_unref(range);
	}

	#define parsenode_contains_whitespace(pn) parsenode_contains_whitespace_f(&(pn))
	static int parsenode_contains_whitespace_f (struct D_ParseNode *pn)
	{
		int len = pn->end - pn->start_loc.s;
		int i;
		/* Parsenode can contain trailing whitespace, this is not part of the token. */
		while (len > 1 && isspace(pn->start_loc.s[len-1]))
			len--;
		/* Check for whitespace in token string. */
		for (i=0; i<len; i++) {
			if (isspace(pn->start_loc.s[i]))
				return 1;
		}
		return 0;
	}

	#define parsenode_to_tfcall(pn, type) parsenode_to_tfcall_f(&(pn), type, #type)
	static struct xvpi_object * parsenode_to_tfcall_f (struct D_ParseNode *pn, PLI_INT32 type, const char *type_string)
	{
		PLI_INT32 name = (type == vpiTaskCall || type == vpiSysTaskCall || type == vpiAnalogSysTaskCall) ? vpiStmt : vpiExpr;
		struct xvpi_object *tfcall = parsenode_xvpi_object_new_f(pn, name, type, type_string);
		int i;
		if (d_get_number_of_children(pn) == 1)
			pn = d_get_child(pn, 0);
		xvpi_object_add_child(tfcall, parsenode_to_xvpi_object_string_f(d_get_child(pn, 0), vpiName));
		xvpi_object_add_child(tfcall, xvpi_object_new(vpiArgument, vpiIterator));
		for (i=1; i<d_get_number_of_children(pn); i++) {
			struct D_ParseNode *arg_pn = d_get_child(pn, i);
			parsenode_attach_renamed_childs_to_xvpi_object_f(arg_pn, tfcall, vpiArgument);
		}
		return tfcall;
	}

	#define parsenode_to_indexed_partselect(pn, left, right, select_type) parsenode_to_indexed_partselect_f(&(pn), &(left), &(right), select_type)
	static struct xvpi_object * parsenode_to_indexed_partselect_f (struct D_ParseNode *pn, struct D_ParseNode *left, struct D_ParseNode *right, PLI_INT32 select_type)
	{
		struct xvpi_object *select = parsenode_to_obj_2props_f(pn, vpiExpr, vpiIndexedPartSelect, "vpiIndexedPartSelect", vpiLeftRange, left, vpiRightRange, right);
		xvpi_object_add_child(select, xvpi_object_new(vpiIndexedPartSelectType, xvpiIntProp, select_type));
		return select;
	}

	#define parsenode_to_iodecl(pn, direction) parsenode_to_iodecl_f(&(pn), direction)
	static struct xvpi_object * parsenode_to_iodecl_f (struct D_ParseNode *pn, PLI_INT32 direction)
	{
		struct xvpi_object *iodecl = parsenode_xvpi_object_new_f(pn, vpiIODecl, vpiIODecl, "vpiIODecl");
		xvpi_object_add_child(iodecl, xvpi_object_new(vpiDirection, xvpiIntProp, direction));
		parsenode_attach_childs_to_xvpi_object_f(pn, iodecl);
		return iodecl;
	}

	#define parsenode_to_primitive(pn) parsenode_to_primitive_f(&(pn))
	static struct xvpi_object * parsenode_to_primitive_f (struct D_ParseNode *pn)
	{
		struct xvpi_object *primitive = parsenode_xvpi_object_new_f(pn, vpiPrimitive, vpiPrimitive, "vpiPrimitive");
		xvpi_object_add_child(primitive, xvpi_object_new(vpiPrimTerm, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, primitive);
		return primitive;
	}

	#define parsenode_to_prim_term(pn, expr_pn, direction) parsenode_to_prim_term_f(&(pn), &(expr_pn), direction)
	static struct xvpi_object * parsenode_to_prim_term_f (struct D_ParseNode *pn, struct D_ParseNode *expr_pn, PLI_INT32 direction)
	{
		struct xvpi_object *prim_term = parsenode_to_obj_1prop_f(pn, vpiPrimTerm, vpiPrimTerm, "vpiPrimTerm", vpiExpr, expr_pn);
		xvpi_object_add_child(prim_term, xvpi_object_new(vpiDirection, xvpiIntProp, direction));
		return prim_term;
	}

	#define parsenode_to_udp_defn(pn) parsenode_to_udp_defn_f(&(pn))
	static struct xvpi_object * parsenode_to_udp_defn_f (struct D_ParseNode *pn)
	{
		struct xvpi_object *udp_defn = parsenode_xvpi_object_new_f(pn, vpiUdpDefn, vpiUdpDefn, "vpiUdpDefn");
		xvpi_object_add_child(udp_defn, xvpi_object_new(vpiIODecl, vpiIterator));
		xvpi_object_add_child(udp_defn, xvpi_object_new(vpiTableEntry, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, udp_defn);
		return udp_defn;
	}

	#define parsenode_to_param(pn, name_pn, expr_pn, value_range_pn) parsenode_to_param_f(&(pn), &(name_pn), &(expr_pn), &(value_range_pn))
	static struct xvpi_object * parsenode_to_param_f (struct D_ParseNode *pn, struct D_ParseNode *name_pn, struct D_ParseNode *expr_pn, struct D_ParseNode *value_range_pn)
	{
		struct xvpi_object *param = parsenode_xvpi_object_new_f(pn, vpiParameter, vpiParameter, "vpiParameter");
		xvpi_object_add_child(param, parsenode_to_xvpi_object_string_f(name_pn, vpiName));
		xvpi_object_add_child(param, parsenode_find_userdata_f(expr_pn));
		xvpi_object_add_child(param, xvpi_object_new(vpiValueRange, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(value_range_pn, param);
		return param;
	}

	static void parsenode_update_param_f (struct xvpi_object *param, struct xvpi_object *param_type, struct xvpi_object *range, PLI_INT32 local_param, int is_signed)
	{
		if (param->type == vpiParameter) {
			xvpi_object_add_child(param, xvpi_object_new(vpiLocalParam, xvpiBoolProp, local_param));
			xvpi_object_add_child(param, xvpi_object_new(vpiSigned, xvpiBoolProp, is_signed));
			if (param_type != NULL)
				xvpi_object_add_child(param, param_type);
		}
		if (range != NULL) {
			xvpi_object_add_child(param, xvpi_object_get_child(range, vpiLeftRange, vpiUndefined));
			xvpi_object_add_child(param, xvpi_object_get_child(range, vpiRightRange, vpiUndefined));
		}

	}

	#define parsenode_update_params(pn, local_param, param_list_pn, parser) parsenode_update_params_f(&(pn), local_param, &(param_list_pn), parser)
	static void parsenode_update_params_f (struct D_ParseNode *pn, PLI_INT32 local_param, struct D_ParseNode *param_list_pn, struct D_Parser *parser)
	{
		int is_signed = parsenode_find_by_name_f(pn, "signed", 2) == NULL ? 0 : 1;
		struct D_ParseNode *param_type_pn = parsenode_find_by_name_f(pn, "parameter_type", 2);
		struct xvpi_object *param_type = parsenode_find_userdata_f(param_type_pn);
		struct D_ParseNode *range_pn = parsenode_find_by_name_f(pn, "range", 2);
		struct xvpi_object *range = parsenode_find_userdata_f(range_pn);
		struct xvpi_object *param;
		int i;
		param = parsenode_find_userdata_f(d_get_child(param_list_pn, 0));
		parsenode_update_param_f(param, param_type, range, local_param, is_signed);
		param_list_pn = d_get_child(param_list_pn, 1);
		for (i=0; i<d_get_number_of_children(param_list_pn); i++) {
			param = parsenode_find_userdata_f(d_get_child(param_list_pn, i));
			parsenode_update_param_f(param, param_type, range, local_param, is_signed);
		}
		if (param_type_pn != NULL) {
			param_type_pn->user = NULL;
			free_D_ParseTreeBelow(parser, param_type_pn);
		}
		if (range_pn != NULL) {
			range_pn->user = NULL;
			free_D_ParseTreeBelow(parser, range_pn);
		}
		if (range != NULL)
			xvpi_object_unref(range);
	}

	#define parsenode_to_value_range(pn, op1, op1_pn, op2, op2_pn) parsenode_to_value_range_f(&(pn), op1, &(op1_pn), op2, &(op2_pn))
	static struct xvpi_object * parsenode_to_value_range_f (struct D_ParseNode *pn, PLI_INT32 op1, struct D_ParseNode *op1_pn, PLI_INT32 op2, struct D_ParseNode *op2_pn)
	{
		struct xvpi_object *value_range = parsenode_xvpi_object_new_f(pn, vpiValueRange, vpiValueRange, "vpiValueRange");
		struct D_ParseNode *type_pn = d_get_child(pn, 0);
		xvpi_object_add_child(value_range, xvpi_object_new(vpiValueRangeType, xvpiIntProp, (type_pn->start_loc.s[0] == 'f') ? vpiFrom : vpiExclude));
		xvpi_object_add_child(value_range, xvpi_object_new(vpiExpr, vpiIterator));
		xvpi_object_add_child(value_range, parsenode_to_operation_f(op1_pn, op1));
		if (op2 != op1)
			xvpi_object_add_child(value_range, parsenode_to_operation_f(op2_pn, op2));
		return value_range;
	}

	#define parsenode_to_ref(pn, parent) parsenode_to_ref_f(&(pn), parent)
	static struct xvpi_object * parsenode_to_ref_f (struct D_ParseNode *pn, struct xvpi_object *parent)
	{
		struct xvpi_object *ref = parent;
		int i;
		for (i=d_get_number_of_children(pn)-1; i>=0; i--) {
			struct D_ParseNode *child_pn = d_get_child(pn, i);
			if (child_pn != NULL) {
				if (child_pn->user != NULL) {
					struct xvpi_object *child = child_pn->user;
					if (child->type != vpiVarSelect && child->type != vpiPartSelect && child->type != vpiIndexedPartSelect) {
						/* convert childs: ref '[' expr ']' -->  vpiVarSelect(ref, parent) */
						struct xvpi_object *var_select = parsenode_xvpi_object_new_f(child_pn, vpiExpr, vpiVarSelect, "vpiVarSelect");
						child->name = vpiIndex;
						xvpi_object_add_child(var_select, child);
						child = var_select;
					}
					ref->name = vpiParent;
					xvpi_object_add_child(child, ref);
					ref = child;
				} else {
					ref = parsenode_to_ref_f(child_pn, ref);
				}
			}
		}
		return ref;
	}

	static struct xvpi_object * parsenode_to_branch_f (struct D_ParseNode *pn, struct D_ParseNode *identifier_pn, struct D_ParseNode *range_pn, struct xvpi_object *pos_node, struct xvpi_object *neg_node, struct D_Parser *parser)
	{
		struct xvpi_object *branch = parsenode_xvpi_object_new_f(pn, vpiBranch, vpiBranch, "vpiBranch");
		struct xvpi_object *range = parsenode_find_userdata_f(range_pn);
		xvpi_object_add_child(branch, parsenode_to_xvpi_object_string_f(identifier_pn, vpiName));
		if (range != NULL) {
			xvpi_object_add_child(branch, xvpi_object_get_child(range, vpiLeftRange, vpiUndefined));
			xvpi_object_add_child(branch, xvpi_object_get_child(range, vpiRightRange, vpiUndefined));
			xvpi_object_unref(range);
		}
		xvpi_object_add_child(branch, pos_node);
		if (neg_node != NULL)
			xvpi_object_add_child(branch, neg_node);
		if (range_pn != NULL) {
			range_pn->user = NULL;
			free_D_ParseTreeBelow(parser, range_pn);
		}
		return branch;
	}

	#define parsenode_to_branches(pn, pos_pn, neg_pn, branch_list_pn, parser) parsenode_to_branches_f(&(pn), &(pos_pn), &(neg_pn), &(branch_list_pn), parser)
	static void parsenode_to_branches_f (struct D_ParseNode *pn, struct D_ParseNode *pos_pn, struct D_ParseNode *neg_pn, struct D_ParseNode *branch_list_pn, struct D_Parser *parser)
	{
		struct xvpi_object *pos_node = parsenode_find_userdata_f(pos_pn);
		struct xvpi_object *neg_node = parsenode_find_userdata_f(neg_pn);
		struct D_ParseNode *identifier_pn = d_get_child(branch_list_pn, 0);
		struct D_ParseNode *range_pn = d_get_child(branch_list_pn, 1);
		int i;
		pos_node->name = vpiPosNode;
		if (neg_node != NULL)
			neg_node->name = vpiNegNode;
		identifier_pn->user = parsenode_to_branch_f(pn, identifier_pn, range_pn, pos_node, neg_node, parser);
		branch_list_pn = d_get_child(branch_list_pn, 2);
		for (i=1; i<d_get_number_of_children(branch_list_pn); i+=3) {
			identifier_pn = d_get_child(branch_list_pn, i);
			range_pn = d_get_child(branch_list_pn, i+1);
			identifier_pn->user = parsenode_to_branch_f(pn, identifier_pn, range_pn, pos_node, neg_node, parser);
		}
		pos_pn->user = NULL;
		free_D_ParseTreeBelow(parser, pos_pn);
		if (neg_pn != NULL) {
			neg_pn->user = NULL;
			free_D_ParseTreeBelow(parser, neg_pn);
		}
	}

	#define parsenode_to_modpath(pn, path_type) parsenode_to_modpath_f(&(pn), path_type)
	static struct xvpi_object * parsenode_to_modpath_f (struct D_ParseNode *pn, PLI_INT32 path_type)
	{
		struct xvpi_object *modpath = parsenode_xvpi_object_new_f(pn, vpiModPath, vpiModPath, "vpiModPath");
		xvpi_object_add_child(modpath, xvpi_object_new(vpiPathType, xvpiIntProp, path_type));
		xvpi_object_add_child(modpath, xvpi_object_new(vpiModPathIn, vpiIterator));
		xvpi_object_add_child(modpath, xvpi_object_new(vpiModPathOut, vpiIterator));
		xvpi_object_add_child(modpath, xvpi_object_new(vpiModDataPathIn, vpiIterator));
		parsenode_attach_childs_to_xvpi_object_f(pn, modpath);
		return modpath;
	}

	#define parsenode_to_strengthval(pn) parsenode_to_strengthvals_f(&(pn))
	static PLI_INT32 parsenode_to_strengthvals_f (struct D_ParseNode *pn)
	{
		/**
		 * dparser ensures restricted input. Thus a minimal perfect hash function to distinguish:
			supply0, supply1,
			strong0, strong1
			pull0, pull1,
			weak0, weak1
		 */
		const char *s = pn->start_loc.s;
		switch (s[2]) {
		case 'p': return ((s[6] == '0') ? vpiSupply0 : vpiSupply1);
		case 'r': return ((s[6] == '0') ? vpiStrong0 : vpiStrong1);
		case 'l': return ((s[4] == '0') ? vpiPull0 : vpiPull1);
		case 'a': return ((s[4] == '0') ? vpiWeak0 : vpiWeak1);
		default: fatal("memory corruption!\n");
		}
		return vpiUndefined;
	}
}

source_text
	: ( description )*
	;

description
	: compiler_directive
	| module_declaration
	| udp_declaration
	| config_declaration
	| paramset_declaration
	| nature_declaration
	| discipline_declaration
	| connectrules_declaration
	;

module_declaration
	: ( attribute_instance )* module_keyword module_identifier ( module_parameter_port_list )? list_of_ports ';' ( module_item )* 'endmodule'
													{ xvpi_object_add_child(NULL, parsenode_to_module($n, $n2, $n2)); }
	| ( attribute_instance )* module_keyword module_identifier ( module_parameter_port_list )? ( list_of_port_declarations )? ';' ( non_port_module_item )* 'endmodule'
													{ xvpi_object_add_child(NULL, parsenode_to_module($n, $n2, $n2)); }
	;

module_keyword
	: 'module'
	| 'macromodule'
	| 'connectmodule'
	;

module_parameter_port_list
	: '#' '(' parameter_declaration ( ',' parameter_declaration )* ')'
	;

list_of_ports
	: '(' port ( ',' port )* ')'
	;

list_of_port_declarations
	: '(' port_declaration ( ',' port_declaration )* ')'
	| '(' ')'
	;

port
	: port_expression
	{
		$$ = parsenode_to_obj_1prop($n, vpiPort, vpiPort, vpiLowConn, $n0);
		xvpi_object_add_child($$, xvpi_object_new(vpiConnByName, xvpiBoolProp, 0));
	}
	| '.' port_identifier '(' ( port_expression )? ')'
	{
		$$ = parsenode_to_obj_1prop($n, vpiPort, vpiPort, vpiLowConn, $n3);
		xvpi_object_add_child($$, xvpi_object_new(vpiConnByName, xvpiBoolProp, 1));
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n1, vpiName));
	}
	;

port_expression
	: port_reference
	| '{' port_reference ( ',' port_reference )* '}'						{ $$ = parsenode_to_operation($n, vpiConcatOp); }
	;

port_reference
	: port_identifier ( '[' constant_range_expression ']' )?					{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

port_declaration
	: ( attribute_instance )* inout_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* input_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* output_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	;

module_item
	: port_declaration ';' 
	| non_port_module_item
	;

module_or_generate_item
	: ( attribute_instance )* module_or_generate_item_declaration					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* local_parameter_declaration ';'					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* parameter_override							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* continuous_assign							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* gate_instantiation							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* udp_instantiation							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* module_instantiation							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* initial_construct							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* always_construct							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* loop_generate_construct						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* conditional_generate_construct					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_construct							{ parsenode_update_list($n0, $n1, ${parser}); }
	;

module_or_generate_item_declaration
	: net_declaration
	| reg_declaration
	| integer_declaration
	| real_declaration
	| time_declaration
	| realtime_declaration
	| event_declaration
	| genvar_declaration
	| task_declaration
	| function_declaration
	| branch_declaration
	| analog_function_declaration
	;

non_port_module_item
	: module_or_generate_item
	| generate_region
	| specify_block
	| ( attribute_instance )* parameter_declaration ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* specparam_declaration 						{ parsenode_update_list($n0, $n1, ${parser}); }
	| aliasparam_declaration
	;

parameter_override
	: 'defparam' list_of_defparam_assignments ';'
	;

config_declaration
	: 'config' config_identifier ';' design_statement ( config_rule_statement )* 'endconfig'
	;

design_statement
	: 'design' ( ( library_identifier '.' )? cell_identifier )* ';'
	;

config_rule_statement
	: default_clause liblist_clause ';'
	| inst_clause liblist_clause ';'
	| inst_clause use_clause ';'
	| cell_clause liblist_clause ';'
	| cell_clause use_clause ';'
	;

default_clause : 'default' ;

inst_clause : 'instance' inst_name ;

inst_name : topmodule_identifier ( '.' instance_identifier )* ;

cell_clause : 'cell' ( library_identifier '.' )? cell_identifier ;

liblist_clause : 'liblist' ( library_identifier )* ;

use_clause : 'use' ( library_identifier '.' )? cell_identifier ( ':' 'config' )? ;

nature_declaration
	: 'nature' nature_identifier ( ':' parent_nature )? ( ';' )?  ( nature_item )* 'endnature'
	{
		struct xvpi_object *nature = parsenode_xvpi_object_new($n, vpiNature, vpiNature);
		xvpi_object_add_child(nature, xvpi_object_new(vpiDiscipline, vpiIterator));
		xvpi_object_add_child(nature, xvpi_object_new(vpiChild, vpiIterator));
		xvpi_object_add_child(nature, xvpi_object_new(vpiParamAssign, vpiIterator));
		xvpi_object_add_child(nature, parsenode_to_xvpi_object_string($n1, vpiName));
		parsenode_attach_childs_to_xvpi_object($n, nature);
		xvpi_object_add_child(NULL, nature);
	} ;

parent_nature
	: nature_identifier										{ $$ = parsenode_to_xvpi_object_ref($n, vpiExpr); }
	| discipline_identifier '.' potential_or_flow							{ $$ = parsenode_to_xvpi_object_ref($n, vpiExpr); } 
	;

nature_item : nature_attribute ;

nature_attribute
	: nature_attribute_identifier '=' nature_attribute_expression ';'
	{
		struct xvpi_object *param_assign = parsenode_xvpi_object_new($n, vpiParamAssign, vpiParamAssign);
		struct xvpi_object *parameter = parsenode_xvpi_object_new($n0, vpiLhs, vpiParameter);
		xvpi_object_add_child(parameter, parsenode_to_xvpi_object_string($n0, vpiName));
		xvpi_object_add_child(param_assign, parameter);
		parsenode_attach_childs_to_xvpi_object($n2, param_assign);
		xvpi_object_rename_child(param_assign, vpiExpr, vpiRhs);
		$$ = param_assign;
	} ;

discipline_declaration
	: 'discipline' discipline_identifier ( ';' )? ( discipline_item )* 'enddiscipline'
	{
		struct xvpi_object *discipline = parsenode_xvpi_object_new($n, vpiDiscipline, vpiDiscipline);
		xvpi_object_add_child(discipline, xvpi_object_new(vpiParamAssign, vpiIterator));
		xvpi_object_add_child(discipline, parsenode_to_xvpi_object_string($n1, vpiName));
		parsenode_attach_childs_to_xvpi_object($n, discipline);
		xvpi_object_add_child(NULL, discipline);
	} ;

discipline_item
	: nature_binding
	| discipline_domain_binding
	| nature_attribute_override
	;

nature_binding
	: potential_or_flow nature_identifier ';'
	{
		PLI_INT32 name = ($n0.start_loc.s[0] == 'p') ? vpiPotentialNature : vpiFlowNature;
		$$ = parsenode_to_xvpi_object_string($n1, name);
	} ;

potential_or_flow
	: 'potential'
	| 'flow'
	;

discipline_domain_binding
	: 'domain' discrete_or_continuous ';' 
	;

discrete_or_continuous
	: 'discrete'												{ $$ = xvpi_object_new(vpiDomain, xvpiIntProp, vpiDiscrete); }
	| 'continuous'												{ $$ = xvpi_object_new(vpiDomain, xvpiIntProp, vpiContinuous); }
	;

nature_attribute_override
	: potential_or_flow '.' nature_attribute
	;

connectrules_declaration
	: 'connectrules' connectrules_identifier ';' ( connectrules_item )* 'endconnectrules'
	;

connectrules_item
	: connect_insertion
	| connect_resolution
	;

connect_insertion
	: 'connect' connectmodule_identifier ( connect_mode )? ( parameter_value_assignment )? ( connect_port_overrides )? ';'
	;

connect_mode
	: 'merged'
	| 'split'
	;

connect_port_overrides
	: discipline_identifier ',' discipline_identifier
	| 'input' discipline_identifier ',' 'output' discipline_identifier
	| 'output' discipline_identifier ',' 'input' discipline_identifier
	| 'inout' discipline_identifier ',' 'inout' discipline_identifier
	;

connect_resolution
	: 'connect' discipline_identifier ( ',' discipline_identifier )* 'resolveto' discipline_identifier
	| 'exclude' ';'
	;

paramset_declaration
	: ( attribute_instance )* 'paramset' paramset_identifier module_or_paramset_identifier ';' paramset_item_declaration ( paramset_item_declaration )* paramset_statement ( paramset_statement )* 'endparamset'
	;

paramset_item_declaration
	: ( attribute_instance )* parameter_declaration ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* local_parameter_declaration ';'					{ parsenode_update_list($n0, $n1, ${parser}); }
	| aliasparam_declaration
	| ( attribute_instance )* integer_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* real_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	;

paramset_statement
	: '.' module_parameter_identifier '=' paramset_constant_expression ';'
	| '.' module_output_variable_identifier '=' paramset_constant_expression ';'
	| '.' system_parameter_identifier '=' paramset_constant_expression ';'
	| analog_function_statement
	;

paramset_constant_expression
	: constant_primary
	| hierarchical_parameter_identifier
	| unary_operator ( attribute_instance )* constant_primary
	| paramset_constant_expression binary_operator ( attribute_instance )* paramset_constant_expression
	| paramset_constant_expression '?' ( attribute_instance )* paramset_constant_expression ':' paramset_constant_expression
	;


local_parameter_declaration
	: 'localparam' ( 'signed' )? ( range )? list_of_param_assignments				{ parsenode_update_params($n, 1, $n3, ${parser}); }
	| 'localparam' parameter_type list_of_param_assignments						{ parsenode_update_params($n, 1, $n2, ${parser}); }
	;

parameter_declaration
	: 'parameter' ( 'signed' )? ( range )? list_of_param_assignments				{ parsenode_update_params($n, 0, $n3, ${parser}); }
	| 'parameter' parameter_type list_of_param_assignments						{ parsenode_update_params($n, 0, $n2, ${parser}); }
	;

specparam_declaration
	: 'specparam' ( range )? list_of_specparam_assignments ';'					{ parsenode_update_params($n, -1, $n2, ${parser}); }
	;

parameter_type
	: 'integer'											{ $$ = xvpi_object_new(vpiConstType, xvpiIntProp, vpiIntConst); }
	| 'real'											{ $$ = xvpi_object_new(vpiConstType, xvpiIntProp, vpiRealConst); }
	| 'realtime'											{ $$ = xvpi_object_new(vpiConstType, xvpiIntProp, vpiRealtimeConst); }
	| 'time'											{ $$ = xvpi_object_new(vpiConstType, xvpiIntProp, vpiTimeConst); }
	| 'string'											{ $$ = xvpi_object_new(vpiConstType, xvpiIntProp, vpiStringConst); }
	;

aliasparam_declaration
	: 'aliasparam' parameter_identifier '=' parameter_identifier ';'
	;

inout_declaration
	: 'inout' ( discipline_identifier )? ( net_type | 'wreal' )? ( 'signed' )? ( range )? list_of_port_identifiers
													{ $$ = parsenode_to_iodecl($n, vpiInout); }
	;

input_declaration
	: 'input' ( discipline_identifier )? ( net_type | 'wreal' )? ( 'signed' )? ( range )? list_of_port_identifiers
													{ $$ = parsenode_to_iodecl($n, vpiInput); }
	;

output_declaration
	:
	( 'output' ( discipline_identifier )? ( net_type | 'wreal' )? ( 'signed' )? ( range )? list_of_port_identifiers
	| 'output' ( discipline_identifier )? 'reg' ( 'signed' )? ( range )? list_of_variable_port_identifiers
	| 'output' output_variable_type list_of_variable_port_identifiers
	)												{ $$ = parsenode_to_iodecl($n, vpiOutput); }
	;

branch_declaration
	: 'branch' '(' branch_terminal ( ',' branch_terminal )? ')' list_of_branch_identifiers ';'	{ parsenode_to_branches($n, $n2, $n3, $n5, ${parser}); }
	;

branch_terminal
	: net_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| net_identifier '[' constant_expression ']'							{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| net_identifier '[' constant_range_expression ']'						{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

event_declaration
	: 'event' list_of_event_identifiers ';'
	;

integer_declaration
	: 'integer' list_of_variable_identifiers ';'
	;

net_declaration
	: net_type ( discipline_identifier )? ( 'signed' )? ( delay3 )? list_of_net_identifiers ';'	{ parsenode_update_nets($n, parsenode_to_nettype($n0), ${parser}); }
	| net_type ( discipline_identifier )? ( drive_strength )? ( 'signed' )? ( delay3 )? list_of_net_decl_assignments ';'
													{ parsenode_update_nets($n, parsenode_to_nettype($n0), ${parser}); }
	| net_type ( discipline_identifier )? ( 'vectored' | 'scalared' )? ( 'signed' )? range ( delay3 )? list_of_net_identifiers ';'
													{ parsenode_update_nets($n, parsenode_to_nettype($n0), ${parser}); }
	| net_type ( discipline_identifier )? ( drive_strength )? ( 'vectored' | 'scalared' )? ( 'signed' )? range ( delay3 )? list_of_net_decl_assignments ';'
													{ parsenode_update_nets($n, parsenode_to_nettype($n0), ${parser}); }
	| 'trireg' ( discipline_identifier )? ( charge_strength )? ( 'signed' )? ( delay3 )? list_of_net_identifiers ';'
													{ parsenode_update_nets($n, vpiTriReg, ${parser}); }
	| 'trireg' ( discipline_identifier )? ( drive_strength )? ( 'signed' )? ( delay3 )? list_of_net_decl_assignments ';'
													{ parsenode_update_nets($n, vpiTriReg, ${parser}); }
	| 'trireg' ( discipline_identifier )? ( charge_strength )? ( 'vectored' | 'scalared' )? ( 'signed' )? range ( delay3 )? list_of_net_identifiers ';'
													{ parsenode_update_nets($n, vpiTriReg, ${parser}); }
	| 'trireg' ( discipline_identifier )? ( drive_strength )? ( 'vectored' | 'scalared' )? ( 'signed' )? range ( delay3 )? list_of_net_decl_assignments ';'
													{ parsenode_update_nets($n, vpiTriReg, ${parser}); }
	| discipline_identifier ( range )? list_of_net_identifiers ';'					{ parsenode_update_nets($n, $g->default_nettype, ${parser}); }
	| discipline_identifier ( range )? list_of_net_decl_assignments ';'				{ parsenode_update_nets($n, $g->default_nettype, ${parser}); }
	| 'wreal' ( discipline_identifier )? ( range )? list_of_net_decl_assignments ';'		{ parsenode_update_nets($n, vpiWreal, ${parser}); }
	| 'ground' ( discipline_identifier )? ( range )? list_of_net_identifiers ';'			{ parsenode_update_nets($n, vpiGround, ${parser}); }
	;

real_declaration
	: 'real' list_of_real_identifiers ';'
	;

realtime_declaration
	: 'realtime' list_of_real_identifiers ';'							{ parsenode_rename_childs($n1, vpiRealVar, vpiRealtimeVar); }
	;

reg_declaration
	: 'reg' ( discipline_identifier )? ( 'signed' )? ( range )? list_of_variable_identifiers ';'	{ parsenode_update_vars_to_regs($n, $n4, ${parser}); }
	;

time_declaration
	: 'time' list_of_variable_identifiers ';'							{ parsenode_rename_childs($n1, vpiIntegerVar, vpiTimeVar); }
	;

net_type
	: 'supply0' | 'supply1' | 'tri' | 'triand' | 'trior' | 'tri0' | 'tri1' | 'uwire' | 'wire' | 'wand' | 'wor'
	;

output_variable_type
	: 'integer' | 'time'
	;

real_type
	:
	( real_identifier ( dimension )* ( '=' constant_arrayinit )?
	| real_identifier '=' constant_expression
	) {
		$$ = parsenode_xvpi_object_new($n, vpiVariables, vpiRealVar);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n0, vpiName));
		parsenode_attach_childs_to_xvpi_object($n, $$);
	} ;

variable_type
	:
	( variable_identifier ( dimension )* ( '=' constant_arrayinit )?
	| variable_identifier '=' constant_expression
	) {
		$$ = parsenode_xvpi_object_new($n, vpiVariables, vpiIntegerVar);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n0, vpiName));
		parsenode_attach_childs_to_xvpi_object($n, $$);
	} ;

drive_strength
	: '(' strength0 ',' strength1 ')'
	| '(' strength1 ',' strength0 ')'
	| '(' strength0 ',' 'highz1' ')'
	| '(' strength1 ',' 'highz0' ')'
	| '(' 'highz0' ',' strength1 ')'
	| '(' 'highz1' ',' strength0 ')'
	;

strength0 : 'supply0' | 'strong0' | 'pull0' | 'weak0' ;

strength1 : 'supply1' | 'strong1' | 'pull1' | 'weak1' ;

charge_strength : '(' 'small' ')' | '(' 'medium' ')' | '(' 'large' ')' ;

delay3
	: '#' delay_value										{ $$ = parsenode_find_userdata($n1); if ($$ != NULL) $$->name = vpiDelay; }
	| '#' '(' mintypmax_expression ( ',' mintypmax_expression ( ',' mintypmax_expression )? )? ')'	{ $$ = parsenode_to_operation($n, vpiListOp); $$->name = vpiDelay; }
	;

delay2
	: '#' delay_value										{ $$ = parsenode_find_userdata($n1); if ($$ != NULL) $$->name = vpiDelay; }
	| '#' '(' mintypmax_expression ( ',' mintypmax_expression )? ')'				{ $$ = parsenode_to_operation($n, vpiListOp); $$->name = vpiDelay; }
	;

delay_value
	: unsigned_number										{ $$ = parsenode_to_constant($n, vpiDecConst); }
	| real_number
	| identifier
	;

list_of_branch_identifiers
	: branch_identifier ( range )? ( ',' branch_identifier ( range )? )*
	;

list_of_defparam_assignments : defparam_assignment ( ',' defparam_assignment )* ;

list_of_event_identifiers : event_identifier ( dimension )* ( ',' event_identifier ( dimension )* )* ;

list_of_net_decl_assignments : net_decl_assignment ( ',' net_decl_assignment )* ;

list_of_net_identifiers : ams_net_identifier ( ',' ams_net_identifier )* ;

list_of_param_assignments : param_assignment ( ',' param_assignment )* ;

list_of_port_identifiers : port_identifier ( ',' port_identifier )* ;

list_of_real_identifiers : real_type ( ',' real_type )* ;

list_of_specparam_assignments : specparam_assignment ( ',' specparam_assignment )* ;

list_of_variable_identifiers : variable_type ( ',' variable_type )* ;

list_of_variable_port_identifiers : port_identifier ( '=' constant_expression )? ( ',' port_identifier ( '=' constant_expression )? )* ;

defparam_assignment
	: hierarchical_parameter_identifier '=' constant_mintypmax_expression				{ $$ = parsenode_to_obj_2props($n, vpiDefParam, vpiDefParam, vpiLhs, $n0, vpiRhs, $n2); }
	;

net_decl_assignment
	: net_identifier '=' expression									{ $$ = parsenode_to_net($n); }
	| net_identifier ( dimension )* ( '=' constant_optional_arrayinit )?				{ $$ = parsenode_to_net($n); }
	;

ams_net_identifier
	: net_identifier ( dimension )*									{ $$ = parsenode_to_net($n); }
	| hierarchical_net_identifier									{ $$ = parsenode_to_net($n); }
	;


param_assignment
	: parameter_identifier '=' constant_mintypmax_expression ( value_range )*			{ $$ = parsenode_to_param($n, $n0, $n2, $n3); }
	| parameter_identifier range '=' constant_arrayinit ( value_range )*				{ $$ = parsenode_to_param($n, $n0, $n2, $n3); }
	;

specparam_assignment
	: specparam_identifier '=' constant_mintypmax_expression
	{
		$$ = parsenode_to_obj_1prop($n, vpiSpecParam, vpiSpecParam, vpiExpr, $n2);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n0, vpiName));
	}
	| pulse_control_specparam
	;

pulse_control_specparam
	: 'PATHPULSE$' '=' '(' reject_limit_value ( ',' error_limit_value )? ')'
	| 'PATHPULSE$' specify_input_terminal_descriptor '$' specify_output_terminal_descriptor '=' '(' reject_limit_value ( ',' error_limit_value )? ')'
	;

error_limit_value : limit_value ;

reject_limit_value : limit_value ;

limit_value : constant_mintypmax_expression ;

dimension
	: '[' dimension_constant_expression ':' dimension_constant_expression ']'			{ $$ = parsenode_to_obj_2props($n, xvpiDimension, xvpiDimension, vpiLeftRange, $n1, vpiRightRange, $n3); }
	;

range
	: '[' msb_constant_expression ':' lsb_constant_expression ']'					{ $$ = parsenode_to_obj_2props($n, vpiRange, vpiRange, vpiLeftRange, $n1, vpiRightRange, $n3); }
	;

value_range
	: value_range_type '(' value_range_expression ':' value_range_expression ')'			{ $$ = parsenode_to_value_range($n, vpiGtOp, $n2, vpiLtOp, $n4); }
	| value_range_type '(' value_range_expression ':' value_range_expression ']'			{ $$ = parsenode_to_value_range($n, vpiGtOp, $n2, vpiLeOp, $n4); }
	| value_range_type '[' value_range_expression ':' value_range_expression ')'			{ $$ = parsenode_to_value_range($n, vpiGeOp, $n2, vpiLtOp, $n4); }
	| value_range_type '[' value_range_expression ':' value_range_expression ']'			{ $$ = parsenode_to_value_range($n, vpiGeOp, $n2, vpiLeOp, $n4); }
	| value_range_type '\'{' ( string ( ',' string )* ) '}'						{ $$ = parsenode_to_value_range($n, vpiListOp, $n2, vpiListOp, $n2); }
	| 'exclude' constant_expression									{ $$ = parsenode_to_value_range($n, vpiEqOp, $n1, vpiEqOp, $n1); }
	;

value_range_type
	: 'from'
	| 'exclude'
	;

value_range_expression
	: constant_expression
	| '-inf'											{ $$ = parsenode_to_constant($n, vpiRealConst); }
	| 'inf'												{ $$ = parsenode_to_constant($n, vpiRealConst); }
	;

analog_function_declaration
	: 'analog' 'function' ( analog_function_type )? analog_function_identifier ';' analog_function_item_declaration ( analog_function_item_declaration )* analog_function_statement 'endfunction'
													{ $$ = parsenode_to_task_or_function($n, vpiAnalogFunction); }
	;

analog_function_type
	: 'integer'
	| 'real'
	;

analog_function_item_declaration
	: analog_block_item_declaration
	| input_declaration ';'
	| output_declaration ';'
	| inout_declaration ';'
	;

function_declaration
	:
	( 'function' ( 'automatic' )? ( function_range_or_type )? function_identifier ';' function_item_declaration ( function_item_declaration )* function_statement 'endfunction'
	| 'function' ( 'automatic' )? ( function_range_or_type )? function_identifier '(' function_port_list ')' ';' ( block_item_declaration )* function_statement 'endfunction'
	)												{ $$ = parsenode_to_task_or_function($n, vpiFunction); }
	;

function_item_declaration
	: block_item_declaration
	| ( attribute_instance )* tf_input_declaration ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	;

function_port_list
	: ( attribute_instance )* tf_input_declaration ( ',' ( attribute_instance )* tf_input_declaration )*
	;

function_range_or_type
	: ( 'signed' )? ( range )?
	| 'integer'
	| 'real'
	| 'realtime'
	| 'time'
	;

task_declaration
	:
	( 'task' ( 'automatic' )? task_identifier ';' ( task_item_declaration )* statement_or_null 'endtask'
	| 'task' ( 'automatic' )? task_identifier '(' ( task_port_list )? ')' ';' ( block_item_declaration )* statement_or_null 'endtask'
	)												{ $$ = parsenode_to_task_or_function($n, vpiTask); }
	;

task_item_declaration
	: block_item_declaration
	| ( attribute_instance )* tf_input_declaration ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* tf_output_declaration ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* tf_inout_declaration ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	;

task_port_list
	: task_port_item ( ',' task_port_item )*
	;

task_port_item
	: ( attribute_instance )* tf_input_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* tf_output_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* tf_inout_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	;

tf_input_declaration
	:
	( 'input' ( discipline_identifier )? ( 'reg' )? ( 'signed' )? ( range )? list_of_port_identifiers
	| 'input' task_port_type list_of_port_identifiers
	)												{ $$ = parsenode_to_iodecl($n, vpiInput); }
	;

tf_output_declaration
	:
	( 'output' ( discipline_identifier )? ( 'reg' )? ( 'signed' )? ( range )? list_of_port_identifiers
	| 'output' task_port_type list_of_port_identifiers
	)												{ $$ = parsenode_to_iodecl($n, vpiOutput); }
	;

tf_inout_declaration
	:
	( 'inout' ( discipline_identifier )? ( 'reg' )? ( 'signed' )? ( range )? list_of_port_identifiers
	| 'inout' task_port_type list_of_port_identifiers
	)												{ $$ = parsenode_to_iodecl($n, vpiInout); }
	;

task_port_type : 'integer' | 'real' | 'realtime' | 'time' ;

analog_block_item_declaration
	: ( attribute_instance )* parameter_declaration ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* integer_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* real_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	;

block_item_declaration
	: ( attribute_instance )* 'reg' ( discipline_identifier )? ( 'signed' )? ( range )? list_of_block_variable_identifiers ';'
													{ parsenode_update_vars_to_regs($n, $n5, ${parser});
													  parsenode_update_list($n0, $n5, ${parser}); }
	| ( attribute_instance )* 'integer' list_of_block_variable_identifiers ';'			{ parsenode_update_list($n0, $n2, ${parser}); }
	| ( attribute_instance )* 'time' list_of_block_variable_identifiers ';'				{ parsenode_rename_childs($n1, vpiIntegerVar, vpiTimeVar);
													  parsenode_update_list($n0, $n2, ${parser}); }
	| ( attribute_instance )* 'real' list_of_block_real_identifiers ';'				{ parsenode_update_list($n0, $n2, ${parser}); }
	| ( attribute_instance )* 'realtime' list_of_block_real_identifiers ';'				{ parsenode_rename_childs($n1, vpiRealVar, vpiRealtimeVar);
													  parsenode_update_list($n0, $n2, ${parser}); }
	| ( attribute_instance )* event_declaration							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* local_parameter_declaration ';'					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* parameter_declaration ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	;

list_of_block_variable_identifiers
	: block_variable_type ( ',' block_variable_type )*
	;

list_of_block_real_identifiers
	: block_real_type ( ',' block_real_type )*
	;

block_variable_type
	: variable_identifier ( dimension )*
	{
		$$ = parsenode_xvpi_object_new($n, vpiVariables, vpiIntegerVar);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n0, vpiName));
		parsenode_attach_childs_to_xvpi_object($n, $$);
	} ;

block_real_type
	: real_identifier ( dimension )*
	{
		$$ = parsenode_xvpi_object_new($n, vpiVariables, vpiRealVar);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n0, vpiName));
		parsenode_attach_childs_to_xvpi_object($n, $$);
	} ;

gate_instantiation
	: 
	( cmos_switchtype ( delay3 )? cmos_switch_instance ( ',' cmos_switch_instance )* ';'
	| enable_gatetype ( drive_strength )? ( delay3 )? enable_gate_instance ( ',' enable_gate_instance )* ';'
	| mos_switchtype ( delay3 )? mos_switch_instance ( ',' mos_switch_instance )* ';'
	| n_input_gatetype ( drive_strength )? ( delay2 )? n_input_gate_instance ( ',' n_input_gate_instance )* ';'
	| n_output_gatetype ( drive_strength )? ( delay2 )? n_output_gate_instance ( ',' n_output_gate_instance )* ';'
	| pass_en_switchtype ( delay2 )? pass_enable_switch_instance ( ',' pass_enable_switch_instance )* ';'
	| pass_switchtype pass_switch_instance ( ',' pass_switch_instance )* ';'
	| 'pulldown' ( pulldown_strength )? pull_gate_instance ( ',' pull_gate_instance )* ';'
	| 'pullup' ( pullup_strength )? pull_gate_instance ( ',' pull_gate_instance )* ';'
	) {
		$$ = parsenode_to_primitive($n);
	} ;

cmos_switch_instance
	: ( name_of_gate_instance )? '(' output_terminal ',' input_terminal ',' ncontrol_terminal ',' pcontrol_terminal ')'
	;

enable_gate_instance
	: ( name_of_gate_instance )? '(' output_terminal ',' input_terminal ',' enable_terminal ')'
	;

mos_switch_instance
	: ( name_of_gate_instance )? '(' output_terminal ',' input_terminal ',' enable_terminal ')'
	;

n_input_gate_instance
	: ( name_of_gate_instance )? '(' output_terminal ',' input_terminal ( ',' input_terminal )* ')'
	;

n_output_gate_instance
	: ( name_of_gate_instance )? '(' output_terminal ( ',' output_terminal )* ',' input_terminal ')'
	;

pass_switch_instance
	: ( name_of_gate_instance )? '(' inout_terminal ',' inout_terminal ')'
	;

pass_enable_switch_instance
	: ( name_of_gate_instance )? '(' inout_terminal ',' inout_terminal ',' enable_terminal ')'
	;

pull_gate_instance
	: ( name_of_gate_instance )? '(' output_terminal ')'
	;

name_of_gate_instance
	: gate_instance_identifier ( range )?
	;

pulldown_strength
	: '(' strength0 ',' strength1 ')'
	| '(' strength1 ',' strength0 ')'
	| '(' strength0 ')'
	;

pullup_strength
	: '(' strength0 ',' strength1 ')'
	| '(' strength1 ',' strength0 ')'
	| '(' strength1 ')'
	;

enable_terminal
	: expression											{ $$ = parsenode_to_prim_term($n, $n0, vpiInput); }
	;

inout_terminal
	: net_lvalue											{ $$ = parsenode_to_prim_term($n, $n0, vpiInout); }
	;

input_terminal
	: expression											{ $$ = parsenode_to_prim_term($n, $n0, vpiInput); }
	;

ncontrol_terminal
	: expression											{ $$ = parsenode_to_prim_term($n, $n0, vpiInput); }
	;

output_terminal
	: net_lvalue											{ $$ = parsenode_to_prim_term($n, $n0, vpiOutput); }
	;

pcontrol_terminal
	: expression											{ $$ = parsenode_to_prim_term($n, $n0, vpiInput); }
	;

cmos_switchtype : 'cmos' | 'rcmos' ;

enable_gatetype : 'bufif0' | 'bufif1' | 'notif0' | 'notif1' ;

mos_switchtype : 'nmos' | 'pmos' | 'rnmos' | 'rpmos' ;

n_input_gatetype : 'and' | 'nand' | 'or' | 'nor' | 'xor' | 'xnor' ;

n_output_gatetype : 'buf' | 'not' ;

pass_en_switchtype : 'tranif0' | 'tranif1' | 'rtranif1' | 'rtranif0' ;

pass_switchtype : 'tran' | 'rtran' ;

module_instantiation
	: module_or_paramset_identifier ( parameter_value_assignment )? module_instance ( ',' module_instance )* ';'
	{
		int i;
		$2 = parsenode_to_module_instance($n, $n0, $n1, $n2);
		for (i=1; i<d_get_number_of_children(&$n3); i+=2) {
			struct D_ParseNode *pn = d_get_child(&$n3, i);
			pn->user = parsenode_to_module_instance_f(&$n, &$n0, &$n1, pn);
		}
		free_D_ParseTreeBelow(${parser}, &$n1);
	}
	;

parameter_value_assignment
	: '#' '(' list_of_parameter_assignments ')'
	;

list_of_parameter_assignments
	: ordered_parameter_assignment ( ',' ordered_parameter_assignment )*
	| named_parameter_assignment ( ',' named_parameter_assignment )*
	;

ordered_parameter_assignment
	: expression
	{
		$$ = parsenode_to_obj_1prop($n, vpiParamAssign, vpiParamAssign, vpiRhs, $n0);
		xvpi_object_add_child($$, xvpi_object_new(vpiConnByName, xvpiBoolProp, 0));
	}
	;

named_parameter_assignment
	: '.' parameter_identifier '(' ( mintypmax_expression )? ')'
	{
		$$ = parsenode_to_obj_1prop($n, vpiParamAssign, vpiParamAssign, vpiRhs, $n3);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n1, vpiName));
		xvpi_object_add_child($$, parsenode_to_xvpi_object_ref($n1, vpiLhs));
		xvpi_object_add_child($$, xvpi_object_new(vpiConnByName, xvpiBoolProp, 1));
	}
	| '.' system_parameter_identifier '(' ( constant_expression )? ')'
	{
		$$ = parsenode_to_obj_1prop($n, vpiParamAssign, vpiParamAssign, vpiRhs, $n3);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n1, vpiName));
		xvpi_object_add_child($$, parsenode_to_xvpi_object_ref($n1, vpiLhs));
		xvpi_object_add_child($$, xvpi_object_new(vpiConnByName, xvpiBoolProp, 1));
	}
	;

module_instance
	: name_of_module_instance '(' ( list_of_port_connections )? ')'
	;

name_of_module_instance
	: module_instance_identifier ( range )?								{ $0 = parsenode_to_xvpi_object_string($n0, vpiName); }
	;

list_of_port_connections
	: ordered_port_connection ( ',' ordered_port_connection )*
	| named_port_connection ( ',' named_port_connection )*
	;

ordered_port_connection
	: ( attribute_instance )* ( expression )?
	{
		$$ = parsenode_to_obj_1prop($n, vpiPort, vpiPort, vpiHighConn, $n1);
		xvpi_object_add_child($$, xvpi_object_new(vpiConnByName, xvpiBoolProp, 0));
		parsenode_attach_childs_to_xvpi_object($n0, $$);
	}
	;

named_port_connection
	: ( attribute_instance )* '.' port_identifier '(' ( expression )? ')'
	{
		$$ = parsenode_to_obj_1prop($n, vpiPort, vpiPort, vpiHighConn, $n4);
		xvpi_object_add_child($$, xvpi_object_new(vpiConnByName, xvpiBoolProp, 1));
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n2, vpiName));
		parsenode_attach_childs_to_xvpi_object($n0, $$);
	}
	;
 
generate_region
	: 'generate' ( module_or_generate_item )* 'endgenerate'
	;

genvar_declaration
	: 'genvar' list_of_genvar_identifiers ';'
	;

list_of_genvar_identifiers
	: genvar_identifier ( ',' genvar_identifier )*
	;

analog_loop_generate_statement
	: 'for' '(' genvar_initialization ';' genvar_expression ';' genvar_iteration ')' analog_statement
	{ $$ = parsenode_to_obj_4props($n, vpiStmt, vpiFor, vpiForInitStmt, $n2, vpiCondition, $n4, vpiForIncStmt, $n6, vpiStmt, $n8); }
	;

loop_generate_construct
	: 'for' '(' genvar_initialization ';' genvar_expression ';' genvar_iteration ')' generate_block
	{ $$ = parsenode_to_obj_4props($n, vpiStmt, vpiFor, vpiForInitStmt, $n2, vpiCondition, $n4, vpiForIncStmt, $n6, vpiStmt, $n8); }
	;

genvar_initialization
	: genvar_identifier '=' constant_expression							{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiAssignment, vpiLhs, $n0, vpiRhs, $n2); }
	;

genvar_expression
	: genvar_primary
	| unary_operator ( attribute_instance )* genvar_primary						{ $$ = parsenode_to_operation($n, vpiUndefined); }
	| genvar_expression binary_operator ( attribute_instance )* genvar_expression			{ $$ = parsenode_to_operation($n, vpiUndefined); }
	| genvar_expression '?' ( attribute_instance )* genvar_expression ':' genvar_expression		{ $$ = parsenode_to_operation($n, vpiConditionOp); }
	;

genvar_iteration
	: genvar_identifier '=' genvar_expression							{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiAssignment, vpiLhs, $n0, vpiRhs, $n2); }
	;

genvar_primary
	: constant_primary
	| genvar_identifier
	;

conditional_generate_construct
	: if_generate_construct
	| case_generate_construct
	;

if_generate_construct
	: 'if' '(' constant_expression ')' generate_block_or_null ( 'else' generate_block_or_null )?	{ $$ = parsenode_to_obj_3props($n, vpiStmt, vpiIfElse, vpiCondition, $n2, vpiStmt, $n4, vpiElseStmt, $n5); }
	;

case_generate_construct
	: 'case' '(' constant_expression ')' case_generate_item ( case_generate_item )* 'endcase'	{ $$ = parsenode_to_case_stmt($n, vpiCaseExact); }
	;

case_generate_item
	: constant_expression ( ',' constant_expression )* ':' generate_block_or_null			{ $$ = parsenode_to_case_item($n); }
	| 'default' ( ':' )? generate_block_or_null							{ $$ = parsenode_to_case_item($n); }
	;

generate_block
	: module_or_generate_item
	| 'begin' ( ':' generate_block_identifier )? ( module_or_generate_item )* 'end'			{ $$ = parsenode_to_stmt_group($n, vpiBegin); }
	;

generate_block_or_null
	: generate_block
	| ';'												{ $$ = parsenode_xvpi_object_new($n, vpiStmt, vpiNullStmt); }
	;

udp_declaration
	:
	( ( attribute_instance )* 'primitive' udp_identifier '(' udp_port_list ')' ';' udp_port_declaration ( udp_port_declaration )* udp_body 'endprimitive'
	| ( attribute_instance )* 'primitive' udp_identifier '(' udp_declaration_port_list ')' ';' udp_body 'endprimitive'
	) {
		xvpi_object_add_child(NULL, parsenode_to_udp_defn($n));
	} ;

udp_port_list
	: output_port_identifier ',' input_port_identifier ( ',' input_port_identifier )*
	;

udp_declaration_port_list
	: udp_output_declaration ',' udp_input_declaration ( ',' udp_input_declaration )*
	;

udp_port_declaration
	: udp_output_declaration ';'
	| udp_input_declaration ';'
	| udp_reg_declaration ';'
	;

udp_output_declaration
	:
	( ( attribute_instance )* 'output' port_identifier
	| ( attribute_instance )* 'output' ( discipline_identifier )? 'reg' port_identifier ( '=' constant_expression )?
	)												{ $$ = parsenode_to_iodecl($n, vpiOutput); }
	;

udp_input_declaration
	: ( attribute_instance )* 'input' list_of_port_identifiers					{ $$ = parsenode_to_iodecl($n, vpiOutput); }
	;

udp_reg_declaration
	: ( attribute_instance )* 'reg' ( discipline_identifier )? variable_identifier
	;

udp_body
	: combinational_body
	| sequential_body
	;

combinational_body
	: 'table' combinational_entry ( combinational_entry )* 'endtable'
	;

combinational_entry
	: level_input_list ':' output_symbol ';'							{ $$ = parsenode_xvpi_object_new($n, vpiTableEntry, vpiTableEntry); }
	;

sequential_body
	: ( udp_initial_statement )? 'table' sequential_entry ( sequential_entry )* 'endtable'
	;

udp_initial_statement
	: 'initial' output_port_identifier '=' init_val ';'
	{
		$$ = parsenode_xvpi_object_new($n, vpiInitial, vpiInitial);
		xvpi_object_add_child($$, parsenode_to_obj_2props($n, vpiStmt, vpiAssignment, vpiLhs, $n1, vpiRhs, $n3));
	} ;

init_val
	: '1\'b0' | '1\'b1' | '1\'bx' | '1\'bX' | '1\'B0' | '1\'B1' | '1\'Bx' | '1\'BX' | '1' | '0'
	;

sequential_entry
	: seq_input_list ':' current_state ':' next_state ';'						{ $$ = parsenode_xvpi_object_new($n, vpiTableEntry, vpiTableEntry); }
	;

seq_input_list
	: level_input_list
	| edge_input_list
	;

level_input_list
	: level_symbol ( level_symbol )*
	;

edge_input_list
	: ( level_symbol )* edge_indicator ( level_symbol )*
	;

edge_indicator
	: '(' level_symbol level_symbol ')'
	| edge_symbol
	;

current_state
	: level_symbol
	;

next_state
	: output_symbol
	| '-'
	;

output_symbol
	: '0' | '1' | 'x' | 'X'
	;

level_symbol
	: '0' | '1' | 'x' | 'X' | '?' | 'b' | 'B'
	;

edge_symbol
	: 'r' | 'R' | 'f' | 'F' | 'p' | 'P' | 'n' | 'N' | '*'
	;

udp_instantiation
	: udp_identifier ( drive_strength )? ( delay2 )? udp_instance ( ',' udp_instance )* ';'		{ $$ = parsenode_to_primitive($n); }
	;

udp_instance
	: ( name_of_udp_instance )? '(' output_terminal ',' input_terminal ( ',' input_terminal )* ')'
	;

name_of_udp_instance
	: udp_instance_identifier ( range )?
	;

continuous_assign
	: 'assign' ( drive_strength )? ( delay3 )? list_of_net_assignments ';'				{ parsenode_update_list($n, $n3, ${parser}); }
	;

list_of_net_assignments
	: lnet_assignment ( ',' lnet_assignment )*
	;

lnet_assignment
	: net_lvalue '=' expression									{ $$ = parsenode_to_obj_2props($n, vpiContAssign, vpiContAssign, vpiLhs, $n0, vpiRhs, $n2); }
	;

analog_construct
	: 'analog' analog_statement
	| 'analog' 'initial' analog_function_statement							{ $$ = parsenode_xvpi_object_new_process($n, vpiInitial); }
	;

analog_procedural_assignment
	: analog_variable_assignment ';'
	;

analog_variable_assignment
	: analog_variable_lvalue '=' analog_expression							{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiAssignment, vpiLhs, $n0, vpiRhs, $n2); }
	;

initial_construct
	: 'initial' statement										{ $$ = parsenode_xvpi_object_new_process($n, vpiInitial); }
	;

always_construct
	: 'always' statement										{ $$ = parsenode_xvpi_object_new_process($n, vpiAlways); }
	;

blocking_assignment
	: variable_lvalue '=' ( delay_or_event_control )? expression
	{
		$$ = parsenode_to_obj_2props($n, vpiStmt, vpiAssignment, vpiLhs, $n0, vpiRhs, $n3);
		parsenode_attach_childs_to_xvpi_object($n2, $$);
		xvpi_object_add_child($$, xvpi_object_new(vpiBlocking, xvpiBoolProp, 1));
	} ;

nonblocking_assignment
	: variable_lvalue '<=' ( delay_or_event_control )? expression
	{
		$$ = parsenode_to_obj_2props($n, vpiStmt, vpiAssignment, vpiLhs, $n0, vpiRhs, $n3);
		parsenode_attach_childs_to_xvpi_object($n2, $$);
		xvpi_object_add_child($$, xvpi_object_new(vpiBlocking, xvpiBoolProp, 0));
	} ;

procedural_continuous_assignments
	: 'assign' variable_assignment									{ if (($$ = $1) != NULL) $$->type = vpiAssignStmt; }
	| 'deassign' variable_lvalue									{ $$ = parsenode_to_obj_1prop($n, vpiStmt, vpiDeassign, vpiLhs, $n1); }
	| 'force' variable_assignment									{ if (($$ = $1) != NULL) $$->type = vpiForce; }
	| 'force' net_assignment
	| 'release' variable_lvalue									{ $$ = parsenode_to_obj_1prop($n, vpiStmt, vpiRelease, vpiLhs, $n1); }
	| 'release' net_lvalue										{ $$ = parsenode_to_obj_1prop($n, vpiStmt, vpiRelease, vpiLhs, $n1); }
	;

net_assignment
	: net_lvalue '=' expression									{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiForce, vpiLhs, $n0, vpiRhs, $n2); }
	;

variable_assignment
	: variable_lvalue '=' expression								{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiAssignment, vpiLhs, $n0, vpiRhs, $n2); }
	;

analog_seq_block
	: 'begin' ( ':' analog_block_identifier ( analog_block_item_declaration )* )? ( analog_statement )* 'end'
													{ $$ = parsenode_to_stmt_group($n, vpiBegin); }
	;

analog_event_seq_block
	: 'begin' ( ':' analog_block_identifier ( analog_block_item_declaration )* )? ( analog_event_statement )* 'end'
													{ $$ = parsenode_to_stmt_group($n, vpiBegin); }
	;

analog_function_seq_block
	: 'begin' ( ':' analog_block_identifier ( analog_block_item_declaration )* )? ( analog_function_statement )* 'end'
													{ $$ = parsenode_to_stmt_group($n, vpiBegin); }
	;

par_block
	: 'fork' ( ':' block_identifier ( block_item_declaration )* )? ( statement )* 'join'		{ $$ = parsenode_to_stmt_group($n, vpiFork); }
	;

seq_block
	: 'begin' ( ':' block_identifier ( block_item_declaration )* )? ( statement )* 'end'		{ $$ = parsenode_to_stmt_group($n, vpiBegin); }
	;

analog_statement
	: generate_statement										{ obsolete($n0.start_loc, "2.0", "generate_statement", "LRM2.0 section 6.7.3"); }
	| ( attribute_instance )* analog_loop_generate_statement					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_loop_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_case_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_conditional_statement						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_procedural_assignment						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_seq_block							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_system_task_enable						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* contribution_statement						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* indirect_contribution_statement					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_event_control_statement					{ parsenode_update_list($n0, $n1, ${parser}); }
	;

analog_statement_or_null
	: analog_statement
	| ( attribute_instance )* ';'									{ $$ = parsenode_xvpi_object_new($n, vpiStmt, vpiNullStmt); }
	;

analog_event_statement
	: ( attribute_instance )* analog_loop_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_case_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_conditional_statement						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_procedural_assignment						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_event_seq_block						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_system_task_enable						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* disable_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* event_trigger								{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* ';'									{ $$ = parsenode_xvpi_object_new($n, vpiStmt, vpiNullStmt); }
	;

analog_function_statement
	: ( attribute_instance )* analog_function_case_statement					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_function_conditional_statement					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_function_loop_statement					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_function_seq_block						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_procedural_assignment						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* analog_system_task_enable						{ parsenode_update_list($n0, $n1, ${parser}); }
	;

analog_function_statement_or_null
	: analog_function_statement
	| ( attribute_instance )* ';'									{ $$ = parsenode_xvpi_object_new($n, vpiStmt, vpiNullStmt); }
	;

statement
	: ( attribute_instance )* blocking_assignment ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* case_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* conditional_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* disable_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* event_trigger								{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* loop_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* nonblocking_assignment ';'						{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* par_block								{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* procedural_continuous_assignments ';'					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* procedural_timing_control_statement					{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* seq_block								{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* system_task_enable							{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* task_enable								{ parsenode_update_list($n0, $n1, ${parser}); }
	| ( attribute_instance )* wait_statement							{ parsenode_update_list($n0, $n1, ${parser}); }
	;

statement_or_null
	: statement
	| ( attribute_instance )* ';'									{ $$ = parsenode_xvpi_object_new($n, vpiStmt, vpiNullStmt); }
	;

function_statement
	: statement
	;

analog_event_control_statement
	: analog_event_control analog_event_statement 							{ $$ = parsenode_find_obj_and_child($n0, $n1); $$->name = vpiStmt; }
	;

analog_event_control
	: '@' hierarchical_event_identifier								{ $$ = parsenode_to_obj_1prop($n, vpiEventControl, vpiEventControl, vpiCondition, $n1); }
	| '@' '(' analog_event_expression ')'								{ $$ = parsenode_to_obj_1prop($n, vpiEventControl, vpiEventControl, vpiCondition, $n2); }
	;

analog_event_expression
	: expression
	| 'posedge' expression										{ $$ = parsenode_to_operation($n, vpiPosedgeOp); }
	| 'negedge' expression										{ $$ = parsenode_to_operation($n, vpiNegedgeOp); }
	| hierarchical_event_identifier
	| 'initial_step' ( '(' '"' analysis_identifier '"' ( ',' '"' analysis_identifier '"' )* ')' )?	{ $$ = parsenode_to_operation($n, vpiInitialStepOp); }
	| 'final_step' ( '(' '"' analysis_identifier '"' ( ',' '"' analysis_identifier '"' )* ')' )?	{ $$ = parsenode_to_operation($n, vpiFinalStepOp); }
	| analog_event_functions
	| analog_event_expression 'or' analog_event_expression						{ $$ = parsenode_to_operation($n, vpiEventOrOp); }
	| analog_event_expression ',' analog_event_expression						{ $$ = parsenode_to_operation($n, vpiEventOrOp); }
	;

analog_event_functions
	: 'cross' '(' analog_expression ( ',' analog_expression_or_null ( ',' constant_expression_or_null ( ',' constant_expression_or_null ( ',' analog_expression )? )? )? )? ')'
													{ $$ = parsenode_to_operation($n, vpiCrossOp); }
	| 'above' '(' analog_expression ( ',' constant_expression_or_null ( ',' constant_expression_or_null ( ',' analog_expression )? )? )? ')'
													{ $$ = parsenode_to_operation($n, vpiAboveOp); }
	| 'timer' '(' analog_expression ( ',' analog_expression_or_null ( ',' constant_expression_or_null ( ',' analog_expression )? )? )? ')'
													{ $$ = parsenode_to_operation($n, vpiTimerOp); }
	;

delay_control
	: '#' delay_value										{ $$ = parsenode_to_obj_1prop($n, vpiDelayControl, vpiDelayControl, vpiDelay, $n1); }
	| '#' '(' mintypmax_expression ')'								{ $$ = parsenode_to_obj_1prop($n, vpiDelayControl, vpiDelayControl, vpiDelay, $n2); }
	;

delay_or_event_control
	: delay_control
	| event_control
	| 'repeat' '(' expression ')' event_control							{ $$ = parsenode_to_obj_2props($n, vpiRepeatControl, vpiRepeatControl, vpiExpr, $n2, vpiEventControl, $n4); }
	;

disable_statement
	: 'disable' hierarchical_task_identifier ';'
	{
		$$ = parsenode_xvpi_object_new($n, vpiStmt, vpiDisable);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n1, vpiExpr));
	}
	| 'disable' hierarchical_block_identifier ';'							
	{
		$$ = parsenode_xvpi_object_new($n, vpiStmt, vpiDisable);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n1, vpiExpr));
	}
	;

event_control
	: '@' hierarchical_event_identifier								{ $$ = parsenode_to_obj_1prop($n, vpiEventControl, vpiEventControl, vpiCondition, $n1); }
	| '@' '(' event_expression ')'									{ $$ = parsenode_to_obj_1prop($n, vpiEventControl, vpiEventControl, vpiCondition, $n2); }
	| '@' '*'											{ $$ = parsenode_xvpi_object_new($n, vpiEventControl, vpiEventControl); }
	| '@' '(' '*' ')'										{ $$ = parsenode_xvpi_object_new($n, vpiEventControl, vpiEventControl); }
	;

event_trigger
	: '->' hierarchical_event_identifier ( '[' expression ']' )* ';'
	{
		$$ = parsenode_xvpi_object_new($n, vpiStmt, vpiEventStmt);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n1, vpiName));
		xvpi_object_add_child($$, xvpi_object_new(vpiIndex, vpiIterator));
		parsenode_attach_renamed_childs_to_xvpi_object($n2, $$, vpiIndex);
	} ;

event_expression
	: expression
	| 'posedge' expression										{ $$ = parsenode_to_operation($n, vpiPosedgeOp); }
	| 'negedge' expression										{ $$ = parsenode_to_operation($n, vpiNegedgeOp); }
	| hierarchical_event_identifier
	| event_expression 'or' event_expression							{ $$ = parsenode_to_operation($n, vpiEventOrOp); }
	| event_expression ',' event_expression								{ $$ = parsenode_to_operation($n, vpiEventOrOp); }
	| analog_event_functions
	| 'driver_update' expression
	| analog_variable_lvalue
	;

procedural_timing_control
	: delay_control
	| event_control
	;

procedural_timing_control_statement
	: procedural_timing_control statement_or_null							{ $$ = parsenode_find_obj_and_child($n0, $n1); $$->name = vpiStmt; }
	;

wait_statement
	: 'wait' '(' expression ')' statement_or_null							{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiWait, vpiCondition, $n2, vpiStmt, $n4); }
	;

analog_conditional_statement
	: 'if' '(' analog_expression ')' analog_statement_or_null ( 'else' analog_statement_or_null )?
	{ $$ = parsenode_to_obj_3props($n, vpiStmt, vpiIfElse, vpiCondition, $n2, vpiStmt, $n4, vpiElseStmt, $n5); }
	;

analog_function_conditional_statement
	: 'if' '(' analog_expression ')' analog_function_statement_or_null ( 'else' analog_function_statement_or_null )?
	{ $$ = parsenode_to_obj_3props($n, vpiStmt, vpiIfElse, vpiCondition, $n2, vpiStmt, $n4, vpiElseStmt, $n5); }
	;

conditional_statement
	: 'if' '(' expression ')' statement_or_null ( 'else' statement_or_null )?
	{ $$ = parsenode_to_obj_3props($n, vpiStmt, vpiIfElse, vpiCondition, $n2, vpiStmt, $n4, vpiElseStmt, $n5); }
	;

analog_case_statement
	: 'case' '(' analog_expression ')' analog_case_item ( analog_case_item )* 'endcase'		{ $$ = parsenode_to_case_stmt($n, vpiCaseExact); }
	| 'casex' '(' analog_expression ')' analog_case_item ( analog_case_item )* 'endcase'		{ $$ = parsenode_to_case_stmt($n, vpiCaseX); }
	| 'casez' '(' analog_expression ')' analog_case_item ( analog_case_item )* 'endcase'		{ $$ = parsenode_to_case_stmt($n, vpiCaseZ); }
	;

analog_case_item
	: analog_expression ( ',' analog_expression )* ':' analog_statement_or_null			{ $$ = parsenode_to_case_item($n); }
	| 'default' ( ':' )? analog_statement_or_null							{ $$ = parsenode_to_case_item($n); }
	;

analog_function_case_statement
	: 'case' '(' analog_expression ')' analog_function_case_item ( analog_function_case_item )* 'endcase'
													{ $$ = parsenode_to_case_stmt($n, vpiCaseExact); }
	;

analog_function_case_item
	: analog_expression ( ',' analog_expression )* ':' analog_function_statement_or_null		{ $$ = parsenode_to_case_item($n); }
	| 'default' ( ':' )? analog_function_statement_or_null						{ $$ = parsenode_to_case_item($n); }
	;

case_statement
	: 'case' '(' expression ')' case_item ( case_item )* 'endcase'					{ $$ = parsenode_to_case_stmt($n, vpiCaseExact); }
	| 'casez' '(' expression ')' case_item ( case_item )* 'endcase'					{ $$ = parsenode_to_case_stmt($n, vpiCaseZ); }
	| 'casex' '(' expression ')' case_item ( case_item )* 'endcase'					{ $$ = parsenode_to_case_stmt($n, vpiCaseX); }
	;

case_item
	: expression ( ',' expression )* ':' statement_or_null						{ $$ = parsenode_to_case_item($n); }
	| 'default' ( ':' )? statement_or_null								{ $$ = parsenode_to_case_item($n); }
	;

analog_loop_statement
	: 'repeat' '(' analog_expression ')' analog_statement						{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiRepeat, vpiCondition, $n2, vpiStmt, $n4); }
	| 'while' '(' analog_expression ')' analog_statement						{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiWhile, vpiCondition, $n2, vpiStmt, $n4); }
	| 'for' '(' analog_variable_assignment ';' analog_expression ';' analog_variable_assignment ')' analog_statement
	{ $$ = parsenode_to_obj_4props($n, vpiStmt, vpiFor, vpiForInitStmt, $n2, vpiCondition, $n4, vpiForIncStmt, $n6, vpiStmt, $n8); }
	;

analog_function_loop_statement
	: 'repeat' '(' analog_expression ')' analog_function_statement					{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiRepeat, vpiCondition, $n2, vpiStmt, $n4); }
	| 'while' '(' analog_expression ')' analog_function_statement					{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiWhile, vpiCondition, $n2, vpiStmt, $n4); }
	| 'for' '(' analog_variable_assignment ';' analog_expression ';' analog_variable_assignment ')' analog_function_statement
	{ $$ = parsenode_to_obj_4props($n, vpiStmt, vpiFor, vpiForInitStmt, $n2, vpiCondition, $n4, vpiForIncStmt, $n6, vpiStmt, $n8); }
	;

loop_statement
	: 'forever' statement										{ $$ = parsenode_to_obj_1prop($n, vpiStmt, vpiForever, vpiStmt, $n1); }
	| 'repeat' '(' expression ')' statement								{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiRepeat, vpiCondition, $n2, vpiStmt, $n4); }
	| 'while' '(' expression ')' statement								{ $$ = parsenode_to_obj_2props($n, vpiStmt, vpiWhile, vpiCondition, $n2, vpiStmt, $n4); }
	| 'for' '(' variable_assignment ';' expression ';' variable_assignment ')' statement
	{ $$ = parsenode_to_obj_4props($n, vpiStmt, vpiFor, vpiForInitStmt, $n2, vpiCondition, $n4, vpiForIncStmt, $n6, vpiStmt, $n8); }
	;

analog_system_task_enable
	: analog_system_task_identifier ( '(' ( analog_expression )? ( ',' ( analog_expression )? )* ')' )? ';'
													{ $$ = parsenode_to_tfcall($n, vpiAnalogSysTaskCall); }
	;

system_task_enable
	: system_task_identifier ( '(' ( expression )? ( ',' ( expression )? )* ')' )? ';'		{ $$ = parsenode_to_tfcall($n, vpiSysTaskCall); }
	;

task_enable
	: hierarchical_task_identifier ( '(' expression ( ',' expression )* ')' )? ';'			{ $$ = parsenode_to_tfcall($n, vpiTaskCall); }
	;

contribution_statement
	: branch_lvalue '<+' analog_expression ';'
	{
		$$ = parsenode_to_obj_2props($n, vpiStmt, vpiContrib, vpiBranch, $n0, vpiRhs, $n2);
		xvpi_object_add_child($$, xvpi_object_new(vpiDirect, xvpiBoolProp, 1));
	}
	;

indirect_contribution_statement
	: branch_lvalue ':' indirect_expression '==' analog_expression ';'
	{
		$$ = parsenode_to_obj_3props($n, vpiStmt, vpiContrib, vpiBranch, $n0, vpiLhs, $n2, vpiRhs, $n4);
		xvpi_object_add_child($$, xvpi_object_new(vpiDirect, xvpiBoolProp, 0));
	}
	;

specify_block
	: 'specify' ( specify_item )* 'endspecify'
	;

specify_item
	: specparam_declaration
	| pulsestyle_declaration
	| showcancelled_declaration
	| path_declaration
	| system_timing_check
	;

pulsestyle_declaration
	: 'pulsestyle_onevent' list_of_path_outputs ';'
	| 'pulsestyle_ondetect' list_of_path_outputs ';'
	;

showcancelled_declaration
	: 'showcancelled' list_of_path_outputs ';'
	| 'noshowcancelled' list_of_path_outputs ';'
	;

path_declaration
	: simple_path_declaration ';'
	| edge_sensitive_path_declaration ';'
	| state_dependent_path_declaration ';'
	;

simple_path_declaration
	: parallel_path_description '=' path_delay_value						{ $$ = parsenode_to_modpath($n, vpiPathParallel); }
	| full_path_description '=' path_delay_value							{ $$ = parsenode_to_modpath($n, vpiPathFull); }
	;

parallel_path_description
	: '(' specify_input_terminal_descriptor ( polarity_operator )? '=>' specify_output_terminal_descriptor ')'
	;

full_path_description
	: '(' list_of_path_inputs ( polarity_operator )? '*>' list_of_path_outputs ')'
	;

list_of_path_inputs
	: specify_input_terminal_descriptor ( ',' specify_input_terminal_descriptor )*
	;

list_of_path_outputs
	: specify_output_terminal_descriptor ( ',' specify_output_terminal_descriptor )*
	;

specify_input_terminal_descriptor
	: input_identifier ( '[' constant_range_expression ']' )?
	;

specify_output_terminal_descriptor
	: output_identifier ( '[' constant_range_expression ']' )?
	;

input_identifier
	: input_port_identifier
	| inout_port_identifier
	;

output_identifier
	: output_port_identifier
	| inout_port_identifier
	;

path_delay_value
	: list_of_path_delay_expressions								{ $$ = parsenode_to_operation($n, vpiListOp); $$->name = vpiDelay; }
	| '(' list_of_path_delay_expressions ')'							{ $$ = parsenode_to_operation($n, vpiListOp); $$->name = vpiDelay; }
	;

list_of_path_delay_expressions
	: t_path_delay_expression
	| trise_path_delay_expression ',' tfall_path_delay_expression
	| trise_path_delay_expression ',' tfall_path_delay_expression ',' tz_path_delay_expression
	| t01_path_delay_expression ',' t10_path_delay_expression ',' t0z_path_delay_expression ',' tz1_path_delay_expression ',' t1z_path_delay_expression ',' tz0_path_delay_expression
	| t01_path_delay_expression ',' t10_path_delay_expression ',' t0z_path_delay_expression ',' tz1_path_delay_expression ',' t1z_path_delay_expression ',' tz0_path_delay_expression ','
	  t0x_path_delay_expression ',' tx1_path_delay_expression ',' t1x_path_delay_expression ',' tx0_path_delay_expression ',' txz_path_delay_expression ',' tzx_path_delay_expression
	;

t_path_delay_expression : path_delay_expression ;
trise_path_delay_expression : path_delay_expression ;
tfall_path_delay_expression : path_delay_expression ;
tz_path_delay_expression : path_delay_expression ;
t01_path_delay_expression : path_delay_expression ;
t10_path_delay_expression : path_delay_expression ;
t0z_path_delay_expression : path_delay_expression ;
tz1_path_delay_expression : path_delay_expression ;
t1z_path_delay_expression : path_delay_expression ;
tz0_path_delay_expression : path_delay_expression ;
t0x_path_delay_expression : path_delay_expression ;
tx1_path_delay_expression : path_delay_expression ;
t1x_path_delay_expression : path_delay_expression ;
tx0_path_delay_expression : path_delay_expression ;
txz_path_delay_expression : path_delay_expression ;
tzx_path_delay_expression : path_delay_expression ;
path_delay_expression : constant_mintypmax_expression ;

edge_sensitive_path_declaration
	: parallel_edge_sensitive_path_description '=' path_delay_value					{ $$ = parsenode_to_modpath($n, vpiPathParallel); }
	| full_edge_sensitive_path_description '=' path_delay_value					{ $$ = parsenode_to_modpath($n, vpiPathFull); }
	;

parallel_edge_sensitive_path_description
	: '(' ( edge_identifier )? specify_input_terminal_descriptor '=>' '(' specify_output_terminal_descriptor ( polarity_operator )? ':' data_source_expression ')' ')'
	;

full_edge_sensitive_path_description
	: '(' ( edge_identifier )? list_of_path_inputs '*>' '(' list_of_path_outputs ( polarity_operator )? ':' data_source_expression ')' ')'
	;

data_source_expression
	: expression
	;

edge_identifier
	: 'posedge'
	| 'negedge'
	;

state_dependent_path_declaration
	: 'if' '(' module_path_expression ')' simple_path_declaration					{ $$ = $4; parsenode_attach_renamed_childs_to_xvpi_object($n2, $$, vpiCondition); }
	| 'if' '(' module_path_expression ')' edge_sensitive_path_declaration				{ $$ = $4; parsenode_attach_renamed_childs_to_xvpi_object($n2, $$, vpiCondition); }
	| 'ifnone' simple_path_declaration								{ $$ = $1; xvpi_object_add_child($$, xvpi_object_new(vpiModPathHasIfNone, xvpiBoolProp, 1)); }
	;

polarity_operator : '+' | '-' ;

system_timing_check
	: setup_timing_check
	| hold_timing_check
	| setuphold_timing_check
	| recovery_timing_check
	| removal_timing_check
	| recrem_timing_check
	| skew_timing_check
	| timeskew_timing_check
	| fullskew_timing_check
	| period_timing_check
	| width_timing_check
	| nochange_timing_check
	;

setup_timing_check
	: '$setup' '(' data_event ',' reference_event ',' timing_check_limit ( ',' ( notifier )? )? ')' ';'
	;

hold_timing_check
	: '$hold' '(' reference_event ',' data_event ',' timing_check_limit ( ',' ( notifier )? )? ')' ';'
	;

setuphold_timing_check
	: '$setuphold' '(' reference_event ',' data_event ',' timing_check_limit ',' timing_check_limit
		( ',' ( notifier )? ( ',' ( stamptime_condition )? ( ',' ( checktime_condition )? ( ',' ( delayed_reference )? ( ',' ( delayed_data )? )? )? )? )? )? ')' ';'
	;

recovery_timing_check
	: '$recovery' '(' reference_event ',' data_event ',' timing_check_limit ( ',' ( notifier )? )? ')' ';'
	;

removal_timing_check
	: '$removal' '(' reference_event ',' data_event ',' timing_check_limit ( ',' ( notifier )? )? ')' ';'
	;

recrem_timing_check
	: '$recrem' '(' reference_event ',' data_event ',' timing_check_limit ',' timing_check_limit
	  ( ',' ( notifier )? ( ',' ( stamptime_condition )? ( ',' ( checktime_condition )? ( ',' ( delayed_reference )? ( ',' ( delayed_data )? )? )? )? )? )? ')' ';'
	;

skew_timing_check
	: '$skew' '(' reference_event ',' data_event ',' timing_check_limit ( ',' ( notifier )? )? ')' ';'
	;

timeskew_timing_check
	: '$timeskew' '(' reference_event ',' data_event ',' timing_check_limit ( ',' ( notifier )? ( ',' ( event_based_flag )? ( ',' ( remain_active_flag )? )? )? )? ')' ';'
	;

fullskew_timing_check
	: '$fullskew' '(' reference_event ',' data_event ',' timing_check_limit ',' timing_check_limit ( ',' ( notifier )? ( ',' ( event_based_flag )? ( ',' ( remain_active_flag )? )? )? )? ')' ';'
	;

period_timing_check
	: '$period' '(' controlled_reference_event ',' timing_check_limit ( ',' ( notifier )? )? ')' ';'
	;

width_timing_check
	: '$width' '(' controlled_reference_event ',' timing_check_limit ( ',' threshold ( ',' notifier )? )? ')' ';'
	;

nochange_timing_check
	: '$nochange' '(' reference_event ',' data_event ',' start_edge_offset ',' end_edge_offset ( ',' ( notifier )? )? ')' ';'
	;

checktime_condition : mintypmax_expression ;

controlled_reference_event : controlled_timing_check_event ;

data_event : timing_check_event ;

delayed_data
	: terminal_identifier
	| terminal_identifier '[' constant_mintypmax_expression ']'
	;

delayed_reference
	: terminal_identifier
	| terminal_identifier '[' constant_mintypmax_expression ']'
	;

end_edge_offset : mintypmax_expression ;

event_based_flag : constant_expression ;

notifier : variable_identifier ;

reference_event : timing_check_event ;

remain_active_flag : constant_expression ;

stamptime_condition : mintypmax_expression ;

start_edge_offset : mintypmax_expression ;

threshold : constant_expression ;

timing_check_limit : expression ;

timing_check_event
	: ( timing_check_event_control )? specify_terminal_descriptor ( '&&&' timing_check_condition )?
	;

controlled_timing_check_event
	: timing_check_event_control specify_terminal_descriptor ( '&&&' timing_check_condition )?
	;

timing_check_event_control
	: 'posedge'
	| 'negedge'
	| edge_control_specifier
	;

specify_terminal_descriptor
	: specify_input_terminal_descriptor
	| specify_output_terminal_descriptor
	;

edge_control_specifier : 'edge' '[' edge_descriptor ( ',' edge_descriptor )* ']' ;

edge_descriptor
	: '01'
	| '10'
	| z_or_x zero_or_one
	| zero_or_one z_or_x
	;

zero_or_one : '0' | '1' ;

z_or_x : 'x' | 'X' | 'z' | 'Z' ;

timing_check_condition
	: scalar_timing_check_condition
	| '(' scalar_timing_check_condition ')'
	;

scalar_timing_check_condition
	: expression
	| '~' expression
	| expression '==' scalar_constant
	| expression '===' scalar_constant
	| expression '!=' scalar_constant
	| expression '!==' scalar_constant
	;

scalar_constant
	: '1\'b0' | '1\'b1' | '1\'B0' | '1\'B1' | '\'b0' | '\'b1' | '\'B0' | '\'B1' | '1' | '0'
	;

analog_concatenation
	: '{' analog_expression ( ',' analog_expression )* '}'						{ $$ = parsenode_to_operation($n, vpiConcatOp); }
	;

analog_multiple_concatenation
	: '{' constant_expression analog_concatenation '}'						{ $$ = parsenode_to_operation($n, vpiMultiConcatOp); }
	;

analog_filter_function_arg
	: parameter_identifier
	| parameter_identifier '[' msb_constant_expression ':' lsb_constant_expression ']'
	| constant_optional_arrayinit
	;

concatenation
	: '{' expression ( ',' expression )* '}'							{ $$ = parsenode_to_operation($n, vpiConcatOp); }
	;

constant_concatenation
	: '{' constant_expression ( ',' constant_expression )* '}'					{ $$ = parsenode_to_operation($n, vpiConcatOp); }
	;

constant_multiple_concatenation
	: '{' constant_expression constant_concatenation '}'						{ $$ = parsenode_to_operation($n, vpiMultiConcatOp); }
	;

module_path_concatenation
	: '{' module_path_expression ( ',' module_path_expression )* '}'				{ $$ = parsenode_to_operation($n, vpiConcatOp); }
	;

module_path_multiple_concatenation
	: '{' constant_expression module_path_concatenation '}'						{ $$ = parsenode_to_operation($n, vpiMultiConcatOp); }
	;

multiple_concatenation
	: '{' constant_expression concatenation '}'							{ $$ = parsenode_to_operation($n, vpiMultiConcatOp); }
	;

constant_arrayinit
	: '\'{' constant_expression ( ',' constant_expression )* '}'
	;

constant_optional_arrayinit
	: '\'{' ( constant_expression )? ( ',' ( constant_expression )? )* '}'
	;

analog_function_call
	: analog_function_identifier ( attribute_instance )* '(' analog_expression ( ',' analog_expression )* ')'
													{ $$ = parsenode_to_tfcall($n, vpiAnalogFuncCall); }
	;

analog_system_function_call
	: analog_system_function_identifier ( '(' ( analog_expression )? ( ',' ( analog_expression )? )* ')' )?
													{ $$ = parsenode_to_tfcall($n, vpiAnalogSysFuncCall); }
	;

analog_built_in_function_call
	: analog_built_in_function_name '(' analog_expression ( ',' analog_expression )? ')'		{ $$ = parsenode_to_tfcall($n, vpiAnalogBuiltinFuncCall); }
	;

analog_built_in_function_name
	: 'ln' | 'log' | 'exp' | 'sqrt' | 'min' | 'max' | 'abs' | 'pow' | 'floor' | 'ceil'
	| 'sin' | 'cos' | 'tan' | 'asin' | 'acos' | 'atan' | 'atan2'
	| 'hypot' | 'sinh' | 'cosh' | 'tanh' | 'asinh' | 'acosh' | 'atanh'
	;

analysis_function_call
	: 'analysis' '(' '"' analysis_identifier '"' ( ',' '"' analysis_identifier '"' )* ')'		{ $$ = parsenode_to_tfcall($n, vpiAnalysisFuncCall); }
	;

analog_filter_function_call
	:
	( 'ddt' '(' analog_expression ( ',' abstol_expression )? ')'
	| 'ddx' '(' analog_expression ',' branch_probe_function_call ')'
	| 'idt' '(' analog_expression ( ',' analog_expression ( ',' analog_expression ( ',' abstol_expression )? )? )? ')'
	| 'idtmod' '(' analog_expression ( ',' analog_expression ( ',' analog_expression ( ',' analog_expression ( ',' abstol_expression )? )? )? )? ')'
	| 'absdelay' '(' analog_expression ',' analog_expression ( ',' constant_expression )? ')'
	| 'transition' '(' analog_expression ( ',' analog_expression ( ',' analog_expression ( ',' analog_expression ( ',' constant_expression )? )? )? )? ')'
	| 'slew' '(' analog_expression ( ',' analog_expression ( ',' analog_expression )? )? ')'
	| 'last_crossing' '(' analog_expression ( ',' analog_expression )? ')'
	| 'limexp' '(' analog_expression ')'
	| laplace_filter_name '(' analog_expression ',' ( analog_filter_function_arg )? ',' ( analog_filter_function_arg )? ( ',' constant_expression )? ')'
	| zi_filter_name '(' analog_expression ',' ( analog_filter_function_arg )? ',' ( analog_filter_function_arg )? ',' constant_expression ( ',' analog_expression ( ',' constant_expression )? )? ')'
	) {
		$$ = parsenode_to_tfcall($n, vpiAnalogFilterFuncCall);
	} ;

analog_small_signal_function_call
	:
	( 'ac_stim' '(' ( '"' analysis_identifier '"' ( ',' analog_expression ( ',' analog_expression )? )? )? ')' 
	| 'white_noise' '(' analog_expression ( ',' string )? ')' 
	| 'flicker_noise' '(' analog_expression ',' analog_expression ( ',' string )? ')' 
	| 'noise_table' '(' noise_table_input_arg ( ',' string )? ')'
	) {
		$$ = parsenode_to_tfcall($n, vpiAnalogSmallSignalFuncCall);
	} ;

noise_table_input_arg
	: parameter_identifier
	| parameter_identifier '[' msb_constant_expression ':' lsb_constant_expression ']'
	| string 
	| constant_arrayinit
	;

laplace_filter_name : 'laplace_zd' | 'laplace_zp' | 'laplace_np' | 'laplace_nd' ;

zi_filter_name : 'zi_zp' | 'zi_zd' | 'zi_np' | 'zi_nd' ;

branch_probe_function_call
	: nature_attribute_identifier '(' branch_reference ')'						{ $$ = parsenode_to_tfcall($n, vpiBranchProbeFuncCall); }
	| nature_attribute_identifier '(' analog_net_reference ( ',' analog_net_reference )? ')'	{ $$ = parsenode_to_tfcall($n, vpiBranchProbeFuncCall); }
	;

port_probe_function_call
	: nature_attribute_identifier '(' '<' analog_port_reference '>' ')'				{ $$ = parsenode_to_tfcall($n, vpiPortProbeFuncCall); }
	;

constant_analog_function_call
	: analog_function_identifier ( attribute_instance )* '(' constant_expression ( ',' constant_expression )* ')'
													{ $$ = parsenode_to_tfcall($n, vpiAnalogFuncCall); }
	;

constant_function_call
	: function_identifier ( attribute_instance )* '(' constant_expression ( ',' constant_expression )* ')'
													{ $$ = parsenode_to_tfcall($n, vpiFuncCall); }
	;

constant_system_function_call
	: system_function_identifier '(' constant_expression ( ',' constant_expression )* ')'		{ $$ = parsenode_to_tfcall($n, vpiSysFuncCall); }
	;

constant_analog_built_in_function_call
	: analog_built_in_function_name '(' constant_expression ( ',' constant_expression )? ')'	{ $$ = parsenode_to_tfcall($n, vpiAnalogBuiltinFuncCall); }
	;

function_call
	: hierarchical_function_identifier ( attribute_instance )* '(' ( expression ( ',' expression )* )? ')'
													{ $$ = parsenode_to_tfcall($n, vpiFuncCall); }
	;

system_function_call
	: system_function_identifier ( '(' expression ( ',' expression )* ')' )?			{ $$ = parsenode_to_tfcall($n, vpiSysFuncCall); }
	;

abstol_expression
	: constant_expression
	| nature_identifier
	;

analog_conditional_expression
	: analog_expression '?' ( attribute_instance )* analog_expression ':' analog_expression		{ $$ = parsenode_to_operation($n, vpiConditionOp); }
	;

analog_range_expression
	: analog_expression										{ $$ = parsenode_to_obj_1prop($n, vpiExpr, vpiVarSelect, vpiIndex, $n0); }
	| msb_constant_expression ':' lsb_constant_expression						{ $$ = parsenode_to_obj_2props($n, vpiExpr, vpiPartSelect, vpiLeftRange, $n0, vpiRightRange, $n2); }
	;

analog_expression_or_null
	: ( analog_expression )?									{ if (parsenode_find_userdata($n0) == NULL) $$ = parsenode_to_operation($n, vpiNullOp); }
	;

analog_expression
	: analog_primary
	| unary_operator ( attribute_instance )* analog_primary						{ $$ = parsenode_to_operation($n, vpiUndefined); }
	| analog_expression binary_operator ( attribute_instance )* analog_expression			{ $$ = parsenode_to_operation($n, vpiUndefined); }
	| analog_conditional_expression
	| string
	;

base_expression
	: expression
	;

conditional_expression
	: expression1 '?' ( attribute_instance )* expression2 ':' expression3				{ $$ = parsenode_to_operation($n, vpiConditionOp); }
	;

constant_base_expression
	: constant_expression
	;

constant_expression_or_null
	: ( constant_expression )?									{ if (parsenode_find_userdata($n0) == NULL) $$ = parsenode_to_operation($n, vpiNullOp); }
	;

constant_expression
	: constant_primary
	| unary_operator ( attribute_instance )* constant_primary					{ $$ = parsenode_to_operation($n, vpiUndefined); }
	| constant_expression binary_operator ( attribute_instance )* constant_expression		{ $$ = parsenode_to_operation($n, vpiUndefined); }
	| constant_expression '?' ( attribute_instance )* constant_expression ':' constant_expression	{ $$ = parsenode_to_operation($n, vpiConditionOp); }
	;

constant_mintypmax_expression
	: constant_expression
	| constant_expression ':' constant_expression ':' constant_expression				{ $$ = parsenode_to_operation($n, vpiMinTypMaxOp); }
	;

constant_range_expression
	: constant_expression										{ $$ = parsenode_to_obj_1prop($n, vpiExpr, vpiVarSelect, vpiIndex, $n0); }
	| msb_constant_expression ':' lsb_constant_expression						{ $$ = parsenode_to_obj_2props($n, vpiExpr, vpiPartSelect, vpiLeftRange, $n0, vpiRightRange, $n2); }
	| constant_base_expression '+:' width_constant_expression					{ $$ = parsenode_to_indexed_partselect($n, $n0, $n2, vpiPosIndexed); }
	| constant_base_expression '-:' width_constant_expression					{ $$ = parsenode_to_indexed_partselect($n, $n0, $n2, vpiNegIndexed); }
	;

dimension_constant_expression
	: constant_expression
	;

expression
	: primary
	| unary_operator ( attribute_instance )* primary						{ $$ = parsenode_to_operation($n, vpiUndefined); }
	| expression binary_operator ( attribute_instance )* expression					{ $$ = parsenode_to_operation($n, vpiUndefined); }
	| conditional_expression
	;

expression1 : expression ;
expression2 : expression ;
expression3 : expression ;

indirect_expression
	: branch_probe_function_call
	| port_probe_function_call
	| 'ddt' '(' branch_probe_function_call ( ',' abstol_expression )? ')'
	| 'ddt' '(' port_probe_function_call ( ',' abstol_expression )? ')'
	| 'idt' '(' branch_probe_function_call ( ',' analog_expression ( ',' analog_expression ( ',' abstol_expression )? )? )? ')'
	| 'idt' '(' port_probe_function_call ( ',' analog_expression ( ',' analog_expression ( ',' abstol_expression )? )? )? ')'
	| 'idtmod' '(' branch_probe_function_call ( ',' analog_expression ( ',' analog_expression ( ',' analog_expression ( ',' abstol_expression )? )? )? )? ')'
	| 'idtmod' '(' port_probe_function_call ( ',' analog_expression ( ',' analog_expression ( ',' analog_expression ( ',' abstol_expression )? )? )? )? ')'
	;

lsb_constant_expression
	: constant_expression
	;

mintypmax_expression
	: expression
	| expression ':' expression ':' expression							{ $$ = parsenode_to_operation($n, vpiMinTypMaxOp); }
	;

module_path_conditional_expression
	: module_path_expression '?' ( attribute_instance )* module_path_expression ':' module_path_expression
													{ $$ = parsenode_to_operation($n, vpiConditionOp); }
	;

module_path_expression
	: module_path_primary
	| unary_module_path_operator ( attribute_instance )* module_path_primary
	| module_path_expression binary_module_path_operator ( attribute_instance )* module_path_expression
	| module_path_conditional_expression
	;

module_path_mintypmax_expression
	: module_path_expression									{ $$ = parsenode_to_operation($n, vpiMinTypMaxOp); }
	| module_path_expression ':' module_path_expression ':' module_path_expression			{ $$ = parsenode_to_operation($n, vpiMinTypMaxOp); }
	;

msb_constant_expression
	: constant_expression
	;

nature_attribute_expression
	: constant_expression
	| nature_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| nature_access_identifier									{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	;

range_expression
	: expression											{ $$ = parsenode_to_obj_1prop($n, vpiExpr, vpiVarSelect, vpiIndex, $n0); }
	| msb_constant_expression ':' lsb_constant_expression						{ $$ = parsenode_to_obj_2props($n, vpiExpr, vpiPartSelect, vpiLeftRange, $n0, vpiRightRange, $n2); }
	| base_expression '+:' width_constant_expression						{ $$ = parsenode_to_indexed_partselect($n, $n0, $n2, vpiPosIndexed); }
	| base_expression '-:' width_constant_expression						{ $$ = parsenode_to_indexed_partselect($n, $n0, $n2, vpiNegIndexed); }
	;

width_constant_expression
	: constant_expression
	;

analog_primary
	: constant_primary
	| number
	| analog_concatenation
	| analog_multiple_concatenation
	| variable_reference
	| net_reference
	| genvar_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| parameter_reference
	| nature_attribute_reference
	| branch_probe_function_call
	| port_probe_function_call
	| analog_function_call
	| analog_system_function_call
	| analog_built_in_function_call 
	| analog_filter_function_call
	| analog_small_signal_function_call 
	| '(' analog_expression ')'
	;

constant_primary
	: number
	| parameter_identifier ( '[' constant_range_expression ']' )?					{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| specparam_identifier ( '[' constant_range_expression ']' )?					{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| constant_concatenation
	| constant_multiple_concatenation
	| constant_function_call
	| constant_system_function_call
	| constant_analog_built_in_function_call 
	| '(' constant_mintypmax_expression ')'
	| string
	| system_parameter_identifier									{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| analysis_function_call
	| nature_attribute_reference
	| constant_analog_function_call
	;

module_path_primary
	: number
	| identifier											{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| module_path_concatenation
	| module_path_multiple_concatenation
	| function_call
	| system_function_call
	| '(' module_path_mintypmax_expression ')'
	;

primary
	: constant_primary
	| number
	| hierarchical_identifier ( ( '[' expression ']' )* '[' range_expression ']' )?			{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| concatenation
	| multiple_concatenation
	| function_call
	| system_function_call
	| '(' mintypmax_expression ')'
	| string
	| branch_probe_function_call
	| port_probe_function_call
	;

analog_variable_lvalue
	: variable_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| variable_identifier '[' analog_expression ']' ( '[' analog_expression ']' )*			{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

branch_lvalue
	: branch_probe_function_call
	;

net_lvalue
	: hierarchical_net_identifier ( ( '[' constant_expression ']' )* '[' constant_range_expression ']' )?
													{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| '{' net_lvalue ( ',' net_lvalue )* '}'							{ $$ = parsenode_to_operation($n, vpiConcatOp); }
	;

variable_lvalue
	: hierarchical_variable_identifier ( ( '[' expression ']' )* '[' range_expression ']' )?	{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| '{' variable_lvalue ( ',' variable_lvalue )* '}'						{ $$ = parsenode_to_operation($n, vpiConcatOp); }
	;

unary_operator
	: '+'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiPlusOp); }
	| '-'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiMinusOp); } 
	| '!'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiNotOp); }
	| '~'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitNegOp); }
	| '&'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryAndOp); }
	| '~&' $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryNandOp); }
	| '|'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryOrOp); }
	| '~|' $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryNorOp); }
	| '^'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryXorOp); }
	| '~^' $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryXNorOp); }
	| '^~' $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryXNorOp); }
	;

binary_operator
	: '**'  $binary_op_left 11									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiPowerOp); }
	| '*'   $binary_op_left 10									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiMultOp); }
	| '/'   $binary_op_left 10									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiDivOp); }
	| '%'   $binary_op_left 10									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiModOp); }
	| '+'   $binary_op_left 9									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiAddOp); }
	| '-'   $binary_op_left 9									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiSubOp); }
	| '>>'  $binary_op_left 8									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiRShiftOp); }
	| '<<'  $binary_op_left 8									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiLShiftOp); }
	| '>>>' $binary_op_left 8									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiArithRShiftOp); }
	| '<<<' $binary_op_left 8									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiArithLShiftOp); }
	| '<'   $binary_op_left 7									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiLtOp); }
	| '<='  $binary_op_left 7									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiLeOp); } 
	| '>'   $binary_op_left 7									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiGtOp); }
	| '>='  $binary_op_left 7									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiGeOp); }
	| '=='  $binary_op_left 6									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiEqOp); }
	| '!='  $binary_op_left 6									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiNeqOp); }
	| '===' $binary_op_left 6									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiCaseEqOp); }
	| '!==' $binary_op_left 6									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiCaseNeqOp); }
	| '&'   $binary_op_left 5									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitAndOp); }
	| '^'   $binary_op_left 4									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitXorOp); }
	| '^~'  $binary_op_left 4									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitXnorOp); }
	| '~^'  $binary_op_left 4									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitXNorOp); }
	| '|'   $binary_op_left 3									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitOrOp); }
	| '&&'  $binary_op_left 2									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiLogAndOp); }
	| '||'  $binary_op_left 1									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiLogOrOp); }
	;

unary_module_path_operator
	: '!'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiNotOp); }
	| '~'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitNegOp); }
	| '&'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryAndOp); }
	| '~&' $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryNandOp); }
	| '|'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryOrOp); }
	| '~|' $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryNorOp); }
	| '^'  $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryXorOp); }
	| '~^' $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryXNorOp); }
	| '^~' $unary_op_right 20									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiUnaryXNorOp); }
	;

binary_module_path_operator
	: '=='  $binary_op_left 6									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiEqOp); }
	| '!='  $binary_op_left 6									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiNeqOp); }
	| '&'   $binary_op_left 5									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitAndOp); }
	| '^'   $binary_op_left 4									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitXorOp); }
	| '^~'  $binary_op_left 4									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitXnorOp); }
	| '~^'  $binary_op_left 4									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitXNorOp); }
	| '|'   $binary_op_left 3									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiBitOrOp); }
	| '&&'  $binary_op_left 2									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiLogAndOp); }
	| '||'  $binary_op_left 1									{ $$ = xvpi_object_new(vpiOpType, xvpiIntProp, vpiLogOrOp); }
	;

number
	: decimal_number										{ $$ = parsenode_to_constant($n, vpiDecConst); }
	| octal_number											{ $$ = parsenode_to_constant($n, vpiOctConst); }
	| binary_number											{ $$ = parsenode_to_constant($n, vpiBinaryConst); }
	| hex_number											{ $$ = parsenode_to_constant($n, vpiHexConst); }
	| real_number											{ $$ = parsenode_to_constant($n, vpiRealConst); }
	;

real_number :
	( unsigned_number '.' unsigned_number
	| unsigned_number ( '.' unsigned_number )? exp ( sign )? unsigned_number
	| unsigned_number ( '.' unsigned_number )? scale_factor
	) [
		if (parsenode_contains_whitespace($n))
			${reject};
	] ;

exp : 'e' | 'E' ;

scale_factor : 'T' | 'G' | 'M' | 'K' | 'k' | 'm' | 'u' | 'n' | 'p' | 'f' | 'a' ;

decimal_number
	: unsigned_number
	| ( size )? decimal_base unsigned_number
	| ( size )? decimal_base x_digit ( '_' )*
	| ( size )? decimal_base z_digit ( '_' )*
	;

binary_number : ( size )? binary_base binary_value ;

octal_number : ( size )? octal_base octal_value ;

hex_number : ( size )? hex_base hex_value ;

sign : '+' | '-' ;

size : non_zero_unsigned_number ;

non_zero_unsigned_number
	: non_zero_decimal_digit ( '_' | decimal_digit )*						[ if (parsenode_contains_whitespace($n)) ${reject}; ]
	;

unsigned_number
	: decimal_digit ( '_' | decimal_digit )*							[ if (parsenode_contains_whitespace($n)) ${reject}; ]
	;

binary_value
	: binary_digit ( '_' | binary_digit )*								[ if (parsenode_contains_whitespace($n)) ${reject}; ]
	;

octal_value
	: octal_digit ( '_' | octal_digit )*								[ if (parsenode_contains_whitespace($n)) ${reject}; ]
	;

hex_value
	: hex_digit ( '_' | hex_digit )*								[ if (parsenode_contains_whitespace($n)) ${reject}; ]
	;

decimal_base : "'[sS]?[dD]" ;

binary_base : "'[sS]?[bB]" ;

octal_base : "'[sS]?[oO]" ;

hex_base : "'[sS]?[hH]" ;

non_zero_decimal_digit : "[1-9]" ;

decimal_digit : "[0-9]" ;

binary_digit : x_digit | z_digit | '0' | '1' ;

octal_digit : x_digit | z_digit | "[0-7]" ;

hex_digit : x_digit | z_digit | "[0-9a-fA-F]" ;

x_digit : 'x' | 'X' ;

z_digit : 'z' | 'Z' | '?' ;

string
	: ( "\"([^\"\\]|\\[^])*\"" )+									{ $$ = parsenode_to_constant($n, vpiStringConst); }
	;

nature_attribute_reference
	: net_identifier '.' potential_or_flow '.' nature_attribute_identifier				{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	;

analog_port_reference
	: port_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| port_identifier '[' constant_expression ']'							{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

analog_net_reference
	: port_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| port_identifier '[' constant_expression ']'							{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| net_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| net_identifier '[' constant_expression ']'							{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

branch_reference
	: hierarchical_branch_identifier								{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| hierarchical_branch_identifier '[' constant_expression ']'					{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

parameter_reference
	: parameter_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| parameter_identifier '[' analog_expression ']'						{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

variable_reference
	: variable_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| variable_identifier '[' analog_expression ']' ( '[' analog_expression ']' )*			{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| real_identifier										{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| real_identifier '[' analog_expression ']' ( '[' analog_expression ']' )*			{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

net_reference
	: hierarchical_net_identifier									{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| hierarchical_net_identifier '[' analog_range_expression ']'					{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	| hierarchical_port_identifier									{ $$ = parsenode_to_xvpi_object_ref($n0, vpiExpr); }
	| hierarchical_port_identifier '[' analog_range_expression ']'					{ $$ = parsenode_to_ref($n, parsenode_to_xvpi_object_ref($n0, vpiExpr)); }
	;

attribute_instance :
	'(*' attr_spec ( ',' attr_spec )* '*)'
	;

attr_spec :
	attr_name ( '=' constant_expression )?
	{
		$$ = parsenode_to_obj_1prop($n, vpiAttribute, vpiAttribute, vpiExpr, $n1);
		xvpi_object_add_child($$, parsenode_to_xvpi_object_string($n0, vpiName));
	}
	;

attr_name : identifier ;

/*      *** handled by implicit whitespace subparser...
comment :
	  one_line_comment
	| block_comment ;

one_line_comment : '//' comment_text '\n' ;

block_comment : '/*' comment_text '*/' ;

comment_text : "[^]*" ;		/* Any_ASCII_character */
*/

analog_block_identifier : block_identifier ;

analog_function_identifier : identifier ;

analog_system_task_identifier : system_task_identifier ;

analog_system_function_identifier : system_function_identifier ;

analysis_identifier : identifier ;

block_identifier : identifier ;

branch_identifier : identifier ;

cell_identifier : identifier ;

config_identifier : identifier ;

connectmodule_identifier : module_identifier ;

connectrules_identifier : identifier ;

discipline_identifier : identifier ;

escaped_identifier : "\\[^ \t\n\r]+" ;	/*  \ {Any_ASCII_character_except_white_space} white_space  */

event_identifier : identifier ;

function_identifier : identifier ;

gate_instance_identifier : identifier ;

generate_block_identifier : identifier ;

genvar_identifier : identifier ;

hierarchical_block_identifier : hierarchical_identifier ;

hierarchical_branch_identifier : hierarchical_identifier ;

hierarchical_event_identifier : hierarchical_identifier ;

hierarchical_function_identifier : hierarchical_identifier ;

hierarchical_identifier : ( identifier ( '[' constant_expression ']' )? '.' )* identifier ;

hierarchical_net_identifier : hierarchical_identifier ;

hierarchical_parameter_identifier : hierarchical_identifier ;

hierarchial_port_identifier : hierarchical_identifier ;

hierarchical_variable_identifier : hierarchical_identifier ;

hierarchical_task_identifier : hierarchical_identifier ;

identifier
	: simple_identifier 
	| escaped_identifier
	;

inout_port_identifier : identifier ;

input_port_identifier : identifier ;

instance_identifier : identifier ;

library_identifier : identifier ;

module_identifier : identifier ;

module_instance_identifier : identifier ;

module_or_paramset_identifier
	: module_identifier
	| paramset_identifier
	;

module_output_variable_identifier : identifier ;

module_parameter_identifier : identifier ;

nature_identifier : identifier ;

nature_attribute_identifier : 'abstol' | 'access' | 'ddt_nature' | 'idt_nature' | 'units' | identifier ;

net_identifier : identifier ;

output_port_identifier : identifier ;

parameter_identifier : identifier ;

paramset_identifier : identifier ;

port_identifier : identifier ;

real_identifier : identifier ;

simple_identifier
	: "[a-zA-Z_][a-zA-Z0-9_\$]*"									[ if (is_keyword($n0.start_loc.s, $n0.end, current_language)) ${reject}; ]
	;

specparam_identifier : identifier ;

system_function_identifier : "\$[a-zA-Z_][a-zA-Z0-9_\$]*" ;

system_parameter_identifier : "\$[a-zA-Z_][a-zA-Z0-9_\$]*" ;

system_task_identifier : "\$[a-zA-Z_][a-zA-Z0-9_\$]*" ;

task_identifier : identifier ;

terminal_identifier : identifier ;

topmodule_identifier : identifier ;

udp_identifier : identifier ;

udp_instance_identifier : identifier ;

variable_identifier : identifier ;


/*** not covered by the spec, but required: ****/

nature_access_identifier : identifier ;

hierarchical_port_identifier : hierarchical_identifier ;


/** compiler directives **/

compiler_directive
	: '`begin_keywords' string									[ if (begin_keywords($n1.start_loc.s, $n1.end) != 0) ${reject}; ] { xvpi_object_unref($1); }
	| '`celldefine'											{ $g->cell_instance = 1; }
	| '`default_discipline' ( identifier ( qualifier )? )?
	| '`default_nettype' ( net_type | 'none' )							{ $g->default_nettype = parsenode_to_nettype($n1); }
	| '`default_transition' constant_expression
	| '`end_keywords'										{ end_keywords(); }
	| '`endcelldefine'										{ $g->cell_instance = 0; }
	| '`line' ${scan scan_lineno_filename}
	| '`nounconnected_drive'									{ $g->unconnected_drive = vpiHighZ; }
	| '`pragma' "[^\n]"
	| '`resetall'											{ reset_globals($g); }
	| '`timescale' time_unit '/' time_precision
	| '`unconnected_drive' ( strength0 | strength1 )						{ $g->unconnected_drive = parsenode_to_strengthval($n1); }
	| '`default_decay_time' ( unsigned_number | real_number | 'infinite' )				{ $g->default_decay_time = parsenode_to_double($n1); }
	;

qualifier
	: 'integer' | 'real' | 'reg' | 'wreal' | 'wire' | 'tri' | 'wand' | 'triand'
	| 'wor' | 'trior' | 'trireg' | 'tri0' | 'tri1' | 'supply0' | 'supply1'
	;

time_unit
	: "1[0]?[0]?" "[munpf]?s"
	{
		char *s = $n0.start_loc.s + 1;
		$g->time_unit = 0;
		while (*s == '0') {
			$g->time_unit++;
			s++;
		}
		switch ($n1.start_loc.s[0]) {
		case 'm': $g->time_unit -= 3; break;
		case 'u': $g->time_unit -= 6; break;
		case 'n': $g->time_unit -= 9; break;
		case 'p': $g->time_unit -= 12; break;
		case 'f': $g->time_unit -= 15; break;
		default: ;
		}
	} ;

time_precision
	: "1[0]?[0]?" "[munpf]?s"
	{
		char *s = $n0.start_loc.s + 1;
		$g->time_precision = 0;
		while (*s == '0') {
			$g->time_precision++;
			s++;
		}
		switch ($n1.start_loc.s[0]) {
		case 'm': $g->time_precision -= 3; break;
		case 'u': $g->time_precision -= 6; break;
		case 'n': $g->time_precision -= 9; break;
		case 'p': $g->time_precision -= 12; break;
		case 'f': $g->time_precision -= 15; break;
		default: ;
		}
	} ;


/** language subset marked as 'obsolete' in LRM **/

generate_statement : 
	'generate' identifier '(' start_expr ',' end_expr ( ',' incr_expr )* ')' analog_statement ;  /** modified from 'statement' to 'analog_statement' **/

start_expr : constant_expression ;

end_expr : constant_expression ;

incr_expr : constant_expression ;


/****** should get into file vams-library-grammar.g
library_text : ( library_description )* ;

library_description :
	  library_declaration
	| include_statement
	| config_declaration ;

library_declaration : 'library' identifier file_path_spec ( ( ',' file_path_spec )* )?
	  ( '-incdir' file_path_spec ( ',' file_path_spec )* )? ';' ;

include_statement : 'include' file_path_spec ';' ;

config_declaration : 'config' identifier ';' design_statement ( config_rule_statement )* 'endconfig' ;

design_statement : 'design' ( ( identifier '.' )? identifier )* ';' ;

config_rule_statement :
	  default_clause liblist_clause ';'
	| inst_clause liblist_clause ';'
	| inst_clause use_clause ';'
	| cell_clause liblist_clause ';'
	| cell_clause use_clause ';' ;

default_clause : 'default' ;

inst_clause : 'instance' inst_name ;

inst_name : identifier ( '.' identifier )* ;

cell_clause : 'cell' ( identifier '.' )? identifier ;

liblist_clause : 'liblist' ( identifier )* ;

use_clause : 'use' ( identifier '.' )? identifier ( ':' 'config' )? ;

file_path_spec : string ;

string : "\"[^\"\n]*\"" ;

identifier :
	  simple_identifier 
	| escaped_identifier ;

escaped_identifier : "\\[^ \t\n\r]+" ;

simple_identifier : "[a-zA-Z_][a-zA-Z0-9_\$]*"
	[
		if (is_keyword($n0.start_loc.s, $n0.end, current_language))
			${reject};
	] ;

*/


