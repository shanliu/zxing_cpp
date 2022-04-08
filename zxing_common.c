/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "zend.h"
#include "php.h"
#include "zend_API.h"
#include "zend_exceptions.h"
#include "zxing_common.h"
#include "zxing_exception_class.h"



int zxing_new_class(zend_class_entry *ce,zval *return_value,zval *params,int num_args){
    zend_class_entry *old_scope;
    zend_function *constructor;
    if (UNEXPECTED(object_init_ex(return_value, ce) != SUCCESS)) {
        zend_throw_exception_ex(zxing_exception_ce_ptr, -1, "Class %s does not init", ZSTR_VAL(ce->name));
        return 0;
    }
	#if PHP_VERSION_ID < 70100 
	old_scope = EG(scope);
    EG(scope) = ce;
    constructor = Z_OBJ_HT_P(return_value)->get_constructor(Z_OBJ_P(return_value));
    EG(scope) = old_scope;
	#else
	old_scope = EG(fake_scope);
    EG(fake_scope) = ce;
    constructor = Z_OBJ_HT_P(return_value)->get_constructor(Z_OBJ_P(return_value));
    EG(fake_scope) = old_scope;
	#endif
   

    if (constructor) {
        zval ret_val;
        int ret, i;
        zend_fcall_info fci;
        zend_fcall_info_cache fcc;

        for (i = 0; i < num_args; i++) {
            if (Z_REFCOUNTED(params[i])) Z_ADDREF(params[i]);
        }
        fci.size = sizeof(fci);
        ZVAL_UNDEF(&fci.function_name);
        fci.object =NULL;
        fci.retval = &ret_val;
        fci.param_count = num_args;
        fci.params = params;
#if PHP_VERSION_ID < 70100
        fci.function_table=EG(function_table);
        fci.symbol_table=NULL;
#endif
#if PHP_VERSION_ID < 80000
        fci.no_separation=1;
#else
        fci.named_params=NULL;
#endif
        fcc.function_handler = constructor;
        fcc.calling_scope = Z_OBJCE_P(return_value);
#if PHP_VERSION_ID < 70100
		fcc.called_scope = zend_get_called_scope(EG(current_execute_data));    
#else
        fcc.called_scope = zend_get_executed_scope();
#endif
        fcc.object = Z_OBJ_P(return_value);
#if PHP_VERSION_ID<70300
        fcc.initialized = 1;
#endif
        ret = zend_call_function(&fci, &fcc);

        zval_ptr_dtor(&ret_val);
        for (i = 0; i < num_args; i++) {
            zval_ptr_dtor(&params[i]);
        }
        if (ret == FAILURE) {
            php_error_docref(NULL, E_WARNING, "Invocation of %s's constructor failed", ZSTR_VAL(ce->name));
            zval_dtor(return_value);
            return 0;
        }
    }
    return 1;
}

void* zval_get_resource(zval *res,const char *type_name,int type_id) {
    if (Z_TYPE_P(res) != IS_RESOURCE) {
        return NULL;
    }
    return zend_fetch_resource(Z_RES_P(res),type_name,type_id);
}


int zxing_check_image_channels(zend_long desired_channels){
    if (desired_channels!=1
        &&desired_channels!=3
        &&desired_channels!=4){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 50, "your give image channels is invalid:[1,3,4]");
        return 0;
    }
    return 1;
}






void zxing_update_property_bool(zval * z_object,const char *name, size_t name_length, zend_long val){
#if PHP_VERSION_ID < 80000
    zend_update_property_bool(Z_OBJCE_P(z_object), z_object,name,name_length,val);
#else
    zend_update_property_bool(Z_OBJCE_P(z_object), Z_OBJ_P(z_object),name,name_length,val);
#endif
}

void zxing_update_property_str(zval * z_object,const char *name, size_t name_length, zend_string *val){
#if PHP_VERSION_ID < 80000
    zend_update_property_str(Z_OBJCE_P(z_object), z_object,name,name_length,val);
#else
    zend_update_property_str(Z_OBJCE_P(z_object), Z_OBJ_P(z_object),name,name_length,val);
#endif
}


void zxing_update_property(zval * z_object,const char *name, size_t name_length, zval * val){
#if PHP_VERSION_ID < 80000
    zend_update_property(Z_OBJCE_P(z_object), z_object,name,name_length,val);
#else
    zend_update_property(Z_OBJCE_P(z_object), Z_OBJ_P(z_object),name,name_length,val);
#endif
}

void zxing_update_property_long(zval * z_object,const char *name, size_t name_length, zend_long val){
#if PHP_VERSION_ID < 80000
    zend_update_property_long(Z_OBJCE_P(z_object), z_object, name,name_length, val);
#else
    zend_update_property_long(Z_OBJCE_P(z_object), Z_OBJ_P(z_object),name,name_length,val);
#endif
}

zval * zxing_read_property(zval * z_object,const char *name, size_t name_length, zend_bool silent){
#if PHP_VERSION_ID < 80000
    return zend_read_property(Z_OBJCE_P(z_object), z_object, name,name_length, silent, NULL);
#else
    return zend_read_property(Z_OBJCE_P(z_object), Z_OBJ_P(z_object), name,name_length, silent, NULL);
#endif
}


