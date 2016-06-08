/*1* Project:   EzJ (Easy JPEG)
 *1* Purpose:   a wrapper designed to simplify JPEG processing
 *1* File:      buffer.h
 *1* Desc:      interface for buffer management
 *1* Version:   0.7 (beta testing)
 *1* Author:    h fernandes, f ferreira, t vilarinho
 *1* Creation:  2004-03-01
 *1* Reference: - None
 *1*/

#if !defined(_BUFFER_H)
#define _BUFFER_H

#if defined(__cplusplus)

extern "C" 
{
#endif /* defined(__cplusplus) */

  //#define GLOBAL(x) x


#define EZJ_VALID_STATUS(s) \
( ((int)s < EZJ_FIRST) || ((int)s > EZJ_LAST)? 0: 1 )


enum _EZJ_STATUS {
	/* BEWARE: 
     * don´t forget to update limits when new status values have been added 
	 */

	/* steps of JPEG processing in libjpeg */
	EZJ_FINISH_DECOMPRESS	= 12, /* decompression finish */
	EZJ_FINISH_COMPRESS		= 11, /* compression finish */
	EZJ_START_DECOMPRESS	= 10, /* decompression start */
	EZJ_START_COMPRESS		= 9,  /* compression start */
	EZJ_READHDR_DECOMPRESS	= 8,  /* reading header for decompression */
	EZJ_READHDR_COMPRESS	= 7,  /* reading header for compression */
	EZJ_SRC_DECOMPRESS		= 6,  /* setting decompression source */
	EZJ_SRC_COMPRESS		= 5,  /* setting compression source */
	EZJ_INIT_COMPRESS		= 4,  /* initializing for compression */
	EZJ_INIT_DECOMPRESS		= 3,  /* initializing for decompression */
	EZJ_DECOMPRESS			= 2,  /* decompression in general */ 
	EZJ_COMPRESS			= 1,  /* compression in general */

	/* all fine */
	EZJ_OK					= 0,

	/* error codes in EZJ processing */
	EZJ_NOT_OPEN_OUT		= -1,  /* file could not be opened for writing*/ 
	EZJ_NOT_OPEN_IN 		= -2,  /* file could not be opened for reading */ 
	EZJ_ERROR_FILENO		= -3,  /* invalid fileno */
	EZJ_JPEG_SIGNALED_ERROR	= -4,  /* ??? Error decoding JPEG */
	EZJ_ERROR_MALLOC		= -5,  /* could allocate memory */
	EZJ_NOT_WRITE_FILE		= -6,  /* file could not be written */
	EZJ_NOT_READ_FILE		= -7,  /* file could not be read */
	EZJ_INVALID_PARAM		= -8,  /* parameter invalid in function call */
	EZJ_ERROR_IJG			= -9, /* ??? Problem in libjpeg? */
	EZJ_ERROR_EOF_INPUT     = -10, /* End-of-file on input */
	EZJ_ERROR_EOF_OUTPUT    = -11, /* End-of-file on output */
	EZJ_ERROR_INTERNAL      = -12, /* Internal (unexpected) error */

	/* limits: don´t forget to update when new status values have been added */
	EZJ_FIRST = EZJ_ERROR_INTERNAL, 
	EZJ_LAST  = EZJ_FINISH_DECOMPRESS 
};

typedef enum _EZJ_STATUS EZJ_STATUS;

EZJ_STATUS GetStatus (void);
EZJ_STATUS SetStatus ( const EZJ_STATUS new_status );
char* GetStatusString (void);

struct _data_JPEG_BUFFER
{
	char* data;
	size_t height;
	size_t width;
	size_t n_components;
	size_t pos;
	size_t total_size;
	size_t quantum;
};// struct _data_JPEG_BUFFER
typedef  struct _data_JPEG_BUFFER *JPEG_BUFFER;

struct _data_BMP_BUFFER
{
	char* data;
	size_t height;
	size_t width;
	size_t n_components;
	size_t pos;
	size_t total_size;
}; // struct _data_BMP_BUFFER
typedef struct _data_BMP_BUFFER  *BMP_BUFFER;

#if !defined(UCHAR)
#define UCHAR unsigned char
#endif /* !defined(UCHAR) */

BMP_BUFFER BMP_BUFFER_alloc ( void );
int BMP_BUFFER_release ( BMP_BUFFER* outbuffer );

int BMP_BUFFER_rewind  ( BMP_BUFFER this1 ); 
int BMP_BUFFER_init    ( BMP_BUFFER outbuffer, const int width, const int height );
int BMP_BUFFER_init2   ( BMP_BUFFER outbuffer, const size_t size);
int BMP_BUFFER_deinit  ( BMP_BUFFER outbuffer );

int BMP_BUFFER_reinit  ( BMP_BUFFER outbuffer, const int width, const int height ); 
int BMP_BUFFER_reinit2 ( BMP_BUFFER outbuffer, const size_t size);

int BMP_BUFFER_putc  ( BMP_BUFFER this1, int c );
int BMP_BUFFER_getc  (BMP_BUFFER this1);
BMP_BUFFER BMP_BUFFER_append (BMP_BUFFER this1, const UCHAR* src, const size_t size);
int BMP_BUFFER_read(BMP_BUFFER this1, unsigned char * dst, const size_t size);
BMP_BUFFER BMP_BUFFER_create_fm (const char* input);
EZJ_STATUS BMP_BUFFER_mf (BMP_BUFFER src, const char *output);

JPEG_BUFFER JPEG_BUFFER_alloc ( void);

EZJ_STATUS JPEG_BUFFER_mf (JPEG_BUFFER src, const char *output);

EZJ_STATUS JPEG_BUFFER_init ( JPEG_BUFFER this1, const size_t total_size ); 
int JPEG_BUFFER_release ( JPEG_BUFFER* outbuffer );
int JPEG_BUFFER_deinit  ( JPEG_BUFFER outbuffer );
int JPEG_BUFFER_rewind  ( JPEG_BUFFER this1 ); 
int JPEG_BUFFER_read    (JPEG_BUFFER this1, UCHAR* dest, const size_t size);
JPEG_BUFFER JPEG_BUFFER_append (JPEG_BUFFER this1, const UCHAR * src, const size_t size);
EZJ_STATUS BMP_BUFFER_mf (BMP_BUFFER src, const char *output);
JPEG_BUFFER JPEG_BUFFER_create_fm (const char* input);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */


#endif /* !defined(_BUFFER_H) */
