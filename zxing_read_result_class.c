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
#include "zxing_read_class.h"
#include "zxing_exception_class.h"
#include "stb_image.h"


//zend_class_entry *zxing_read_ce_ptr;

static void zxing_read_result_property_declare(zend_class_entry *zxing_ce_ptr){
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("res"), ZEND_ACC_PRIVATE);
    zend_declare_property_bool(zxing_ce_ptr, ZEND_STRL("angle_escape"),1, ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("width"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("height"),ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("src_width"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("src_height"), ZEND_ACC_PRIVATE);
}

#define GET_THIS_RES() zxing_read_property( getThis(),ZEND_STRL("res"),1)

ZEND_METHOD(zxing_read_result_class, __construct){
    zval *result;
    zend_long width;
    zend_long height;
    zend_long src_width=0;
    zend_long src_height=0;
    ZEND_PARSE_PARAMETERS_START(3,5)
        Z_PARAM_RESOURCE(result)
        Z_PARAM_LONG(width)
        Z_PARAM_LONG(height)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(src_width)
        Z_PARAM_LONG(src_height)
    ZEND_PARSE_PARAMETERS_END();

    if(width<=0||height<=0){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 100, "your scan image width or height is invalid [w:%ld;h:%ld]",(long)width,(long)height);
        return ;
    }
    if (zval_get_resource(result, ZXING_RES_READ ,resource_id_get_read_result())==NULL){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 100, "your give resource is invalid : %s", ZSTR_VAL(zxing_read_ce_ptr->name));
        return ;
    }
    if (src_width<=0) src_width=width;
    if (src_height<=0) src_height=height;
    zxing_update_property(getThis(),ZEND_STRL("res"),result);
    zxing_update_property_long(getThis(),ZEND_STRL("width"),width);
    zxing_update_property_long(getThis(),ZEND_STRL("height"),height);
    zxing_update_property_long(getThis(),ZEND_STRL("src_width"),src_width);
    zxing_update_property_long(getThis(),ZEND_STRL("src_height"),src_height);
}
ZEND_METHOD(zxing_read_result_class, setAngleEscape){
    PROPERTY_SET_BOOL_ONE_PARAM_BOOL("angle_escape");
}
ZEND_METHOD(zxing_read_result_class, getText){
    zend_bool ac;
    if (!object_get_bool(&ac,getThis(),ZEND_STRL("angle_escape"),"your set angle_escape is wrong"))return ;
    zxing_read_result_text(GET_THIS_RES(),ac,return_value);
}
ZEND_METHOD(zxing_read_result_class, compare){
    zval *read_result;
    ZEND_PARSE_PARAMETERS_START(1,1)
            Z_PARAM_OBJECT_OF_CLASS(read_result,zxing_read_result_ce_ptr)
    ZEND_PARSE_PARAMETERS_END();
    zval *res= zxing_read_property(read_result, ZEND_STRL("res"), 1);
    zxing_read_result_compare(GET_THIS_RES(),res,return_value);
}

#define ZXING_RESULT_PROXY_SIZE_METHOD(method,to_size_field,src_size_filed) ZEND_METHOD(zxing_read_result_class, method){ \
    zend_long pos;                                                                                                        \
    zend_bool revert=0;                                                                                                  \
    ZEND_PARSE_PARAMETERS_START(1,2)                                                                                     \
    Z_PARAM_LONG(pos)                                                                                                   \
    Z_PARAM_OPTIONAL                                                                                                    \
    Z_PARAM_BOOL(revert)                                                                                                \
    ZEND_PARSE_PARAMETERS_END();                                                                                         \
    zend_long to=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL(to_size_field),1));                                  \
    zend_long src=Z_LVAL_P(zxing_read_property(getThis(),ZEND_STRL(src_size_filed),1));                                 \
    zxing_read_result_size(GET_THIS_RES(),pos,revert,(float)src/(float)to,return_value);                                \
}

ZXING_RESULT_PROXY_SIZE_METHOD(getWidth,"width","src_width")
ZXING_RESULT_PROXY_SIZE_METHOD(getHeight,"height","src_height")

