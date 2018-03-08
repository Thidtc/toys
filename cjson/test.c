#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson.h"

static int test_fail = 0; static int test_pass = 0;

#define EXPECT_EQ_BASE(equation, actual, expectation, format)\
    do {\
        if (equation) {\
            test_pass++;\
        } else {\
            test_fail++;\
            fprintf(stderr, "%s:%d : actual: " format " expect: " format "\n",\
                    __FILE__, __LINE__, actual, expectation);\
        }\
    } while (0)

#define EXPECT_EQ_INT(expr, expectation)\
    EXPECT_EQ_BASE((expr) == (expectation), expr, expectation, "%d")
#define EXPECT_EQ_DOUBLE(expr, expectation)\
    EXPECT_EQ_BASE((expr) == (expectation), expr, expectation, "%lf")
#define EXPECT_EQ_STRING(expr, str, len)\
    EXPECT_EQ_BASE(strncmp((expr), (str), (len)) == 0, (expr), (str), "%s")
#define EXPECT_TRUE(expr)\
    EXPECT_EQ_BASE((expr), (expr), 1, "%d")
#define EXPECT_FALSE(expr)\
    EXPECT_EQ_BASE(!(expr), (expr), 0, "%d")
#define EXPECT_EQ_SIZE_T(expr, expectation)\
    EXPECT_EQ_BASE((expr) == (expectation), (expr), (expectation), "%zu")
#define EXPECT_NEQ_POINTER(expr, expectation)\
    EXPECT_EQ_BASE((expr) != (expectation), (expr), (expectation), "%p")


#define TEST_PARSE_GET(parse_res, type, str)\
    do {\
        cjson_value v;\
        EXPECT_EQ_INT(cjson_parse(&v, str), parse_res);\
        EXPECT_EQ_INT(cjson_get_type(&v), type);\
    } while (0)

static void test_parse_null() {
    TEST_PARSE_GET(CJSON_PARSE_OK, CJSON_NULL, "null");
}

static void test_parse_true() {
    TEST_PARSE_GET(CJSON_PARSE_OK, CJSON_TRUE, "true");
    TEST_PARSE_GET(CJSON_PARSE_OK, CJSON_TRUE, "\ttrue");
}

static void test_parse_false() {
    TEST_PARSE_GET(CJSON_PARSE_OK, CJSON_FALSE, "false");
    TEST_PARSE_GET(CJSON_PARSE_OK, CJSON_FALSE, "\nfalse\r");
}

static void test_parse_empty() {
    TEST_PARSE_GET(CJSON_PARSE_EXPECT_VALUE, CJSON_NULL, " ");
    TEST_PARSE_GET(CJSON_PARSE_EXPECT_VALUE, CJSON_NULL, "");
}

#define TEST_PARSE_NUMBER(value, str)\
    do {\
        cjson_value v;\
        EXPECT_EQ_INT(cjson_parse(&v, str), CJSON_PARSE_OK);\
        EXPECT_EQ_INT(cjson_get_type(&v), CJSON_NUMBER);\
        EXPECT_EQ_DOUBLE(cjson_get_number(&v), value);\
    } while (0)

static void test_parse_number() {
    TEST_PARSE_NUMBER(0., "0");
    TEST_PARSE_NUMBER(0., "-0.0");
    TEST_PARSE_NUMBER(1., "1");
    TEST_PARSE_NUMBER(-1., "-1");
    TEST_PARSE_NUMBER(1.5, "1.5");
    TEST_PARSE_NUMBER(2.56, "2.56");
    TEST_PARSE_NUMBER(1e10, "1e10");
    TEST_PARSE_NUMBER(1e10, "1E10");
    TEST_PARSE_NUMBER(-1e10, "-1e10");
    TEST_PARSE_NUMBER(-1e10, "-1E10");
    TEST_PARSE_NUMBER(1e0, "1e0");
    TEST_PARSE_NUMBER(0., "1e-10000");
}

