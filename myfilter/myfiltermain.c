#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>

//Funktion Deklaration:
void usage(const char *prog_name);
char* find_first_textfile();
int count_characters(const char *filename);
int count_words(const char *filename);
int count_lines(const char *filename);

//Main:
int main(int argc, char *argv[]){
    int opt;
    char *filename=NULL;

    // Überprüfen, ob keine Argumente übergeben wurden
    if (argc == 1) {
        usage(argv[0]);
    }

    // Optionen parsen
    while ((opt = getopt(argc, argv, "i:cwl")) != -1) {
        switch (opt) {
            case 'i':
                filename = optarg;  // Der Wert nach -i ist der Dateiname
                break;
            case 'c':
                if (filename == NULL) {
                    filename = find_first_textfile(); // Standard-Textdatei verwenden
                    if (filename == NULL) {
                        printf("Keine Textdatei gefunden!\n");
                        return 1;
                    }
                }

                int char_count = count_characters(filename);
                if (char_count >= 0) {
                    printf("Anzahl der Zeichen in %s: %d\n", filename, char_count);
                }
                break;
            case 'w':
            if (filename == NULL) {
                    filename = find_first_textfile(); // Standard-Textdatei verwenden
                    if (filename == NULL) {
                        printf("Keine Textdatei gefunden!\n");
                        return 1;
                    }
                }

                int word_count = count_words(filename);
                if (word_count >= 0) {
                    printf("Anzahl der Wörter in %s: %d\n", filename, word_count);
                }
                break;
            case 'l':
            if (filename == NULL) {
                    filename = find_first_textfile(); // Standard-Textdatei verwenden
                    if (filename == NULL) {
                        printf("Keine Textdatei gefunden!\n");
                        return 1;
                    }
                }

                int line_count = count_lines(filename);
                if (line_count >= 0) {
                    printf("Anzahl der Zeilen in %s: %d\n", filename, line_count);
                }
                break; 
            default:
                usage(argv[0]);
                break;
        }
    }


    return 0;
}

//Function definition:

//"Usage" Ausgabe bei keinem oder ungültigem Eingabeparameter
void usage(const char *prog_name) {
    printf("Usage: %s [-i \"Dateiname\" Textdatei wechseln] [-c Zähle alle Zeichen] [-w Zähle alle Wörter] [-l Zähle alle Zeilen]\n", prog_name);
    exit(1);
}

// Funktion, um die erste Textdatei im aktuellen Verzeichnis zu finden
char* find_first_textfile() {
    DIR *dir = opendir(".");  // Aktuelles Verzeichnis öffnen
    struct dirent *entry;
    if (dir == NULL) {
        perror("Fehler beim Öffnen des Verzeichnisses");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        // Nur reguläre Dateien mit der Endung ".txt"
        if (strstr(entry->d_name, ".txt") != NULL) {
            closedir(dir);
            return strdup(entry->d_name);  // Erster Treffer wird zurückgegeben
        }
    }

    closedir(dir);
    return NULL;  // Keine Textdatei gefunden
}

//Zeichen zählen
int count_characters(const char *filename) {
    FILE *file = fopen(filename, "r"); // Datei im Lesemodus öffnen
    if (file == NULL) {
        perror("Fehler beim Öffnen der Datei");
        return -1;
    }

    int count = 0;
    int ch;

    // Zeichenweise durch die Datei iterieren
    while ((ch = fgetc(file)) != EOF) {
        count++;
    }

    fclose(file); // Datei schließen
    return count;
}

//Zähle Wörter
int count_words(const char *filename) {
    FILE *file = fopen(filename, "r"); // Datei im Lesemodus öffnen
    if (file == NULL) {
        perror("Fehler beim Öffnen der Datei");
        return -1;
    }

    int word_count = 0;
    int in_word = 0; // Flag, ob wir gerade innerhalb eines Wortes sind
    int ch, prev_ch = 0;

    // Zeichenweise durch die Datei iterieren
    while ((ch = fgetc(file)) != EOF) {
        if (isspace(ch)) { // Leerzeichen, Tabulator oder Zeilenumbruch beendet ein Wort
            if (in_word) {
                in_word = 0; // Ende des Wortes
                word_count++;
            }
        } else if (isalnum(ch) || ch == '-' || ch == '\'') {
            // Ein alphanumerisches Zeichen, Bindestrich oder Apostroph gehört zu einem Wort
            if (!in_word) {
                in_word = 1; // Anfang eines neuen Wortes
            }
        } else {
            // Sonderzeichen beendet ein Wort
            if (in_word && !(prev_ch == '-' || prev_ch == '\'')) {
                in_word = 0;
                word_count++;
            }
        }
        prev_ch = ch; // Vorheriges Zeichen speichern
    }

    // Falls die Datei mit einem Wort endet, das nicht von Leerzeichen abgeschlossen ist
    if (in_word) {
        word_count++;
    }

    fclose(file); // Datei schließen
    return word_count;
}

//Zähle Zeilen
int count_lines(const char *filename) {
    FILE *file = fopen(filename, "r"); // Datei im Lesemodus öffnen
    if (file == NULL) {
        perror("Fehler beim Öffnen der Datei");
        return -1;
    }

    int line_count = 0;
    int ch;

    // Zeichenweise durch die Datei iterieren
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') { // Zeilenumbruch-Zeichen gefunden
            line_count++;
        }
    }

    fclose(file); // Datei schließen
    return line_count + 1; // +1, um auch die letzte Zeile ohne '\n' zu zählen
}