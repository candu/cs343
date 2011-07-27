TYPE:=int

UXX_ROOT=/home/fearlesstost/u++
CXX=${UXX_ROOT}/bin/u++
CXXFLAGS=-g -Wall -Werror -Wfatal-errors -Wno-unused-label -MMD -DTYPE="${TYPE}"
CXXFLAGS_EXEC2=-g -Wall -MMD -DTYPE="${TYPE}"
UXX_EXTRA_FLAGS=-I${UXX_ROOT}/u++-5.7.0/inc -L${UXX_ROOT}/u++-5.7.0/lib
MAKEFILE_NAME=${firstword ${MAKEFILE_LIST}}

q1%.o : q1%.cc
	${CXX} ${CXXFLAGS} ${UXX_EXTRA_FLAGS} -c $< -o $@

OBJECTS1=q1cardgame.o
EXEC1=cardgame

EXEC2A=uniproc
EXEC2B=multiproc
EXEC2=${EXEC2A} ${EXEC2B}

OBJECTS3=q3driver.o
EXEC3=quicksort

OBJECTS=${OBJECTS1} ${OBJECTS3}
DEPENDS=${OBJECTS:.o=.d}
EXECS=${EXEC1} ${EXEC2} ${EXEC3} prngtest

all: ${EXECS}

${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} ${UXX_EXTRA_FLAGS} $^ -o $@

${EXEC2A} :
	${CXX} -o $@ q2unimulti.cc

${EXEC2B} :
	${CXX} -multi -o $@ q2unimulti.cc

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

prngtest:
	g++ -o prngtest PRNGTest.cc

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean:
	rm -f *.d *.o ${EXECS} ImplType prngtest
