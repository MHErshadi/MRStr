# (MRStr) MetaReal String Library version 1.0.0

## MRFStr (MetaReal Fast String Library) version 1.0.0

## MRTStr (MetaReal Threaded String Library) version 1.0.0

MRTStr (MetaReal Threaded String Library) is a library for manipulating, comparing, searching, matching, and classifying strings in C. It's optimized and utilizes all of the power of the CPU. So, MRTStr functions are much faster than the standard string C ones. \
The raison d'etre of the MRTStr is for search engines, NLP systems, databases, and file compressors to run as quickly as possible. \
MRTStr is asynchronous, meaning that the function calls don't necessarily imply the end of the procedure. To keep things simple, the MRTStr library provides all the essential instructions to avoid program malfunction. So, the programmer usually would not have to interact with the implemented data structures to cause errors accidentally.

### MRTStr Functions

```c
mrtstr_res_enum_t mrtstr_init_threads(mrtstr_bit_t size)
```

It initializes the immortal threads (*size* number of them) and returns an error if the process fails. The function call should be at the beginning of the program.

Errors:
MRTSTR_RES_MEM_ERROR: Could not allocate the list of threads from memory.
MRTSTR_RES_THREAD_ERROR: Could not create all of the threads or the mutex.

```c
void mrtstr_free_threads()
```

It waits for the immortal threads to join the main thread and frees them. The function call should be at the end of the program.

```c
mrtstr_t mrtstr_init()
```

It initializes the MRTStr data structure (default initialization) and returns NULL if the process fails.

```c
mrtstr_t mrtstr_init2(mrtstr_data_t data)
```

It initializes the MRTStr data structure according to the *data* and returns NULL if the process fails. \
The *data* pointer must be aligned and valid or NULL. \
The function won't copy the *data*.

```c
mrtstr_t mrtstr_init3(mrtstr_data_t data, mrtstr_size_t size)
```

It initializes the MRTStr data structure according to the *data* and its *size* and returns NULL if the process fails. The function design is to avoid the ``strlen`` function call (present in the ``mrtstr_init2`` function). \
The *data* pointer must be aligned and valid or NULL. \
The function won't copy the *data*. \
The *size* must equal the size of the *data*.

```c
mrtstr_res_enum_t mrtstr_alloc(mrtstr_t str, mrtstr_size_t size)
```

It allocates the *str* according to the *size*. The function returns an error if the process fails. \
The *str* pointer must be valid. \
The *str* must be initialized and empty.

Errors:
MRTSTR_RES_MEM_ERROR: Could not allocate the specified size from memory.

```c
void mrtstr_free(mrtstr_t str)
```

It deallocates the *str* and its data from memory. \
The *str* pointer must be valid. \
The *str* must be initialized.

```c
void mrtstr_clear(mrtstr_t str)
```

Clears the data of the *str*. But it doesn't deallocate the *str* from memory. \
The *str* pointer must be valid.
