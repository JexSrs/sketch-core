/* functions.c */
#include <string.h>
#include <malloc.h>
#include <dirent.h>
#include <stdarg.h>
#include <time.h>

#include <include/functions.h>
#include <mem.h>

int ends_with(const char *str, const char *suffix) {
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

char *str_add(const char *str1, ...) {
    va_list args;
    va_start(args, str1);

    char *result;
    ALLOCATE_MEM(result, strlen(str1) + 1, sizeof(char));
    strcpy(result, str1);

    char *str;

    while (1) {
        str = va_arg(args, char *);

        if (str == NULL) break;

        REALLOCATE_MEM(result, (strlen(result) + strlen(str) + 1) * sizeof(char));
        strcat(result, str);
    }
    va_end(args);

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
    if (add_slash == TRUE && ends_with(path, "/") == FALSE) {
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
    char **ret;
    ALLOCATE_MEM(ret, 1, sizeof(char *));
    int a = 0;
    *n = 0;

    char *token;
    ALLOCATE_MEM(token, strlen(str) + 1, sizeof(char));
    int k = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != delimiter || (i > 0 && str[i - 1] == prev_delim_except)) {
            token[k] = str[i];
            k++;
            continue;
        }

        ret[a] = str_add(token, NULL);
        a++;
        REALLOCATE_MEM(ret, (a + 1) * sizeof(char *));
        // Free the previous pointer.
        free(token);
        // Allocate space for new token.
        ALLOCATE_MEM(token, strlen(str) + 1, sizeof(char));
        k = 0;
    }

    ret[a] = str_add(token, NULL);

    *n = a + 1;
    free(token);
    return ret;
}

void read_contents_of(const char *directory, queue *c_queue) {
    DIR *dir = opendir(directory);
    struct dirent *dir_contents = NULL;
    char *tmp_path;

    // Read the files and folder inside the dir.
    while ((dir_contents = readdir(dir)) != NULL) {
        // Save the directory.
        tmp_path = str_add(directory, dir_contents->d_name, NULL);
        // Add it to the queue.
        add(c_queue, tmp_path);
    }
    closedir(dir);
}

char *rand_string(size_t size) {
    srand(time(NULL));
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    char *str;
    ALLOCATE_MEM(str, size + 1, sizeof(char));
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

char *analyze_string_spaces(char *str) {
    int space_count = 0;
    // count spaces
    for (int i = 0; str[i]; i++, str[i] == ' ' ? space_count++ : 0);

    char *analyzed;
    ALLOCATE_MEM(analyzed, strlen(str) + space_count + 1, sizeof(char));
    for (int i = 0, k = 0; str[i]; i++, k++) {
        if (str[i] == ' ') {
            analyzed[k] = '\\';
            k++;
        }
        analyzed[k] = str[i];
    }

    return analyzed;
}