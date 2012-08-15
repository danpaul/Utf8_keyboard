/******************************************************************************
  Title          : hwk2_breczinski.c
  Author         : Daniel Breczinski
  Created on     : September, 2011
  Description    : GUI UTF8 keyboard. Use this keyboard to create GUI based
		   keyboard to generate custom Utf8 keyboards. See the fucntion 
		   comments regarding specifics of functions. If you simply want
		   to add additional alphabets, see the buildNotebook() function 
		   and follow the form. If you are using a Utf8 text editor, 
		   simply paste in the alphabet and follow the form within the
		   function to add a new international alphabet.
  Purpose        :
  Usage          : ./Utf8_keyboard
  Build with     : gcc -o  Utf8_keyboard Utf8_keyboard.c
  Modifications  :

******************************************************************************/

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>


#define WINDOW_LABEL        "Non-ASCII Charecter Select"
#define LABEL_TEXT1         "Click on a tab to select language.  Click on a charecter to add it to the text box."
#define LABEL_TEXT2         "Click on a charecter to add it to the text box."
#define INPUT_BOX_LABEL     "Text to copy:"


const static char toplabel1[] = "Click on a tab to select language.";


/*******************************************************************************
PRECONDITION:   Takes pointer to notebook and entry widget.
POSTCONDITION:  Constructs notebook.  Each page of notebook contains characters
                for a language.  Each character set and the dimensions of the
                buttons columns and rows are defined withing this function.
*******************************************************************************/
static void buildNotebook(GtkNotebook *notebook, GtkEntry *entryBox);


/*******************************************************************************
PRECONDITION:   Helper function for buildNotebook.
                Takes pointer to notebook, entry, a gchar, null-pointed char
                string and the pointer to gchar string.
POSTCONDITION:  Creates a page and adds it to the notebook.  Each page is a
                grid of buttons.  Each buttons corresponds to a letter of
                alphabet passed to function.
*******************************************************************************/
static void buildPage(GtkNotebook *notebook, GtkEntry *entryBox,
                      gchar *alphabet, gchar *pageTitle);

/*******************************************************************************
PRECONDITION:   Takes pointers to a button and GtkEntry.
POSTCONDITION:  Appends contents of button's label to entryBox.
*******************************************************************************/
static void addCharToEntryBox(GtkWidget *button, GtkEntry *entryBox);


static void destroy (GtkWidget*, gpointer);


int main (int argc,
          char *argv[])
{
  GtkWidget *window, *vbox, *hbox, *notebook, *topLabel,
    *textBoxLabel, *quitButton;

  GtkEntry *entryBox;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), WINDOW_LABEL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 1);
  gtk_window_set_default_size(window, 250, 100);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  vbox = gtk_vbox_new(FALSE,0);
  hbox = gtk_hbox_new(FALSE,0);

  topLabel = gtk_label_new(LABEL_TEXT1);
  gtk_label_set_line_wrap(topLabel, TRUE);

  gtk_box_pack_start(GTK_BOX(hbox), topLabel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

/*******************************************************************************
                              NOTEBOOK and ENTRY BOX
*******************************************************************************/
    notebook = gtk_notebook_new();
    entryBox = gtk_entry_new ();
    buildNotebook(notebook, entryBox);
    textBoxLabel = gtk_label_new(INPUT_BOX_LABEL);

/*******************************************************************************
                              PACKING and QUIT BUTTON
*******************************************************************************/

    quitButton = gtk_button_new_with_mnemonic("Q_uit");

    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
    hbox = gtk_hbox_new(FALSE,0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_box_pack_start(GTK_BOX(hbox), textBoxLabel, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), entryBox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), quitButton, TRUE, TRUE, 0);

/*******************************************************************************
                              SIGNAL CONNECT and INITIALIZATION
*******************************************************************************/

    g_signal_connect(G_OBJECT(quitButton), "clicked",
                    G_CALLBACK(destroy), NULL);

    g_signal_connect(G_OBJECT(window), "destroy",
                    G_CALLBACK(destroy), NULL);

    gtk_widget_show_all (window);

    gtk_main ();

    return 0;
}


static void buildNotebook(GtkNotebook *notebook, GtkEntry *entryBox){

    gchar russian[] = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя\n";
    gchar hebrew[] = "שנבגקכעיןחלךצמםפ/רדאוה'סטז\n";
    gchar arabic[] =   "يوهنملكقفغعظطضصشسزرذدخحجثتبأ\n";

    gchar title[]="Russian";
    buildPage(notebook, entryBox, russian, title);
    gchar title2[]="Hebrew";
    buildPage(notebook, entryBox, hebrew, title2);
    gchar title3[]="Arabic";
    buildPage(notebook, entryBox, arabic, title3);
}


static void buildPage(GtkNotebook *notebook, GtkEntry *entryBox,
                      gchar *alphabet, gchar *pageTitle){


    GtkWidget *label, *charLabel, *button, *charTable1, *vbox;

    unsigned int buttonSize = 35;
    unsigned int numberOfRows = 3;
    unsigned int numberOfColumns = 8;
    int rowCount = 0;
    int tmpCounter = 0;
    gchar nextChar[4];

    gchar *placeInString = alphabet;

    charTable1 = gtk_table_new(numberOfRows, numberOfColumns, TRUE);

    for(rowCount; rowCount < numberOfRows; rowCount++){
      int columnCount = 0;
      for(columnCount; columnCount < numberOfColumns; columnCount++){
          g_utf8_strncpy(nextChar, placeInString, 1);
          placeInString = g_utf8_find_next_char(placeInString,NULL);
          g_utf8_strncpy(nextChar, placeInString, 1);
          charLabel = gtk_label_new(nextChar);

          button = gtk_button_new_with_label(nextChar);
          gtk_widget_set_size_request(button, buttonSize, buttonSize);
          gtk_table_attach (GTK_TABLE (charTable1), button, columnCount,
                            columnCount+1, rowCount, rowCount+1,
                            GTK_EXPAND, GTK_SHRINK, 0, 0);

        gunichar toPass = nextChar;
        g_signal_connect(G_OBJECT(button), "clicked",
                         G_CALLBACK (addCharToEntryBox), (gpointer) entryBox);
        tmpCounter++;
      }

    }
    //g_free(placeInString); //need to free gchar
    label = gtk_label_new (pageTitle);
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), charTable1, label);
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
}

static void addCharToEntryBox(GtkWidget *button, GtkEntry *entryBox){
    gchar *labelText = gtk_button_get_label(button);
    guint tmp_pos = GTK_ENTRY(entryBox)->text_length;
    gtk_editable_insert_text (GTK_EDITABLE (entryBox),
                              labelText, -1, &tmp_pos);
}

static void
destroy (GtkWidget *window,
         gpointer data)
{
  gtk_main_quit ();
}
