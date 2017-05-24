CC = gcc
FLAGS = -Wall -pedantic -Werror
ODIR=obj
LDIR=include
SRCDIR=src
BDIR=cgi-bin
PATHOBJECTS_SERVER=$(addprefix $(ODIR)/,$(OBJECTS_SERVER))
PATHOBJECTS_KSAMP=$(addprefix $(ODIR)/,$(OBJECTS_KSAMP))
OBJECTS_KSAMP=ksamp.o
OBJECTS_SERVER=estaciones.o
obj-m += ksrc/helloWorld.o
SVRDIR = html

all: make_dirs $(BDIR)/estaciones.cgi $(BDIR)/ksamp.cgi $(BDIR)/modules.cgi helloWorld create_server_folder

make_dirs:
	mkdir -p obj
	mkdir -p cgi-bin
	mkdir -p build

$(BDIR)/estaciones.cgi: $(PATHOBJECTS_SERVER)
	$(CC) $(PATHOBJECTS_SERVER) -o $@

$(ODIR)/estaciones.o: $(SRCDIR)/estaciones.c $(LDIR)/funciones_estaciones.h
	$(CC) $(FLAGS) -c $< -o $@

$(BDIR)/ksamp.cgi: $(PATHOBJECTS_KSAMP)
	$(CC) $(PATHOBJECTS_KSAMP) -o $@

$(ODIR)/ksamp.o: $(SRCDIR)/ksamp.c $(LDIR)/ksamp.h
	$(CC) $(FLAGS) -c $< -o $@

$(BDIR)/modules.cgi: $(ODIR)/modules.o
	$(CC) obj/modules.o -o $@

$(ODIR)/modules.o: $(SRCDIR)/modules.c
	$(CC) $(FLAGS) -c $< -o $@

helloWorld:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

create_server_folder:
	cp -a web_pages/. $(SVRDIR)/
	cp -ar icons/ $(SVRDIR)/
	mv cgi-bin/ $(SVRDIR)/
	cp -ar style/ $(SVRDIR)/
	mkdir -p $(SVRDIR)/uploads
	mkdir -p $(SVRDIR)/files
	cp datos_meteorologicos.CSV $(SVRDIR)/
	cp -a scripts/. $(SVRDIR)/cgi-bin/

cppcheck:
	@echo
	@echo Realizando verificacion CppCheck
	@echo Los resultados de la misma se encuentran en err.txt
	cppcheck --enable=all . 2> err.txt

clean:
	@echo Borrando archivos
	rm -rf *o $(ODIR) $(BDIR) $(LDIR)/*.gch err.txt
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean