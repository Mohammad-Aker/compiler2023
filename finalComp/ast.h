#ifndef __AST__H
#define __AST__H
//ast.h
#include "stable.h"
/* Definitions of list datatypes */
/* List of AST nodes */

typedef struct ast_list_cell 
{
    struct ast_node *head; // pointer to  a tree
    struct ast_list_cell *tail;  // this is the next pointer that you use typically in the list
} ast_list;

/* List of symbol table entries */
typedef struct ste_list_cell
{
    STEntry *head; // pointer to a symbol table entry
    struct ste_list_cell *tail; //  this is the next pointer that you use typically in the list
} ste_list;

////////////////////////////////////////////////////////////
/* Type of an AST node. */
typedef enum
{
	ast_var_decl,		 /* Top-level variable declaration */
	ast_const_decl,		 /* II constant declaration */
	ast_routine_decl,	 /* II procedure or function decl. */
	ast_assign,			 /* Assignment statement */
	ast_if,				 /* If-then-else statement */
	ast_while,			 /* While statement */
	ast_for,			 /* For statement */
	ast_read,			 /* Read statement */
	ast_write,			 /* Write statement */
	ast_call,			 /* Procedure call */
	ast_block,			 /* Begin-end block */
	ast_return,			 /* Return statement */
	ast_var,			 /* Variable reference */
	ast_integer,		 /* Integer literal */
	ast_string,			 /* String literal */
	ast_boolean,		 /* Boolean literal */
	ast_times,			 /*  operator */
	ast_divide,			/* / operator */
	ast_plus,			 /* + operator */
	ast_minus,			 /* -operator */
	ast_eq,				/* = operator */
	ast_neq,			/* != operator */

	ast_lt, /* < operator */
	ast_le, /* <= operator */
	ast_gt, /* > operator */
	ast_ge, /* >= operator */
	ast_and, /* and operator */
	ast_or, /* or operator */
	ast_cand, /* cand operator */
	ast_cor, /* cor operator */
	ast_not, /* not operator */
	ast_uminus, /* unary -operator */
	ast_eof, /* End of File */
	ast_float, /* float */
	ast_itof, /* convert integr to float */
    ast_none
}AST_type;

///////////////////////////////////////////////////////////////////////
/* The actual datatype for an abstract syntax tree (AST) node. */
typedef struct ast_node
{
 AST_type      type; /* Type of the AST node */
 union /* The fields depend on a node's type */
	{
	 struct{
            STEntry *name; /* Variable's symbol table entry */
			STE_TYPE type; /* Declared type of variable */
	 } a_var_decl;
	
	 struct{
            STEntry *name; /* Constant's symbol table entry */
			int value; /* Evaluated value of constant */
	 } a_const_decl;
	
	 struct{
			STEntry *name; /* Routine's symbol table entry */
			ste_list *formals; /* List of formal parameters */
			STE_TYPE result_type; /* Type of result (none_type for procedures) */
			struct ast_node *body; /* Body of routine */
	} a_routine_decl;
	
	 struct{
		     STEntry *lhs; /* Target of assignment */
			 struct ast_node *rhs; /* Right side of assignment */
	} a_assign;
	
	 struct{
			struct ast_node *predicate; /* Predicate */
			struct ast_node *conseq; /* Consequent */
			struct ast_node *altern; /* Alternative (NULL for if-then stmt) */
	} a_if;
	
	struct{
			struct ast_node *predicate; /* Predicate */
			struct ast_node *body; /* Body */
	} a_while;
	
	struct {
			STEntry *var; /* Index variable */
			struct ast_node *lower_bound; /* Lower iteration bound */
			struct ast_node *upper_bound; /* Upper iteration bound */
			struct ast_node *body; /* Body */
	}a_for;
	
	struct{
			STEntry *var; /* Target variable */
	} a_read;

	struct{
				STEntry *var; /* Source variable */
	} a_write;

	struct{
			STEntry *callee; /* Symbol table entry for function */
			ast_list *arg_list; /* List of actual arguments */
	} a_call;
  
	struct{
          ste_list *vars; /* Symbol table entries of local variables */
		  ast_list *stmts; /* Statements in block */
	} a_block;

	struct{
		struct ast_node *expr; /* Return value */
	} a_return;

	struct{
		STEntry *var; /* Symbol table entry for variable */
	} a_var;

	struct{
		int value; /* Integer value */
	} a_integer;

	struct{
		float value;
	} a_float;

	struct{
		char *string; /* String value */
	} a_string;

	struct{
		int value; /* Boolean value */
	}a_boolean;

	struct{ 
		struct ast_node *arg; /* Argument */
		AST_type type;
	} a_unary_op;

	struct{
		struct ast_node *larg; /* Argument 1 */
		struct ast_node *rarg; /* Argument 2 */
        AST_type type;
		//STE_TYPE rel_type;
	} a_binary_op;

	struct {
		struct ast_node *arg;
	} a_itof; 
  
 } f;  // union 
} AST; // ASt structure

 /* Externally-visible functions: */
    ast_list *cons_ast (AST *, ast_list *);
    ste_list *cons_ste (STEntry *, ste_list *);
    int eval_ast_expr (FileDescriptor *, AST *);
    AST *make_ast_node (...);
    void print_ast_node (FILE *, AST *);
    static void print_ast_list (FILE *f, ast_list *L, char *sep, int d);
    static void print_ste_list (FILE *f, ste_list *L, char *prefix, char *sep, int d);
    static void nl_indent (FILE *f, int d);
#endif




