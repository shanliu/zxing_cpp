<?php
/**
 * zxing-cpp wrappers for php extension document
 * If you find a bug, please contact the author
 * Author: shanliu  <shan.liu@msn.com>
 */
namespace ZXing;
class Exception extends \Exception {
}
class Read {
	/**
	 * setEanAddOnSymbol param
	 */
	const EanAddOnSymbolIgnore = "0";
	const EanAddOnSymbolRead = "1";
	const EanAddOnSymbolRequire = "2";
	/**
	 * setBinaries param
	 */
	const BinarizerBoolCast = "3";
	const BinarizerFixedThreshold = "2";
	const BinarizerGlobalHistogram = "1";
	const BinarizerLocalAverage = "0";
	/**
	 * setFormats param
	 */
	const BarcodeFormatAztec = "1";
	const BarcodeFormatCodabar = "2";
	const BarcodeFormatCode39 = "4";
	const BarcodeFormatCode93 = "8";
	const BarcodeFormatCode128 = "16";
	const BarcodeFormatDataBar = "32";
	const BarcodeFormatDataBarExpanded = "64";
	const BarcodeFormatDataMatrix = "128";
	const BarcodeFormatEAN8 = "256";
	const BarcodeFormatEAN13 = "512";
	const BarcodeFormatITF = "1024";
	const BarcodeFormatMaxiCode = "2048";
	const BarcodeFormatPDF417 = "4096";
	const BarcodeFormatQRCode = "8192";
	const BarcodeFormatUPCA = "16384";
	const BarcodeFormatUPCE = "32768";
	const BarcodeFormatOneDCodes = "51070";
	const BarcodeFormatTwoDCodes = "14465";
	const BarcodeFormatAny = "65535";

	/* properties */
	private $ean_add_on_symbol = NULL;
	private $try_harder = NULL;
	private $try_rotate = NULL;
	private $pure = NULL;
	private $binaries = NULL;
	private $format = NULL;
	private $character = NULL;
	private $code39_extend = NULL;
	private $code39_check = NULL;
	private $check_digit = NULL;
	private $cobra_start_end = NULL;

	/**
	 * Specify whether to ignore, read or require EAN-2/5 add-on symbols while scanning EAN/UPC codes
	 * @param int $ean_add_on_symbol see EanAddOnSymbol* const
	 * @return $this
	 */
	public function setEanAddOnSymbol($ean_add_on_symbol) {
	}
	/**
	 * Spend more time to try to find a barcode; optimize for accuracy, not speed.
	 * @param bool $harder
	 * @return $this
	 */
	public function setTryHarder($harder) {
	}
	/**
	 * Also try detecting code in 90, 180 and 270 degree rotated images.
	 * @param bool $rotate
	 * @return $this
	 */
	public function setTryRotate($rotate) {
	}
	/**
	 * Set to true if the input contains nothing but a perfectly aligned barcode (generated image)
	 * @param bool $pure
	 * @return $this
	 */
	public function setIsPure($pure) {
	}
	/**
	 * Binarizer to use internally when using the ReadBarcode function
	 * @param int $binarizer see Binarizer* const
	 * @return $this
	 */
	public function setBinaries($binarizer) {
	}
	/**
	 * Specify a set of BarcodeFormats that should be searched for, the default is all supported formats.
	 * @param int $format see BarcodeFormat* const
	 * @return $this
	 */
	public function setFormats($format) {
	}
	/**
	 * Specifies what character encoding to use when decoding, where applicable.
	 * @param string $character You can set to "utf8"
	 * @return $this
	 */
	public function setCharacter($character) {
	}
	/**
	 *  If true, the Code-39 reader will try to read extended mode.
	 * @param bool $code39_extend
	 * @return $this
	 */
	public function setTryCode39ExtendedMode($code39_extend) {
	}
	/**
	 * Assume Code-39 codes employ a check digit.
	 * @param bool $code39_check
	 * @return $this
	 */
	public function setAssumeCode39CheckDigit($code39_check) {
	}
	/**
	 * Assume ITF codes employ a GS1 check digit.
	 * @param bool $check_digit
	 * @return $this
	 */
	public function setAssumeITFCheckDigit($check_digit) {
	}
	/**
	 * If true, return the start and end digits in a Codabar barcode instead of stripping them. They
	 * are alpha, whereas the rest are numeric. By default, they are stripped, but this causes them
	 * to not be.
	 * @param bool $check_digit
	 * @return $this
	 */
	public function setReturnCodabarStartEnd($coba_start_end) {
	}
	/**
	 * scan and return sanc result
	 * @param Image $image
	 * @throw Exception
	 * @return ReadResult[]
	 */
	public function scan($image) {
	}
}


