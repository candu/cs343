TYPE:=int

UXX_ROOT=/home/fearlesstost/u++
CXX=${UXX_ROOT}/bin/u++
CXXFLAGS=-g -Wall -Wno-unused-label -MMD -DTYPE="${TYPE}"
UXX_EXTRA_FLAGS=-I${UXX_ROOT}/u++-5.7.0/inc -L${UXX_ROOT}/u++-5.7.0/lib
MAKEFILE_NAME=${firstword ${MAKEFILE_LIST}}

OBJECTS1=q1cardgame.o
EXEC1=cardgame

OBJECTS2=q2interference.o
EXEC2=interference

OBJECTS3=q3driver.o
EXEC3=quicksort

OBJECTS=${OBJECTS1} ${OBJECTS2} ${OBJECTS3}
DEPENDS=${OBJECTS:.o=.d}
EXECS=${EXEC1} ${EXEC2} ${EXEC3}

all: ${EXECS}

${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} ${UXX_EXTRA_FLAGS} $^ -o $@

${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} ${UXX_EXTRA_FLAGS} $^ -o $@

-include ImplType

ifeq (${IMPLTYPE},${TYPE})
${EXEC3} : ${OBJECTS3}
	${CXX} ${CXXFLAGS} ${UXX_EXTRA_FLAGS} $^ -o $@
else
ifeq (${TYPE},)
TYPE=${IMPLTYPE}
${EXEC3} : ${OBJECTS3}
	${CXX} ${CXXFLAGS} ${UXX_EXTRA_FLAGS} $^ -o $@
else
.PHONY : ${EXEC3}
${EXEC3} :
	rm -f ImplType
	touch q3quicksort.h
	${MAKE} TYPE="${TYPE}"
endif
endif

ImplType :
	echo "IMPLTYPE=${TYPE}" > ImplType

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean:
	rm -f *.d *.o ${EXECS} interference ImplType
