#include <stdlib.h>
#include <stdio.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

typedef enum{
    true,false
} bool;

struct User {
    int id;
    char *name;
    char *username;
    char *password;
    bool status;
    char *created_at;
    bool is_admin;
};

int main(int argc, char** argv) {
    GError *error = NULL;

    if (argc < 2) {
        printf("Usage: test <filename.json>\n");
        return EXIT_FAILURE;
    }

    // Parse the JSON from the file
    JsonParser *parser = json_parser_new();
    json_parser_load_from_file(parser, argv[1], &error); //load json file
    if(error) {
        printf("Unable to parse `%s': %s\n", argv[1], error->message);
        g_error_free(error);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }

    // Get the root
    JsonNode *root = json_parser_get_root(parser);
    // Turn the root into a JSON object
    
    JsonObject *stuff = json_node_get_object(root);

    // Get each object member and assign it to the struct.
    struct User user = {
        .id =  json_object_get_int_member(stuff, "id"),
        .name = g_strdup(json_object_get_string_member(stuff, "name")),
        .username = g_strdup(json_object_get_string_member(stuff, "username")),
        .password = g_strdup(json_object_get_string_member(stuff, "password")),

        // Duplicate the strings to avoid pointing to memory inside the parser.
        .status = json_object_get_int_member(stuff, "status"),
        .created_at = g_strdup(json_object_get_string_member(stuff, "created_at")),
        .is_admin = json_object_get_int_member(stuff, "is_admin")

    };

    printf(
        " \nid = %d, name = %s,  username = %s, password = %s, status = %d, created_at = %s \n",
        user.id, user.name, user.username, user.password, user.status, user.created_at
    );

    // We're finished working with the parser. Deallocate the
    // parser and all the memory it has allocated, including
    // the nodes.
    g_object_unref(parser);

    return EXIT_SUCCESS;
}