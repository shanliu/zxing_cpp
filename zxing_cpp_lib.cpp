
#include <cctype>
#include <chrono>
#include <memory>
#include <stdlib.h>
#include "php.h"
#include "zend_exceptions.h"

#ifdef PHP_WIN32
#pragma push_macro(min)
#undef min
#pragma push_macro(max)
#undef max
#endif

#include "ReadBarcode.h"
#include "zxing_cpp_lib.h"
#include "TextUtfEncoding.h"
#include "GTIN.h"
#include "BarcodeFormat.h"
#include "BitMatrix.h"
#include "MultiFormatWriter.h"
#include "CharacterSetECI.h"

#ifdef PHP_WIN32
#pragma pop_macro(max)
#pragma pop_macro(min)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "zxing_common.h"
#include "zxing_exception_class.h"

using namespace ZXing;
using namespace TextUtfEncoding;


zend_string * zxing_cpp_str_to_zend_string(std::string &cpp_str){
    size_t len=cpp_str.length();
    if (len==0){
        return ZSTR_EMPTY_ALLOC();
    }
    size_t mlen=sizeof(char)*len+1;
    char* cstr=(char*)malloc(mlen);
    memcpy(cstr,cpp_str.c_str(),len+1);
    cstr[len]='\0';
    zend_string * zs= zend_string_init(cstr,len,0);
    free(cstr);
    return zs;
}

//read
#ifdef __cplusplus
extern "C" {
#endif
void zxing_read_const_declare(zend_class_entry *zxing_read_ce_ptr) {
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("EanAddOnSymbolIgnore"), (zend_long)EanAddOnSymbol::Ignore);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("EanAddOnSymbolRead"), (zend_long)EanAddOnSymbol::Read);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("EanAddOnSymbolRequire"), (zend_long)EanAddOnSymbol::Require);

    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BinarizerBoolCast"), (zend_long)Binarizer::BoolCast);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BinarizerFixedThreshold"), (zend_long)Binarizer::FixedThreshold);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BinarizerGlobalHistogram"), (zend_long)Binarizer::GlobalHistogram);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BinarizerLocalAverage"), (zend_long)Binarizer::LocalAverage);

    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatAztec"), (zend_long)BarcodeFormat::Aztec);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatCodabar"), (zend_long)BarcodeFormat::Codabar);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatCode39"), (zend_long)BarcodeFormat::Code39);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatCode93"), (zend_long)BarcodeFormat::Code93);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatCode128"), (zend_long)BarcodeFormat::Code128);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatDataBar"), (zend_long)BarcodeFormat::DataBar);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatDataBarExpanded"), (zend_long)BarcodeFormat::DataBarExpanded);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatDataMatrix"), (zend_long)BarcodeFormat::DataMatrix);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatEAN8"), (zend_long)BarcodeFormat::EAN8);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatEAN13"), (zend_long)BarcodeFormat::EAN13);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatITF"), (zend_long)BarcodeFormat::ITF);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatMaxiCode"), (zend_long)BarcodeFormat::MaxiCode);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatPDF417"), (zend_long)BarcodeFormat::PDF417);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatQRCode"), (zend_long)BarcodeFormat::QRCode);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatUPCA"), (zend_long)BarcodeFormat::UPCA);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatUPCE"), (zend_long)BarcodeFormat::UPCE);
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatOneDCodes"), (zend_long)BarcodeFormat::OneDCodes);// = Codabar | Code39 | Code93 | Code128 | EAN8 | EAN13 | ITF | DataBar | DataBarExpanded | UPCA | UPCE,
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatTwoDCodes"), (zend_long)BarcodeFormat::TwoDCodes);// = Aztec | DataMatrix | MaxiCode | PDF417 | QRCode,
    zend_declare_class_constant_long(zxing_read_ce_ptr,ZEND_STRL("BarcodeFormatAny"), (zend_long)BarcodeFormat::Any);
}

#define ZXING_READ_RESULT_TYPE_WIDTH_TOP 1
#define ZXING_READ_RESULT_TYPE_WIDTH_BOTTOM 2
#define ZXING_READ_RESULT_TYPE_HEIGHT_LEFT 3
#define ZXING_READ_RESULT_TYPE_HEIGHT_RIGHT 4

