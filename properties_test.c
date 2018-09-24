// libproperties.cpp : 定义控制台应用程序的入口点。
//
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>

#include "properties.h"

int     handle(void* context, char* key, int key_len, char* val, int val_len)
{
    printf("[%s]\t[%s]\n", key, val);
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    char* str[] = 
    {
        "a=1\nb=2",
        "a=1\n#dsdsd",
        "a\\=1    1111111111    ",
        "a=1    1111111111    ",
    };

    for (int i = 0; i < (sizeof(str) / sizeof(str[0])); i++)
    {
        printf("------\n");
        struct parse_source_t* source = parse_source_new_from_string(str[i], NULL);
        properties_load(source, handle, NULL);
        parse_source_del(source);
    }

    getchar();
	return 0;
}

