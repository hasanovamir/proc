#ifndef SPU_h
#define SPU_h

//————————————————————————————————————————————————————————————————————————————————

struct parameters_t
{
    int  num_el;
    int  num_compl_com;
    int* com_arr;
};

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    PUSH  = 1,
    OUT   = 2,
    ADD   = 3,
    SUB   = 4,
    MUL   = 5,
    DIV   = 6,
    SQRT  = 7,
    HLT   = 8,
    IN    = 9,
    PUSHR = 33,
    POPR  = 42,
} operation_t;

//————————————————————————————————————————————————————————————————————————————————

typedef enum
{
    SPU_SUCCESS        = 0,
    SPU_ALLOCATION_ERR = 1,
    SPU_OPEN_FILE_ERR  = 2,
} spu_err_t;

//————————————————————————————————————————————————————————————————————————————————

spu_err_t   allocation_com_arr       (parameters_t* data, const char* title    );
spu_err_t   read_commands            (parameters_t* data, const char* title    );
stack_err_t use_stack_func           (parameters_t* data, stack_t* stk,int* reg);
const char* error_spu_code_to_string (spu_err_t status                         );
spu_err_t   spu_destroy_com_arr      (parameters_t* data                       );
spu_err_t   get_file_size            (parameters_t* data, const char* title    );
//————————————————————————————————————————————————————————————————————————————————

#define RETURN_IF_SPU_ERR(status) do \
{\
    spu_err_t _status_val_ = status;\
    if (_status_val_)\
    {\
        fprintf(stderr, "%s in %s:%d func:%s\n", error_spu_code_to_string (_status_val_), __FILE__, __LINE__, __PRETTY_FUNCTION__);\
        return _status_val_;\
    }\
}while (0)

//————————————————————————————————————————————————————————————————————————————————

#endif //SPU.H