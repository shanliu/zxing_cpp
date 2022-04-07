/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef PHP_ZXING_IMAGE_CLASS_H
#define PHP_ZXING_IMAGE_CLASS_H

//read image class
#if PHP_VERSION_ID < 70200 
    ZEND_BEGIN_ARG_INFO_EX(zxing_image_load_file_arginfo, 0, 0, 1)
#else
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(zxing_image_load_file_arginfo, 0, 1,IS_OBJECT , 1)
#endif
    ZEND_ARG_TYPE_INFO(0, file, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, desired_channels, IS_LONG,0)
    ZEND_ARG_TYPE_INFO(0, use_include_path, IS_FALSE|IS_TRUE,0)
    ZEND_ARG_TYPE_INFO(0, zcontext, IS_RESOURCE,1)
    ZEND_ARG_TYPE_INFO(0, offset, IS_LONG,0)
    ZEND_ARG_TYPE_INFO(0, maxlen, IS_LONG,1)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID < 70200 
    ZEND_BEGIN_ARG_INFO_EX(zxing_image_load_data_arginfo, 0, 0, 1)
#else
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(zxing_image_load_data_arginfo, 0, 1, IS_OBJECT, 1)
#endif
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, desired_channels, IS_LONG,0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(zxing_image_resize_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, height)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(zxing_image_resize_edge_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, edge)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(zxing_image_resize_filter_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, filter)
ZEND_END_ARG_INFO()



extern zend_class_entry *zxing_image_ce_ptr;
void zxing_image_class_init();

#endif


#ifdef __cplusplus
}
#endif



