#include "utility.h"

gboolean is_valid_youtube_url(const gchar *url) {
    const gchar *pattern = "^(http(s)?:\\/\\/)?((w){3}.)?youtu(be|.be)?(\\.com)?\\/.+";
    regex_t regex;
    
    if (regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE) != 0) {
        return FALSE;
    }

    int result = regexec(&regex, url, 0, NULL, 0);
    regfree(&regex);

    return result == 0 ? TRUE : FALSE;
}
