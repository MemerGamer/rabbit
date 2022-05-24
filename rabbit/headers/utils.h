//
// General purpose helper functions
//

#ifndef CROSS_FOX_UTILS_H
#define CROSS_FOX_UTILS_H

#include <string.h>
#include <stdbool.h>

typedef enum RabbitContentType{
    RBT_CT_NOT_DEFINED,

    // Text
    RBT_CT_TXT_HTML,
    RBT_CT_TXT_CSS,
    RBT_CT_TXT_CSV,
    RBT_CT_TXT_PLAIN,
    RBT_CT_TXT_XML,

    // Application
    RBT_CT_APP_EDI_X_12,
    RBT_CT_APP_EDIFACT,
    RBT_CT_APP_JAVASCRIPT,
    RBT_CT_APP_OCTET_STREAM,
    RBT_CT_APP_OGG,
    RBT_CT_APP_PDF,
    RBT_CT_APP_XHTML_HTML,
    RBT_CT_APP_X_SHOCKWAVE_FLASH,
    RBT_CT_APP_JSON,
    RBT_CT_APP_LD_JSON,
    RBT_CT_APP_XML,
    RBT_CT_APP_ZIP,
    RBT_CT_APP_X_WWW_FORM_URLENCODED,

    // Audio
    RBT_CT_AUDIO_MPEG,
    RBT_CT_AUDIO_X_MS_WMA,
    RBT_CT_AUDIO_VND_RN_REALAUDIO,
    RBT_CT_AUDIO_X_WAV,

    // Image
    RBT_CT_IMG_GIF,
    RBT_CT_IMG_JPEG,
    RBT_CT_IMG_PNG,
    RBT_CT_IMG_TIFF,
    RBT_CT_IMG_VND_MICROSOFT_ICON,
    RBT_CT_IMG_X_ICON,
    RBT_CT_IMG_VND_DJVU,
    RBT_CT_IMG_SVG_XML,

    // Multipart
    RBT_CT_MULTIPART_MIXED,
    RBT_CT_MULTIPART_ALTERNATIVE,
    RBT_CT_MULTIPART_RELATED,
    RBT_CT_MULTIPART_FORM_DATA,

    //Video
    RBT_CT_VID_MPEG,
    RBT_CT_VID_MP4,
    RBT_CT_VID_QUICKTIME,
    RBT_CT_VID_X_MS_WMV,
    RBT_CT_VID_X_MSVIDEO,
    RBT_CT_VID_X_FLV,
    RBT_CT_VID_WEBM,


}RabbitContentType;

bool rbt_str_equals(char* str1, char* str2);
unsigned int rbt_get_num_of_chars(char* string, char find);
char* rbt_get_file_content_type(char* filepath);
void rbt_str_replace(char *target, const char *needle, const char *replacement);

#endif //CROSS_FOX_UTILS_H
