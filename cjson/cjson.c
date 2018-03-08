#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "cjson.h"

static int cjson_parse_whitespaces(cjson_context *c);
static int cjson_parse_keyword(cjson_context *c, cjson_value *v,
        const char *keyword, cjson_type type);
static int cjson_parse_number(cjson_context *c, cjson_value *v);
static int cjson_parse_string_raw(cjson_context *c, char **s, size_t *len);
static int cjson_parse_string(cjson_context *c, cjson_value *v);
static int cjson_parse_array(cjson_context *c, cjson_value *v);
static int cjson_parse_value(cjson_context *c, cjson_value *v);
static void cjson_kv_free(cjson_kv *kv);

static void cjson_set_string_shadow(cjson_value *v, char *s, size_t len);

static int cjson_stringify_value(cjson_context *c, const cjson_value *v);

#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while (0)

#ifndef CJSON_CONTEXT_STACK_INIT_SIZE
#define CJSON_CONTEXT_STACK_INIT_SIZE 256
#endif

/* Context stack push, return the beginning address of 
 * pushed data, or NULL on error*/
static char *cjson_context_push(cjson_context *c, size_t size) {
    char *ret;
    assert(size > 0);
    if (c->top + size >= c->size) {
        if (c->size == 0)
            c->size = CJSON_CONTEXT_STACK_INIT_SIZE;
        while (c->top + size >= c->size)
            c->size = c->size << 1;
        c->stack = (char *)realloc(c->stack, c->size);
        if (c->stack == NULL)
            return NULL;
    }
    ret = c->stack + c->top;
    c->top += size;
    return ret;
}

/* Context stack pop, return the top address after data popped */
static char *cjson_context_pop(cjson_context *c, size_t size) {
    assert(c->top >= size);
    return c->stack + (c->top -= size);
}

/* Push a char on the context stack */
#define PUTC(c, ch) do { *(char *)cjson_context_push((c), sizeof(char)) = (ch); }\
    while (0)

/* Push a jsonobject on the context stack */
#define PUTJSON(c, json) do { *(cjson_value *)cjson_context_push((c), sizeof(cjson_value))\
    = (json); } while (0)

/* Push a key value pair on the context stack */
#define PUTKV(c, kv) do { *(cjson_kv *)cjson_context_push((c), sizeof(cjson_kv))\
    = (kv); } while (0)

/* Push a part of string on the context stack */
#define PUTSTR_LEN(c, str, len) do { strncpy(cjson_context_push((c), len), str, len);}\
    while (0)

/* Push a string on the context stack */
#define PUTSTR(c, str) PUTSTR_LEN(c, str, strlen(str))

/* whitespace = [' '|'\t'|'\n'|'\r'] */
static int cjson_parse_whitespaces(cjson_context *c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p;
    c->json = p;
    return CJSON_PARSE_OK;
}

/* ["null"|"true"|"false"] */
static int cjson_parse_keyword(cjson_context *c, cjson_value *v,
        const char *keyword, cjson_type type) {
    size_t i;
    for (i = 0; i < strlen(keyword); ++i) {
        if (keyword[i] != c->json[i]) {
            v->type = CJSON_NULL;
            return CJSON_PARSE_INVALID_VALUE;
        }
    }
    c->json += i;
    v->type = type;
    return CJSON_PARSE_OK;
}

static int cjson_parse_number(cjson_context *c, cjson_value *v) {
    char *end = (char *)c->json;
    if (*end == '-') end++;
    if (!isdigit(*end)) return CJSON_PARSE_INVALID_VALUE;
    while (isdigit(*end)) end++;
    if (*end == '.') {
        end++;
    }
    while (isdigit(*end)) end++; if (*end == 'e' || *end == 'E') {
        end++;
        if (*end == '+' || *end == '-') end++;
        if (!isdigit(*end)) {
            return CJSON_PARSE_INVALID_VALUE;
        }
        while (isdigit(*end)) end++;
    }
    errno = 0;
    v->u.n = strtod(c->json, &end);
    /* Return error if overflow or underflow occurs */
    if (errno == ERANGE && (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL))
        return CJSON_PARSE_INVALID_VALUE;
    if (c->json == end)
        return CJSON_PARSE_INVALID_VALUE;
    c->json = end;
    v->type = CJSON_NUMBER;
    return CJSON_PARSE_OK;
}

