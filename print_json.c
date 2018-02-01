#include <stdio.h>
#include <stdlib.h>
#include <json-glib/json-glib.h>
#include "ast.h"

static const char *get_op(int op) {
  switch (op) {
    case AST_ADD: return "add";
    case AST_SUB: return "sub";
    case AST_MUL: return "mul";
    case AST_DIV: return "div";
    case AST_GT: return "gt";
    case AST_LT: return "lt";
    case AST_EQ: return "eq";
    case AST_NE: return "ne";
    case AST_ASSIGN: return "assign";
    case AST_INT_CONSTANT: return "int_constant";
    case AST_FLOAT_CONSTANT: return "float_constant";
    case AST_IDENTIFIER: return "identifier";
    default:
      fprintf(stderr, "Uknown operator: %d\n", op);
      exit(1);
  }
}

static void
dump_node(JsonBuilder *builder, struct ast_node *node);

static void
dump_statement_list(JsonBuilder *builder, struct ast_statement_list *list) {
  json_builder_begin_object(builder);
  json_builder_set_member_name(builder, "ast_statement_list");

  json_builder_begin_array(builder);
  for (int i = 0; i < list->number_of_statements; i++)
    dump_node(builder, list->statement[i]);
  json_builder_end_array(builder);

  json_builder_end_object(builder);
}

static void
dump_declaration(JsonBuilder *builder, struct ast_declaration *decl) {
  json_builder_begin_object(builder);

  json_builder_set_member_name(builder, "ast_declaration");
  json_builder_begin_object(builder);

  json_builder_set_member_name(builder, "identifier");
  json_builder_add_string_value(builder, decl->identifier);

  json_builder_set_member_name(builder, "type");
  json_builder_add_string_value(builder,
        decl->type_specifier == TYPE_INT ? "int" : "float");

  json_builder_end_object(builder);

  json_builder_end_object(builder);
}

static void
dump_expression(JsonBuilder *builder, struct ast_expression *expression) {
  json_builder_begin_object(builder);

  json_builder_set_member_name(builder, "ast_expression");
  json_builder_begin_object(builder);

  json_builder_set_member_name(builder, "type");
  json_builder_add_string_value(builder, get_op(expression->operator));

  switch (expression->operator) {
    case AST_ADD: case AST_SUB: case AST_MUL: case AST_DIV:
    case AST_GT: case AST_LT: case AST_EQ: case AST_NE:
      json_builder_set_member_name(builder, "expressions");
      json_builder_begin_array(builder);
      dump_expression(builder, expression->subexpr[0]);
      dump_expression(builder, expression->subexpr[1]);
      json_builder_end_array(builder);
      break;

    case AST_ASSIGN:
      json_builder_set_member_name(builder, "identifier");
      json_builder_add_string_value(builder, expression->primary_expr.identifier);

      json_builder_set_member_name(builder, "expression");
      dump_expression(builder, expression->subexpr[0]);
      break;

    case AST_INT_CONSTANT:
      json_builder_set_member_name(builder, "value");
      json_builder_add_int_value(builder, expression->primary_expr.int_constant);
      break;

    case AST_FLOAT_CONSTANT:
      json_builder_set_member_name(builder, "value");
      json_builder_add_double_value(builder, expression->primary_expr.float_constant);
      break;

    case AST_IDENTIFIER:
      json_builder_set_member_name(builder, "value");
      json_builder_add_string_value(builder, expression->primary_expr.identifier);
      break;

    default:
      fprintf(stderr, "Unknown expression tag: %d\n", expression->tag);
  }

  json_builder_end_object(builder);
  json_builder_end_object(builder);

  return;
}

static void
dump_compound_statement(JsonBuilder *builder,
                        struct ast_compound_statement *compound_statement) {
  json_builder_begin_object(builder);

  json_builder_set_member_name(builder, "ast_compound_statement");
  dump_statement_list(builder, compound_statement->statement_list);

  json_builder_end_object(builder);


  return;
}

static void
dump_selection_statement(JsonBuilder *builder,
                        struct ast_selection_statement *selection_statement) {
  json_builder_begin_object(builder);
  json_builder_set_member_name(builder, "ast_selection_statement");
  json_builder_begin_object(builder);
  json_builder_set_member_name(builder, "condition");
  dump_expression(builder, selection_statement->condition);

  json_builder_set_member_name(builder, "then_body");
  dump_compound_statement(builder, selection_statement->then_body);

  if (selection_statement->else_body) {
    json_builder_set_member_name(builder, "else_body");
    dump_compound_statement(builder, selection_statement->then_body);
  }

  json_builder_end_object(builder);
  json_builder_end_object(builder);
}

static void
dump_while_statement(JsonBuilder *builder,
                     struct ast_while_statement *while_statement) {
  json_builder_begin_object(builder);
  json_builder_set_member_name(builder, "ast_while_statement");
  json_builder_begin_object(builder);
  json_builder_set_member_name(builder, "condition");
  dump_expression(builder, while_statement->condition);

  json_builder_set_member_name(builder, "body");
  dump_compound_statement(builder, while_statement->body);
  json_builder_end_object(builder);
  json_builder_end_object(builder);
}

static void
dump_translation_unit(JsonBuilder *builder, struct ast_translation_unit *translation_unit) {
  json_builder_begin_object(builder);

  json_builder_set_member_name(builder, "ast_translation_unit");
  dump_statement_list(builder, translation_unit->statement_list);

  json_builder_end_object(builder);
}

static void
dump_node(JsonBuilder *builder, struct ast_node *node) {
  int tag = node->tag;
  void *nodep = node;

  switch (tag) {
    case AST_NODE:
      fprintf(stderr, "Request to dump JSON for generic node.\n");
      exit(EXIT_FAILURE);

    case AST_DECLARATION:
      dump_declaration(builder, nodep);
      break;

    case AST_EXPRESSION:
      dump_expression(builder, nodep);
      break;

    case AST_STATEMENT_LIST:
      dump_statement_list(builder, nodep);
      break;

    case AST_COMPOUND_STATEMENT:
      dump_compound_statement(builder, nodep);
      break;

    case AST_SELECTION_STATEMENT:
      dump_selection_statement(builder, nodep);
      break;

    case AST_WHILE_STATEMENT:
      dump_while_statement(builder, nodep);
      break;

    case AST_TRANSLATION_UNIT:
      dump_translation_unit(builder,  nodep);
      break;

    default:
      fprintf(stderr, "Request to dump unknown node type: %d\n", tag);
  }
}

void
dump_ast(struct ast_node *node) {
  JsonBuilder *builder = json_builder_new();
  dump_node(builder, node);

  JsonGenerator *gen = json_generator_new();
  JsonNode * root = json_builder_get_root(builder);
  json_generator_set_root(gen, root);
  gchar *str = json_generator_to_data(gen, NULL);

  g_print("%s\n", str);
}
