#include "utility.h"

#if defined(__APPLE__)
    #define PYTHONFILE "./src/downloader.py"
#elif defined(__linux__)
    #define PYTHONFILE "./src/downloader.py"
    #define ICONPATH "imgs/icon.png"
#endif

void run_python_process(const gchar *url, GdkPixbuf *icon) {
    pid_t pid = fork();
    if (pid == -1) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Fork Error!");
        gtk_window_set_icon(GTK_WINDOW(dialog), icon);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else if (pid == 0) {
        if (!is_valid_youtube_url(url)) {
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Invalid YouTube URL!");
            gtk_window_set_icon(GTK_WINDOW(dialog), icon);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }

        if (access(PYTHONFILE, R_OK) == -1) {
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s not found!", PYTHONFILE);
            gtk_window_set_icon(GTK_WINDOW(dialog), icon);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }

        execlp("python", "python", PYTHONFILE, url, NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            if (exit_code == 0) {
                GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Download completed!");
                gtk_window_set_icon(GTK_WINDOW(dialog), icon);
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
            } else {
                GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Error during video download!");
                gtk_window_set_icon(GTK_WINDOW(dialog), icon);
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
            }
        }
    }
}

void on_download_button_clicked(GtkButton *button, gpointer user_data) {
    // Ignore parameter not used
    (void)button;

    GtkWidget *entry = GTK_WIDGET(user_data);
    const gchar *url = gtk_entry_get_text(GTK_ENTRY(entry));
    GdkPixbuf *icon = gdk_pixbuf_new_from_file(ICONPATH, NULL);

    run_python_process(url, icon);
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
    gtk_window_set_icon_from_file(GTK_WINDOW(window), ICONPATH, NULL);

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

