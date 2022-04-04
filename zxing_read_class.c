/*
 * Author: shanliu  <shan.liu@msn.com>
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "zend.h"
#include "zend_types.h"
#include "zend_API.h"
#include "zend_exceptions.h"
#include "zxing_read_class.h"
#include "zxing_cpp_lib.h"
#include "zxing_common.h"
#include "stb_image.h"
#include "zxing_exception_class.h"
#include "zxing_image_class.h"
#include "zxing_read_result_class.h"

static void zxing_read_property_declare(zend_class_entry *zxing_ce_ptr){
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("ean_add_on_symbol"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("try_harder"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("try_rotate"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("pure"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("binaries"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("format"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("character"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("code39_extend"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("code39_check"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("check_digit"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("cobra_start_end"), ZEND_ACC_PRIVATE);
}
ZEND_METHOD(zxing_read_class, setEanAddOnSymbol){
    PROPERTY_SET_LONG_ONE_PARAM_LONG("ean_add_on_symbol");
}
ZEND_METHOD(zxing_read_class, setTryHarder){
    PROPERTY_SET_BOOL_ONE_PARAM_BOOL("try_harder");
}
ZEND_METHOD(zxing_read_class, setTryRotate){
    PROPERTY_SET_BOOL_ONE_PARAM_BOOL("try_rotate");
}
ZEND_METHOD(zxing_read_class, setIsPure){
    PROPERTY_SET_BOOL_ONE_PARAM_BOOL("pure");
}
ZEND_METHOD(zxing_read_class, setBinaries){
    PROPERTY_SET_LONG_ONE_PARAM_LONG("binaries");
}
ZEND_METHOD(zxing_read_class, setFormats){
    PROPERTY_SET_LONG_ONE_PARAM_LONG("format");
}
ZEND_METHOD(zxing_read_class, setCharacter){
    PROPERTY_SET_STR_ONE_PARAM_STR("character");
}
ZEND_METHOD(zxing_read_class, setTryCode39ExtendedMode){
    PROPERTY_SET_BOOL_ONE_PARAM_BOOL("code39_extend");
}
ZEND_METHOD(zxing_read_class, setAssumeCode39CheckDigit){
    PROPERTY_SET_BOOL_ONE_PARAM_BOOL("code39_check");
}
ZEND_METHOD(zxing_read_class, setAssumeITFCheckDigit){
    PROPERTY_SET_BOOL_ONE_PARAM_BOOL("check_digit");
}
ZEND_METHOD(zxing_read_class, setReturnCodabarStartEnd){
    PROPERTY_SET_BOOL_ONE_PARAM_BOOL("cobra_start_end");
}



ZEND_METHOD(zxing_read_class, scan){
    zval *result;
    ZEND_PARSE_PARAMETERS_START(1,1)
        Z_PARAM_OBJECT_OF_CLASS(result,zxing_image_ce_ptr)
    ZEND_PARSE_PARAMETERS_END();
    zval *res_= zxing_read_property(result, ZEND_STRL("res"), 1);
    void* res=zval_get_resource(res_, ZXING_RES_IMAGE ,resource_id_get_image());
    if (res==NULL){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 100, "your give image is invalid : %s", ZSTR_VAL(zxing_read_ce_ptr->name));
        return ;
    }
    zend_long width,height,channel;


    if (!object_get_gt_long(&width,0, result,ZEND_STRL("width"),"your give image width is invalid"))return ;
    if (!object_get_gt_long(&height,0,result,ZEND_STRL("height"),"your give height width is invalid"))return ;
    if (!object_get_gt_long(&channel,0,result,ZEND_STRL("channel"),"your give image channel is invalid"))return ;
    if (!zxing_check_image_channels(channel)) return ;

    zval *character_= zxing_read_property(getThis(), ZEND_STRL("character"), 0);

    zend_string * character=NULL;
    if (Z_TYPE_P(character_) == IS_STRING && Z_STRLEN_P(character_)>0){
        character= Z_STR_P(character_);
    }

    zend_long
        ean_add_on_symbol=0,
        binaries=0,
        format=0;
    zend_long * ean_add_on_symbol_p=&ean_add_on_symbol;
    zend_long * binaries_p=&binaries;
    zend_long * format_p=&format;
    if (!object_get_gt_long_or_null(&ean_add_on_symbol_p,0,getThis(),ZEND_STRL("ean_add_on_symbol"),"your set ean_add_on_symbol is wrong"))return ;
    if (!object_get_long_or_null(&binaries_p,getThis(),ZEND_STRL("binaries"),"your set binaries is wrong"))return ;
    if (!object_get_gt_long_or_null(&format_p,0,getThis(),ZEND_STRL("format"),"your set format is wrong"))return ;

    zend_bool
        try_harder=0,
        try_rotate=0,
        pure=0,
        code39_extend=0,
        code39_check=0,
        check_digit=0,
        cobra_start_end=0;
    zend_bool* try_harder_p=&try_harder;
    zend_bool* try_rotate_p=&try_rotate;
    zend_bool* pure_p=&pure;
    zend_bool* code39_extend_p=&code39_extend;
    zend_bool* code39_check_p=&code39_check;
    zend_bool* check_digit_p=&check_digit;
    zend_bool* cobra_start_end_p=&cobra_start_end;
    if (!object_get_bool_or_null(&try_harder_p,getThis(),ZEND_STRL("try_harder"),"your set try_harder is wrong"))return ;
    if (!object_get_bool_or_null(&try_rotate_p,getThis(),ZEND_STRL("try_rotate"),"your set try_rotate is wrong"))return ;
    if (!object_get_bool_or_null(&pure_p,getThis(),ZEND_STRL("pure"),"your set pure is wrong"))return ;
    if (!object_get_bool_or_null(&code39_extend_p,getThis(),ZEND_STRL("code39_extend"),"your set code39_extend is wrong"))return ;
    if (!object_get_bool_or_null(&code39_check_p,getThis(),ZEND_STRL("code39_check"),"your set code39_check is wrong"))return ;
    if (!object_get_bool_or_null(&check_digit_p,getThis(),ZEND_STRL("check_digit"),"your set check_digit is wrong"))return ;
    if (!object_get_bool_or_null(&cobra_start_end_p,getThis(),ZEND_STRL("cobra_start_end"),"your set cobra_start_end is wrong"))return ;
    zval out;

    zxing_read_code(
        (stbi_uc*)res,
        channel,
        width,
        height,
        character,
        ( unsigned char*)ean_add_on_symbol_p,
        ( unsigned char*)binaries_p,
        pure_p,
        code39_check_p,
        code39_extend_p,
        check_digit_p,
        cobra_start_end_p,
        try_harder_p,
        try_rotate_p,
        format_p,
        zxing_read_result_ce_ptr,
        &out
    );
    RETURN_ZVAL(&out,1,1);
}
static zend_function_entry zxing_read_class_method[] = {
    ZEND_ME(zxing_read_class, setEanAddOnSymbol,zxing_read_set_ean_add_on_symbol_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setTryHarder,zxing_read_set_try_harder_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setTryRotate,zxing_read_set_try_rotate_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setIsPure,zxing_read_set_is_pure_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setBinaries,zxing_read_set_binaries_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setFormats,zxing_read_set_formats_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setCharacter,zxing_read_set_character_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setTryCode39ExtendedMode,zxing_read_set_try_code39_extended_mode_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setAssumeCode39CheckDigit,zxing_read_set_assume_code39_check_digit_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setAssumeITFCheckDigit,zxing_read_set_assume_ITF_check_digit_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, setReturnCodabarStartEnd,zxing_read_set_return_codabar_start_end_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_class, scan,zxing_read_scan_arginfo,ZEND_ACC_PUBLIC)
    ZEND_FE_END
};


 zend_class_entry *zxing_read_ce_ptr;
void zxing_read_class_init() {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, ZXING_NS, "Read", zxing_read_class_method);
    zxing_read_ce_ptr = zend_register_internal_class(&ce);
    zxing_read_property_declare(zxing_read_ce_ptr);
    zxing_read_const_declare(zxing_read_ce_ptr);
}


#ifdef __cplusplus
}
#endif
