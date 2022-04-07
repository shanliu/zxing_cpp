/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_zxing_cpp.h"
#include "zxing_cpp_lib.h"
#include "zxing_common.h"
#include "zxing_read_class.h"
#include "zxing_read_result_class.h"
#include "zxing_image_class.h"
#include "zxing_write_class.h"
#include "zxing_write_result_class.h"
#include "zxing_exception_class.h"
#include "ZXVersion.h"
#include "stb_image.h"

static int le_image,le_read_result,le_write;


int resource_id_get_image(void){
	return le_image;
}
int resource_id_get_read_result(void){
	return le_read_result;
}
int resource_id_get_write_result(void){
	return le_write;
}

static void resource_free_read_result(zend_resource *res)
{
	zxing_release_read_result(res->ptr);
}
static void resource_free_image(zend_resource *res)
{
	stbi_image_free(res->ptr);
}
static void resource_free_write_result(zend_resource *res)
{
	zxing_release_write_result(res->ptr);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zxing_cpp)
{
    le_image = zend_register_list_destructors_ex(resource_free_image, NULL, ZXING_RES_IMAGE, module_number);
    le_read_result = zend_register_list_destructors_ex(resource_free_read_result, NULL, ZXING_RES_READ, module_number);
    le_write = zend_register_list_destructors_ex(resource_free_write_result, NULL, ZXING_RES_WRITE, module_number);
    zxing_exception_class_init();
	zxing_read_class_init();
	zxing_read_result_class_init();
	zxing_write_class_init();
	zxing_write_result_class_init();
	zxing_image_class_init();
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */


/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(zxing_cpp)
{
#if defined(ZTS) && defined(COMPILE_DL_ZXING_CPP)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(zxing_cpp)
{
    char buf[32];
	php_info_print_table_start();
	php_info_print_table_header(2, "zxing_cpp", "enabled");
#ifdef STBI_VERSION
    snprintf(buf, sizeof(buf), "%d", STBI_VERSION);
	php_info_print_table_row(2, "stb version",buf );
#endif
#ifdef ZXING_VERSION_MAJOR
    snprintf(buf, sizeof(buf), "%d.%d.%d", ZXING_VERSION_MAJOR,ZXING_VERSION_MINOR,ZXING_VERSION_PATCH);
    php_info_print_table_row(2, "zxing version",buf );
#endif
	php_info_print_table_end();
}
/* }}} */

/* {{{ zxing_cpp_module_entry */
zend_module_entry zxing_cpp_module_entry = {
	STANDARD_MODULE_HEADER,
	"zxing_cpp",					/* Extension name */
	NULL,					/* zend_function_entry */
	PHP_MINIT(zxing_cpp),							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(zxing_cpp),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(zxing_cpp),			/* PHP_MINFO - Module info */
	PHP_ZXING_CPP_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ZXING_CPP
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(zxing_cpp)
#endif
