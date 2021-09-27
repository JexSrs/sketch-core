#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include "include/functions.h"
#include <include/codes.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

int endsWith(const char *str, const char *suffix) {
    if (!str || !suffix)
        return FALSE;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return FALSE;

    if (strncmp(str + lenstr - lensuffix, suffix, lensuffix) != 0)
        return FALSE;

    return TRUE;
}

int startsWith(const char *str, const char *pre) {
    size_t lenpre = strlen(pre),
            lenstr = strlen(str);
    return lenstr < lenpre ? FALSE : memcmp(pre, str, lenpre) == 0;
}

char *str_add(const char *str1, ...) {
    va_list args;
    va_start(args, str1);

    char *result = calloc(strlen(str1) + 1, sizeof(char));
    strcpy(result, str1);

    char *p = "";

    while (1) {
        p = va_arg(args, char *);

        if (p == NULL) break;

        result = realloc(result, (strlen(result) + strlen(p) + 1) * sizeof(char));
        strcat(result, p);
    }
    va_end(args);

    return result;
}

char *get_home_path() {
    char *result = str_add("/home/", getlogin(), "/", NULL);

    return result;
}

char *fix_path(const char *path, int add_slash) {
    char *ret = NULL;

    // If starts with ~ replace with /home/username
    if (path[0] == '~') {
        char *home = get_home_path();
        ret = str_add(home, path + 2, NULL);  // Delete ~/
        free(home);
    }

    // Ends with /
    if (add_slash == TRUE && endsWith(path, "/") == FALSE) {
        char *tmp = str_add(ret, "/", NULL);
        if (ret != NULL)
            free(ret);

        ret = tmp;
    }

    if (ret == NULL)
        return str_add(path, NULL);

    return ret;
}

char **split_except(char *str, char delimiter, char prev_delim_except, size_t *n) {
    char **ret = (char **) calloc(1, sizeof(char *));
    int a = 0;
    *n = 0;

    char *token = calloc(strlen(str) + 1, sizeof(char));
    int k = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != delimiter || (i > 0 && str[i - 1] == prev_delim_except)) {
            token[k] = str[i];
            k++;
            continue;
        }

        ret[a] = str_add(token, NULL);
        a++;
        ret = realloc(ret, (a + 1) * sizeof(char *));
        // Free the previous pointer.
        free(token);
        // Allocate space for new token.
        token = calloc(strlen(str) + 1, sizeof(char));
        k = 0;
    }

    ret[a] = str_add(token, NULL);

    *n = a + 1;
    free(token);
    return ret;
}

int is_dir(const char *path) {
    struct stat path_stat;

    if (stat(path, &path_stat) == -1)
        return FALSE;

    // do an and statement with bits of st_mode and bits of S_IFDIR.
    // If is the same the result is ok, otherwise the result is zero.
    if (path_stat.st_mode == S_IFDIR)
        return TRUE;

    return SUCCESS;
}

void read_contents_of(const char *path, queue *c_queue) {
    DIR *dir = opendir(path);
    struct dirent *dir_contents = NULL;
    char *tmp_path;

    // Read the files and folder inside the dir.
    while ((dir_contents = readdir(dir)) != NULL) {
        // Save the path.
        tmp_path = str_add(path, dir_contents->d_name, NULL);
        // Add it to the queue.
        add(c_queue, tmp_path);
    }
}

__mode_t get_permissions_of(const char *path) {
    struct stat path_stat;

    if (stat(path, &path_stat) == -1)
        return -1;

    return path_stat.st_mode;
}

char *rand_string(size_t size) {
    srand(time(NULL));
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    char *str = (char *) calloc(size + 1, sizeof(char));
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char *analyze_spaces_to_path(char *path) {
    int space_count = 0;
    // count spaces
    for (int i = 0; path[i]; i++, path[i] == ' ' ? space_count++ : 0);

    char *analyzed_path = (char *) calloc(strlen(path) + space_count + 1, sizeof(char));
    for (int i = 0, k = 0; path[i]; i++, k++) {
        if (path[i] == ' ') {
            analyzed_path[k] = '\\';
            k++;
        }
        analyzed_path[k] = path[i];
    }

    return analyzed_path;
}