#pragma once

#include <gtk/gtk.h>
#include <regex.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

gboolean is_valid_youtube_url(const gchar *url);