#define TEST_PARSE_STRING(expectation, str)\
    do {\
        cjson_value v;\
        cjson_init(&v);\
        EXPECT_EQ_INT(cjson_parse(&v, str), CJSON_PARSE_OK);\
        EXPECT_EQ_INT(cjson_get_type(&v), CJSON_STRING);\
        EXPECT_EQ_STRING(expectation, cjson_get_string(&v),\
                cjson_get_string_length(&v));\
        cjson_free(&v);\
    } while (0)

static void test_parse_string() {
    TEST_PARSE_STRING("", "\"\"");
    TEST_PARSE_STRING("Hello", "\"Hello\"");
    TEST_PARSE_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_PARSE_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");

    TEST_PARSE_STRING("\xE2\x82\xAC", "\"\\u20AC\"");    
    TEST_PARSE_STRING("\x24", "\"\\u0024\"");
    TEST_PARSE_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");
    TEST_PARSE_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");
}

static void test_access_string() {
    cjson_value v;
    cjson_init(&v);
    cjson_set_string(&v, "", 0);
    EXPECT_EQ_STRING(cjson_get_string(&v), "",\
            cjson_get_string_length(&v));
    cjson_set_null(&v);
    cjson_set_string(&v, "hello", 0);
    EXPECT_EQ_STRING(cjson_get_string(&v), "hello",\
            cjson_get_string_length(&v));
    cjson_set_null(&v);
}

static void test_access_boolean() {
    cjson_value v;
    cjson_init(&v);
    cjson_set_boolean(&v, 1);
    EXPECT_TRUE(cjson_get_boolean(&v));
    cjson_set_boolean(&v, 0);
    EXPECT_FALSE(cjson_get_boolean(&v));
}

static void test_access_number() {
    cjson_value v;
    cjson_init(&v);
    cjson_set_number(&v, 1.2);
    EXPECT_EQ_DOUBLE(cjson_get_number(&v), 1.2);
    cjson_set_number(&v, 3.1);
    EXPECT_EQ_DOUBLE(cjson_get_number(&v), 3.1);
}

static void test_access_value() {
    cjson_value v;
    cjson_init(&v);
    cjson_set_string(&v, "he", 2);
    EXPECT_EQ_STRING(cjson_get_string(&v), "he",\
            cjson_get_string_length(&v));
    cjson_set_number(&v, 10.01);
    EXPECT_EQ_DOUBLE(cjson_get_number(&v), 10.01);
    cjson_set_boolean(&v, 1);
    EXPECT_TRUE(cjson_get_boolean(&v));
    cjson_set_boolean(&v, 0);
    EXPECT_FALSE(cjson_get_boolean(&v));
} 

#define TEST_ERROR(parse_res, str)\
    do {\
        cjson_value v;\
        EXPECT_EQ_INT(cjson_parse(&v, str), parse_res);\
    } while (0)

