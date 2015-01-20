#pragma once
#include <stdio.h>

/** 
 * Frees memory using jl_heap, reports error message to stdout, shutdowns program
 */
extern void jl_error(const char* message);

/**
 * Remove filename from file system path leaving directory path
 */
extern void jl_strip_filename(const char* path, const int separator);

/**
 * 获取env 文件或 系统中的java路径
 */
extern char * get_java_path();
/**
 * 获取文件的一行
 */
extern char * get_line(FILE*fp);
/**
 * 获取java_home下的java路径
 */
extern char * java_path_from_java_home();
/**
 * 获取合并两个字符串
 */
extern char * merge_string(char * first, char * second);
/**
 * 读取文件的第一行
 */
extern char * read_firstline(char * filename);
