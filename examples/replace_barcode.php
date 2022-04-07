<?php
$a=file_get_contents(__DIR__."/IMG_0348.jpg");
if($img=replace_barcode($a,function (ZXing\Image $image){
    $image->resize(900, 0);
},function (ZXing\ReadResult $tmp){
    $write=new ZXing\Write(
        ZXing\Write::BarcodeFormatQRCode,
        $tmp->getwidth(ZXing\ReadResult::WidthTop,true),
        $tmp->getHeight(ZXing\ReadResult::HeightLeft,true)
    );
    $write->setMargin(0);
    $out=$write->render("bbbb");
    //$out->setColor(0xFF000000,0xFFFFFFFF);
    return $out;
})){
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
    imagepng($img,__DIR__."/save.png");
}

//lib
function replace_barcode($filedata,
    callable $image_callback=null,
    callable $data_callback=null,
    array &$error=[]){
    $image=ZXing\Image::loadData($filedata);
    if (is_callable($image_callback)) {
        $image_callback($image);
    }
    $img = imagecreatefromstring($filedata);
    $error=[];
    $b=new ZXing\Read();
    foreach ($b->scan($image) as $tmp){
        try{
            $b=call_user_func($data_callback,$tmp);
            assert($b instanceof ZXing\WriteResult);
            $to_image=$b->data(ZXing\WriteResult::ImagePng);
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