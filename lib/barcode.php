<?php
namespace ZXing;
/**
 * remove barcode from the image
 * @param string $data image file data
 * @param callable $image_callback
 * @param int $color
 * @return resource
 */
function removeBarcode($data,callable $image_callback=null,$color) {
    while (true) {
        $image=Image::loadData($data,1);
        if (is_callable($image_callback)) {
            $image_callback($image);
        }
        $b=new Read();
        $img = imagecreatefromstring($data);
        $res=$b->scan($image);
        if (count($res)==0)return $img;
        foreach ($res as $tmp){
            $pt=$tmp->getPosition(true);
            $p= array_merge(...array_map('array_values',$pt));
            imagefilledpolygon($img,$p, 4,$color);
        }
        ob_start();
        imagepng($img);
        imagedestroy($img);
        $data=ob_get_clean();
    }
    return $img;
}


function orientationRotate($img){
    switch(@exif_read_data(__DIR__."/IMG_0348.jpg")['Orientation']??'0') {
        case 8:
            $img = imagerotate($img,90,0);
            break;
        case 3:
            $img = imagerotate($img,180,0);
            break;
        case 6:
            $img = imagerotate($img,-90,0);
            break;
    }
    return $img;
}


/**
 * replace barcode from the image
 * @param string $filedata image data
 * @param callable $image_callback
 * @param callable $data_callback
 * @param array $error
 * @return resource
 */
function replaceBarcode($filedata,
    callable $data_callback,
    callable $image_callback=null,
    array &$error=[]){
        $image=Image::loadData($filedata);
        if (is_callable($image_callback)) {
            $image_callback($image);
        }
        $img = imagecreatefromstring($filedata);
        $error=[];
        $b=new Read();
        foreach ($b->scan($image) as $tmp){
            try{
                $b=call_user_func($data_callback,$tmp);
                assert($b instanceof WriteResult);
                $to_image=$b->data(WriteResult::ImagePng);
            }catch(\Exception $e){
                $error[]=$e->getMessage();
                continue;
            }
            $to_image=imagecreatefromstring($to_image);
            imagesavealpha($to_image , true);
            $pngTransparency = imagecolorallocatealpha($to_image , 0, 0, 0, 127);
            $to_image= imagerotate($to_image, -$tmp->getOrientation(), $pngTransparency,0);
            $pt=$tmp->getPosition(true);
            $x=$pt[0]['x'];
            $y= $pt[1]['y'];
            $w=$pt[2]['x']-$pt[0]['x'];
            $h= $pt[3]['y']-$pt[1]['y'];
            imagecopy(
                $img,
                $to_image,
                $x,
                $y,
                0,
                0,
                $w,
                $h
                );
        }
        return $img;
}