void zxing_read_result_const_declare(zend_class_entry *zxing_read_result_ce_ptr) {
    zend_declare_class_constant_long(zxing_read_result_ce_ptr,ZEND_STRL("StatusChecksumError"), (zend_long)DecodeStatus::ChecksumError);
    zend_declare_class_constant_long(zxing_read_result_ce_ptr,ZEND_STRL("StatusFormatError"), (zend_long)DecodeStatus::FormatError);
    zend_declare_class_constant_long(zxing_read_result_ce_ptr,ZEND_STRL("StatusNoError"), (zend_long)DecodeStatus::NoError);
    zend_declare_class_constant_long(zxing_read_result_ce_ptr,ZEND_STRL("StatusNotFound"), (zend_long)DecodeStatus::NotFound);

    zend_declare_class_constant_long(zxing_read_result_ce_ptr,ZEND_STRL("WidthTop"), ZXING_READ_RESULT_TYPE_WIDTH_TOP);
    zend_declare_class_constant_long(zxing_read_result_ce_ptr,ZEND_STRL("WidthBottom"), ZXING_READ_RESULT_TYPE_WIDTH_BOTTOM);
    zend_declare_class_constant_long(zxing_read_result_ce_ptr,ZEND_STRL("HeightLeft"), ZXING_READ_RESULT_TYPE_HEIGHT_LEFT);
    zend_declare_class_constant_long(zxing_read_result_ce_ptr,ZEND_STRL("HeightRight"), ZXING_READ_RESULT_TYPE_HEIGHT_RIGHT);

}

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
        zend_bool* codabarStartEndVar,
        zend_bool* tryHarderVar,
        zend_bool* tryRotateVar,
        zend_long* formatsVar,
        zend_class_entry *result_ce_ptr,
        zval *return_value
){
    DecodeHints hints;
    if (formatsVar!=nullptr){
        hints.setFormats(BarcodeFormat(*formatsVar));
    }
    if (eanAddOnSymbolVar!= nullptr){
        auto eanAddOnSymbolSet=EanAddOnSymbol(*eanAddOnSymbolVar);
        if (EanAddOnSymbol::Ignore==eanAddOnSymbolSet
            || EanAddOnSymbol::Read==eanAddOnSymbolSet
            || EanAddOnSymbol::Require==eanAddOnSymbolSet){
            hints.setEanAddOnSymbol(eanAddOnSymbolSet);
        }else{
            zend_throw_exception_ex(zxing_exception_ce_ptr, 61, "eanAddOnSymbol not support");
            return -1;
        }
    }
    if (binarizerVar!= nullptr) {
        auto binarizer = Binarizer(*binarizerVar);
        if (Binarizer::BoolCast == binarizer
            || Binarizer::FixedThreshold == binarizer
            || Binarizer::GlobalHistogram == binarizer
            || Binarizer::LocalAverage == binarizer
                ) {
            hints.setBinarizer(binarizer);
        } else {
            zend_throw_exception_ex(zxing_exception_ce_ptr, 61, "binarizer not support");
            return -2;
        }
    }
    auto format=ImageFormat::None;
    switch (desiredChannels) {
        case 1:
            format = ImageFormat::Lum;
            break;
        case 3:
            format = ImageFormat::RGB;
            break;
        case 4:
            format = ImageFormat::RGBX;
            break;
        default:
            zend_throw_exception_ex(zxing_exception_ce_ptr, 63, "format not support");
            return -3;
    }
    if ((bool *)isPureVar!= nullptr){
        hints.setIsPure((bool)*isPureVar);
    }
    if ((bool *)assumeCode39CheckDigitVar!= nullptr){
        hints.setAssumeCode39CheckDigit((bool)*assumeCode39CheckDigitVar);
    }
    if ((bool *)assumeCode39CheckDigitVar!= nullptr){
        hints.setTryCode39ExtendedMode((bool)*tryCode39ExtendedMode);
    }
    if ((bool *)assumeITFCheckDigitVar!= nullptr){
        hints.setAssumeITFCheckDigit((bool)*assumeITFCheckDigitVar);
    }
    if ((bool *)codabarStartEndVar!= nullptr){
        hints.setReturnCodabarStartEnd((const bool)*codabarStartEndVar);
    }
    if ((bool *)tryHarderVar!= nullptr){
        hints.setTryHarder((bool)*tryHarderVar);
    }
    if ((bool *)tryRotateVar!=nullptr){
        hints.setTryRotate((bool)*tryRotateVar);
    }
    if (character!= nullptr){
        std::string Character;
        Character.assign(ZSTR_VAL(character), ZSTR_LEN(character));
        hints.setCharacterSet(Character);
    }
    ImageView image{ic, width, height,format};
    auto results= ReadBarcodes(image, hints);

    array_init(return_value);
    for (auto&& result : results) {
        auto res = new Result(result);
        zval param[1];
        zval out;
        ZVAL_RES(&param[0], zend_register_resource(res, resource_id_get_read_result()));
        if (!zxing_new_class(result_ce_ptr, &out, param, 1)) {
            zval_ptr_dtor(&param[0]);
            return 0;
        }
        zval_ptr_dtor(&param[0]);
        if (EG(exception)){
            zval_ptr_dtor(&out);
            return 0;
        }
        add_next_index_zval(return_value, &out);
        //zval_ptr_dtor(&out);
    }
    return 1;
}
#ifdef __cplusplus
}
#endif

