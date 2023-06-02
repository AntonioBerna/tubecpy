#pragma once

#include <gtk/gtk.h>
#include <unistd.h>
#include <regex.h>

gboolean is_valid_youtube_url(const gchar *url);
