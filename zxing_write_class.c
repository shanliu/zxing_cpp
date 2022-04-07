/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "zend.h"
#include "zend_API.h"
#include "zxing_write_class.h"
#include "zxing_common.h"
#include "zend_exceptions.h"
#include "zxing_cpp_lib.h"
#include "zxing_exception_class.h"
#include "zxing_write_result_class.h"

static void zxing_write_property_declare(zend_class_entry *zxing_ce_ptr) {
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("format"), ZEND_ACC_PRIVATE);
    zend_declare_property_long(zxing_ce_ptr, ZEND_STRL("width"),100, ZEND_ACC_PRIVATE);
    zend_declare_property_long(zxing_ce_ptr, ZEND_STRL("height"),100, ZEND_ACC_PRIVATE);
    zend_declare_property_long(zxing_ce_ptr, ZEND_STRL("margin"),0, ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("encoding"),ZEND_ACC_PRIVATE);
    zend_declare_property_long(zxing_ce_ptr, ZEND_STRL("ecc_level"),0, ZEND_ACC_PRIVATE);
}

#define ZXING_WRITE_CHECK_AND_SET(name) do{ \
    if (name<=0) {\
    zend_throw_exception_ex(zxing_exception_ce_ptr, 90, "your give %s is invalid",#name);\
    return;\
    }\
    zxing_update_property_long(getThis(),ZEND_STRL(#name),name);\
}while(0)

ZEND_METHOD(zxing_write_class, __construct){
    zend_long width=100,height=100,format;
    ZEND_PARSE_PARAMETERS_START(1,3)
            Z_PARAM_LONG(format)
            Z_PARAM_OPTIONAL
            Z_PARAM_LONG(width)
            Z_PARAM_LONG(height)
    ZEND_PARSE_PARAMETERS_END();
    ZXING_WRITE_CHECK_AND_SET(width);
    ZXING_WRITE_CHECK_AND_SET(height);
    ZXING_WRITE_CHECK_AND_SET(format);
}

#define ZXING_WRITE_METHOD_LONG(method,name) ZEND_METHOD(zxing_write_class, method){ \
    zend_long name; \
    ZEND_PARSE_PARAMETERS_START(1,1) \
    Z_PARAM_LONG(name) \
    ZEND_PARSE_PARAMETERS_END(); \
    ZXING_WRITE_CHECK_AND_SET(name);                                                 \
    RETURN_ZVAL(getThis(),1,0);                                                \
}

ZXING_WRITE_METHOD_LONG(setFormat,format)
ZXING_WRITE_METHOD_LONG(setEncoding,encoding)
ZXING_WRITE_METHOD_LONG(setWidth,width)
ZXING_WRITE_METHOD_LONG(setHeight,height)
ZEND_METHOD(zxing_write_class, setMargin){
    PROPERTY_SET_LONG_ONE_PARAM_LONG("margin");
}
ZEND_METHOD(zxing_write_class, setEccLevel){
    PROPERTY_SET_LONG_ONE_PARAM_LONG("ecc_level");
}
ZEND_METHOD(zxing_write_class, render){
    zend_string* val;
    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(val)
    ZEND_PARSE_PARAMETERS_END();
    if (ZSTR_LEN(val)<=0){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 35, "data can't be empty");
        return;
    }
    zend_long width,height,format,eccLevel,margin,encoding=0;
    zend_long* encoding_p=&encoding;
    if (!object_get_gt_long(&width,10, getThis(),ZEND_STRL("width"),"your give width is invalid"))return ;
    if (!object_get_gt_long(&height,10,getThis(),ZEND_STRL("height"),"your give width is invalid"))return ;
    if (!object_get_gt_long(&format,0,getThis(),ZEND_STRL("format"),"your give format is invalid"))return ;
    if (!object_get_long(&eccLevel,getThis(),ZEND_STRL("ecc_level"),"your give ecc_level is invalid"))return ;
    if (!object_get_long_or_null(&encoding_p,getThis(),ZEND_STRL("encoding"),"your give encoding is invalid"))return ;
    if (!object_get_long(&margin,getThis(),ZEND_STRL("margin"),"your give margin is invalid"))return ;
    zxing_write_create_result(
        format,
        width,
        height,
        eccLevel,
        margin,
        encoding_p,
        val,
        zxing_write_result_ce_ptr,
        return_value
    );

}
static zend_function_entry zxing_write_class_method[] = {
        ZEND_ME(zxing_write_class,__construct, zxing_write_controller_arginfo, ZEND_ACC_PUBLIC)
        ZEND_ME(zxing_write_class, setFormat,zxing_write_set_format_arginfo,ZEND_ACC_PUBLIC)
        ZEND_ME(zxing_write_class, setWidth,zxing_write_set_width_arginfo,ZEND_ACC_PUBLIC)
        ZEND_ME(zxing_write_class, setHeight,zxing_write_set_height_arginfo,ZEND_ACC_PUBLIC)
        ZEND_ME(zxing_write_class, setMargin,zxing_write_set_margin_arginfo,ZEND_ACC_PUBLIC)
        ZEND_ME(zxing_write_class, setEncoding,zxing_write_set_encoding_arginfo,ZEND_ACC_PUBLIC)
        ZEND_ME(zxing_write_class, setEccLevel,zxing_write_set_ecc_level_arginfo,ZEND_ACC_PUBLIC)
        ZEND_ME(zxing_write_class, render,zxing_write_render_arginfo,ZEND_ACC_PUBLIC)
        ZEND_FE_END
};

 zend_class_entry *zxing_write_ce_ptr;
void zxing_write_class_init() {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, ZXING_NS, "Write", zxing_write_class_method);
    zxing_write_ce_ptr = zend_register_internal_class(&ce);
    zxing_write_property_declare(zxing_write_ce_ptr);
    zxing_write_const_declare(zxing_write_ce_ptr);
}


#ifdef __cplusplus
}
#endif

