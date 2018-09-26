#	libproperties

libproperties is a library to parse the Java .properties files. It was writen in pure C. 
And fullly compatible with the Java .properties file format.

#	Build

This library is very small. I am not ready provide the makefile for it. 
You can add this file in your project and compile with you project together.

You need just two file below to your project:

- properties.c
- buf.c



#	Usage

See the header file of `properties.h` to see how to call the function.

### First: Create a 'struct parse_source_t' object

`struct parse_source_t` is used to define the input text.
Ofcouse you can define it your self, 
But, there are two way to create `struct parse_source_t` by default.

- To create from the file, use `parse_source_new_from_file`
- To create from the string, use `parse_source_new_from_string`


### Second: Call the function `properties_load` to parse the input text

```C
EXTERN  int     properties_load(struct parse_source_t* source, HANDLE_PROPERTY handle, void* context);
```

`properties_load` need tree parameters:

- `source` is a object that provide the input text. It is a pointer to  `struct parse_source_t`.
- `handle` is a callback function, which will be called when `properties_load` recogenized any one of the key-value pair.
- `context` is a pointer which will be the first parameter of the `handle`. 

The `properties_load` function will do not help you to store any of the key-value pairs. 
So, you need put your code in the function `handle`. 

### Third: Destroy the source object.

Just call the function `parse_source_del`.


#	Format

This library is complete compatible with the Java .properties file format which defined in 
[Properties.java(JDK9)](https://docs.oracle.com/javase/9/docs/api/java/util/Properties.html).
Please jump to the `load` function to checkout the full of the definition of the format.
This document is completely and exactly but too much complex. 
And there is a simple one which provide by `Wikipedia` [here](https://en.wikipedia.org/wiki/.properties).

#	Encode

UTF-8 or ASCII 


#	Custom the input source

Generally speaking, the `parse_source_new_from_file` and `parse_source_new_from_string` is enough.
But some times we have to custom ourself input source. 
Such as, the input source gived is not a file name, but file handle. 
This time, we can create a new type of input source:

#### 1. Declare your new `parse_source_t`:

```c
struct my_file_source_t
{
	struct parse_source_t	source;
	FILE*					file;
};
```

#### 2. Implement the `PARSE_READ` and `PARSE_FREE` function:

```c
int     file_source_read(struct parse_source_t* source, char* buf, int size)
{
	struct my_file_source_t* my_source = (struct my_file_source_t*)source;
	FILE* file = my_source->file;
	if (feof(file))
	{
		return 0;
	}

	if (ferror(file))
	{
		return -1;
	}

	size_t read_size = fread(buf, size, file);
	if (read_size > 0)
	{
		return read_size;
	}

	return read_size;
}

void    file_source_free(struct parse_source_t* source)
{
	struct my_file_source_t* my_source = (struct my_file_source_t*)source;
	fclose(my_source->file);
}

```

#### 3. Define a function to initialize your input source:

```C
struct my_file_source_t* my_file_source_init(struct my_file_source_t* my_source, FILE* file)
{
	my_source->source.read = file_source_read;
	my_source->source.free = file_source_free;
	my_source->file        = file;
	return my_source;
}
```

#### 4. Enjoy your new input source:

```c
struct my_file_source_t my_source;
properties_load(my_file_source_init(&my_source, file), handle, NULL);
```

#	BOM

If the input file have BOM at the begining of the file.
You'd better custom a new input source for it.
And skip the BOM bytes when the read function called at the first time.
