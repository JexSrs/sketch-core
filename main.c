#include <argument_parser.h>
#include <file_handler.h>


int main(int argc, char **argv) {
    char *config =  "config.conf";
    char *sketch =  "sketch/";
    char *session = "sessions/";

    check_requirements(config, sketch, session);
    parse(argc, argv);
    return 0;
}
