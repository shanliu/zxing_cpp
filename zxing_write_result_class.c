/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "zend.h"
#include "zend_API.h"
#include "zxing_write_result_class.h"
#include "zxing_common.h"
#include "zend_exceptions.h"
#include "zxing_cpp_lib.h"
#include "zxing_exception_class.h"
#include "zxing_read_class.h"
#include "zxing_image_class.h"


static void zxing_write_result_property_declare(zend_class_entry *zxing_ce_ptr) {
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("background_res"),ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("res"), ZEND_ACC_PRIVATE);
    zend_declare_property_long(zxing_ce_ptr, ZEND_STRL("background_color"), 0xFF000000,ZEND_ACC_PRIVATE);
    zend_declare_property_long(zxing_ce_ptr, ZEND_STRL("front_color"), 0xFFFFFFFF,ZEND_ACC_PRIVATE);
}

ZEND_METHOD(zxing_write_result_class, __construct){
    zval *result;
    ZEND_PARSE_PARAMETERS_START(1,1)
            Z_PARAM_RESOURCE(result)
    ZEND_PARSE_PARAMETERS_END();
    if (zval_get_resource(result, ZXING_RES_WRITE ,resource_id_get_write_result())==NULL){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 100, "your give resource is invalid : %s", ZSTR_VAL(zxing_write_result_ce_ptr->name));
        return ;
    }
    zxing_update_property(getThis(),ZEND_STRL("res"),result);
}

ZEND_METHOD(zxing_write_result_class, setBackground){
    zval *image_obj;
    ZEND_PARSE_PARAMETERS_START(1,1)
        Z_PARAM_OBJECT_OF_CLASS(image_obj,zxing_image_ce_ptr)
    ZEND_PARSE_PARAMETERS_END();
    zxing_update_property(getThis(),ZEND_STRL("background_res"),image_obj);
    RETURN_ZVAL(getThis(),1,0);
}

ZEND_METHOD(zxing_write_result_class, setColor){
    zend_long front_color;
    zend_long background_color;
    ZEND_PARSE_PARAMETERS_START(2,2)
            Z_PARAM_LONG(front_color)
            Z_PARAM_LONG(background_color)
    ZEND_PARSE_PARAMETERS_END();
    zxing_update_property_long(getThis(),ZEND_STRL("front_color"),front_color);
    zxing_update_property_long(getThis(),ZEND_STRL("background_color"),background_color);
    RETURN_ZVAL(getThis(),1,0);
}

ZEND_METHOD(zxing_write_result_class, save){
    char *file;
    size_t file_len;
    zend_long type;
    zend_long quality=0;
    ZEND_PARSE_PARAMETERS_START(2,3)
        Z_PARAM_LONG(type)
        Z_PARAM_STRING(file,file_len)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(quality)
    ZEND_PARSE_PARAMETERS_END();

    zxing_write_result_to_file(
        zxing_read_property( getThis(),ZEND_STRL("res"),1),
        zxing_read_property( getThis(),ZEND_STRL("background_res"),0),
        (unsigned long)Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("background_color"),1)),
        (unsigned long)Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("front_color"),1)),
        type,
        file,
        return_value,
        quality
    );
}
ZEND_METHOD(zxing_write_result_class, data){
    zend_long type;
    zend_long quality=0;
    ZEND_PARSE_PARAMETERS_START(1,2)
            Z_PARAM_LONG(type)
            Z_PARAM_OPTIONAL
            Z_PARAM_LONG(quality)
    ZEND_PARSE_PARAMETERS_END();



    zxing_write_result_to_data(
            zxing_read_property( getThis(),ZEND_STRL("res"),0),
            zxing_read_property( getThis(),ZEND_STRL("background_res"),0),
            (unsigned long)Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("background_color"),0)),
            (unsigned long)Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("front_color"),0)),
        type,
        return_value,
        quality
    );
}
static zend_function_entry zxing_write_result_class_method[] = {
    ZEND_ME(zxing_write_result_class,__construct, zxing_write_result_controller_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_write_result_class,setColor, zxing_write_result_set_color_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_write_result_class,setBackground, zxing_write_result_set_background_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_write_result_class,save, zxing_write_result_save_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_write_result_class,data,zxing_write_result_data_arginfo,ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

 zend_class_entry *zxing_write_result_ce_ptr;
void zxing_write_result_class_init() {
    zend_class_entry cer;
    INIT_NS_CLASS_ENTRY(cer, ZXING_NS, "WriteResult", zxing_write_result_class_method);
    zxing_write_result_ce_ptr = zend_register_internal_class(&cer);
    zxing_write_result_property_declare(zxing_write_result_ce_ptr);
    zxing_write_result_const_declare(zxing_write_result_ce_ptr);
}

#ifdef __cplusplus
}
#endif
