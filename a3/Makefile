UXX_ROOT=/home/fearlesstost/u++
CXX=${UXX_ROOT}/bin/u++
BUSY=BUSY
NOBUSY=NOBUSY
TYPEFLAGS=
ifeq (${TYPE},${BUSY})
  TYPEFLAGS=-DBUSY
else
ifeq (${TYPE},${NOBUSY})
  TYPEFLAGS=-DNOBUSY
endif
endif
   
CXXFLAGS=-g -Wall -Werror -Wfatal-errors -Wno-unused-label -MMD ${TYPEFLAGS} -I${UXX_ROOT}/u++-5.7.0/inc -L${UXX_ROOT}/u++-5.7.0/lib
MAKEFILE_NAME=${firstword ${MAKEFILE_LIST}}

%.o : %.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

OBJECTS1=NoBusyWaitBoundedBuffer.o BusyWaitBoundedBuffer.o BoundedBuffer.o Producer.o Consumer.o q1Main.o
EXEC1=buffer

OBJECTS2=Sequencer.o EventCounter.o q2Main.o
EXEC2=eventcounter

OBJECTS=${OBJECTS1} ${OBJECTS2}
DEPENDS=${OBJECTS:.o=.d}
EXECS=${EXEC1} ${EXEC2}

all: ${EXECS}

${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
 
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean:
	rm -f *.d *.o ${EXECS} 
