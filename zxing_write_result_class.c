/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "zend.h"
#include "ext/standard/file.h"
#include "ext/standard/flock_compat.h"
#include "zend_API.h"
#include "zxing_write_result_class.h"
#include "zxing_common.h"
#include "zend_exceptions.h"
#include "zxing_cpp_lib.h"
#include "zxing_exception_class.h"
#include "zxing_read_class.h"
#include "zxing_image_class.h"
#include <fcntl.h>

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
    php_stream *stream;
    char *filename;
    size_t filename_len;
    zend_long type;
    zend_long quality=0;
    ssize_t numbytes;
    zval *zcontext = NULL;
    php_stream_context *context = NULL;

#if PHP_VERSION_ID < 80000
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "lp|lr!",&type, &filename, &filename_len, &quality, &zcontext) == FAILURE) {
		return;
	}
#else
    ZEND_PARSE_PARAMETERS_START(2, 4)
            Z_PARAM_LONG(type)
            Z_PARAM_PATH(filename, filename_len)
            Z_PARAM_OPTIONAL
            Z_PARAM_LONG(quality)
            Z_PARAM_RESOURCE_OR_NULL(zcontext)
    ZEND_PARSE_PARAMETERS_END();
#endif
    zval data;
    if(!zxing_write_result_to_data(
            zxing_read_property( getThis(),ZEND_STRL("res"),0),
            zxing_read_property( getThis(),ZEND_STRL("background_res"),0),
            (unsigned long)Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("background_color"),0)),
            (unsigned long)Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("front_color"),0)),
            type,
            &data,
            quality
    )){
        RETURN_FALSE;
    }
    context = php_stream_context_from_zval(zcontext,0);
    /* check to make sure we are dealing with a regular file */
    if (php_memnstr(filename, "://", sizeof("://") - 1, filename + filename_len)) {
        if (strncasecmp(filename, "file://", sizeof("file://") - 1)) {
            zval_dtor(&data);
            zend_throw_exception_ex(zxing_exception_ce_ptr, 29,"Exclusive locks may only be set for regular files");
            RETURN_FALSE;
        }
    }
    stream = php_stream_open_wrapper_ex(filename, "cb\0", REPORT_ERRORS, NULL, context);
    if (stream == NULL) {
        zval_dtor(&data);
        if (EG(exception)){
            eg_ec_convert_zxing_ec();
        }else{
            zend_throw_exception_ex(zxing_exception_ce_ptr, 14, "open path error : %s",filename);
        }
        RETURN_FALSE;
    }
    if (!php_stream_supports_lock(stream) || php_stream_lock(stream, LOCK_EX)) {
        php_stream_close(stream);
        zval_dtor(&data);
        zend_throw_exception_ex(zxing_exception_ce_ptr, 46,"Exclusive locks are not supported for this stream");
        RETURN_FALSE;
    }
    php_stream_truncate_set_size(stream, 0);
    numbytes = php_stream_write(stream, Z_STRVAL(data), Z_STRLEN(data));
    if (numbytes != -1 && numbytes != Z_STRLEN(data)) {
        zend_throw_exception_ex(zxing_exception_ce_ptr, 49,"Only %zd of %zd bytes written, possibly out of free disk space", numbytes, Z_STRLEN(data));
        numbytes = -1;
    }
    zval_dtor(&data);
    php_stream_close(stream);
    if (numbytes < 0) {
        RETURN_FALSE;
    }
    RETURN_LONG(numbytes);
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
