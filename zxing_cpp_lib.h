/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef PHP_ZXING_CPP_LIB_H
#define PHP_ZXING_CPP_LIB_H
#include "stb_image.h"
int zxing_image_resize(
        stbi_uc* bg_res,
        stbi_uc* odata,
        int width,
        int height,
        int channel,
        int to_width,
        int to_height,
        zend_long edge,
        zend_long filter);
void zxing_image_const_declare(zend_class_entry *zxing_ce_ptr);
void zxing_read_const_declare(zend_class_entry *zxing_read_ce_ptr);
void zxing_read_result_const_declare(zend_class_entry *zxing_read_result_ce_ptr);
int zxing_read_code(
        stbi_uc* ic,
        int desiredChannels,
        int width,
        int height,
        zend_string * character,
        unsigned char* eanAddOnSymbolVar,
        const unsigned char* binarizerVar,
        zend_bool* isPureVar,
        zend_bool* assumeCode39CheckDigitVar,
        zend_bool* tryCode39ExtendedMode,
        zend_bool* assumeITFCheckDigitVar,
        zend_bool* returnCodabarStartEndVar,
        zend_bool* tryHarderVar,
        zend_bool* tryRotateVar,
        zend_long* formatsVar,
        zend_class_entry *result_ce_ptr,
        zval *return_value
);
void zxing_read_result_size(zval *res,int type,zval *return_value);
void zxing_read_result_text(zval *res,zend_bool angleEscape,zval *return_value);
void zxing_read_result_status(zval *res,zval *return_value);
void zxing_read_result_is_valid(zval *res,zval *return_value);
void zxing_read_result_num_bits(zval *res,zval *return_value);
void zxing_read_result_format(zval *res,zval *return_value);
void zxing_read_result_format_name(zval *res,zval *return_value);
void zxing_read_result_symbology_identifier(zval *res,zval *return_value);
void zxing_read_result_position(zval *res,zval *return_value);
void zxing_read_result_orientation(zval *res,zval *return_value);
void zxing_read_result_ec_level(zval *res,zval *return_value);
void zxing_read_result_line_count(zval *res,zval *return_value);
void zxing_read_result_GTIN_country(zval *res,zval *return_value);
void zxing_read_result_GTIN_price(zval *res,zval *return_value);
void zxing_read_result_GTIN_ean_addon(zval *res,zval *return_value);
void zxing_read_result_GTIN_issueNr(zval *res,zval *return_value);
void zxing_read_result_is_part_of_sequence(zval *res,zval *return_value);
void zxing_read_result_is_last_in_sequence(zval *res,zval *return_value);
void zxing_read_result_compare(zval *res,zval *cp_res,zval *return_value);
void zxing_read_result_get_sequence_index(zval *res,zval *return_value);
void zxing_read_result_get_sequence_size(zval *res,zval *return_value);
void zxing_read_result_get_sequence_id(zval *res,zval *return_value);
void zxing_read_result_is_reader_init(zval *res,zval *return_value);


//write
void zxing_write_const_declare(zend_class_entry *zxing_write_ce_ptr);
void zxing_write_result_const_declare(zend_class_entry *zxing_write_result_ce_ptr);
int zxing_write_create_result(
    int format,
    int width,
    int height,
    int eccLevel,
    int margin,
    zend_long *encoding,
    zend_string *  text,
    zend_class_entry * write_ce_ptr,
    zval *return_value
);
void zxing_write_result_to_file(zval *res,zval *bg_res,unsigned long bgc,unsigned long fc,zend_long type,char* file,zval *return_value,int quality);
void zxing_write_result_to_data(zval *res,zval *bg_res,unsigned long bgc,unsigned long fc,zend_long type,zval *return_value,int quality);



//free
void zxing_release_read_result(void* res);
void zxing_release_write_result(void* res);

#endif

#ifdef __cplusplus
}
#endif