static Result* result_zval_get_result(zval *res,zval *return_value) {
    Result* result=(Result*)zval_get_resource(res, ZXING_RES_READ ,resource_id_get_read_result());
    if (result==NULL) {
        ZVAL_NULL(return_value);
        return nullptr;
    }
    return result;
}
#ifdef __cplusplus
extern "C" {
#endif

void zxing_read_result_text(zval *res,zend_bool angleEscape,zval *return_value) {
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    auto cpp_str= ToUtf8(result->text(), angleEscape);
    RETURN_NEW_STR(zxing_cpp_str_to_zend_string(cpp_str));
}
void zxing_read_result_status(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    RETURN_LONG((zend_long)result->status());
}
void zxing_read_result_compare(zval *res,zval *cp_res,zval *return_value){
    auto result1=result_zval_get_result(res,return_value);
    if (result1== nullptr)return;
    if (!result1->isValid()){
        RETURN_FALSE;
    }
    auto result2=result_zval_get_result(cp_res,return_value);
    if (result2== nullptr)return;
    if (!result2->isValid()){
        RETURN_FALSE;
    }
    RETURN_BOOL(*result1==*result2);
}
void zxing_read_result_is_valid(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    RETURN_BOOL(result->isValid());
}
void zxing_read_result_num_bits(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    RETURN_LONG(result->numBits());
}
void zxing_read_result_format_name(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    auto str=ToString(result->format());
    RETURN_NEW_STR(zend_string_init(str, strlen(str),0));
}
void zxing_read_result_format(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    RETURN_LONG((zend_long)result->format());
}
void zxing_read_result_symbology_identifier(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    auto str=result->symbologyIdentifier();
    RETURN_NEW_STR(zxing_cpp_str_to_zend_string(str));
}

#define ZXING_RET_XY_POINT_AYY(name,px,py) do { \
    zval name;\
    array_init(&name);\
    add_assoc_long(&name,"x",px);\
    add_assoc_long(&name,"y",py);\
    add_next_index_zval(return_value,&name);\
}while(0)

void zxing_read_result_position(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    auto pos=result->position();
    array_init(return_value);
    ZXING_RET_XY_POINT_AYY(ret0,pos.topLeft().x,pos.topLeft().y);
    ZXING_RET_XY_POINT_AYY(ret1,pos.topRight().x,pos.topRight().y);
    ZXING_RET_XY_POINT_AYY(ret2,pos.bottomRight().x,pos.bottomRight().y);
    ZXING_RET_XY_POINT_AYY(ret3,pos.bottomLeft().x,pos.bottomLeft().y);
}


void zxing_read_result_size(zval *res,int type,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    auto pos=result->position();
    int w,h;
    switch (type) {
        case ZXING_READ_RESULT_TYPE_WIDTH_TOP:
            w=abs(pos.topRight().x-pos.topLeft().x);
            h=abs(pos.topRight().y-pos.topLeft().y);
            break;
        case ZXING_READ_RESULT_TYPE_WIDTH_BOTTOM:
            w=abs(pos.bottomRight().x-pos.bottomLeft().x);
            h=abs(pos.bottomRight().y-pos.bottomLeft().y);
            break;
        case ZXING_READ_RESULT_TYPE_HEIGHT_LEFT:
            w=abs(pos.topLeft().x-pos.bottomLeft().x);
            h=abs(pos.topLeft().y-pos.bottomLeft().y);
            break;
        case ZXING_READ_RESULT_TYPE_HEIGHT_RIGHT:
            w=abs(pos.topRight().x-pos.bottomRight().x);
            h=abs(pos.topRight().y-pos.bottomRight().y);
            break;
        default:
            zend_throw_exception_ex(zxing_exception_ce_ptr, 100, "your give type is invalid [%d] ",type);
            return ;
    }
    zend_long size;
    size=sqrt(pow(w,2)+pow(h,2));
    RETURN_LONG(size);
}





void zxing_read_result_orientation(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
     RETURN_LONG(result->orientation());
}
void zxing_read_result_ec_level(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    auto str=ToUtf8(result->ecLevel());
    RETURN_NEW_STR(zxing_cpp_str_to_zend_string(str));
}
void zxing_read_result_line_count(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    RETURN_LONG(result->lineCount());
}
void zxing_read_result_GTIN_country(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    if ((BarcodeFormat::EAN13 | BarcodeFormat::EAN8 | BarcodeFormat::UPCA | BarcodeFormat::UPCE).testFlag(result->format())) {
        auto str=GTIN::LookupCountryIdentifier(ToUtf8(result->text()), result->format());
        RETURN_NEW_STR(zxing_cpp_str_to_zend_string(str));
    } else  if (result->format() == BarcodeFormat::ITF && result->text().length() == 14) {
        auto str=GTIN::LookupCountryIdentifier(ToUtf8(result->text()), result->format());
         RETURN_NEW_STR(zxing_cpp_str_to_zend_string(str));
    }else{
        RETURN_NULL();
    }
}
void zxing_read_result_GTIN_price(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    if ((BarcodeFormat::EAN13 | BarcodeFormat::EAN8 | BarcodeFormat::UPCA | BarcodeFormat::UPCE).testFlag(result->format())) {
        auto str=GTIN::Price(GTIN::EanAddOn(*result));
        RETURN_NEW_STR(zxing_cpp_str_to_zend_string(str));
    } else{
        RETURN_NULL();
    }
}
void zxing_read_result_GTIN_ean_addon(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    if ((BarcodeFormat::EAN13 | BarcodeFormat::EAN8 | BarcodeFormat::UPCA | BarcodeFormat::UPCE).testFlag(result->format())) {
        auto str=GTIN::EanAddOn(*result);
        RETURN_NEW_STR(zxing_cpp_str_to_zend_string(str));
    } else{
        RETURN_NULL();
    }
}
void zxing_read_result_GTIN_issueNr(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    if ((BarcodeFormat::EAN13 | BarcodeFormat::EAN8 | BarcodeFormat::UPCA | BarcodeFormat::UPCE).testFlag(result->format())) {
        auto str=GTIN::IssueNr(GTIN::EanAddOn(*result));
        RETURN_NEW_STR(zxing_cpp_str_to_zend_string(str));
    } else{
        RETURN_NULL();
    }
}
void zxing_read_result_is_part_of_sequence(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    RETURN_BOOL(result->isPartOfSequence());
}
void zxing_read_result_is_last_in_sequence(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    RETURN_LONG(result->isLastInSequence());
}

void zxing_read_result_get_sequence_index(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()||!result->isPartOfSequence()){
        RETURN_NULL();
    }
    RETURN_LONG(result->sequenceIndex());
}
void zxing_read_result_get_sequence_id(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()||!result->isPartOfSequence()){
        RETURN_NULL();
    }
    auto str=result->sequenceId();
    RETURN_NEW_STR(zxing_cpp_str_to_zend_string(str));
}
void zxing_read_result_get_sequence_size(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()||!result->isPartOfSequence()){
        RETURN_NULL();
    }
    RETURN_LONG(result->sequenceSize());
}
void zxing_read_result_is_reader_init(zval *res,zval *return_value){
    auto result=result_zval_get_result(res,return_value);
    if (result== nullptr)return;
    if (!result->isValid()){
        RETURN_NULL();
    }
    RETURN_BOOL(result->readerInit());
}

//write

void zxing_write_const_declare(zend_class_entry *zxing_write_ce_ptr){
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatAztec"), (zend_long)BarcodeFormat::Aztec);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatCodabar"), (zend_long)BarcodeFormat::Codabar);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatCode39"), (zend_long)BarcodeFormat::Code39);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatCode93"), (zend_long)BarcodeFormat::Code93);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatCode128"), (zend_long)BarcodeFormat::Code128);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatDataMatrix"), (zend_long)BarcodeFormat::DataMatrix);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatEAN8"), (zend_long)BarcodeFormat::EAN8);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatEAN13"), (zend_long)BarcodeFormat::EAN13);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatITF"), (zend_long)BarcodeFormat::ITF);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatMaxiCode"), (zend_long)BarcodeFormat::MaxiCode);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatPDF417"), (zend_long)BarcodeFormat::PDF417);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatQRCode"), (zend_long)BarcodeFormat::QRCode);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatUPCA"), (zend_long)BarcodeFormat::UPCA);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("BarcodeFormatUPCE"), (zend_long)BarcodeFormat::UPCE);

    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetUnknown"), (zend_long)CharacterSet::Unknown);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetASCII"), (zend_long)CharacterSet::ASCII);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_1"), (zend_long)CharacterSet::ISO8859_1);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_2"), (zend_long)CharacterSet::ISO8859_2);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_3"), (zend_long)CharacterSet::ISO8859_3);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_4"), (zend_long)CharacterSet::ISO8859_4);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_5"), (zend_long)CharacterSet::ISO8859_5);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_6"), (zend_long)CharacterSet::ISO8859_6);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_7"), (zend_long)CharacterSet::ISO8859_7);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_8"), (zend_long)CharacterSet::ISO8859_8);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_9"), (zend_long)CharacterSet::ISO8859_9);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_10"), (zend_long)CharacterSet::ISO8859_10);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_11"), (zend_long)CharacterSet::ISO8859_11);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_13"), (zend_long)CharacterSet::ISO8859_13);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_14"), (zend_long)CharacterSet::ISO8859_14);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_15"), (zend_long)CharacterSet::ISO8859_15);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetISO8859_16"), (zend_long)CharacterSet::ISO8859_16);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetCp437"), (zend_long)CharacterSet::Cp437);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetCp1250"), (zend_long)CharacterSet::Cp1250);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetCp1251"), (zend_long)CharacterSet::Cp1251);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetCp1252"), (zend_long)CharacterSet::Cp1252);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetCp1256"), (zend_long)CharacterSet::Cp1256);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetShift_JIS"), (zend_long)CharacterSet::Shift_JIS);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetBig5"), (zend_long)CharacterSet::Big5);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetGB2312"), (zend_long)CharacterSet::GB2312);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetGB18030"), (zend_long)CharacterSet::GB18030);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetEUC_JP"), (zend_long)CharacterSet::EUC_JP);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetEUC_KR"), (zend_long)CharacterSet::EUC_KR);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetUnicodeBig"), (zend_long)CharacterSet::UnicodeBig);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetUTF8"), (zend_long)CharacterSet::UTF8);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetBINARY"), (zend_long)CharacterSet::BINARY);
    zend_declare_class_constant_long(zxing_write_ce_ptr,ZEND_STRL("CharacterSetCharsetCount"), (zend_long)CharacterSet::CharsetCount);

}
#ifdef __cplusplus
}
#endif

