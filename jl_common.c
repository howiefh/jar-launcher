#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jl_heap.h"
#include "jl_common.h"

void jl_error(const char* message) {
    jl_heap->free();
    if(errno) perror(message);
    else printf("ERROR: %s\n", message);
    exit(1);
}  

// http://stackoverflow.com/a/5711554/314015
void jl_strip_filename(const char* path, const int separator) {
    char* lastSlash = strrchr(path, separator);
    if(!lastSlash) jl_error("cannot get parent dir");
    *(lastSlash + 1) = '\0';
}

char * get_line(FILE*fp) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(fp);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if(c == '\n')
            break;
        *line++ = c;
    }
    *line = '\0';
    return linep;
}

char * read_firstline(char * filename){
    FILE * fp;
    if ((fp=fopen(filename,"r"))==NULL) {
        printf("open %s error\n", filename);
        return NULL;
    }
    char * firstline= get_line(fp);
    fclose(fp);
    return firstline;
}
char * merge_string(char * first, char * second){
    int len = strlen(first) + strlen(second) + 1;
    char * res = malloc(len);
    memset(res, 0, len);
    strncpy(res, first, strlen(first));
    strcat(res, second);
    return res;
}
char * java_path_from_java_home(){
    char jdk_bin[10] = "/bin/java"; 
    char jre_bin[14] = "/jre/bin/java";
    char * path;
    path = getenv("JAVA_HOME");
    if (path == NULL) {
        return NULL;
    }
    char * jdk_bin_fullpath = merge_string(path, jdk_bin);
    char * jre_bin_fullpath = merge_string(path, jre_bin);
    if (!access(jdk_bin_fullpath,0)) {
        free(jre_bin_fullpath);
        return jdk_bin_fullpath; 
    } else if (!access(jre_bin_fullpath,0)) {
        free(jdk_bin_fullpath);
        return jre_bin_fullpath; 
    }
    free(jre_bin_fullpath);
    free(jdk_bin_fullpath);
    return NULL;
}


char * get_java_path(){
    char env[4] = "env";
    char * real_java_path = NULL;

    if (!access(env,0)) {
        real_java_path = read_firstline(env);
        if (real_java_path==NULL) {
            printf("read error.try to delete env, or write the right java location in the %s file.\n", env);
        }
    } else if ((real_java_path=java_path_from_java_home())==NULL) {
        printf("con't find JAVA_HOME, or java file not exist.\n");
    }

    if (real_java_path!=NULL){
        if (!access(real_java_path,0)) {
            return real_java_path;
        } else {
            free(real_java_path);
        }
    } else {
        printf("con't find java.\n");
    }
    return NULL;
}
