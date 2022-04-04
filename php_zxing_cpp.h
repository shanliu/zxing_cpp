/*
 * Author: shanliu  <shan.liu@msn.com>
 */
/* zxing_cpp extension for PHP */
#ifndef PHP_ZXING_CPP_H
#define PHP_ZXING_CPP_H

extern zend_module_entry zxing_cpp_module_entry;
# define PHP_ZXING_CPP_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_ZXING_CPP)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_ZXING_CPP_H */