#define IMAGE_TYPE_PNG 1
#define IMAGE_TYPE_JPG 2
#define IMAGE_TYPE_BMP 3

void zxing_write_result_const_declare(zend_class_entry *zxing_write_result_ce_ptr){
    zend_declare_class_constant_long(zxing_write_result_ce_ptr,ZEND_STRL("ImagePng"), IMAGE_TYPE_PNG);
    zend_declare_class_constant_long(zxing_write_result_ce_ptr,ZEND_STRL("ImageJpg"),  IMAGE_TYPE_JPG);
    zend_declare_class_constant_long(zxing_write_result_ce_ptr,ZEND_STRL("ImageBmp"), IMAGE_TYPE_BMP);
}
static Matrix<uint8_t>* result_zval_write_result(zval *res,zval *return_value) {
    Matrix<uint8_t>* result=(Matrix<uint8_t>*)zval_get_resource(res, ZXING_RES_WRITE ,resource_id_get_write_result());
    if (result==NULL) {
        ZVAL_NULL(return_value);
        return nullptr;
    }
    return result;
}
#ifdef __cplusplus
extern "C" {
#endif


void zxing_image_const_declare(zend_class_entry *zxing_ce_ptr) {


    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeEdgeClamp"), (zend_long)STBIR_EDGE_CLAMP);
    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeEdgeZero"), (zend_long)STBIR_EDGE_ZERO);
    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeEdgeReflect"), (zend_long)STBIR_EDGE_REFLECT);
    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeEdgeWrap"), (zend_long)STBIR_EDGE_WRAP);


    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeFilterDefault"), (zend_long)STBIR_FILTER_DEFAULT);
    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeFilterBox"), (zend_long)STBIR_FILTER_BOX);
    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeFilterTriangle"), (zend_long)STBIR_FILTER_TRIANGLE);
    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeFilterCubicBSpline"), (zend_long)STBIR_FILTER_CUBICBSPLINE);
    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeFilterCatmullRom"), (zend_long)STBIR_FILTER_CATMULLROM);
    zend_declare_class_constant_long(zxing_ce_ptr,ZEND_STRL("ResizeFilterMitchell"), (zend_long)STBIR_FILTER_MITCHELL);
}

int zxing_image_resize(
        stbi_uc* bg_res,
        stbi_uc* odata,
        int width,
        int height,
        int channel,
        int to_width,
        int to_height,
        zend_long edge,
        zend_long filter){
    if (edge!=STBIR_EDGE_CLAMP
        &&edge!=STBIR_EDGE_ZERO
        &&edge!=STBIR_EDGE_REFLECT
        &&edge!=STBIR_EDGE_WRAP
        ){
        edge=STBIR_EDGE_CLAMP;
    }
    if (filter!=STBIR_FILTER_DEFAULT
        &&filter!=STBIR_FILTER_BOX
        &&filter!=STBIR_FILTER_TRIANGLE
        &&filter!=STBIR_FILTER_CUBICBSPLINE
        &&filter!=STBIR_FILTER_CATMULLROM
        &&filter!=STBIR_FILTER_MITCHELL
    ){
        filter=STBIR_FILTER_BOX;
    }
    int ret= stbir_resize(bg_res,
                          width,
                          height,
                          0,
                          odata,
                          to_width,
                          to_height,
                          0,
                          STBIR_TYPE_UINT8,
                          channel,
                          STBIR_ALPHA_CHANNEL_NONE,
                          0,
                          (stbir_edge)edge,
                          (stbir_edge)edge,
                          (stbir_filter)filter,
                          (stbir_filter)filter,
                          STBIR_COLORSPACE_SRGB,
                          nullptr
    );
    if (ret==0){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 7, "your give background resize fail : %s",stbi_failure_reason());
        return ret;
    }
    return ret;
}

