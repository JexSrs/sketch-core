#ifndef SKETCH_CORE_FUNCTIONS_H
#define SKETCH_CORE_FUNCTIONS_H 1


#include <include/queue.h>

/**
 * Checks if a string starts with another string.
 * @param str The base string
 * @param pre The prefix.
 * @return TRUE or FALSE if it starts with or not
 */
extern int startsWith(const char *str, const char *pre);

/**
 * Checks if a string ends with another string.
 * @param str The base string.
 * @param suffix The possible ending string.
 * @return TRUE or FALSE if it ends with or not.
 */
extern int endsWith(const char *str, const char *suffix);

/**
 * Return the concat of two strings.
 * @param str1 The str1 string
 * @param str2 The str2 string.
 * @return The concat of the the two strings.
 */
extern char *str_add(const char *str1, ...);

/**
 * Return the home directory.
 * @return The home directory.
 */
extern char *get_home_path();

/**
 * Replace ~ with home directory and add / at the end if missing.
 */
extern char *fix_path(const char *path, int add_slash);

/**
 * Returns the path with the home directory at the start.
 * @param filename The file's filename.
 * @param relative_path The relative path.
 * @return The fixed path.
 */
extern char *merge_home_relative_filename(const char *filename, const char *relative_path);

/**
 * Splits a string to an array of strings by a specific delimiter. It will skip the split_except by delimiter if the previous character is the same as the prev_delim_except.
 * @param str The string that will be split_except.
 * @param delimiter The delimiter.
 * @param prev_delim_except The exception character that is located before the delimiter.
 * @param n The size of the array that will be returned.
 * @return A string array that contains the split_except form of the str.
 */
extern char **split_except(char *str, char delimiter, char prev_delim_except, size_t *n);

/**
 * Checks if a path is a directory.
 * @param path The path to check.
 * @return TRUE if the path is directory, FALSE otherwise.
 */
extern int is_dir(const char *path);

extern void read_contents_of(const char *path, queue *c_queue);

extern __mode_t get_permissions_of(const char *path);

extern char *rand_string(size_t size);

extern char *analyze_spaces_to_path(char *path);

#endif
