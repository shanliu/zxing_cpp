/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef PHP_ZXING_READ_RESULT_CLASS_H
#define PHP_ZXING_READ_RESULT_CLASS_H

//read result class


ZEND_BEGIN_ARG_INFO_EX(zxing_read_result_controller_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, res)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_result_set_angle_escape_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, angle_escape)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_result_compare_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(zxing_read_result_get_size_escape_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()


extern zend_class_entry *zxing_read_result_ce_ptr;
void zxing_read_result_class_init();



#endif


#ifdef __cplusplus
}
#endif