class ReadResult {
	/**
	 * getStatus return result
	 */
	const StatusChecksumError = "3";
	const StatusFormatError = "2";
	const StatusNoError = "0";
	const StatusNotFound = "1";
	/**
	 * getWidth param
	 */
	const WidthTop = "1"; //top line withd
	const WidthBottom = "2";//bottom line withd
	/**
	 * getHeight param
	 */
	const HeightLeft = "3";//left line height
	const HeightRight = "4";//right line height

	/* properties */
	private $res = NULL;
	private $angle_escape = "1";
	private $width = NULL;
	private $height = NULL;
	private $src_width = NULL;
	private $src_height = NULL;

	/**
	 * scan result
	 */
	public function __construct($res, $width, $height, $src_width = NULL, $src_height = NULL) {
	}
	/**
	 * Same as `ToUtf8()` above, except if angleEscape set, places non-graphical characters in angle brackets with text name
	 * default is true
	 * @param bool $angle_escape 
	 * @return $this
	 */
	public function setAngleEscape($angle_escape) {
	}
	/**
	 * get horizontally size
	 * @param int $type see Width* const
	 * @param bool $revert is get before width
	 * @return int
	 */
	public function getWidth($type,$revert=false) {
	}
	/**
	 * get Vertically size
	 * @param int $type see Height* const
	 * @param bool $revert is get before height
	 * @return int
	 */
	public function getHeight($type,$revert=false) {
	}
	/**
	 * get status
	 * @return int see Status* const
	 */
	public function getStatus() {
	}
	/**
	 * getStatus()==StatusNoError
	 * @return bool
	 */
	public function isValid() {
	}
	/**
	 * see zxing-cpp doc
	 * @return int
	 */
	public function getNumBits() {
	}
	/**
	 * result data
	 * @return string
	 */
	public function getText() {
	}
	/**
	 * result format, see @Read::BarcodeFormat* const
	 * @return int
	 */
	public function getFormat() {
	}
	/**
	 * format name string out
	 * @return string
	 */
	public function getFormatName() {
	}
	/**
	 * symbologyIdentifier Symbology identifier "]cm" where "c" is symbology code character, "m" the modifier.
	 * @return string
	 */
	public function getSymbologyIdentifier() {
	}
	/**
	 * result Position
	 * [][x,y]
	 * @param bool $revert is get before resize point?
	 * @return array
	 */
	public function getPosition($revert=false) {
	}
	/**
	 * orientation of barcode in degree
	 * @return int
	 */
	public function getOrientation() {
	}
	/**
	 * error-correction level of the QR Code
	 * @return string
	 */
	public function getEcLevel() {
	}
	/**
	 * How many lines have been detected with this code (applies only to 1D symbologies)
	 * @return int
	 */
	public function getLineCount() {
	}
	/**
	 *  output on format is
	 * 	BarcodeFormat::EAN13 | BarcodeFormat::EAN8 | BarcodeFormat::UPCA | BarcodeFormat::UPCE
	 *  BarcodeFormat::ITF 
	 * @return string
	 */
	public function getGTINCountry() {
	}
	/**
	 * output on format is
	 * BarcodeFormat::EAN13 | BarcodeFormat::EAN8 | BarcodeFormat::UPCA | BarcodeFormat::UPCE
	 * @return string
	 */
	public function getGTINEanAddOn() {
	}
	/**
	 * output on format is
	 * BarcodeFormat::EAN13 | BarcodeFormat::EAN8 | BarcodeFormat::UPCA | BarcodeFormat::UPCE
	 * @return string
	 */
	public function getGTINPrice() {
	}
	/**
	 * output on format is
	 * BarcodeFormat::EAN13 | BarcodeFormat::EAN8 | BarcodeFormat::UPCA | BarcodeFormat::UPCE
	 * @return string
	 */
	public function getGTINIssueNr() {
	}
	/**
	 * see zxing-cpp doc
	 * @return bool
	 */
	public function isPartOfSequence() {
	}
	/**
	 * see zxing-cpp doc
	 * @return bool
	 */
	public function isLastInSequence() {
	}
	/**
	 * compare ReadResult is equal
	 * @param ReadResult $result
	 * @return bool
	 */
	public function compare($result) {
	}
	/**
	 * the 0-based index of this symbol in a structured append sequence.
	 * @return int
	 */
	public function getSequenceIndex() {
	}
	/**
	 * sequenceSize number of symbols in a structured append sequence.
	 *
	 * If this is not part of a structured append sequence, the returned value is -1.
	 * If it is a structured append symbol but the total number of symbols is unknown, the
	 * returned value is 0 (see PDF417 if optional "Segment Count" not given).
	 * @return int
	 */
	public function getSequenceSize() {
	}
	/**
	 * id to check if a set of symbols belongs to the same structured append sequence.
	 * If the symbology does not support this feature, the returned value is empty (see MaxiCode).
	 * For QR Code, this is the parity integer converted to a string.
	 * For PDF417 and DataMatrix, this is the "fileId".
	 * @return int
	 */
	public function getSequenceId() {
	}
	/**
	 * readerInit Set if Reader Initialisation/Programming symbol.
	 * @return bool
	 */
	public function isReaderInit() {
	}
}
class Write {
	/**
	 * setFormat param
	 */
	const BarcodeFormatAztec = "1";
	const BarcodeFormatCodabar = "2";
	const BarcodeFormatCode39 = "4";
	const BarcodeFormatCode93 = "8";
	const BarcodeFormatCode128 = "16";
	const BarcodeFormatDataMatrix = "128";
	const BarcodeFormatEAN8 = "256";
	const BarcodeFormatEAN13 = "512";
	const BarcodeFormatITF = "1024";
	const BarcodeFormatMaxiCode = "2048";
	const BarcodeFormatPDF417 = "4096";
	const BarcodeFormatQRCode = "8192";
	const BarcodeFormatUPCA = "16384";
	const BarcodeFormatUPCE = "32768";
	/**
	 * setCharacter param
	 */
	const CharacterSetUnknown = "0";
	const CharacterSetASCII = "1";
	const CharacterSetISO8859_1 = "2";
	const CharacterSetISO8859_2 = "3";
	const CharacterSetISO8859_3 = "4";
	const CharacterSetISO8859_4 = "5";
	const CharacterSetISO8859_5 = "6";
	const CharacterSetISO8859_6 = "7";
	const CharacterSetISO8859_7 = "8";
	const CharacterSetISO8859_8 = "9";
	const CharacterSetISO8859_9 = "10";
	const CharacterSetISO8859_10 = "11";
	const CharacterSetISO8859_11 = "12";
	const CharacterSetISO8859_13 = "13";
	const CharacterSetISO8859_14 = "14";
	const CharacterSetISO8859_15 = "15";
	const CharacterSetISO8859_16 = "16";
	const CharacterSetCp437 = "17";
	const CharacterSetCp1250 = "18";
	const CharacterSetCp1251 = "19";
	const CharacterSetCp1252 = "20";
	const CharacterSetCp1256 = "21";
	const CharacterSetShift_JIS = "22";
	const CharacterSetBig5 = "23";
	const CharacterSetGB2312 = "24";
	const CharacterSetGB18030 = "25";
	const CharacterSetEUC_JP = "26";
	const CharacterSetEUC_KR = "27";
	const CharacterSetUnicodeBig = "28";
	const CharacterSetUTF8 = "29";
	const CharacterSetBINARY = "30";
	const CharacterSetCharsetCount = "31";

