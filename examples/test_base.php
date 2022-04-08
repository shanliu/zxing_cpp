<?php
//more detail,see ../zxing_cpp.php
//write qrcode to image
try{
    $a=new ZXing\Write(ZXing\Write::BarcodeFormatQRCode);
    $b=$a->render("1111");
    $b->setColor(0xCCFFFFFF,0xCC000000);
   // $b->setBackground(ZXing\Image::loadFile('https://h5.sinaimg.cn/upload/1005/526/2021/09/09/logo.png'));//set qrcode background image
    file_put_contents(__DIR__."/cc.jpg",$b->data(ZXing\WriteResult::ImagePng));
}catch(\Exception $e){
    print_r($e);
}


//write qrcode to image
try{
    $a=new ZXing\Write(ZXing\Write::BarcodeFormatQRCode);
    $b=$a->render("1111");
    $b->setColor(0x88000000,0xFFFFFFFF);
    $b->save(ZXing\WriteResult::ImagePng,__DIR__."/cc11.jpg");//test save method
    $b->save(ZXing\WriteResult::ImagePng,'');//test save path error
}catch(\Exception $e){
    print_r($e);
}


//read qrcode form image
try{
    $a=ZXing\Image::loadFile(__DIR__."/cc.jpg");//support url
    if( $a->getWidth()>1024){
        $a->resize(1024,0);
    }
    if( $a->getHeight()>1024){
       $a->resize(0,1024);
    }
    $b=new ZXing\Read();
    $tmp=$b->scan($a);
    if(count($tmp)>0){
        foreach($tmp as $v){
            print_r($v->getStatus()."\n");
            print_r($v->isValid()."\n");
            print_r($v->getNumBits()."\n");
            print_r($v->getFormat()."\n");
            print_r($v->getFormatName()."\n");
            print_r($v->getSymbologyIdentifier()."\n");
            print_r($v->getPosition());
            print_r($v->getOrientation()."\n");
            print_r($v->getEcLevel()."\n");
            print_r($v->getLineCount()."\n");
            print_r($v->getGTINCountry()."\n");
            print_r($v->getGTINPrice()."\n");
            print_r($v->getGTINEanAddOn()."\n");
            print_r($v->getGTINIssueNr()."\n");
            print_r($v->isPartOfSequence()."\n");
            print_r($v->isLastInSequence()."\n");
            print_r($v->getSequenceIndex()."\n");
            print_r($v->getSequenceId()."\n");
            print_r($v->getSequenceSize()."\n");
            print_r($v->isReaderInit()."\n");
            print_r($v->getText()."\n");
        }
    }
}catch(\Exception $e){
    print_r($e);
}


//test read file error
try{
    $a=ZXing\Image::loadFile('');
    print_r($a);
}catch(\Exception $e){
    print_r($e);
}