#define AlphaSee(mask,source,alpha) (float)source * ((float)alpha/255.0) + (float)mask * (float)((0xff-alpha)/255.0)

uint8_t * malloc_channel_data(
        zval * bg_obj,
        unsigned long bgc,
        unsigned long fc,
        uint8_t * data,
        size_t len,
        int width,
        int height){

    uint8_t fa=(fc&0xFF000000)>>24;
    uint8_t fr=(fc&0xFF0000)>>16;
    uint8_t fg=(fc&0xFF00)>>8;
    uint8_t fb=fc&0xFF;
    uint8_t bga=(bgc&0xFF000000)>>24;
    uint8_t bgr=(bgc&0xFF0000)>>16;
    uint8_t bgg=(bgc&0xFF00)>>8;
    uint8_t bgb=bgc&0xFF;

    if (Z_TYPE_P(bg_obj)==IS_OBJECT){

        zval *res_= zxing_read_property(bg_obj, ZEND_STRL("res"), 1);
        stbi_uc* res=(stbi_uc*)zval_get_resource(res_, ZXING_RES_IMAGE ,resource_id_get_image());
        if (res==NULL){
            zend_throw_exception_ex(zxing_exception_ce_ptr, 100, "your give background image is invalid");
            return NULL;
        }
        zend_long bg_width,bg_height,channel;
        if (!object_get_gt_long(&bg_width,0, bg_obj,ZEND_STRL("width"),"your give background image width is invalid"))return NULL;
        if (!object_get_gt_long(&bg_height,0,bg_obj,ZEND_STRL("height"),"your give background height width is invalid"))return NULL;
        if (!object_get_gt_long(&channel,0,bg_obj,ZEND_STRL("channel"),"your give background image desired channel is invalid"))return NULL;
        if (!zxing_check_image_channels(channel)) return NULL;
        if (len!=(width *height)){
            zend_throw_exception_ex(zxing_exception_ce_ptr,10,"background not match out image");
            return NULL;
        }
        stbi_uc *bg_data;
        int need_free=0;
        if (width!=bg_width||bg_height!=height){
            bg_data = (stbi_uc *) malloc(width *height * channel);
            if(bg_data== NULL){
                zend_throw_exception_ex(zxing_exception_ce_ptr,10,"malloc fail");
                return bg_data;
            }

            zend_long edge=Z_LVAL_P(zxing_read_property( bg_obj,ZEND_STRL("resize_edge"),1));
            zend_long filter=Z_LVAL_P(zxing_read_property( bg_obj,ZEND_STRL("resize_filter"),1));

            int ret=zxing_image_resize(
                    res,
                    bg_data,
                    bg_width,
                    bg_height,
                    channel,
                    width,
                    height,
                    edge,
                    filter
            );
            if(ret==0){
                free(bg_data);
                return NULL;
            }
            need_free=1;
        }else{
            bg_data=res;
        }

        uint8_t * bg_out_data=(uint8_t*)malloc(len*4);
        if(bg_out_data== NULL){
            if(need_free){
                free(bg_data);
            }
            zend_throw_exception_ex(zxing_exception_ce_ptr,10,"malloc fail");
            return bg_out_data;
        }

        size_t i=0,olen=len/sizeof(uint8_t);
        for(;i<olen;i++){
            size_t t=i*4;
            size_t bt=i*channel;
            if(data[i]==0){
                switch (channel) {
                    case 1:
                        bg_out_data[t]= AlphaSee(bg_data[bt],fr,fa);
                        bg_out_data[t+1]=AlphaSee(bg_data[bt],fg,fa);
                        bg_out_data[t+2]=AlphaSee(bg_data[bt],fb,fa);
                        bg_out_data[t+3]=~0;
                        break;
                    case 3:
                        bg_out_data[t]= AlphaSee(bg_data[bt],fr,fa);
                        bg_out_data[t+1]=AlphaSee(bg_data[bt+1],fg,fa);
                        bg_out_data[t+2]=AlphaSee(bg_data[bt+2],fb,fa);
                        bg_out_data[t+3]=~0;
                        break;
                    case 4:
                        bg_out_data[t]= AlphaSee(bg_data[bt],fr,fa);
                        bg_out_data[t+1]=AlphaSee(bg_data[bt+1],fg,fa);
                        bg_out_data[t+2]=AlphaSee(bg_data[bt+2],fb,fa);
                        bg_out_data[t+3]= bg_data[bt+3];
                        break;
                }
            }else{
                switch (channel) {
                    case 1:
                        bg_out_data[t]= AlphaSee(bg_data[bt],bgr,bga);
                        bg_out_data[t+1]=AlphaSee(bg_data[bt],bgg,bga);
                        bg_out_data[t+2]=AlphaSee(bg_data[bt],bgb,bga);
                        bg_out_data[t+3]=~0;
                        break;
                    case 3:
                        bg_out_data[t]= AlphaSee(bg_data[bt],bgr,bga);
                        bg_out_data[t+1]=AlphaSee(bg_data[bt+1],bgg,bga);
                        bg_out_data[t+2]=AlphaSee(bg_data[bt+2],bgb,bga);
                        bg_out_data[t+3]=~0;
                        break;
                    case 4:
                        bg_out_data[t]= AlphaSee(bg_data[bt],bgr,bga);
                        bg_out_data[t+1]=AlphaSee(bg_data[bt+1],bgg,bga);
                        bg_out_data[t+2]=AlphaSee(bg_data[bt+2],bgb,bga);
                        bg_out_data[t+3]= bg_data[bt+3];
                        break;
                }

            }
        }
        if(need_free){
            free(bg_data);
        }
        return bg_out_data;
    }
    uint8_t * out_data=(uint8_t*)malloc(len*4);
    if(out_data== NULL){
        zend_throw_exception_ex(zxing_exception_ce_ptr,10,"malloc fail");
        return out_data;
    }
    size_t i=0,olen=len/sizeof(uint8_t);
    for(;i<olen;i++){
        size_t t=i*4;
        if(data[i]==0){
            out_data[t]=fr;
            out_data[t+1]=fg;
            out_data[t+2]=fb;
            out_data[t+3]=fa;
        }else{
            out_data[t]=bgr;
            out_data[t+1]=bgg;
            out_data[t+2]=bgb;
            out_data[t+3]=bga;
        }
    }
    return out_data;
}

