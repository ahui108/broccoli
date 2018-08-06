#include <stdio.h>
#include <libintl.h>
#include <locale.h>

int main(int argc, char** argv)
{
    //Thanks you for your contribution to this program
    //printf(gettext("My Language is %s.\n"), my_language);
    setlocale(LC_ALL, "");
    textdomain("hello");
    bindtextdomain("hello", ".");
    printf(gettext("My Language is chinese\n"));
    //printf(_("Language is %s.\n"), my_language);
}
