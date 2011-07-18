UXX_ROOT=/home/fearlesstost/u++
CXX=${UXX_ROOT}/bin/u++
CXXFLAGS=-g -Wall -Wno-unused-label -MMD -I${UXX_ROOT}/u++-5.7.0/inc -L${UXX_ROOT}/u++-5.7.0/lib
MAKEFILE_NAME=${firstword ${MAKEFILE_LIST}}

OBJECTS1=q1localglobalfixup.o
EXEC1=localglobalfixup

OBJECTS2=q2.o
EXEC2=postal

OBJECTS=${OBJECTS_1} ${OBJECTS_2}
DEPENDS=${OBJECTS:.o=.d}
EXECS=${EXEC1} ${EXEC2}

all: ${EXECS}

q1%.o : q1%.cc
	g++ ${CXXFLAGS} -c $< -o $@

${EXEC1} : ${OBJECTS1}
	g++ ${CXXFLAGS} $^ -o $@

${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean:
	rm -f *.d *.o ${EXECS}
