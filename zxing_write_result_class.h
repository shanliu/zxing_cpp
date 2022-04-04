/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef PHP_ZXING_WRITE_RESULT_CLASS_H
#define PHP_ZXING_WRITE_RESULT_CLASS_H

//write result
ZEND_BEGIN_ARG_INFO_EX(zxing_write_result_controller_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, res)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_result_set_color_arginfo, 0, 0,2)
    ZEND_ARG_INFO(0, front_color)
    ZEND_ARG_INFO(0, background_color)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_result_set_background_arginfo, 0, 0,1)
    ZEND_ARG_INFO(0, image_obj)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_result_save_arginfo, 0, 0,2)
    ZEND_ARG_INFO(0, type)
    ZEND_ARG_INFO(0, path)
    ZEND_ARG_INFO(0, quality)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_result_data_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, type)
    ZEND_ARG_INFO(0, quality)
ZEND_END_ARG_INFO()
extern zend_class_entry *zxing_write_result_ce_ptr;
void zxing_write_result_class_init();

#endif


#ifdef __cplusplus
}
#endif



