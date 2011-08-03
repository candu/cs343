UXX_ROOT=/home/fearlesstost/u++
CXX=${UXX_ROOT}/bin/u++
CXXFLAGS=-g -Wall -Werror -Wfatal-errors -Wno-unused-label -MMD -DTYPE="${TYPE}" -I${UXX_ROOT}/u++-5.7.0/inc -L${UXX_ROOT}/u++-5.7.0/lib
MAKEFILE_NAME=${firstword ${MAKEFILE_LIST}}

%.o : %.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

OBJECTS1=BoundedBuffer.o Producer.o Consumer.o q1Main.o
EXEC1=buffer

OBJECTS2=q2Main.o
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
