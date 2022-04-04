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
#include "zxing_image_class.h"
#include "zxing_common.h"
#include "zxing_exception_class.h"
#include "zxing_cpp_lib.h"
#include "stb_image.h"
#include "stb_image_resize.h"

static void zxing_image_property_declare(zend_class_entry *zxing_ce_ptr){
    zend_declare_property_long(zxing_ce_ptr, ZEND_STRL("resize_edge"),STBIR_EDGE_CLAMP, ZEND_ACC_PRIVATE);
    zend_declare_property_long(zxing_ce_ptr, ZEND_STRL("resize_filter"),STBIR_FILTER_BOX, ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("path"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("channel"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("height"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("width"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(zxing_ce_ptr, ZEND_STRL("res"), ZEND_ACC_PRIVATE);
}


ZEND_METHOD(zxing_image_class, __construct){

}


static int init_image_object(stbi_uc* ic, zval *return_value,int channels,int height,int width){
    zval obj;
    if(!zxing_new_class(zxing_image_ce_ptr,&obj,NULL,0)){
        return 0;
    }
    zval ic_res;
    ZVAL_RES(&ic_res, zend_register_resource(ic, resource_id_get_image()));
    zxing_update_property(&obj,ZEND_STRL("res"),&ic_res);
    zval_dtor(&ic_res);
    zxing_update_property_long(&obj,ZEND_STRL("channel"),channels);
    zxing_update_property_long(&obj,ZEND_STRL("height"),height);
    zxing_update_property_long(&obj,ZEND_STRL("width"),width);
    ZVAL_ZVAL(return_value,&obj,1,1);
    return 1;
}

ZEND_METHOD(zxing_image_class, loadFile){
    char *path=NULL;
    size_t path_len = 0;
    zend_long desired_channels=0;
    ZEND_PARSE_PARAMETERS_START(1, 2)
            Z_PARAM_STRING(path, path_len)
            Z_PARAM_OPTIONAL
            Z_PARAM_LONG(desired_channels)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_NULL());
    if (desired_channels>0&&!zxing_check_image_channels(desired_channels)) RETURN_NULL();
    int width,height,channels;
    stbi_uc* ic=stbi_load(path, &width, &height, &channels, (int)desired_channels);

    if (ic==NULL){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 1, "your give path invalid [%s] : %s",path, stbi_failure_reason());
        RETURN_NULL();
    }
    if(init_image_object(ic,return_value,desired_channels>0?desired_channels:channels,height,width)){
#if PHP_VERSION_ID < 80000
        zend_update_property_stringl(Z_OBJCE_P(return_value), return_value,ZEND_STRL("path"),path,path_len);
#else
        zend_update_property_stringl(Z_OBJCE_P(return_value), Z_OBJ_P(return_value),ZEND_STRL("path"),path,path_len);
#endif

          }else{
        stbi_image_free(ic);
        RETURN_NULL();
    }
}
ZEND_METHOD(zxing_image_class, loadData){
    char *data=NULL;
    size_t data_len = 0;
    zend_long desired_channels=0;
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_STRING(data, data_len)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(desired_channels)
    ZEND_PARSE_PARAMETERS_END();
    if (desired_channels>0&&!zxing_check_image_channels(desired_channels)) return ;
    int width,height,channels;
    stbi_uc* ic=stbi_load_from_memory((const stbi_uc *)data,(int)data_len, &width, &height, &channels, (int)desired_channels);
    if (ic==NULL){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 1, "your give image data can't parse : %s", stbi_failure_reason());
        return;
    }
    if(!init_image_object(ic,return_value,desired_channels>0?desired_channels:channels,height,width)){
        stbi_image_free(ic);
    }
}

ZEND_METHOD(zxing_image_class, setResizeEdge){
    PROPERTY_SET_LONG_ONE_PARAM_LONG("resize_edge");
}
ZEND_METHOD(zxing_image_class, setResizeFilter){
    PROPERTY_SET_LONG_ONE_PARAM_LONG("resize_filter");
}
ZEND_METHOD(zxing_image_class, resize){
    zend_long width=0;
    zend_long height=0;
    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_LONG(width)
            Z_PARAM_LONG(height)
    ZEND_PARSE_PARAMETERS_END();
    if(width<=0&&height<=0){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 1, "your resize width and height is wrong [%d,%d]", (int)width,(int)height);
        return;
    }
    zend_long o_height=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("height"),1));
    zend_long o_width=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("width"),1));
    zend_long channel=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("channel"),1));
    if(width<=0){
        width=((float)height/(float)o_height)*o_width;
    }
    if(height<=0){
        height=((float)width/(float)o_width)*o_height;
    }
    if (width<=0||height<=0||o_height<=0||o_width<=0||channel<=0){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 100, "your give image width or height or channel is invalid");
        return;
    }
    if (width==o_width&&height==o_height){
        RETURN_ZVAL(getThis(),1,0);
    }
    zval *res_= zxing_read_property(getThis(), ZEND_STRL("res"), 1);
    stbi_uc* res=(stbi_uc*)zval_get_resource(res_, ZXING_RES_IMAGE ,resource_id_get_image());
    if (res==NULL){
        zend_throw_exception_ex(zxing_exception_ce_ptr, 100, "your give image is invalid");
        return;
    }
    stbi_uc *re_data = (stbi_uc *) malloc(width *height * channel);
    if(re_data== NULL){
        zend_throw_exception_ex(zxing_exception_ce_ptr,10,"malloc fail");
        return;
    }

    zend_long edge=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("resize_edge"),1));
    zend_long filter=Z_LVAL_P(zxing_read_property( getThis(),ZEND_STRL("resize_filter"),1));

    int ret=zxing_image_resize(
        res,
        re_data,
        o_width,
        o_height,
        channel,
        width,
        height,
        edge,
        filter
    );
    if(ret==0){
        free(re_data);
        return;
    }
    zval ic_res;
    ZVAL_RES(&ic_res, zend_register_resource(re_data, resource_id_get_image()));
    zxing_update_property(getThis(),ZEND_STRL("res"),&ic_res);
    zval_dtor(&ic_res);
    zxing_update_property_long(getThis(),ZEND_STRL("height"),height);
    zxing_update_property_long(getThis(),ZEND_STRL("width"),width);
    RETURN_ZVAL(getThis(),1,0);
}

