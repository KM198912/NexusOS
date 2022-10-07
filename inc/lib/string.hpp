#pragma once
#include <stdint.h>
#include <stddef.h>

static inline void memset(void* ptr, uint8_t value, size_t num)
{
    uint8_t* p = (uint8_t*)ptr;
    for (size_t i = 0; i < num; i++)
    {
        p[i] = value;
    }
}
static inline void memcpy(void* dest, const void* src, size_t num)
{
    uint8_t* d = (uint8_t*)dest;
    uint8_t* s = (uint8_t*)src;
    for (size_t i = 0; i < num; i++)
    {
        d[i] = s[i];
    }
}
static inline int memcmp(const void* ptr1, const void* ptr2, size_t num)
{
    uint8_t* p1 = (uint8_t*)ptr1;
    uint8_t* p2 = (uint8_t*)ptr2;
    for (size_t i = 0; i < num; i++)
    {
        if (p1[i] != p2[i])
        {
            return p1[i] - p2[i];
        }
    }
    return 0;
}
static inline size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}
static inline char* strcpy(char* dest, const char* src)
{
    size_t len = strlen(src);
    memcpy(dest, src, len + 1);
    return dest;
}
static inline char* strncpy(char* dest, const char* src, size_t num)
{
    size_t len = strlen(src);
    if (len > num)
    {
        len = num;
    }
    memcpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}
static inline int strcmp(const char* str1, const char* str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len = len1;
    if (len2 < len)
    {
        len = len2;
    }
    int res = memcmp(str1, str2, len);
    if (res == 0)
    {
        if (len1 < len2)
        {
            return -1;
        }
        else if (len1 > len2)
        {
            return 1;
        }
    }
    return res;
}
static inline int strncmp(const char* str1, const char* str2, size_t num)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len = len1;
    if (len2 < len)
    {
        len = len2;
    }
    if (len > num)
    {
        len = num;
    }
    int res = memcmp(str1, str2, len);
    if (res == 0)
    {
        if (len1 < len2)
        {
            return -1;
        }
        else if (len1 > len2)
        {
            return 1;
        }
    }
    return res;
}
static inline char* strcat(char* dest, const char* src)
{
    size_t len = strlen(dest);
    strcpy(dest + len, src);
    return dest;
}
static inline char* strncat(char* dest, const char* src, size_t num)
{
    size_t len = strlen(dest);
    strncpy(dest + len, src, num);
    return dest;
}
static inline char* strchr(char* str, int character)
{
    while (*str != '\0')
    {
        if (*str == character)
        {
            return str;
        }
        str++;
    }
    return NULL;
}
static inline const char* strchr(const char* str, int character)
{
    while (*str != '\0')
    {
        if (*str == character)
        {
            return str;
        }
        str++;
    }
    return NULL;
}
static inline char* strrchr(char* str, int character)
{
    char* last = NULL;
    while (*str != '\0')
    {
        if (*str == character)
        {
            last = str;
        }
        str++;
    }
    return last;
}