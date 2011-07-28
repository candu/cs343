KIND:=NOBUSY
TYPE:=int
SENTINEL:=-1

UXX_ROOT=/home/fearlesstost/u++
CXX=${UXX_ROOT}/bin/u++
CXXFLAGS=-g -Wall -Werror -Wfatal-errors -Wno-unused-label -MMD -DTYPE="${TYPE}" -I${UXX_ROOT}/u++-5.7.0/inc -L${UXX_ROOT}/u++-5.7.0/lib
MAKEFILE_NAME=${firstword ${MAKEFILE_LIST}}

q1%.o : q1%.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

q2%.o : q2%.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

OBJECTS1=q1BoundedBuffer.o q1Producer.o q1Consumer.o q1Main.o
EXEC1=buffer

OBJECTS2=q2Main.o
EXEC2=eventcounter

OBJECTS=${OBJECTS1} ${OBJECTS2}
DEPENDS=${OBJECTS:.o=.d}
EXECS=${EXEC1} ${EXEC2}

all: ${EXECS} ImplKind ImplType

-include ImplKind

ifeq (${IMPLKIND},${KIND})
${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
else
ifeq (${KIND},)
KIND=${IMPLKIND}
${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
else
.PHONY : ${EXEC1}
${EXEC1} :
	rm -f ImplKind
	touch q1buffer.h
	${MAKE} ${EXEC1} KIND="${KIND}"
endif
endif

ImplKind:
	echo "IMPLKIND=${KIND}" > ImplKind

-include ImplType

ifeq (${IMPLTYPE},${TYPE})
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@
else
ifeq (${TYPE},)
TYPE=${IMPLTYPE}
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@
else
.PHONY : ${EXEC2}
${EXEC2} :
	rm -f ImplType
	touch q2buffer.h
	${MAKE} ${EXEC2} TYPE="${TYPE}" SENTINEL="${SENTINEL}"
endif
endif

ImplType :
	echo "IMPLTYPE=${TYPE}" > ImplType

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean:
	rm -f *.d *.o ${EXECS} ImplKind ImplType