void free_channel_data(uint8_t * data){
    if (data==NULL)return;
    free(data);
}


void zxing_write_result_to_file(zval *res,zval *bg_res,unsigned long bgc,unsigned long fc,zend_long type,char* file,zval *return_value,int quality){
    auto bitmap=result_zval_write_result(res,return_value);
    if (bitmap== nullptr)return ;
    uint8_t * data=malloc_channel_data(bg_res,bgc,fc,(uint8_t *)bitmap->data(),bitmap->size(),bitmap->width(),bitmap->height());
    if(data==NULL){
        return;
    }
    try {
        int success = 0;
        switch (type) {
            case IMAGE_TYPE_BMP:
                success = stbi_write_bmp(file, bitmap->width(), bitmap->height(), 4, data);
                break;
            case IMAGE_TYPE_JPG:
                success = stbi_write_jpg(file, bitmap->width(), bitmap->height(), 4, data, quality);
                break;
            case IMAGE_TYPE_PNG:
                success = stbi_write_png(file, bitmap->width(), bitmap->height(), 4, data, 0);
                break;
            default:
                zend_throw_exception_ex(zxing_exception_ce_ptr,8,"your give image type not support :%d ",(int)type );
                break;
        }
        if (success<=0){
            if (!EG(exception)){
                zend_throw_exception_ex(zxing_exception_ce_ptr,28,"write fail [%s] :%s  ",file,stbi_failure_reason() );
            }
        }
        free_channel_data(data);
    } catch (const std::exception& e) {
        free_channel_data(data);
        zend_throw_exception_ex(zxing_exception_ce_ptr, 22, "exception :%s ",e.what() );
    }
}

