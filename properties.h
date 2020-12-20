#ifndef __properties_H_
#define __properties_H_

#ifdef LIBPROPERTIES_EXPORT
#ifdef _WIN32
#define LIBPROPERTIES_API __declspec(dllexport)
#else
#define LIBPROPERTIES_API __attribute__((visibility("default")))
#endif
#elif !defined(LIBPROPERTIES_STATIC)
#ifdef _WIN32
#define LIBPROPERTIES_API __declspec(dllimport)
#else
#define LIBPROPERTIES_API
#endif
#else
#define LIBPROPERTIES_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

//! This function will be called when the parser need more text.
//
//  \param  context The context of the this function, it is from the first parameter of `properties_parse`.
//  \param  buf     Store the read data.
//  \param  size    The max size of the `buf` as input parameter, and the real
//                  byte have fill into buf as the output parameter.
//
//  \return 0   The input source is no data to provide.
//          <0  Hit the error, the parse process will be terminated.
//          1   End of the input source.
typedef int (*PROPERTIES_SOURCE_READ)(void* context, char* buf, int* size);

//! This function will be called when the parser recogenized the key-value pair.
//
//  \param  context This parameter is equal with the last parameter of properties_parse.
//  \param  key     The key of the propretie item.
//  \param  key_len
//  \param  val
//  \param  val_len
//
//  \return 0       The parser will be continue unness hit the end of the input source.
//          others  The parse process will be terminated imediately.
typedef int (*PROPERTYS_HANDLER)(void* context, char* key, int key_len, char* val, int val_len);

//! Load and parse the input text from source, once recogenize a key-value pair, call `handler`.
//
//  \param  source_context  The context object which will be the first parameter of `source_read`.
//  \param  source_read     Once the parser need more data, this function will be called.
//  \param  handler_context This parameter will be the first parameter of `handler`.
//  \param  handler         The callback function, to notify the user that the parser find a new key-value pair.
//
//  \return If there is no error, return 0, otherwise return -1.
LIBPROPERTIES_API int properties_parse(void* source_context, PROPERTIES_SOURCE_READ source_read, void* handler_context, PROPERTYS_HANDLER handler);

//! The function `properties_source_file_read` is the default implement for read from a file.
//  \notice The `file` Should be a `FILE*`.
LIBPROPERTIES_API int properties_source_file_read(void* file, char* buf, int* size);

//! The function `properties_source_string_read` is the default implement for read from a string.
//  To use this function, you need fill the `struct properties_source_string_t`, and give it is as
//  the first parameter of function `properties_parse`.
struct properties_source_string_t {
    char* str; ///<    The start address of the input string.
    char* end; ///<    The end address of the input string.
};
LIBPROPERTIES_API int properties_source_string_read(void* source, char* buf, int* size);

#ifdef __cplusplus
}
#endif

#endif //__properties_H_
