#	libproperties

libproperties is a library to parse the Java .properties files. It was writen in pure C.

#	Build

This library is very small. I am not ready provide the makefile for it. 
You can add this file in your project and compile with you project together.

You need just two file below to your project:

- properties.c
- buf.c



#	Usage

#### First: Create a 'struct parse_source_t' object

`struct parse_source_t` is used to define the input text.
Ofcouse you can define it your self, 
But, there are two way to create `struct parse_source_t` by default.

- To create from the file, use `parse_source_new_from_file`
- To create from the string, use `parse_source_new_from_string`


#### Second: Call the function `properties_load` to parse the input text

```C
EXTERN  int     properties_load(struct parse_source_t* source, HANDLE_PROPERTY handle, void* context);
```

`properties_load` need tree parameters:

- `source` is a object that provide the input text. It is a pointer to  `struct parse_source_t`.
- `handle` is a callback function, which will be called when `properties_load` recogenized any one of the key-value pair.
- `context` is a pointer which will be the first parameter of the `handle`. 

The `properties_load` function will do not help you to store any of the key-value pairs. 
So, you need put your code in the function `handle`. 

#### Third: Destroy the source object.

Just call the function `parse_source_del`.


#	How to

#	Format


#	Custom the input source


#	Encode


#	BOM


 
