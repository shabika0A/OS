#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>
#include <string.h>

void *searchFilesThread(void *arg);
void searchFiles(const char *dirPath, const char *searchQuery) {
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("directory is null");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char subDirPath[300];
                snprintf(subDirPath, sizeof(subDirPath), "%s/%s", dirPath, entry->d_name);
                pthread_t thread;
                const char *args[] = {subDirPath, searchQuery};
                pthread_create(&thread, NULL, searchFilesThread, args);
                pthread_join(thread, NULL);
            }
        } else {
            if (strstr(entry->d_name, searchQuery) != NULL) {
                printf("[+] Found:\n%s/%s\n", dirPath, entry->d_name);
            }
        }
    }
    closedir(dir);
}

void *searchFilesThread(void *arg) {
    const char **args = (const char **)arg;
    searchFiles(args[0], args[1]);
    pthread_exit(NULL);
}

int main() {
    char searchDir[100];
    char start[10];
    char searchQuery[100];

    scanf("%s", start);
    scanf("%99s", searchDir);
    scanf("%99s", searchQuery);

    if (strcmp(start, "search") == 0) {
        pthread_t thread;
        const char *args[] = {searchDir, searchQuery};
        pthread_create(&thread, NULL, searchFilesThread, args);
        pthread_join(thread, NULL);
    } else {
        printf("Input has to start with the search word\n");
    }

    pthread_exit(NULL);

    return 0;
}