/* Parse unicode to unsigned integer, return the pointer to
 * the string, or NULL if fail */
static char *cjson_parse_hex4(const char *p, unsigned int *u) {
    size_t i;
    *u = 0;
    for (i = 0; i < 4; ++i) {
        if (!isxdigit(p[i])) return NULL;
        *u = ((*u) << 4) + (isdigit(p[i]) ? p[i] - '0' :
                (isupper(p[i]) ? p[i] - 'A' : p[i] - 'a') + 10);
    }
    return (char *)p + i;
}

/* Push the UTF-8 encoded unicode to the context stack */
static void cjson_encode_utf8(cjson_context *c, unsigned int u) {
    if (u >= 0x0 && u < 0x80) { /* U+0000~U+007F */
        PUTC(c, u & 0x7F);
    } else if (u >= 0x80 && u < 0x800) { /* U+0080~U+07FF */
        PUTC(c, 0xC0 | ((u >> 6) & 0x1F));
        PUTC(c, 0xC0 | (u & 0x3f));
    } else if (u >= 0x800 && u < 0x10000) { /* U+0800~U+FFFF */
        PUTC(c, 0xE0 | ((u >> 12) & 0x0F));
        PUTC(c, 0x80 | ((u >> 6) & 0x3F));
        PUTC(c, 0x80 | (u & 0x3F));
    } else if (u >= 0x10000 && u < 0x110000) { /* U+10000~U+10FFFF */
        PUTC(c, 0xF0 | ((u >> 18) & 0x07));
        PUTC(c, 0x80 | ((u >> 12) & 0x3F));
        PUTC(c, 0x80 | ((u >> 6) & 0x3F));
        PUTC(c, 0x80 | (u & 0x3F));
    }
}

static void cjson_kv_free(cjson_kv *kv) {
    assert(kv != NULL);
    free(kv->k);
    cjson_free(&(kv->v));
}

/* Release the memory occupied by the json string object */
void cjson_free(cjson_value *v) {
    assert(v != NULL);
    if (v->type == CJSON_STRING) {
        free(v->u.s.s);
    } else if (v->type == CJSON_ARRAY) {
        size_t i;
        for (i = 0; i < v->u.a.size; ++i) {
            cjson_free(v->u.a.e + i);
        }
        free(v->u.a.e);
    } else if (v->type == CJSON_OBJECT) {
        size_t i;
        for (i = 0; i < v->u.o.size; ++i) {
            cjson_kv_free(v->u.o.kvs + i);
        }
        free(v->u.o.kvs);
    }
    v->type = CJSON_NULL;
}

/* Copy string into the json string object */
void cjson_set_string(cjson_value *v, const char *s, size_t len) {
    assert(v != NULL && s != NULL && len >= 0);
    cjson_free(v);
    v->u.s.s = (char *)malloc((len + 1) * sizeof(char));
    memcpy(v->u.s.s, s, len);
    v->u.s.s[len] = 0;
    v->u.s.len = len;
    v->type = CJSON_STRING;
}

static void cjson_set_string_shadow(cjson_value *v, char *s,
        size_t len) {
    assert(v != NULL && s != NULL && len >= 0);
    cjson_free(v);
    v->u.s.s = s;
    v->u.s.len = len;
    v->type = CJSON_STRING;
}

