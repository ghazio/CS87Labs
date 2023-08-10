# This Makefile builds a pthreads gol program that uses ParaVis library
# (updated 2021)
#
# make: builds gol executable
# make report: builds report.pdf
# make clean: clean up built stuff
#
C = gcc
C++ = g++
CUDA = nvcc

# two different versions of CFLAGS (uncomment the one you want to use)
# for debugging:
CFLAGS = -g -Wall -Wvla -Werror -Wno-error=unused-variable
# for faster code build (with optimization on)
#CFLAGS = -02 -Wall -Wvla -Werror -Wno-error=unused-variable


# TODO this needs to be set for each card type
#CUDA architecture. Use deviceQuery |grep Major to find out version
#ARCH = 75
# thyme is version number 6.1
ARCH = 61

#cuda include/library paths
CUDAINCDIR = -I/usr/local/cuda-11.2/include
CUDALIBDIR = -L/usr/local/cuda-11.2/lib64

#qtvis include path
INCLUDEDIR = -I/usr/local/include/qtvis

#qt5 include path
QTINCDIR = -I/usr/include/x86_64-linux-gnu/qt5

#CUDA libraries, options
CUDALIBS = -lqtviscuda -lcudadevrt -lcudart_static -lrt -lpthread -ldl
COMPUTE = -forward-unknown-to-host-compiler \
          --generate-code=arch=compute_$(ARCH),code=[compute_$(ARCH),sm_$(ARCH)]

#Qt5 stuff
QTINCLUDES = $(QTINCDIR) \
	     $(QTINCDIR)/QtOpenGL \
	     $(QTINCDIR)/QtWidgets \
	     $(QTINCDIR)/QtGui \
	     $(QTINCDIR)/QtCore
DEFINES = -DQT_CORE_LIB -DQT_GUI_LIB -DQT_OPENGL_LIB -DQT_WIDGETS_LIB
OPTIONS = -fPIC
LIBS = $(LIBDIR) -lqtvis -lQt5OpenGL -lQt5Widgets -lQt5Gui -lQt5Core -lGLX -lOpenGL -lpthread

MAINPROG=gol
CUDAPROG=gol


all: $(CUDAPROG)

#linking with link path and libs
$(CUDAPROG): $(MAINPROG).o
	$(C++) $(CUDALIBDIR) -o $(CUDAPROG) \
	   $(MAINPROG).o $(LIBS) $(CUDALIBS)

#build the Qt5 side with no CUDA code/compiler
# this is to compile qt code that does not have CUDA
# (does need to include cudadir to compile), but can't include cuda
# kernel or device code in this, but can include cuda.h
$(MAINPROG).o: $(MAINPROG).c
	$(C++) $(CFLAGS) $(CUDAINCDIR) $(QTINCLUDES) $(INCLUDEDIR)\
		$(OPTIONS) -c $(MAINPROG).c


report: report.tex
	pdflatex report.tex

clean:
	$(RM) $(CUDAPROG) *.o *.pdf *.aux *.log *.out
