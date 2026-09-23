CXX = clang++

TARGET = app
BUILD = build

QT_HEADERS != qmake6 -query QT_INSTALL_HEADERS
QT_LIBS != qmake6 -query QT_INSTALL_LIBS
QT_PLUGINS != qmake6 -query QT_INSTALL_PLUGINS
QT_LIBEXECS != qmake6 -query QT_INSTALL_LIBEXECS

RCC = ${QT_LIBEXECS}/rcc
QRCFILE = resources.qrc
QRC_SRC = ${BUILD}/qrc_resources.cpp
QRC_OBJ = ${BUILD}/qrc_resources.o

INCLUDES = \
	-Iinclude \
	-I${QT_HEADERS} \
	-I${QT_HEADERS}/QtCore \
	-I${QT_HEADERS}/QtGui \
	-I${QT_HEADERS}/QtWidgets

CXXFLAGS = \
	-std=c++23 \
	-Wall \
	-Wextra \
	-O2 \
	-DQT_INSTALL_PLUGINS=\"${QT_PLUGINS}\" \
	${INCLUDES}

LIBS = \
	-L${QT_LIBS} \
	-lQt6Widgets \
	-lQt6Gui \
	-lQt6Core

SOURCES != find src -type f -name "*.cpp"

OBJECTS =

.for src in ${SOURCES}
OBJECTS += ${BUILD}/${src:T:R}.o
.endfor

all: ${TARGET}

${TARGET}: ${OBJECTS} ${QRC_OBJ}
	${CXX} ${OBJECTS} ${QRC_OBJ} ${LIBS} -o ${TARGET}

.for src in ${SOURCES}
${BUILD}/${src:T:R}.o: ${src}
	@mkdir -p ${BUILD}
	@echo " CXX     ${src}"
	${CXX} ${CXXFLAGS} -c ${src} -o ${.TARGET}
.endfor

${QRC_SRC}: ${QRCFILE} new_logo.png
	@mkdir -p ${BUILD}
	@echo " RCC     ${QRCFILE}"
	${RCC} ${QRCFILE} -o ${.TARGET}

${QRC_OBJ}: ${QRC_SRC}
	@echo " CXX     ${QRC_SRC}"
	${CXX} ${CXXFLAGS} -c ${QRC_SRC} -o ${.TARGET}

clean:
	rm -rf ${BUILD} ${TARGET}

run: ${TARGET}
	./${TARGET}

.PHONY: all clean run
