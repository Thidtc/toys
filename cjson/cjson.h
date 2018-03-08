#ifndef CJSON_H__
#define CJSON_H__

/* Json data type */
typedef enum {
    CJSON_NULL,
    CJSON_FALSE,
    CJSON_TRUE,
    CJSON_NUMBER,
    CJSON_STRING,
    CJSON_ARRAY,
    CJSON_OBJECT,
} cjson_type;


typedef struct cjson_value cjson_value;
/* Key value pair for json object */
typedef struct cjson_kv cjson_kv;

struct cjson_value {
    cjson_type type;
    union {
        double n;
        struct { char *s; size_t len; } s; /* String */
        struct { cjson_value *e; size_t size; } a; /* Array */
        struct { cjson_kv *kvs; size_t size; } o; /* Object */
    } u;
};

struct cjson_kv {
    char *k;
    size_t klen;
    cjson_value v;
};

/* Parsing error code */
enum {
    CJSON_PARSE_OK = 0,
    CJSON_PARSE_EXPECT_VALUE,
    CJSON_PARSE_INVALID_VALUE,
    CJSON_ROOT_NOT_SINGULAR,
    CJSON_PARSE_MISS_QUOTATION_MARK,
    CJSON_PARSE_INVALID_STRING_ESCAPE,
    CJSON_PARSE_INVALID_UNICODE_HEX,
    CJSON_PARSE_INVALID_UNICODE_SURROGATE,
    CJSON_PARSE_MISS_COMMA_OR_BRACKET,
    CJSON_PARSE_MISS_KEY,
    CJSON_PARSE_MISS_COLON,
    CJSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET,
};

/* Stringify error code */
enum {
    CJSON_STRINGIFY_OK,
    CJSON_STRINGIFY_FAIL,
};

/* Context of the json string being parsed */
typedef struct {
    const char *json; /* Pointer to the position of json string
                         being parsed */
    char *stack;
    size_t size, top;
} cjson_context;

#define cjson_init(v) do { (v)->type = CJSON_NULL; } while (0)

/* Release the heap memory occupied by the json object */
void cjson_free(cjson_value *v);

#define cjson_set_null(v) cjson_free(v)

/* Parse json string into v, and return the parse error code */
int cjson_parse(cjson_value *v, const char *str);

/* Stringify a json value into a string */
char *cjson_stringify(const cjson_value *v, size_t *len);

/* Return the type of json object */
cjson_type cjson_get_type(const cjson_value *v);

int cjson_get_boolean(const cjson_value *v);

void cjson_set_boolean(cjson_value *v, int b);

/* Return the value of json number */
double cjson_get_number(const cjson_value *v);

void cjson_set_number(cjson_value *v, double n);

void cjson_set_string(cjson_value *v, const char *s, size_t len);
const char *cjson_get_string(const cjson_value *v);
size_t cjson_get_string_length(const cjson_value *v);

size_t cjson_get_array_size(const cjson_value *v);
cjson_value *cjson_get_array_element(const cjson_value *v, size_t index);

size_t cjson_get_object_size(const cjson_value *v);
const char *cjson_get_object_key(const cjson_value *v, size_t index);
size_t cjson_get_object_key_length(const cjson_value *v, size_t index);
cjson_value *cjson_get_object_value(const cjson_value *v, size_t index);

#endif /* CJSON_H__ */