	/* properties */
	private $format = NULL;
	private $width = "100";
	private $height = "100";
	private $margin = "0";
	private $encoding = NULL;
	private $ecc_level = "0";

	/**
	 * output Barcode image
	 * @param int $format see BarcodeFormat* const
	 * @param int $width default is 100
	 * @param int $height default is 100
	 */
	public function __construct($format, $width=100, $height=100) {
	}
	/**
	 * reset format
	 * @param int $format see BarcodeFormat* const
	 * @return $this
	 */
	public function setFormat($format) {
	}
	/**
	 * reset width
	 * @param int $width
	 * @return $this
	 */
	public function setWidth($width) {
	}
	/**
	 * reset height
	 * @param int $height
	 * @return $this
	 */
	public function setHeight($height) {
	}
	/**
	 * set output image margin
	 * @param int $margin
	 * @return $this
	 */
	public function setMargin($margin) {
	}
	/**
	 * set render data encode
	 * @param int $encoding see  CharacterSet* const
	 * @return $this
	 */
	public function setEncoding($encoding) {
	}
	/**
	 * error-correction level of the QR Code
	 * range: 0-8
	 * @param int $ecc_level
	 * @return $this
	 */
	public function setEccLevel($ecc_level) {
	}
	/**
	 * return WriteResult object
	 * @param string $data render data
	 * @throw Exception
	 * @return WriteResult
	 */
	public function render($data) {
	}
}

class WriteResult {
	/**
	 * save or data $type param 
	 */
	const ImagePng = "1";
	const ImageJpg = "2";
	const ImageBmp = "3";

