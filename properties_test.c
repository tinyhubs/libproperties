#if defined(_MSC_VER)
#include <SDKDDKVer.h>
#include <tchar.h>
#endif

#include <stdio.h>

#include "properties.h"

int     test_handler(void* context, char* key, int key_len, char* val, int val_len)
{
    printf("[%s]=[%s]\n", key, val);
    return 0;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Missing parameters: properties_test <FILE>");
        return  1;
    }

    FILE* file = fopen(argv[1], "r");
    if (NULL == file)
    {
        return  NULL;
    }

    int ret = properties_parse(file, properties_source_file_read, NULL, test_handler);
    fclose(file);
    
    getchar();
    return ret;
}
// 
// int main(int argc, char* argv[])
// {
//     char str[] = 
//     "a=1\n"
//     "b=2\n"
//     "a=1\n"
//     "#dsdsd\n"
//     "a\\=1    1111111111    \n"
//     "a=1    1111111111    \n"
//     "a=1 \\\n"
//     "   1111111111    \n"
//     "a=1  \n  1111111111    \n"
//     ;
// 
//     struct properties_source_string_t source = 
//     {
//         str,
//         str + strlen(str)
//     };
//     properties_parse(&source, properties_source_string_read, NULL, test_handler);
// 
//     getchar();
// 	return 0;
// }
// 