typedef struct {
    int len;
    int cap;
    char *body;
} zxing_stbi_mem_context;
static void zxing_stbi_mem_context_free(zxing_stbi_mem_context *context) {
    if(context->len==0)return;
    free(context->body);
    context->body= nullptr;
    context->len= 0;
    context->cap= 0;
}
#define ZXING_STBI_IMAGE_WRITE_MEM_SIZE 512
static void zxing_stbi_mem_context_write(void *context, void *data, int size) {
    if (size<=0)
        return ;
    zxing_stbi_mem_context *c = (zxing_stbi_mem_context*)context;
    int cap=c->cap;
    while (c->len+size+1>cap){
        cap+=ZXING_STBI_IMAGE_WRITE_MEM_SIZE;
    }
    if (cap>c->cap){
        char * body =(char *)malloc(cap);
        memset(body,'\0',cap);
        if (c->len>0){
            memcpy(body,c->body,c->len);
            free(c->body);
        }
        c->body=body;
        c->cap=cap;
    }
    char *src = (char *)data;
    int i = 0;
    for (; i < size; i++) {
        int pos=c->len+i;
        c->body[pos] = src[i];
    }
    c->len = c->len+size;
}
void zxing_write_result_to_data(zval *res,zval *bg_res,unsigned long bgc,unsigned long fc,zend_long type,zval *return_value,int quality){
    auto bitmap=result_zval_write_result(res,return_value);
    if (bitmap== nullptr)return ;
    uint8_t * data = malloc_channel_data(bg_res,bgc,fc,(uint8_t *)bitmap->data(),bitmap->size(),bitmap->width(),bitmap->height());
    if(data==NULL){
        return;
    }

    try {
        int success = 0;
        if (type==IMAGE_TYPE_BMP){
            zxing_stbi_mem_context context{0,0,nullptr};
            success = stbi_write_bmp_to_func(zxing_stbi_mem_context_write,&context,bitmap->width(), bitmap->height(), 4, data);
            if (success&&context.len>0){
                ZVAL_NEW_STR(return_value,zend_string_init(context.body,context.len,0));
            }
            zxing_stbi_mem_context_free(&context);
        }else if (type==IMAGE_TYPE_JPG){
            zxing_stbi_mem_context context{0,0,nullptr};
            success = stbi_write_jpg_to_func(zxing_stbi_mem_context_write,&context,bitmap->width(), bitmap->height(), 4, data,quality);
            if (success&&context.len>0){
                ZVAL_NEW_STR(return_value,zend_string_init(context.body,context.len,0));
            }
            zxing_stbi_mem_context_free(&context);
        }else if(type==IMAGE_TYPE_PNG){
            int len;
            const char *png =(char *) stbi_write_png_to_mem((const unsigned char *) data, 0, bitmap->width(), bitmap->height(), 4, &len);
            if (png == NULL){
                success=0;
            }else{
                success=1;
                RETVAL_NEW_STR(zend_string_init(png,len,0));
                STBIW_FREE((void *)png);
            }
        }else{
            zend_throw_exception_ex(zxing_exception_ce_ptr,8,"your give image type not support :%d ",(int)type );
        }
        if (success<=0){
            if (!EG(exception)){
                zend_throw_exception_ex(zxing_exception_ce_ptr,28,"get data fail :%s  ",stbi_failure_reason() );
            }
        }
        free_channel_data(data);
    } catch (const std::exception& e) {
        free_channel_data(data);
        zend_throw_exception_ex(zxing_exception_ce_ptr, 22, "exception :%s ",e.what() );
    }
}


int zxing_write_create_result(
    int format,
    int width,
    int height,
    int eccLevel,
    int margin,
    zend_long* encoding,
    zend_string *  text,
    zend_class_entry *write_result_ce_ptr,
    zval *return_value
){
    CharacterSet encode;
    if (encoding!= nullptr){
        encode = CharacterSet(*encoding);
    }else{
        encode= CharacterSet::UTF8;
    }
    std::string data;
    data.assign(ZSTR_VAL(text),ZSTR_LEN(text));
    bool find= false;
    for (auto f : BarcodeFormats::all()) {
        if (f==BarcodeFormat(format)){
            find=true;
            break;
        }
    }
    if (!find){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 30, "format[%d] not support",format);
        return -1;
    }
    if (eccLevel<0||eccLevel>8){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 30, "ecc-Level need in [0-8],you give %d",eccLevel);
        return -1;
    }
    if (margin<0){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 30, "margin need >=0,your give %d",margin);
        return -1;
    }
    Matrix<uint8_t>* mat= nullptr;
    try {
        auto writer = MultiFormatWriter(BarcodeFormat(format)).setMargin(margin).setEncoding(encode).setEccLevel(eccLevel);
        auto in =writer.encode(TextUtfEncoding::FromUtf8(data), width, height);
        mat=new Matrix<uint8_t> (in.width(), in.height());
        for (int y = 0; y < in.height(); ++y)
            for (int x = 0; x < in.width(); ++x)
                mat->set(x, y, in.get(x, y) ? 0 : ~0);
    } catch (const std::exception& e) {
        if(mat!= nullptr)delete mat;
        zend_throw_exception_ex(zxing_exception_ce_ptr, 32, "render error:%s",e.what());
        return -2;
    }
    zval param[1];
    ZVAL_RES(&param[0], zend_register_resource(mat, resource_id_get_write_result()));
    zval ret;
    if (!zxing_new_class(write_result_ce_ptr,&ret , param, 1)) {
        zval_ptr_dtor(&param[0]);
        return 0;
    }
    zval_ptr_dtor(&param[0]);
    ZVAL_ZVAL(return_value,&ret,1,1);
    return 1;
}

//free
void zxing_release_write_result(void * res){
    if (res!= nullptr){
        delete (Matrix<uint8_t>*)res;
    }
    res=nullptr;
}

void zxing_release_read_result(void * res){
    if (res!= nullptr){
        delete (Result*)res;
    }
    res=nullptr;
}


#ifdef __cplusplus
}
#endif


