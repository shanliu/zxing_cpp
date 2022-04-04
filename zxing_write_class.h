/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef PHP_ZXING_WRITE_CLASS_INFO_H
#define PHP_ZXING_WRITE_CLASS_INFO_H


ZEND_BEGIN_ARG_INFO_EX(zxing_write_controller_arginfo, 0, 0, 3)
    ZEND_ARG_INFO(0, format)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_set_format_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_set_width_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, width)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_set_height_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_set_margin_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, margin)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_set_encoding_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, encoding)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_write_set_ecc_level_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, ecc_level)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID < 70200 
ZEND_BEGIN_ARG_INFO_EX(zxing_write_render_arginfo, 0, 0, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(zxing_write_render_arginfo, 0, 1,IS_OBJECT, 1)
#endif
    ZEND_ARG_TYPE_INFO(0, data,IS_STRING,0)
ZEND_END_ARG_INFO()
extern zend_class_entry *zxing_write_ce_ptr;
void zxing_write_class_init();
#endif

#ifdef __cplusplus
}
#endif