#if PHP_VERSION_ID < 70200 
#define THROW_EXCEPTION_VA_LIST_CREATE(format,code) do{ \
    va_list arg;\
    char *message;\
    va_start(arg, format);\
	zend_throw_exception_ex(zxing_exception_ce_ptr, code,message,arg);\
    va_end(arg);\
    efree(message);\
}while(0)
#else 
#define THROW_EXCEPTION_VA_LIST_CREATE(format,code) do{ \
    va_list arg;\
    char *message;\
    va_start(arg, format);\
    zend_vspprintf(&message, 0, format, arg);\
    va_end(arg);\
    zend_throw_exception(zxing_exception_ce_ptr, message, code);\
    efree(message);\
}while(0)
#endif


void eg_ec_convert_zxing_ec(){
    #if PHP_VERSION_ID >= 72000
        zval* msg=zend_read_property_ex(EG(exception)->ce,EG(exception),ZSTR_KNOWN(ZEND_STR_MESSAGE),1,NULL);
        zval* code=zend_read_property_ex(EG(exception)->ce,EG(exception),ZSTR_KNOWN(ZEND_STR_CODE),1,NULL);
        zval ex;
        object_init_ex(&ex, zxing_exception_ce_ptr);
        zend_update_property_ex(zxing_exception_ce_ptr, Z_OBJ(ex), ZSTR_KNOWN(ZEND_STR_MESSAGE), msg);
        zend_update_property_ex(zxing_exception_ce_ptr, Z_OBJ(ex), ZSTR_KNOWN(ZEND_STR_CODE), code);
        zend_throw_exception_internal(Z_OBJ(ex));
    #else
        zval tmp;
        ZVAL_OBJ(&tmp,EG(exception));
        zval* msg=zend_read_property(EG(exception)->ce,&tmp,"message", sizeof("message")-1,1,NULL);
        zval* code=zend_read_property(EG(exception)->ce,&tmp,"code", sizeof("code")-1,1,NULL);
        zval ex;
        object_init_ex(&ex, zxing_exception_ce_ptr);
        zend_update_property(zxing_exception_ce_ptr,&ex, "message", sizeof("message")-1, msg);
        zend_update_property(zxing_exception_ce_ptr,&ex, "code", sizeof("code")-1, code);
        zend_throw_exception_internal(&ex);
    #endif
}



int object_get_bool(zend_bool *val,zval * object,const char *name, size_t name_length,const char *format, ...){
    zval *tmp= zxing_read_property(object, name,name_length,1);
    if (Z_TYPE_INFO_P(tmp)==IS_TRUE) {
        *val=1;
        return 1;
    }
    if (Z_TYPE_INFO_P(tmp)==IS_FALSE) {
        *val=0;
        return 1;
    }
    THROW_EXCEPTION_VA_LIST_CREATE(format,81);
    return 0;
}

int object_get_bool_or_null(zend_bool **val,zval * object,const char *name, size_t name_length,const char *format, ...){
    zval *tmp= zxing_read_property(object, name,name_length,0);
    if (Z_TYPE_P(tmp) == IS_NULL) {
        *val=NULL;
        return 2;
    }
    va_list arg;
    va_start(arg, format);
    int ret=object_get_bool(*val,object,name,name_length,format,arg);
    va_end(arg);
    return ret;
}

int object_get_gt_long(zend_long *val,zend_long min,zval * object,const char *name, size_t name_length,const char *format, ...){
    zval *tmp= zxing_read_property(object, name,name_length,1);
    if (Z_TYPE_P(tmp) != IS_LONG||Z_LVAL_P(tmp)<=min) {
        THROW_EXCEPTION_VA_LIST_CREATE(format,81);
        return 0;
    }
    *val=Z_LVAL_P(tmp);
    return 1;
}
int object_get_gt_long_or_null(zend_long **val,zend_long min,zval * object,const char *name, size_t name_length,const char *format, ...){
    zval *tmp= zxing_read_property(object, name,name_length,0);
    if (Z_TYPE_P(tmp) == IS_NULL) {
        *val=NULL;
        return 2;
    }
    va_list arg;
    va_start(arg, format);
    int ret=object_get_gt_long(*val,min,object,name,name_length,format,arg);
    va_end(arg);
    return ret;
}
int object_get_long(zend_long *val,zval * object,const char *name, size_t name_length,const char *format, ...){
    zval *tmp= zxing_read_property(object, name,name_length,1);
    if (Z_TYPE_P(tmp) != IS_LONG) {
        THROW_EXCEPTION_VA_LIST_CREATE(format,81);
        return 0;
    }
    *val=Z_LVAL_P(tmp);
    return 1;
}
int object_get_long_or_null(zend_long **val,zval * object,const char *name, size_t name_length,const char *format, ...){
    zval *tmp= zxing_read_property(object, name,name_length,0);
    if (Z_TYPE_P(tmp) == IS_NULL) {
        *val=NULL;
        return 2;
    }
    va_list arg;
    va_start(arg, format);
    int ret=object_get_long(*val,object,name,name_length,format,arg);
    va_end(arg);
    return ret;
}
#ifdef __cplusplus
}
#endif
