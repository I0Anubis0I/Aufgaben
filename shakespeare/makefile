# Compiler und Flags
CC = gcc
CFLAGS = -Wall -Wextra -O2  

# Objektdateien
OBJ = shakespeare.o binarytree.o 

# Name des ausführbaren Programms
TARGET = shakespeare

# Standard-Target (Erstellung des Programms)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Kompiliere .o-Dateien
shakespeare.o: shakespeare.c binarytree.h
	$(CC) $(CFLAGS) -c shakespeare.c

binarytree.o: binarytree.c binarytree.h
	$(CC) $(CFLAGS) -c binarytree.c

# Bereinigung
clean:
	rm -f $(OBJ) $(TARGET)