/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef PHP_ZXING_READ_CLASS_H
#define PHP_ZXING_READ_CLASS_H

//read class

ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_ean_add_on_symbol_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, ean_add_on_symbol)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_try_harder_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, harder)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_try_rotate_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, rotate)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_is_pure_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, pure)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_binaries_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, binarizer)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_formats_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_character_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, character)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_try_code39_extended_mode_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, code39_extend)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_assume_code39_check_digit_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, code39_check)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_assume_ITF_check_digit_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, check_digit)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(zxing_read_set_return_codabar_start_end_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, coba_start_end)
ZEND_END_ARG_INFO()


#if PHP_VERSION_ID < 70200 
ZEND_BEGIN_ARG_INFO_EX(zxing_read_scan_arginfo, 0, 0, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(zxing_read_scan_arginfo, 0, 1,IS_ARRAY, 1)
#endif
    ZEND_ARG_TYPE_INFO(0, image,IS_OBJECT,0)
ZEND_END_ARG_INFO()

extern zend_class_entry *zxing_read_ce_ptr;
void zxing_read_class_init();


#endif


#ifdef __cplusplus
}
#endif