#define ZXING_RESULT_PROXY_METHOD(method,fn) ZEND_METHOD(zxing_read_result_class, method){fn(GET_THIS_RES(),return_value);}
ZXING_RESULT_PROXY_METHOD(getStatus,zxing_read_result_status)
ZXING_RESULT_PROXY_METHOD(isValid,zxing_read_result_is_valid)
ZXING_RESULT_PROXY_METHOD(getNumBits,zxing_read_result_num_bits)
ZXING_RESULT_PROXY_METHOD(getFormat,zxing_read_result_format)
ZXING_RESULT_PROXY_METHOD(getFormatName,zxing_read_result_format_name)
ZXING_RESULT_PROXY_METHOD(getSymbologyIdentifier,zxing_read_result_symbology_identifier)
ZXING_RESULT_PROXY_METHOD(getOrientation,zxing_read_result_orientation)
ZXING_RESULT_PROXY_METHOD(getEcLevel,zxing_read_result_ec_level)
ZXING_RESULT_PROXY_METHOD(getLineCount,zxing_read_result_line_count)
ZXING_RESULT_PROXY_METHOD(getGTINCountry,zxing_read_result_GTIN_country)
ZXING_RESULT_PROXY_METHOD(getGTINPrice,zxing_read_result_GTIN_price)
ZXING_RESULT_PROXY_METHOD(getGTINEanAddOn,zxing_read_result_GTIN_ean_addon)
ZXING_RESULT_PROXY_METHOD(getGTINIssueNr, zxing_read_result_GTIN_issueNr)
ZXING_RESULT_PROXY_METHOD(isPartOfSequence,zxing_read_result_is_part_of_sequence)
ZXING_RESULT_PROXY_METHOD(isLastInSequence,zxing_read_result_is_last_in_sequence)
ZXING_RESULT_PROXY_METHOD(getSequenceIndex,zxing_read_result_get_sequence_index)
ZXING_RESULT_PROXY_METHOD(getSequenceId, zxing_read_result_get_sequence_id)
ZXING_RESULT_PROXY_METHOD(getSequenceSize,zxing_read_result_get_sequence_size)
ZXING_RESULT_PROXY_METHOD(isReaderInit,zxing_read_result_is_reader_init)

ZEND_METHOD(zxing_read_result_class, getPosition){
    zend_bool revert=0;
    ZEND_PARSE_PARAMETERS_START(0,1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(revert)
    ZEND_PARSE_PARAMETERS_END();
    zend_long width=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("width"),1));
    zend_long height=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("height"),1));
    zend_long src_width=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("src_width"),1));
    zend_long src_height=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("src_height"),1));
    zxing_read_result_position(GET_THIS_RES(),revert,width,height,src_width,src_height,return_value);
}

static zend_function_entry zxing_read_result_class_method[] = {
    ZEND_ME(zxing_read_result_class, __construct, zxing_read_result_controller_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, setAngleEscape, zxing_read_result_set_angle_escape_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getWidth, zxing_read_result_get_size_escape_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getHeight, zxing_read_result_get_size_escape_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getStatus,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, isValid,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getNumBits,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getText,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getFormat,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getFormatName,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getSymbologyIdentifier,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getPosition,zxing_read_get_position_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getOrientation,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getEcLevel,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getLineCount,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getGTINCountry,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getGTINEanAddOn,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getGTINPrice,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getGTINIssueNr,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, isPartOfSequence,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, isLastInSequence,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, compare,zxing_read_result_compare_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getSequenceIndex,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getSequenceSize,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, getSequenceId,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_read_result_class, isReaderInit,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

  zend_class_entry *zxing_read_result_ce_ptr;
void zxing_read_result_class_init() {
    zend_class_entry cer;
    INIT_NS_CLASS_ENTRY(cer, ZXING_NS, "ReadResult", zxing_read_result_class_method);
    zxing_read_result_ce_ptr = zend_register_internal_class(&cer);
    zxing_read_result_const_declare(zxing_read_result_ce_ptr);
    zxing_read_result_property_declare(zxing_read_result_ce_ptr);
}

#ifdef __cplusplus
}
#endif
