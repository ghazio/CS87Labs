#
# makefile for lab 4, Fall 2021
#
# make: builds oddevensort
# make clean: remove all compled files
# make questions: build .pdf from questions.tex 
#


CC = mpicc
CFLAGS = -g -Wall -Wvla -Werror -Wno-error=unused-variable -Wno-error=unused-but-set-variable

LIBDIR = -L./
INCDIR = -I./

TARGET = oddevensort

.PHONY: clean

all: $(TARGET)  questions

$(TARGET): $(TARGET).c 
	$(CC) $(INDIRS) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIBS) 


questions: questions.tex
	pdflatex questions.tex

clean:
	$(RM) $(TARGET) questions.pdf *.aux *.log *.out *.dvi