static void test_parse_invalid_value() {
    TEST_PARSE_GET(CJSON_PARSE_INVALID_VALUE, CJSON_NULL, "nul");
    TEST_PARSE_GET(CJSON_PARSE_INVALID_VALUE, CJSON_NULL, "?");

    TEST_ERROR(CJSON_PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(CJSON_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(CJSON_PARSE_INVALID_VALUE, ".1");
    TEST_ERROR(CJSON_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(CJSON_PARSE_INVALID_VALUE, "NAN");

    TEST_ERROR(CJSON_PARSE_INVALID_UNICODE_HEX, "\"\\ugggg\"");
    TEST_ERROR(CJSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD834\"");
    TEST_ERROR(CJSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD834\\u0000\"");
    TEST_ERROR(CJSON_PARSE_INVALID_UNICODE_HEX, "\"\\uD834\\uagh?\"");

    cjson_value v;
    cjson_init(&v);
    EXPECT_EQ_INT(CJSON_PARSE_MISS_COMMA_OR_BRACKET, cjson_parse(&v, "[1 "));
    cjson_free(&v);
}

static void test_parse_array() {
    cjson_value v;
    cjson_init(&v);
    EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, "[ ]"));
    EXPECT_EQ_INT(CJSON_ARRAY, cjson_get_type(&v));
    EXPECT_EQ_SIZE_T((size_t)0, cjson_get_array_size(&v));
    cjson_free(&v);

    EXPECT_EQ_INT(cjson_parse(&v, "[\"abc\", [1, 2], 3]"), CJSON_PARSE_OK);
    EXPECT_EQ_INT(cjson_get_type(&v), CJSON_ARRAY);
    EXPECT_EQ_SIZE_T(cjson_get_array_size(&v), (size_t)3);
    cjson_free(&v);

    cjson_init(&v);
    EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, "[ null, false, true, 123, \"abc\" ]"));
    EXPECT_EQ_INT(cjson_get_type(&v), CJSON_ARRAY);
    EXPECT_EQ_SIZE_T(cjson_get_array_size(&v), (size_t)5);
    EXPECT_EQ_INT(cjson_get_type(cjson_get_array_element(&v, 0)), CJSON_NULL);
    EXPECT_EQ_INT(cjson_get_type(cjson_get_array_element(&v, 1)), CJSON_FALSE);
    EXPECT_EQ_INT(cjson_get_type(cjson_get_array_element(&v, 2)), CJSON_TRUE);
    EXPECT_EQ_INT(cjson_get_type(cjson_get_array_element(&v, 3)), CJSON_NUMBER);
    EXPECT_EQ_INT(cjson_get_type(cjson_get_array_element(&v, 4)), CJSON_STRING);
    EXPECT_EQ_DOUBLE(cjson_get_number(cjson_get_array_element(&v, 3)), 123.);
    EXPECT_EQ_STRING(cjson_get_string(cjson_get_array_element(&v, 4)), "abc",
            cjson_get_string_length(cjson_get_array_element(&v, 4)));
    cjson_free(&v);
}

static void test_parse_object() {
    cjson_value v;
    cjson_init(&v);
    EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, "{}"));
    cjson_free(&v);

    EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, "{\"a\":1, \"b\":\"123\"}"));
    EXPECT_EQ_SIZE_T(cjson_get_object_size(&v), (size_t)2);
    cjson_free(&v);

    EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, "{\"a\":false, \"b\":{}, \"cc\":[]}"));
    EXPECT_EQ_SIZE_T(cjson_get_object_size(&v), (size_t)3);
    cjson_free(&v);
}

static void test_stringify() {
#define TEST_STRINGIFY(json)\
    do {\
        cjson_value v;\
        char *json_res = NULL;\
        size_t len;\
        cjson_init(&v);\
        EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, json));\
        json_res = cjson_stringify(&v, &len);\
        EXPECT_NEQ_POINTER(NULL, json_res);\
        EXPECT_EQ_STRING(json, json_res, len);\
        cjson_free(&v);\
        free(json_res);\
    } while (0)

    TEST_STRINGIFY("null");
    TEST_STRINGIFY("false");
    TEST_STRINGIFY("true");
    TEST_STRINGIFY("\"true\"");
    TEST_STRINGIFY("[\"a\",\"b\"]");
    TEST_STRINGIFY("[1,2]");
    TEST_STRINGIFY("{\"a\":1,\"b\":false}");
    TEST_STRINGIFY("{\"a\":1,\"b\":[true,false,null]}");
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_empty();
    test_parse_number();
    test_parse_string();
    test_parse_array();
    test_parse_object();
    test_access_string();
    test_access_boolean();
    test_access_number();
    test_access_value();
    test_parse_invalid_value();
}

int main() {
    test_parse();
    test_stringify();
    printf("TOTAL: %d tests\n", test_fail + test_pass);
    printf("FAIL: %d tests\n", test_fail);
    printf("PASS: %d tests\n", test_pass);

    return 0;
}
