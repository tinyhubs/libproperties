#	libproperties

libproperties is a library to parse the Java .properties files. It was writen in pure C.

#	Build

This library is very small. I am not ready provide the makefile for it. 
You can add this file in your project and compile with you project together.

You need just two file below to your project:

- properties.c
- buf.c



#	Usage

This library provide the only one function to parse the  `.properties` file:

```C
EXTERN  int     properties_load(struct parse_source_t* source, HANDLE_PROPERTY handle, void* context);
```

`properties_load` need two parameters:

- `source` is a object that provide the input text. It is a pointer to  `struct parse_source_t`.
- `handle` is a callback function, which will be called when `properties_load` recogenized any one of the key-value pair.
- `context` is a pointer which will be the first parameter of the `handle`. 

The  struct `parse_source_t` is defined below:

```C
typedef int     (*PARSE_READ)(struct parse_source_t* source, char* buf, int size);
typedef void    (*PARSE_FREE)(struct parse_source_t* source);
struct parse_source_t
{
    PARSE_READ  read;
    PARSE_FREE  free;
};
```

`parse_source_t` give the `properties_load` a way to scan the text from any input datasource.

#	Format



#	Custom the input source


#	Encode


#	BOM


 
