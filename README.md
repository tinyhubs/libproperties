#	libproperties

libproperties is a library to parse the Java .properties files. It was writen in pure C. 
And fully compatible with the Java .properties file format.


##	Build



### Raw file way

If you do not want to use this project as a library, You can add these files in your project and compile with you project together.

- properties.c
- buf.c

### Build shared library with cmake

```bash
mkdir build
cd build/
cmake ../
make
```

### Build static library with cmake

```bash
mkdir build
cd build/
cmake -D BUILD_SHARED_LIBS=off ../
make
```

### Build and test

After build, we just need type command below:

```bash
make test
```

### Clean

```bash
make clean
```


### Install

If you do not need special the install directory, you need to setup the `CMAKE_INSTALL_PREFIX` options:

```bash
mkdir build
cd build/
cmake -DCMAKE_INSTALL_PREFIX=./output ../
make
make install
```

If you do not need special the install directory, the default installed directory is `/usr/local/lib/`:

```bash
mkdir build
cd build/
cmake ../
make
make install
```


##	Usage

The kernel function of this library is `properties_parse`

```C
int     properties_parse(void* source_context, PROPERTIES_SOURCE_READ source_read, void* handler_context, PROPERTYS_HANDLER handler);
```

It need four parameters:

- `source_context` and `source_read` is used to tell where to read the text from.
- `handler_context` and `handler` is used to tell the function how to precess the key-value pairs of the property.

This library have provided two `source_read` function by default.

- Read input text from a file, please use `properties_source_file_read`.
- Read input text from simple string, please use `properties_source_file_string`.

##### *Sample 1: Read from file*

```C
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
```

##### *Sample 2: Read from string*

```C
int     test_handler(void* context, char* key, int key_len, char* val, int val_len)
{
    printf("[%s]=[%s]\n", key, val);
    return 0;
}


int main(int argc, char* argv[])
{
    char str[] = 
    "a=b\n"
    "c=123\n"
    ;

    struct properties_source_string_t source = 
    {
        str,
        str + strlen(str)
    };
    properties_parse(&source, properties_source_string_read, NULL, test_handler);

    getchar();
	return 0;
}
```


###	Test

#####  0. Requrement

*Windows*

   * [cmake](https://cmake.org/)
   * [MSYS2](http://www.msys2.org/)
   * Visual Studio 2010 or later

*Linux/Unix*

   * [premake](https://premake.github.io/)

#####  1. Generage the makefile(Linux/Unix) or project file

*Windows*

```sh
premake5 --file=libproperties_test.premake vs2010
```

The `vs2010` should be replace with one of below, which is depended the version of the Visual Studio you have installed.
  
  * vs2008 
  * vs2010
  * vs2013
  * vs2015
  * vs2017

*Linux/Unix*

```sh
premake5 --file=libproperties_test.premake gmake
```

#####  2. Build the test tool

*Windows*

Open the `libproperties_test.sln` with your IDE, and then press F7

*Linux*

```sh
make clean && make
```

##### 3. Run test cases

If your system is Windows, you need open the bash of MSYS2 first.

```sh
./test.sh
```

#	Format

This library is complete compatible with the Java .properties file format which defined in 
[Properties.java(JDK9)](https://docs.oracle.com/javase/9/docs/api/java/util/Properties.html).
Please jump to the `load` function to checkout the full of the definition of the format.
This document is completely and exactly but too much complex. 
And there is a simple one which provide by `Wikipedia` [here](https://en.wikipedia.org/wiki/.properties).


#	Encode

UTF-8


#	Custom the input source

Generally speaking, the `properties_source_file_read` and `properties_source_string_read` is enough.
If you want, you can provided a new function yourself. But the function must flow the prototype of `PROPERTIES_SOURCE_READ`.
Please refer the `properties.h` to see rules of function `PROPERTIES_SOURCE_READ`.


#	BOM

If the input file have BOM at the begining of the file.
You'd better custom a new input source for it.
And skip the BOM bytes when the read function called at the first time.