static int cjson_parse_string(cjson_context *c, cjson_value *v) {
    int ret;
    char *s;
    size_t len;
    if ((ret = cjson_parse_string_raw(c, &s, &len)) == CJSON_PARSE_OK) {
        cjson_set_string_shadow(v, s, len);
    }
    return ret;
    //unsigned int lcode, hcode, codepoint;
    //size_t head = c->top, len;
    //EXPECT(c, '\"');
    //const char *p = c->json;
    //while (1) {
    //    char ch = *p++;
    //    switch (ch) {
    //        case '\"': {
    //            len = c->top - head;
    //            if (len == 0) { /* When the len is 0,
    //                               the context stack may be NULL */
    //                cjson_set_string(v, "", 0);
    //            } else {
    //                cjson_set_string(v,
    //                    (const char *)cjson_context_pop(c, len), len);
    //            }
    //            c->json = p;
    //            return CJSON_PARSE_OK;
    //                   }
    //        case '\\': {
    //            switch (*p++) {
    //                case '\"' : PUTC(c, '\"'); break;
    //                case '\\' : PUTC(c, '\\'); break;
    //                case '/' : PUTC(c, '/'); break;
    //                case 'b' : PUTC(c, '\b'); break;
    //                case 'f' : PUTC(c, '\f'); break;
    //                case 'n' : PUTC(c, '\n'); break;
    //                case 'r' : PUTC(c, '\r'); break;
    //                case 't' : PUTC(c, '\t'); break;
    //                case 'u' : {
    //                    /* Unicode to Codepoint */
    //                    if ((p = cjson_parse_hex4(p, &hcode)) == NULL) {
    //                        c->top = head;
    //                        return CJSON_PARSE_INVALID_UNICODE_HEX;
    //                    }
    //                    /* Surrogate pair */
    //                    if (hcode >= 0xd800 && hcode <= 0xdbff) {
    //                        if (p[0] != '\\' || p[1] != 'u') {
    //                            c->top = head;
    //                            return CJSON_PARSE_INVALID_UNICODE_SURROGATE;
    //                        }
    //                        /* Skip the "\\u" */
    //                        p += 2;
    //                        if ((p = cjson_parse_hex4(p, &lcode)) == NULL) {
    //                            c->top = head;
    //                            return CJSON_PARSE_INVALID_UNICODE_HEX;
    //                        }
    //                        if (!(lcode >= 0xdc00 && lcode <= 0xdfff)) {
    //                            c->top = head;
    //                            return CJSON_PARSE_INVALID_UNICODE_SURROGATE;
    //                        }
    //                        codepoint = 0x10000 + (hcode - 0xD800) * 0x400 +
    //                            (lcode - 0xDC00);
    //                    } else {
    //                        codepoint = hcode; 
    //                    }
    //                    assert(codepoint >= 0x0 && codepoint <= 0x10ffff);

    //                    /* Codepoint to UTF-8 */
    //                    cjson_encode_utf8(c, codepoint);
    //                    break;
    //                           }
    //                default : {
    //                    if ((unsigned char)ch < 0x20) {
    //                        c->top = head;
    //                        return CJSON_PARSE_INVALID_STRING_ESCAPE;
    //                    }
    //                    PUTC(c, ch);
    //                          }
    //            }
    //            break;
    //                   }
    //        case '\0': {
    //            c->top = head;
    //            return CJSON_PARSE_MISS_QUOTATION_MARK;
    //                   }
    //        default: {
    //            PUTC(c, ch);         
    //                 }
    //    }
    //}
}

