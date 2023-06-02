#include "utility.h"

#define PYTHONFILE "downloader.py"

void on_download_button_clicked(GtkButton *button, gpointer user_data) {
    // Ignore parameter not used
    (void)button;

    GtkWidget *entry = GTK_WIDGET(user_data);
    const gchar *url = gtk_entry_get_text(GTK_ENTRY(entry));

    if (!is_valid_youtube_url(url)) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Invalid YouTube URL!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    if (access(PYTHONFILE, R_OK) == -1) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s not found!", PYTHONFILE);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    gchar *command = g_strdup_printf("python %s %s", PYTHONFILE, url);
    system(command);
    g_free(command);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Download completed!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

gboolean on_window_closed(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    // ignore parameters not used
    (void)widget;
    (void)event;
    (void)user_data;

    gtk_main_quit();
    return TRUE;
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "YouTube Downloader");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 70);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "delete-event", G_CALLBACK(on_window_closed), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter the URL of the video");
    gtk_box_pack_start(GTK_BOX(box), entry, TRUE, TRUE, 0);

    GtkWidget *download_button = gtk_button_new_with_label("Download");
    g_signal_connect(download_button, "clicked", G_CALLBACK(on_download_button_clicked), entry);
    gtk_box_pack_start(GTK_BOX(box), download_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

