<?php
$a=file_get_contents(__DIR__."/IMG_0348.jpg");
if ($img=remove_barcode($a,function (ZXing\Image $image){
    $image->resize(900, 0);
},0)) {
    imagepng($img,__DIR__."/remove.png");
}


//lib
function remove_barcode($data,callable $image_callback=null,$color) {
    $image=ZXing\Image::loadData($data,1);
    if (is_callable($image_callback)) {
        $image_callback($image);
    }
    $b=new ZXing\Read();
    $img = imagecreatefromstring($data);
    $res=$b->scan($image);
    if (count($res)==0)return $img;
    foreach ($res as $tmp){
        $pt=$tmp->getPosition(true);
        $p= array_merge(...array_map('array_values',$pt));
        imagefilledpolygon($img,$p, 4,$color);
    }
    return $img;
}