static int cjson_parse_string_raw(cjson_context *c, char **str, size_t *len) {
    unsigned int lcode, hcode, codepoint;
    size_t head = c->top;
    EXPECT(c, '\"');
    const char *p = c->json;
    while (1) {
        char ch = *p++;
        switch (ch) {
            case '\"': {
                *len = c->top - head;
                if (len == 0) { /* When the len is 0,
                                   the context stack may be NULL */
                    *str = (char *) malloc(sizeof(char));
                    (*str)[0] = 0;
                } else {
                    *str = (char *)malloc(sizeof(char) * ((*len) + 1));
                    memcpy(*str, cjson_context_pop(c, *len), *len);
                    (*str)[(*len)] = 0;
                }
                c->json = p;
                return CJSON_PARSE_OK;
                       }
            case '\\': {
                switch (*p++) {
                    case '\"' : PUTC(c, '\"'); break;
                    case '\\' : PUTC(c, '\\'); break;
                    case '/' : PUTC(c, '/'); break;
                    case 'b' : PUTC(c, '\b'); break;
                    case 'f' : PUTC(c, '\f'); break;
                    case 'n' : PUTC(c, '\n'); break;
                    case 'r' : PUTC(c, '\r'); break;
                    case 't' : PUTC(c, '\t'); break;
                    case 'u' : {
                        /* Unicode to Codepoint */
                        if ((p = cjson_parse_hex4(p, &hcode)) == NULL) {
                            c->top = head;
                            return CJSON_PARSE_INVALID_UNICODE_HEX;
                        }
                        /* Surrogate pair */
                        if (hcode >= 0xd800 && hcode <= 0xdbff) {
                            if (p[0] != '\\' || p[1] != 'u') {
                                c->top = head;
                                return CJSON_PARSE_INVALID_UNICODE_SURROGATE;
                            }
                            /* Skip the "\\u" */
                            p += 2;
                            if ((p = cjson_parse_hex4(p, &lcode)) == NULL) {
                                c->top = head;
                                return CJSON_PARSE_INVALID_UNICODE_HEX;
                            }
                            if (!(lcode >= 0xdc00 && lcode <= 0xdfff)) {
                                c->top = head;
                                return CJSON_PARSE_INVALID_UNICODE_SURROGATE;
                            }
                            codepoint = 0x10000 + (hcode - 0xD800) * 0x400 +
                                (lcode - 0xDC00);
                        } else {
                            codepoint = hcode; 
                        }
                        assert(codepoint >= 0x0 && codepoint <= 0x10ffff);

                        /* Codepoint to UTF-8 */
                        cjson_encode_utf8(c, codepoint);
                        break;
                               }
                    default : {
                        if ((unsigned char)ch < 0x20) {
                            c->top = head;
                            return CJSON_PARSE_INVALID_STRING_ESCAPE;
                        }
                        PUTC(c, ch);
                              }
                }
                break;
                       }
            case '\0': {
                c->top = head;
                return CJSON_PARSE_MISS_QUOTATION_MARK;
                       }
            default: {
                PUTC(c, ch);         
                     }
        }
    }

}

static int cjson_parse_array(cjson_context *c, cjson_value *v) {
    int res;
    size_t i;
    size_t head = c->top, len;
    EXPECT(c, '[');
    while (1) {
        cjson_value vt;
        cjson_init(&vt);

        /* Skip the white spaces */
        cjson_parse_whitespaces(c);

        /* If the array is empty */
        if (c->json[0] == ']') {
            c->json++;
            v->type = CJSON_ARRAY;
            v->u.a.size = 0;
            v->u.a.e = NULL;
            res = CJSON_PARSE_OK;
            goto lexit;
        }

        if ((res = cjson_parse_value(c, &vt)) != CJSON_PARSE_OK) {
            goto lexit;
        }
        /* Push the json object on the context stack */
        PUTJSON(c, vt);

        /* Skip the white spaces */
        cjson_parse_whitespaces(c);
        
        switch (c->json[0]) {
            case ']' : {
                c->json++;
                len = c->top - head;
                cjson_free(v);
                v->u.a.e = (cjson_value *)malloc(len);
                v->u.a.size = len / sizeof(cjson_value);
                memcpy(v->u.a.e, (const char *)cjson_context_pop(c,
                            len), len);
                v->type = CJSON_ARRAY;
                
                res = CJSON_PARSE_OK;
                return res;
                       }
            case ',' : {
                c->json++;
                cjson_parse_whitespaces(c);
                break;
                       }
            default : {
                res = CJSON_PARSE_MISS_COMMA_OR_BRACKET;
                goto lexit;
                      }
        }
    }
lexit:
    /* Delete the json object stored on the stack so far */
    for (i = 0; i < (c->top - head) / sizeof(cjson_value); ++i) {
        cjson_free((cjson_value *)(cjson_context_pop(c,
                        sizeof(cjson_value))));
    }
    c->top = head;
    return res;
}