	/* properties */
	private $background_res = NULL;
	private $res = NULL;
	private $background_color = "4278190080";
	private $front_color = "4294967295";

	/**
	 * Barcode image object
	 */
	public function __construct($res) {
	}
	/**
	 * set barcode color
	 * color format 0xFFFFFFFF ARGB
	 * your set setBackground,plase set A is not FF
	 * @param int $front_color
	 * @param int $background_color
	 * @return $this
	 */
	public function setColor($front_color, $background_color) {
	}
	/**
	 * set barcode background
	 * plase set setColor and set A is not FF
	 * @param Image $image_obj
	 * @return $this
	 */
	public function setBackground($image_obj) {
	}
	/**
	 * save image to path
	 * save fail throw exception
	 * @throw Exception 
	 * @param int $type see Image* const
	 * @param string $path save image path
	 * @param int $quality only jpg format
	 * @param resource|null $context eq file_put_contents $context param
	 */
	public function save($type, $path, $quality = NULL,$context=NULL) {
	}
	/**
	 * return image data is string
	 * render fail throw exception
	 * @throw Exception 
	 * @param int $type see Image* const
	 * @param int $quality only jpg format
	 */
	public function data($type, $quality = NULL) {
	}
}

class Image {
	/**
	 * setResizeEdge param
	 */
	const ResizeEdgeClamp = "1";
	const ResizeEdgeZero = "4";
	const ResizeEdgeReflect = "2";
	const ResizeEdgeWrap = "3";
	/**
	 * setResizeFilter param
	 */
	const ResizeFilterDefault = "0";// use same filter type that easy-to-use API chooses
	const ResizeFilterBox = "1";// A trapezoid w/1-pixel wide ramps, same result as box for integer scale ratios
	const ResizeFilterTriangle = "2";// On upsampling, produces same results as bilinear texture filtering
	const ResizeFilterCubicBSpline = "3";// The cubic b-spline (aka Mitchell-Netrevalli with B=1,C=0), gaussian-esque
	const ResizeFilterCatmullRom = "4";// An interpolating cubic spline
	const ResizeFilterMitchell = "5";// Mitchell-Netrevalli filter with B=1/3, C=1/3

	/* properties */
	private $resize_edge = "1";
	private $resize_filter = "1";
	private $path = NULL;
	private $channel = NULL;
	private $height = NULL;
	private $width = NULL;
	private $res = NULL;
	private function __construct() {
	}
	/**
	 * new Image object form image file
	 * @param string $file image file
	 * @param int $desired_channels set load image channel value: [0,1,3,4]
	 * @param int $use_include_patheq file_get_contents $use_include_patheq param
	 * @param resource|null $context eq file_get_contents $context param
	 * @param int $offset eq file_get_contents $offset param
	 * @param int $maxlen eq file_get_contents $maxlen param
	 * @return Image
	 */
	public static function loadFile(string $file,int $desired_channels = 0,bool $use_include_path = null, $context = null, int $offset = null, int $maxlen = null) {
	}
	/**
	 * new Image object form image string,may be form file_get_contents
	 * @param string $data image string
	 * @param int $desired_channels set load image channel value: [0,1,3,4]
	 * @return Image
	 */
	public static function loadData($data, $desired_channels = NULL) {
	}
	/**
	 * only loadFile create object,is file path
	 * @return string
	 */
	public function getPath() {
	}
	/**
	 * image channel,value emun:0 1 3 4
	 * @return int 
	 */
	public function getChannel() {
	}
	/**
	 * get resize image height
	 * @return int 
	 */
	public function getHeight() {
	}
	/**
	 * get resize image width
	 * @return int 
	 */
	public function getWidth() {
	}
	/**
	 * get image width
	 * @return int 
	 */
	public function getSrcWidth() {
	}
	/**
	 * get image width
	 * @return int 
	 */
	public function getSrcHeight() {
	}
	/**
	 * image resource,may be debug use it.
	 * @return resource 
	 */
	public function getRes() {
	}
	/**
	 * resize image width or height
	 * Compress the picture so that the width or height of barcode[qrcode] is less than 200px
	 * @param int $width set 0,from height scale size
	 * @param int $height set 0,from width scale size
	 * @return $this 
	 */
	public function resize($width, $height) {
	}
	/**
	 * set resize edge
	 * see ResizeEdge* const
	 * @param int $edge
	 * @return $this
	 */
	public function setResizeEdge($edge) {
	}
	/**
	 * set resize filter
	 * see ResizeFilter* const
	 * @param int $filter
	 * @return $this
	 */
	public function setResizeFilter($filter) {
	}
}

