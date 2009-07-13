#  Make procedure for memmon.ksh
#  @(#) makefile 6.1 09/07/06
#

CFLAGS=

# Install Directory
INSTDIR=/usr/local/bin

#  Memmon filter file
MEMFILT = memfilt

#  memmon.ksh script
V_MEMMON  = memmon.ksh

#  Target Dependencies
all: getdate $(V_MEMMON) $(MEMFILT)

install: getdate $(V_MEMMON) $(MEMFILT)
	@for FILE in getdate ${V_MEMMON} $(MEMFILT); do \
		cp $${FILE} ${INSTDIR}; \
		chmod 755 ${INSTDIR}/$${FILE}; \
	done

clean:
	rm -rf *.o

#  Rule Sets

getdate : getdate.o parse.o
		$(CC) -o $@ $(@F).o parse.o;

getdate.o: getdate.c

parse.o: parse.c

$(OBJS): $(SRC)
	for SOURCE in ${SRC}; do \
		$(CC) -c $${SOURCE} ; \
	done

$(SRC): parse.c getdate.c

$(MEMFILT): 

$(V_MEMMON):

.c.o:
	$(CC) $(CFLAGS) -c $<
