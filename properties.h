#ifndef __properties_H_
#define __properties_H_




#ifndef EXTERN
#ifdef __cplusplus
#define EXTERN  extern "C"
#else
#define EXTERN
#endif
#endif//EXTERN


//! This function will be called when the parser need more text.
//
//  \param  source  The input source object.
//  \param  buf     Store the read data.
//  \param  size    The max size of the `buf` as input parameter, and the real 
//                  byte have fill into buf as the output parameter.
//
//  \return 0   The input source is no data to provide.
//          <0  Hit the error, the parse process will be terminated.
//          1   End of the input source.
typedef int     (*PARSE_READ)(struct parse_source_t* source, char* buf, int* size);

//! This function is used to close the `parse_source_t` object.
//
//  \param  source  The object will be closed.
typedef void    (*PARSE_FREE)(struct parse_source_t* source);

//! `parse_source_t` is the interface of the input source.
struct parse_source_t
{
    PARSE_READ  read;   //< The paser call this function to read more data;
    PARSE_FREE  free;   //< When you call `parse_source_del` this function will be called. 
                        //< If your customed input source do not need close, just assign it with NULL.
};



//! Create a new input source from the file.
//  
//  \param  filename    Which file need to parse.
//
//  \return If open then file success, return the `struct parse_source_t` object. 
//          Return NULL when hit any error.
EXTERN  struct parse_source_t*  parse_source_new_from_file      (char* filename);

//! Create a new input source from the string.
//  
//  \param  str     The address of the input string which will be parsed leter.
//  \param  end     The end pos of the string. *pos will never be read.
//
//  \return If open then file success, return the `struct parse_source_t` object. 
//          Return NULL when hit any error.
EXTERN  struct parse_source_t*  parse_source_new_from_string    (char* str, char* end);


//! Close any of the input source.
//  
//  \param  source  The address of the input source object.
EXTERN  void                    parse_source_del                (struct parse_source_t* source);



//! This function will be called when the parser recogenized the key-value pair.
//
//  \param  context This parameter is equal with the last parameter of properties_load.
//  \param  key     The key of the propretie item.
//  \param  key_len
//  \param  val
//  \param  val_len
//
//  \return 0       The parser will be continue unness hit the end of the input source.
//          others  The parse process will be terminated imediately.
typedef int     (*HANDLE_PROPERTY)(void* context, char* key, int key_len, char* val, int val_len);



//! Load and parse the input text from source, once recogenize a key-value pair, call `handle`.
//
//  \param  source  Where to read the input text from.
//  \param  handle  The callback function, to notify the user that the parser find a new key-value pair.
//  \param  context This parameter will be the first parameter of handle.
//  
//  \return If there is no error, return 0, otherwise return -1.
EXTERN  int     properties_load(struct parse_source_t* source, HANDLE_PROPERTY handle, void* context);




#endif//__properties_H_
