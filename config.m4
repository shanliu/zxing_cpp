
PHP_ARG_WITH([zxing_cpp],
    [for zxing_cpp support],
    [AS_HELP_STRING([--with-zxing_cpp],
    [Include zxing_cpp support])])

PHP_ARG_WITH(zxing-cpp-dir,	for zxing_cpp,
[	zxing-cpp-dir[=DIR]	 Set the path to zxing-cpp install prefix.], yes)

PHP_ARG_WITH(stb-dir, for zxing_cpp,
[	stb-dir[=DIR]	 Set the path to stb-dir install prefix.], yes)


if test "$PHP_ZXING_CPP" != "no"; then
	AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
	
	CFLAGS="-std=c11 -g -O2 -pedantic"
	CXXFLAGS="-std=c++17 -fexceptions -fno-rtti -g -O2 -pedantic"
	PHP_REQUIRE_CXX()

	case $host in
		*darwin*)
			PHP_ADD_LIBRARY(c++,1,ZXING_CPP_SHARED_LIBADD)
		  ;;
		*)
		PHP_ADD_LIBRARY(stdc++,1,ZXING_CPP_SHARED_LIBADD)
		  ;;
	esac
	
	if test "$PHP_STB_DIR" = "yes"; then
		if test -r /usr/include/stb/stb_image.h;
		then
			PHP_ADD_INCLUDE(/usr/include/stb)	
		else
			AC_MSG_ERROR([stb image not found])
		fi
	else 
		if test -r $PHP_STB_DIR/stb_image.h;
		then
			PHP_ADD_INCLUDE($PHP_STB_DIR)	
		else
			AC_MSG_ERROR([stb image path not find stb_image.h])
		fi
	fi
	
	if test "$PHP_ZXING_CPP_DIR" = "yes" -a -x $PKG_CONFIG; then
		
		AC_MSG_CHECKING([for zxing-cpp using pkg-config])

		if ! $PKG_CONFIG --exists zxing ; then
			AC_MSG_ERROR([zxing not found])
		fi

		PHP_ZXING_VERSION=`$PKG_CONFIG zxing --modversion`
		AC_MSG_RESULT([found version $PHP_ZXING_VERSION])

		if ! $PKG_CONFIG zxing --atleast-version 1.2.0 ; then
			AC_MSG_ERROR([zxing must be version 1.2.0 or greater])
		fi

		PHP_ZXING_LIBS=`$PKG_CONFIG zxing --libs`
		PHP_ZXING_INCS=`$PKG_CONFIG zxing --cflags`

	    PHP_EVAL_LIBLINE($PHP_ZXING_LIBS, ZXING_CPP_SHARED_LIBADD)
	    PHP_EVAL_INCLINE($PHP_ZXING_INCS)
	else
		if test -r $PHP_ZXING_CPP_DIR/lib/libZXing.so;
		then
			PHP_ADD_LIBRARY_WITH_PATH(ZXing, $PHP_ZXING_CPP_DIR/lib/, ZXING_CPP_SHARED_LIBADD)
		else
			AC_MSG_ERROR([zxing path $PHP_ZXING_CPP_DIR not find libZXing.so])
		fi
		
		if test -r $PHP_ZXING_CPP_DIR/include/ZXing/ReadBarcode.h;
		then
			PHP_ADD_INCLUDE($PHP_ZXING_CPP_DIR/include/ZXing/)	
		else
			AC_MSG_ERROR([zxing path $PHP_ZXING_CPP_DIR not find hander file])
		fi
	fi
  
	PHP_SUBST(ZXING_CPP_SHARED_LIBADD)
	AC_DEFINE(HAVE_ZXING_CPP, 1, [ Have zxing_cpp support ])
	PHP_NEW_EXTENSION(zxing_cpp, zxing_common.c zxing_cpp_lib.cpp zxing_cpp.c zxing_read_class.c zxing_exception_class.c zxing_image_class.c zxing_read_result_class.c zxing_write_result_class.c zxing_write_class.c ,$ext_shared)
fi