static int cjson_parse_object(cjson_context *c, cjson_value *v) {
    int res;
    size_t i;
    size_t head = c->top, len;
    EXPECT(c, '{');
    while (1) {
        cjson_kv kv;

        /* Skip the white spaces */
        cjson_parse_whitespaces(c);

        /* If the object is empty */
        if (c->json[0] == '}') {
            c->json++;
            v->type = CJSON_OBJECT;
            v->u.o.size = 0;
            v->u.o.kvs = NULL;
            res = CJSON_PARSE_OK;
            goto lexit;
        }

        /* Parse the key */
        if ((res = cjson_parse_string_raw(c, &(kv.k), &kv.klen))
                != CJSON_PARSE_OK) {
            res = CJSON_PARSE_MISS_KEY;
            goto lexit;
        }

        /* Skip the white spaces */
        cjson_parse_whitespaces(c);
        /* Expect colon */
        if ((c->json[0]) != ':') {
            res = CJSON_PARSE_MISS_COLON;
            goto lexit;
        }
        c->json++;

        /* Skip the white spaces */
        cjson_parse_whitespaces(c);
        /* Parse value */
        if ((res = cjson_parse_value(c, &(kv.v))) != CJSON_PARSE_OK) {
            goto lexit;
        }
        /* Push the key value pair on the context stack */
        PUTKV(c, kv);

        /* Skip the white spaces */
        cjson_parse_whitespaces(c);

        switch (c->json[0]) {
            case '}' : {
                c->json++;
                len = c->top - head;
                cjson_free(v);
                v->u.o.kvs = (cjson_kv *)malloc(len); 
                v->u.o.size = len / sizeof(cjson_kv);
                memcpy(v->u.o.kvs, (const char *)cjson_context_pop(c,
                            len), len);
                v->type = CJSON_OBJECT;
                res = CJSON_PARSE_OK;
                return res;
                       }
            case ',' : {
                c->json++;
                cjson_parse_whitespaces(c);
                break;
                       }
            default : {
                res = CJSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
                goto lexit;
                      }
        }
    }
lexit:
    /* Delete the key value pair stored on the stack so far */
    for (i = 0; i < (c->top - head) / sizeof(cjson_kv); ++i) {
        cjson_kv_free((cjson_kv *)(cjson_context_pop(c,
                        sizeof(cjson_kv))));
    }
    c->top = head;
    return res;
}

static int cjson_parse_value(cjson_context *c, cjson_value *v) {
    cjson_parse_whitespaces(c);
    switch (*(c->json)) {
        //case 'n' : return cjson_parse_null(c, v);
        //case 't' : return cjson_parse_true(c, v);
        //case 'f' : return cjson_parse_false(c, v);
        case 'n' : return cjson_parse_keyword(c, v, "null", CJSON_NULL);
        case 't' : return cjson_parse_keyword(c, v, "true", CJSON_TRUE);
        case 'f' : return cjson_parse_keyword(c, v, "false", CJSON_FALSE);
        case '\"' : return cjson_parse_string(c, v);
        case '[' : return cjson_parse_array(c, v);
        case '{' : return cjson_parse_object(c, v);
        default : return cjson_parse_number(c, v);
        case 0 : v->type = CJSON_NULL; return CJSON_PARSE_EXPECT_VALUE;
    }
}

int cjson_parse(cjson_value *v, const char *json) {
    int ret;
    cjson_context c;
    assert(v != NULL);
    c.json = json;
    c.stack = NULL;
    c.size = c.top = 0;
    

    cjson_parse_whitespaces(&c);
    if ((ret = cjson_parse_value(&c, v)) == CJSON_PARSE_OK) {

    }
    assert(c.top == 0);
    free(c.stack);
    return ret;
}

static int cjson_stringify_value(cjson_context *c, const cjson_value *v) {
    size_t i;
    int res;
    assert(c != NULL);
    if (v == NULL) {
        return CJSON_STRINGIFY_OK;
    }
    switch (v->type) {
        case CJSON_NULL:
        {
            PUTSTR(c, "null");
            break;
        }
        case CJSON_FALSE:
        {
            PUTSTR(c, "false");
            break;
        }
        case CJSON_TRUE:
        {
            PUTSTR(c, "true");
            break;
        }
        case CJSON_NUMBER:
        {
            char buf[32];
            sprintf(buf, "%.17g", v->u.n);
            PUTSTR(c, buf);
            break;
        }
        case CJSON_STRING:
        {
            PUTC(c, '"');
            PUTSTR_LEN(c, v->u.s.s, v->u.s.len);
            PUTC(c, '"');
            break;
        }
        case CJSON_ARRAY:
        {
            PUTC(c, '[');
            for (i = 0; i < v->u.a.size; i++) {
                res = cjson_stringify_value(c, &v->u.a.e[i]);
                if (res != CJSON_STRINGIFY_OK) {
                    goto lexit;
                }
                if (i != v->u.a.size - 1) {
                    PUTC(c, ',');
                }
            }
            PUTC(c, ']');
            break;
        }
        case CJSON_OBJECT:
        {
            PUTC(c, '{');
            for (i = 0; i < v->u.o.size; i++) {
                PUTC(c, '"');
                PUTSTR_LEN(c, v->u.o.kvs[i].k, v->u.o.kvs[i].klen);
                PUTC(c, '"');
                PUTC(c, ':');
                res = cjson_stringify_value(c, &v->u.o.kvs[i].v);
                if (res != CJSON_STRINGIFY_OK) {
                    goto lexit;
                }
                if (i != v->u.o.size - 1) {
                    PUTC(c, ',');
                }
            }
            PUTC(c, '}');
            break;
        }
        default:
        goto lexit;
    }
    return CJSON_STRINGIFY_OK;
lexit:
    return CJSON_STRINGIFY_FAIL;
}