#define ZXING_IMAGE_GET_METHOD(method,name) ZEND_METHOD(zxing_image_class, method){\
    zval * val=zxing_read_property(getThis(),ZEND_STRL(name),1); \
    RETURN_ZVAL(val,1,0);\
}

ZXING_IMAGE_GET_METHOD(getPath,"path")
ZXING_IMAGE_GET_METHOD(getChannel,"channel")
ZXING_IMAGE_GET_METHOD(getHeight,"height")
ZXING_IMAGE_GET_METHOD(getWidth,"width")
ZXING_IMAGE_GET_METHOD(getRes,"res")

static zend_function_entry zxing_read_class_method[] = {
    ZEND_ME(zxing_image_class, __construct, zxing_none_arginfo, ZEND_ACC_PRIVATE)
    ZEND_ME(zxing_image_class, loadFile,zxing_image_load_file_arginfo,ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    ZEND_ME(zxing_image_class, loadData,zxing_image_load_data_arginfo,ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    ZEND_ME(zxing_image_class, getPath,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_image_class, getChannel,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_image_class, getHeight,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_image_class, getWidth,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_image_class, getRes,zxing_none_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_image_class, resize,zxing_image_resize_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_image_class, setResizeEdge,zxing_image_resize_edge_arginfo,ZEND_ACC_PUBLIC)
    ZEND_ME(zxing_image_class, setResizeFilter,zxing_image_resize_filter_arginfo,ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

 zend_class_entry *zxing_image_ce_ptr;
void zxing_image_class_init() {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, ZXING_NS, "Image", zxing_read_class_method);
    zxing_image_ce_ptr = zend_register_internal_class(&ce);
    zxing_image_property_declare(zxing_image_ce_ptr);
    zxing_image_const_declare(zxing_image_ce_ptr);
}

#ifdef __cplusplus
}
#endif
