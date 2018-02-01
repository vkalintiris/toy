#include <stdio.h>
#include <stdlib.h>
#include <json-glib/json-glib.h>
#include "ast.h"

static void
dump_statement_list(JsonBuilder *builder, struct ast_statement_list *list) {
  json_builder_begin_object(builder);

  json_builder_set_member_name(builder, "ast_statement_list");
  json_builder_add_string_value(builder, "http://www.gnome.org/img/flash/two-thirty.png");

  json_builder_end_object(builder);
}

void
dump_ast_json(struct ast_translation_unit *translation_unit) {
  JsonBuilder *builder = json_builder_new ();

  json_builder_begin_object(builder);

  json_builder_set_member_name (builder, "ast_translation_unit");
  dump_statement_list(builder, translation_unit->statement_list);

  json_builder_end_object (builder);

  JsonGenerator *gen = json_generator_new();
  JsonNode * root = json_builder_get_root(builder);
  json_generator_set_root(gen, root);
  gchar *str = json_generator_to_data(gen, NULL);

  g_print("str: %s", str);
}
