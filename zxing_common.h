/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef PHP_ZXING_COMMON_H
#define PHP_ZXING_COMMON_H
#include "php.h"

#define ZXING_NS  "ZXing"
#define ZXING_RES_IMAGE "zxing_image"
#define ZXING_RES_READ "zxing_read"
#define ZXING_RES_WRITE "zxing_write"

ZEND_BEGIN_ARG_INFO_EX(zxing_none_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

int resource_id_get_image(void);
int resource_id_get_read_result(void);
int resource_id_get_write_result(void);
void* zval_get_resource(zval *res,const char *type_name,int type_id);
int zxing_new_class(zend_class_entry *ce,zval *return_value,zval *params,int num_args);
void eg_ec_convert_zxing_ec();
int zxing_check_image_channels(zend_long desired_channels);


#define PROPERTY_SET_BOOL_ONE_PARAM_BOOL(property_name) do {			\
		zend_bool val;  \
        ZEND_PARSE_PARAMETERS_START(1, 1) \
        Z_PARAM_BOOL(val)   \
        ZEND_PARSE_PARAMETERS_END();    \
        zxing_update_property_bool(getThis(),ZEND_STRL(property_name),val);  \
        RETURN_ZVAL(getThis(),1,0); \
	} while (0)

#define PROPERTY_SET_LONG_ONE_PARAM_LONG(property_name) do {			\
        zend_long val;  \
        ZEND_PARSE_PARAMETERS_START(1, 1) \
        Z_PARAM_LONG(val) \
        ZEND_PARSE_PARAMETERS_END(); \
        zxing_update_property_long(getThis(),ZEND_STRL(property_name),val); \
        RETURN_ZVAL(getThis(),1,0);\
    } while (0)


#define PROPERTY_SET_STR_ONE_PARAM_STR(property_name) do {			\
        zend_string* val;  \
        ZEND_PARSE_PARAMETERS_START(1, 1) \
        Z_PARAM_STR(val) \
        ZEND_PARSE_PARAMETERS_END(); \
        zxing_update_property_str(getThis(),ZEND_STRL(property_name),val); \
        RETURN_ZVAL(getThis(),1,0);\
    } while (0)

void zxing_update_property_bool(zval * z_object,const char *name, size_t name_length, zend_long val);
void zxing_update_property_str(zval * z_object,const char *name, size_t name_length, zend_string *val);
void zxing_update_property(zval * z_object,const char *name, size_t name_length, zval * val);
void zxing_update_property_long(zval * z_object,const char *name, size_t name_length, zend_long val);
zval * zxing_read_property(zval * z_object,const char *name, size_t name_length, zend_bool silent);
int object_get_bool(zend_bool *val,zval * object,const char *name, size_t name_length,const char *format, ...);
int object_get_bool_or_null(zend_bool **val,zval * object,const char *name, size_t name_length,const char *format, ...);
int object_get_long(zend_long *val,zval * object,const char *name, size_t name_length,const char *format, ...);
int object_get_long_or_null(zend_long **val,zval * object,const char *name, size_t name_length,const char *format, ...);
int object_get_gt_long(zend_long *val,zend_long min,zval* object,const char *name, size_t name_length,const char *format, ...);
int object_get_gt_long_or_null(zend_long **val,zend_long min,zval* object,const char *name, size_t name_length,const char *format, ...);
#endif


#ifdef __cplusplus
}
#endif
