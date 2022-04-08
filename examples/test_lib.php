<?php
if (is_file(__DIR__ . '/../vendor/autoload.php')) {
    require __DIR__ . '/../vendor/autoload.php';
} elseif (is_file(__DIR__ . '/../../../autoload.php')) {
    require __DIR__ . '/../../../autoload.php';
} else {
    echo 'Cannot find the vendor directory, have you executed composer install?' . PHP_EOL;
    echo 'See https://getcomposer.org to get Composer.' . PHP_EOL;
    exit(1);
}

//Remove the QR code from the image
$a=file_get_contents("https://img.t.sinajs.cn/t6/style/images/common/footer_code.jpg");
if ($img=ZXing\removeBarcode($a,function (ZXing\Image $image){
    $image->resize(900, 0);
},0xFFFF00/*RGB*/)) {
    imagepng($img,__DIR__."/remove.png");
}

//replace the QR code from the image
$a=file_get_contents("https://img.t.sinajs.cn/t6/style/images/common/footer_code.jpg");
if($img=ZXing\replaceBarcode($a,function (ZXing\ReadResult $tmp){
    $write=new ZXing\Write(
        ZXing\Write::BarcodeFormatQRCode,
        $tmp->getwidth(ZXing\ReadResult::WidthTop,true),
        $tmp->getHeight(ZXing\ReadResult::HeightLeft,true)
    );
    $write->setMargin(0);
    $out=$write->render("bbbbffffffffffffffffffff");
    //$out->setColor(0xFF000000,0xFFFFFFFF);
    return $out;
},function (ZXing\Image $image){
    $image->resize(900, 0);
})){
    $img=ZXing\orientationRotate($img);
    imagepng($img,__DIR__."/save.png");
}
