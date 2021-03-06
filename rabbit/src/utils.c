//
// General purpose helper functions
//

#include "../headers/utils.h"

bool rbt_str_equals(char* str1, char* str2){
    return strcmp(str1, str2) == 0;
}

unsigned int rbt_get_num_of_chars(char* string, char find){
    unsigned int count = 0;
    for (int i = 0; string[i]; i++){
        count += (string[i] == '.');
    }
    return count;
}

char *rbt_get_file_content_str(RabbitContentType contentType) {
    switch (contentType) {
        // Application
        case RBT_CT_APP_EDI_X_12: return "application/EDI-X12";
        case RBT_CT_APP_EDIFACT: return "application/EDIFACT";
        case RBT_CT_APP_JAVASCRIPT: return "application/javascript";
        case RBT_CT_APP_OCTET_STREAM: return "application/octet-stream";
        case RBT_CT_APP_OGG: return "application/ogg";
        case RBT_CT_APP_PDF: return "application/pdf";
        case RBT_CT_APP_XHTML_HTML: return "application/xhtml+xml";
        case RBT_CT_APP_X_SHOCKWAVE_FLASH: return "application/x-shockwave-flash";
        case RBT_CT_APP_JSON: return "application/json";
        case RBT_CT_APP_LD_JSON: return "application/ld+json";
        case RBT_CT_APP_XML: return "application/xml";
        case RBT_CT_APP_ZIP: return "application/zip";
        case RBT_CT_APP_X_WWW_FORM_URLENCODED: return "application/x-www-form-urlencoded";

        // Audio
        case RBT_CT_AUDIO_MPEG: return "audio/mpeg";
        case RBT_CT_AUDIO_X_MS_WMA: return "audio/x-ms-wma";
        case RBT_CT_AUDIO_VND_RN_REALAUDIO: return "audio/vnd.rn-realaudio";
        case RBT_CT_AUDIO_X_WAV: return "audio/x-wav";

        // Image
        case RBT_CT_IMG_GIF: return "image/gif";
        case RBT_CT_IMG_JPEG: return "image/jpeg";
        case RBT_CT_IMG_PNG: return "image/png";
        case RBT_CT_IMG_TIFF: return "image/tiff";
        case RBT_CT_IMG_VND_MICROSOFT_ICON: return "image/vnd.microsoft.icon";
        case RBT_CT_IMG_X_ICON: return "image/x-icon";
        case RBT_CT_IMG_VND_DJVU: return "image/vnd.djvu";
        case RBT_CT_IMG_SVG_XML: return "image/svg+xml";

        // Multipart
        case RBT_CT_MULTIPART_MIXED: return "multipart/mixed";
        case RBT_CT_MULTIPART_ALTERNATIVE: return "multipart/alternative";
        case RBT_CT_MULTIPART_RELATED: return "multipart/related";
        case RBT_CT_MULTIPART_FORM_DATA: return "multipart/form-data";

        // Text
        case RBT_CT_TXT_CSS: return "text/css";
        case RBT_CT_TXT_CSV: return "text/csv";
        case RBT_CT_TXT_HTML: return "text/html";
        case RBT_CT_TXT_JAVASCRIPT: return  "text/javascript";
        case RBT_CT_TXT_PLAIN: return "text/plain";
        case RBT_CT_TXT_XML: return "text/xml";

        // Video
        case RBT_CT_VID_MPEG: return "video/mpeg";
        case RBT_CT_VID_MP4: return "video/mp4";
        case RBT_CT_VID_QUICKTIME: return "video/quicktime";
        case RBT_CT_VID_X_MS_WMV: return "video/x-ms-wmv";
        case RBT_CT_VID_X_MSVIDEO: return "video/x-msvideo";
        case RBT_CT_VID_X_FLV: return "video/x-flv";
        case RBT_CT_VID_WEBM: return "video/webm";

        // VND (Application)
        case RBT_CT_VND_OASIS_OPENDOC_TXT: return "application/vnd.oasis.opendocument.text";
        case RBT_CT_VND_OASIS_OPENDOC_SPREADSHEET: return "application/vnd.oasis.opendocument.spreadsheet";
        case RBT_CT_VND_OASIS_OPENDOC_PRESENTATION: return "application/vnd.oasis.opendocument.presentation";
        case RBT_CT_VND_OASIS_OPENDOC_GRAPHICS: return "application/vnd.oasis.opendocument.graphics";
        case RBT_CT_VND_MS_EXCEL: return "application/vnd.ms-excel";
        case RBT_CT_VND_OPENXMLFORMATS_OFFICEDOC_SPREDSHEETML_SHEET: return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        case RBT_CT_VND_MS_POWERPOINT: return "application/vnd.ms-powerpoint";
        case RBT_CT_VND_OPENXMLFORMATS_OFFICEDOC_PRESENTATION: return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
        case RBT_CT_VND_MSWORD: return "application/msword";
        case RBT_CT_VND_OPENXMLFORMATS_OFFICEDOC_WORDPROCESSINGML_DOCUMENT: return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
        case RBT_CT_VND_MOZZILLA_XUL_XML: return "application/vnd.mozilla.xul+xml";

        case RBT_CT_NOT_DEFINED: return "text/plain"; //not defined data will be read as plain text
        default: return "text/html";    //default text/html
    }
}

char* rbt_get_file_content_type(char* filepath){
    char ext[10];
    strcpy(ext, strrchr(filepath, '.'));
    return rbt_get_file_content_str(rbt_filetype_to_content_type(ext));
}

void rbt_str_replace(char *target, const char *needle, const char *replacement){
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
}

RabbitContentType rbt_filetype_to_content_type(char *filetype) {
    //Prioritised
    if(rbt_str_equals(filetype, ".html")){
        return RBT_CT_TXT_HTML;
    }
    if(rbt_str_equals(filetype, ".css")){
        return RBT_CT_TXT_CSS;
    }
    if(rbt_str_equals(filetype, ".png")){
        return RBT_CT_IMG_PNG;
    }
    if(rbt_str_equals(filetype,".ico")){
        return RBT_CT_IMG_X_ICON;
    }
    if(rbt_str_equals(filetype,".svg")){
        return RBT_CT_IMG_SVG_XML;
    }
    if(rbt_str_equals(filetype, ".jpg")){
        return RBT_CT_IMG_JPEG;
    }
    if(rbt_str_equals(filetype,".json")){
        return RBT_CT_APP_JSON;
    }
    if(rbt_str_equals(filetype, ".js")){
        return RBT_CT_APP_JAVASCRIPT;
    }

    // Text
    if(rbt_str_equals(filetype, ".csv")){
        return RBT_CT_TXT_CSV;
    }
    if(rbt_str_equals(filetype, ".txt")){
        return RBT_CT_TXT_PLAIN;
    }


    // Images
    if(rbt_str_equals(filetype,".gif")){
        return RBT_CT_IMG_GIF;
    }
    if(rbt_str_equals(filetype,".tiff")){
        return RBT_CT_IMG_TIFF;
    }
    if(rbt_str_equals(filetype,".ICO")){
        return RBT_CT_IMG_VND_MICROSOFT_ICON;
    }
    if(rbt_str_equals(filetype,".djvu") || rbt_str_equals(filetype,".djv")){
        return RBT_CT_IMG_VND_DJVU;
    }

    // Audio
    if(rbt_str_equals(filetype,".mpeg")){
        return RBT_CT_AUDIO_MPEG;
    }
    if(rbt_str_equals(filetype,".x-ms-wma")){
        return RBT_CT_AUDIO_X_MS_WMA;
    }
    if(rbt_str_equals(filetype,".ra") || rbt_str_equals(filetype,".ram")){
        return RBT_CT_AUDIO_VND_RN_REALAUDIO;
    }
    if(rbt_str_equals(filetype,".wav") || rbt_str_equals(filetype,".wav")){
        return RBT_CT_AUDIO_X_WAV;
    }

    // Application
    if(rbt_str_equals(filetype,".zip")){
        return RBT_CT_APP_ZIP;
    }
    if(rbt_str_equals(filetype,".x12")){
        return RBT_CT_APP_EDI_X_12;
    }
    if(rbt_str_equals(filetype,".edi")){
        return RBT_CT_APP_EDIFACT;
    }
    if(rbt_str_equals(filetype, ".ogg")){
        return RBT_CT_APP_OGG;
    }
    if(rbt_str_equals(filetype, ".pdf")){
        return RBT_CT_APP_PDF;
    }
    if(rbt_str_equals(filetype, ".htm")||
        rbt_str_equals(filetype, ".xhtml") ||
        rbt_str_equals(filetype, ".xht")){
        return RBT_CT_APP_XHTML_HTML;
    }
    if(rbt_str_equals(filetype,".swf")){
        return RBT_CT_APP_X_SHOCKWAVE_FLASH;
    }
    if(rbt_str_equals(filetype,".jsonld")){
        return RBT_CT_APP_X_SHOCKWAVE_FLASH;
    }
    if(rbt_str_equals(filetype, ".xml")){
        return RBT_CT_TXT_XML;
    }
    if(rbt_str_equals(filetype, ".xml")){
        return RBT_CT_APP_X_WWW_FORM_URLENCODED;
    }

    // Video
    if(rbt_str_equals(filetype, ".mpg")){
        return RBT_CT_VID_MPEG;
    }
    if(rbt_str_equals(filetype,".mp4") ||
            rbt_str_equals(filetype,".m4a") ||
            rbt_str_equals(filetype,".m4p") ||
            rbt_str_equals(filetype,".m4b") ||
            rbt_str_equals(filetype,".m4r") ||
            rbt_str_equals(filetype,".m4v")){
        return RBT_CT_VID_MP4;
    }
    if(rbt_str_equals(filetype,".mov")){
        return RBT_CT_VID_QUICKTIME;
    }
    if(rbt_str_equals(filetype,".wmv")){
        return RBT_CT_VID_X_MS_WMV;
    }
    if(rbt_str_equals(filetype,".avi")){
        return RBT_CT_VID_X_MSVIDEO;
    }
    if(rbt_str_equals(filetype,".flv")){
        return RBT_CT_VID_X_FLV;
    }
    if(rbt_str_equals(filetype,".mkv")){
        return RBT_CT_VID_WEBM;
    }

    //Todo: VND and Multipart

    // Common files
    if(rbt_str_equals(filetype, ".ex_") ||
            rbt_str_equals(filetype, ".dl_") ||
            rbt_str_equals(filetype, ".saf_") ||
            rbt_str_equals(filetype, ".ini") ||
            rbt_str_equals(filetype, ".dst") ||
            // BlueZone Scripting
            rbt_str_equals(filetype, ".bzs") ||
            rbt_str_equals(filetype, ".bbs") ||
            rbt_str_equals(filetype, ".bbh") ||
            // BlueZone MainFrame
            rbt_str_equals(filetype, ".e3d") ||
            rbt_str_equals(filetype, ".e3p") ||
            rbt_str_equals(filetype, ".zmd") ||
            rbt_str_equals(filetype, ".zmp") ||
            rbt_str_equals(filetype, ".mds") ||
            rbt_str_equals(filetype, ".tn3") ||
            rbt_str_equals(filetype, ".mdt") ||
            rbt_str_equals(filetype, ".mdd") ||
            rbt_str_equals(filetype, ".mdk") ||
            rbt_str_equals(filetype, ".mdf") ||
            rbt_str_equals(filetype, ".mdb") ||
            rbt_str_equals(filetype, ".mdp") ||
            rbt_str_equals(filetype, ".mdr") ||
            // BlueZone ISeries
            rbt_str_equals(filetype, ".e5d") ||
            rbt_str_equals(filetype, ".e5p") ||
            rbt_str_equals(filetype, ".zad") ||
            rbt_str_equals(filetype, ".zap") ||
            rbt_str_equals(filetype, ".ads") ||
            rbt_str_equals(filetype, ".tn5") ||
            rbt_str_equals(filetype, ".adt") ||
            rbt_str_equals(filetype, ".add") ||
            rbt_str_equals(filetype, ".adk") ||
            rbt_str_equals(filetype, ".adf") ||
            rbt_str_equals(filetype, ".adb") ||
            rbt_str_equals(filetype, ".adp") ||
            rbt_str_equals(filetype, ".adr") ||
            // BlueZone VT
            rbt_str_equals(filetype, ".evt") ||
            rbt_str_equals(filetype, ".zvt") ||
            rbt_str_equals(filetype, ".vds") ||
            rbt_str_equals(filetype, ".vdd") ||
            rbt_str_equals(filetype, ".vdk") ||
            rbt_str_equals(filetype, ".vdf") ||
            rbt_str_equals(filetype, ".vdb") ||
            rbt_str_equals(filetype, ".vdp") ||
            rbt_str_equals(filetype, ".vdr") ||
            // BlueZone FTP
            rbt_str_equals(filetype, ".eft") ||
            rbt_str_equals(filetype, ".zft") ||
            // BlueZone TCP/IP
            rbt_str_equals(filetype, ".ztp") ||
            // BlueZone ICL 7561
            rbt_str_equals(filetype, ".z7d") ||
            rbt_str_equals(filetype, ".ids") ||
            rbt_str_equals(filetype, ".idc") ||
            rbt_str_equals(filetype, ".idd") ||
            rbt_str_equals(filetype, ".idk") ||
            rbt_str_equals(filetype, ".idf") ||
            rbt_str_equals(filetype, ".idb") ||
            rbt_str_equals(filetype, ".idp") ||
            rbt_str_equals(filetype, ".idr") ||
            // BlueZone T27
            rbt_str_equals(filetype, ".ztd") ||
            rbt_str_equals(filetype, ".tds") ||
            rbt_str_equals(filetype, ".tdd") ||
            rbt_str_equals(filetype, ".tdk") ||
            rbt_str_equals(filetype, ".tdf") ||
            rbt_str_equals(filetype, ".tdb") ||
            rbt_str_equals(filetype, ".tdp") ||
            rbt_str_equals(filetype, ".tdr") ||
            // BlueZone UTS
            rbt_str_equals(filetype, ".zud") ||
            rbt_str_equals(filetype, ".uds") ||
            rbt_str_equals(filetype, ".udd") ||
            rbt_str_equals(filetype, ".udk") ||
            rbt_str_equals(filetype, ".udf") ||
            rbt_str_equals(filetype, ".udb") ||
            rbt_str_equals(filetype, ".udp") ||
            rbt_str_equals(filetype, ".udr") ||
            // BlueZone Host File Transfer
            rbt_str_equals(filetype, ".adl") ||
            rbt_str_equals(filetype, ".adf")){
        return RBT_CT_APP_OCTET_STREAM;
    }
    return RBT_CT_NOT_DEFINED;
}
