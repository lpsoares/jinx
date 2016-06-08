#if!defined( __EJZ_H)
#define __EZJ_H

#include "buffer.h"


#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

char* 
PPM_alloc_buffer (const size_t width, const size_t height, const size_t maxval ); 

void 
PPM_free_buffer (char* buffer); 

EZJ_STATUS 
PPMset_bits ( BMP_BUFFER dest, const char* src, const size_t src_size ); 

EZJ_STATUS 
PPMset_all ( BMP_BUFFER dest, char* data, const size_t width, const size_t height, const size_t maxval ); 

/* JPEG<-> BMP */

EZJ_STATUS
JPEGToBMP_fm (char* input, BMP_BUFFER dest); // FileJPEG To BufferBMP

EZJ_STATUS
JPEGToBMP_mm (JPEG_BUFFER src, BMP_BUFFER dest); //BufferJPEG To BufferBMP

EZJ_STATUS
JPEGToBMP_ff (const char *input, const char *output); //FileJPEG To FileBMP

EZJ_STATUS
BMPToJPEG_ff (const char *input, const char *output); //FileBMP To FileJPEG

/* PPM <-> JPEG */

EZJ_STATUS
PPMToJPEG_ff (const char *input, const char *output); //FilePPM To FileJPEG

/* JPEG<-> PPM */

EZJ_STATUS
JPEGToPPM_fm (char* input, BMP_BUFFER dest); // FileJPEG To BufferPPM

EZJ_STATUS
JPEGToPPM_mm (JPEG_BUFFER src, BMP_BUFFER dest); //BufferJPEG To BufferPPM

EZJ_STATUS
JPEGToPPM_ff (const char *input, const char *output); //FileJPEG To FilePPM

EZJ_STATUS
PPMToJPEG_ff (const char *input, const char *output); //FilePPM To FileJPEG

EZJ_STATUS
PPMToJPEG_mm (BMP_BUFFER src, JPEG_BUFFER dest); //BufferPPM To BufferJPEG

// BufferPPM To BufferJPEG, compression
EZJ_STATUS
PPMToJPEG_mm_q (BMP_BUFFER src, JPEG_BUFFER dest, const int compress); 

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /*#define __EZJ_H */
