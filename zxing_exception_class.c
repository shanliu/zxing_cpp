/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "zend.h"
#include "zend_API.h"
#include "zend_exceptions.h"
#include "zxing_read_result_class.h"
#include "zxing_cpp_lib.h"
#include "zxing_common.h"
#include "stb_image.h"

//zend_class_entry *zxing_exception_ce_ptr;
static zend_function_entry zxing_exception_class_method[] = {
        ZEND_FE_END
};

zend_class_entry *zxing_exception_ce_ptr;
void zxing_exception_class_init(){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce,ZXING_NS,"Exception",zxing_exception_class_method);
    zxing_exception_ce_ptr = zend_register_internal_class_ex(&ce, zend_ce_exception);
}

#ifdef __cplusplus
}
#endif
