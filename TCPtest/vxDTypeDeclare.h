/**
* @file    vxDTypeDeclare.h
* <pre>
* Copyright (c) 2010-2015, ����ǧ�� All rights reserved.
* 
* �ļ����ƣ�vxDTypeDeclare.h
* �������ڣ�2015��6��19��
* �ļ���ʶ��
* �ļ�ժҪ��������������������
* 
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�
* ������ڣ�2015��6��19��
* �汾ժҪ��
* 
* ȡ���汾��
* ԭ����  ��
* ������ڣ�
* �汾ժҪ��
* </pre>
*/

#ifndef __VX_DTYPE_DECLARE__
#define __VX_DTYPE_DECLARE__

////////////////////////////////////////////////////////////////////////////////

#define X_FALSE           0
#define X_TRUE            1
#define X_NULL            0
#define X_ERR_OK          0
#define X_ERR_UNKNOW      (-1)

typedef char              x_char_t;
typedef const char        x_cchar_t;
typedef unsigned char     x_uchar_t;
typedef wchar_t           x_wchar_t;

typedef char              x_int8_t;
typedef short             x_int16_t;
typedef int               x_int32_t;
typedef __int64           x_int64_t;

typedef long              x_long_t;
typedef long long         x_llong_t;

typedef unsigned char     x_uint8_t;
typedef unsigned short    x_uint16_t;
typedef unsigned int      x_uint32_t;
typedef unsigned __int64  x_uint64_t;

typedef unsigned int      x_bool_t;
typedef char            * x_string_t;
typedef const char      * x_cstring_t;
typedef wchar_t         * x_wstring_t;
typedef const wchar_t   * x_cwstring_t;
typedef void            * x_handle_t;
typedef void              x_void_t;
typedef void            * x_pvoid_t;

#ifdef _WIN64
typedef unsigned __int64  x_size_t;
typedef unsigned __int64  x_lptr_t;
typedef unsigned __int64  x_sockfd_t;
#else // _WIN64
typedef unsigned int      x_size_t;
typedef unsigned int      x_lptr_t;
typedef unsigned int      x_sockfd_t;
#endif // _WIN64

/*
* @brief
* @verbatim
* <pre>
* 
* Variable name prefix
* @ xct_  : align at  8, for x_char_t, x_cchar_t, x_uchar_t, x_int8_t
* @ xwt_  : align at 16, for x_wchar_t, x_int16_t, x_uint16_t
* @ xit_  : align at 32, for x_int32_t
* @ xut_  : align at 32, for x_uint32_t
* @ xlt_  : align at 32, for x_long_t
* @ xlit_ : align at 64, for x_int64_t, x_llong_t
* @ xlut_ : align at 64, for x_uint64_t
* @ xbt_  : for x_bool_t
* @ xszt_ : for x_string_t, x_cstring_t
* @ xht_  : for x_handle_t
* @ xpt_  : for x_pvoid_t
* @ xst_  : for x_size_t
* @ xlpt_ : for x_lptr_t
* @ xfdt_ : for x_sockfd_t
* 
* </pre>
* @endverbatim
*/

////////////////////////////////////////////////////////////////////////////////

#define TEXT_LEN_16       16
#define TEXT_LEN_24       24
#define TEXT_LEN_32       32
#define TEXT_LEN_64       64
#define TEXT_LEN_96       96
#define TEXT_LEN_128      128
#define TEXT_LEN_256      256
#define TEXT_LEN_512      512
#define TEXT_LEN_768      768
#define TEXT_LEN_1K       1024
#define TEXT_LEN_2K       2048
#define TEXT_LEN_PATH     260

////////////////////////////////////////////////////////////////////////////////

#endif // __VX_DTYPE_DECLARE__