char *cjson_stringify(const cjson_value *v, size_t *len) {
    int res;
    cjson_context c;
    assert(v != NULL);
    assert(len != NULL);
    /* Init context */
#define CJSON_STRINGIFY_INIT_SIZE 256
    c.stack = (char *)malloc(CJSON_STRINGIFY_INIT_SIZE);
    c.size = CJSON_STRINGIFY_INIT_SIZE;
    c.top = 0;

    if ((res = cjson_stringify_value(&c, v)) != CJSON_STRINGIFY_OK) {
        free(c.stack);
        return NULL;
    }
    if (c.top) {
        *len = c.top;
    }
    c.stack[*len] = 0;
    return c.stack;
}

cjson_type cjson_get_type(const cjson_value *v) {
    assert(v != NULL);
    return v->type;
}

int cjson_get_boolean(const cjson_value *v) {
    assert(v != NULL && (v->type == CJSON_TRUE || v->type == CJSON_FALSE));
    return v->type == CJSON_TRUE ? 1 : 0;
}

void cjson_set_boolean(cjson_value *v, int b) {
    assert(v != NULL);
    cjson_set_null(v);
    v->type = b != 0 ? CJSON_TRUE : CJSON_FALSE;
}

double cjson_get_number(const cjson_value *v) {
    assert(v != NULL && v->type == CJSON_NUMBER);
    return v->u.n;
}

void cjson_set_number(cjson_value *v, double n) {
    assert(v != NULL);
    cjson_set_null(v);
    v->type = CJSON_NUMBER;
    v->u.n = n;
}

const char *cjson_get_string(const cjson_value *v) {
    assert(v != NULL && v->type == CJSON_STRING && v->u.s.s != NULL);
    return v->u.s.s;
}

size_t cjson_get_string_length(const cjson_value *v) {
    assert(v != NULL && v->type == CJSON_STRING && v->u.s.len >= 0);
    return v->u.s.len;
}

size_t cjson_get_array_size(const cjson_value *v) {
    assert(v != NULL && v->type == CJSON_ARRAY);
    return v->u.a.size;
}

cjson_value *cjson_get_array_element(const cjson_value *v, size_t index) {
    assert(v != NULL && v->type == CJSON_ARRAY);
    assert(index < v->u.a.size);
    return v->u.a.e + index;
}

size_t cjson_get_object_size(const cjson_value *v) {
    assert(v != NULL && v->type == CJSON_OBJECT);
    return v->u.o.size;
}

const char *cjson_get_object_key(const cjson_value *v, size_t index) {
    assert(v != NULL && v->type == CJSON_OBJECT);
    assert(index < v->u.o.size);
    return (v->u.o.kvs + index)->k;
}

size_t cjson_get_object_key_length(const cjson_value *v, size_t index) {
    assert(v != NULL && v->type == CJSON_OBJECT);
    assert(index < v->u.o.size);
    return (v->u.o.kvs + index)->klen;
}

cjson_value *cjson_get_object_value(const cjson_value *v, size_t index) {
    assert(v != NULL && v->type == CJSON_OBJECT);
    assert(index < v->u.o.size);
    return &((v->u.o.kvs + index)->v);
}
