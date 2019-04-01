TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/compiler/CharHelpers.cpp \
    src/compiler/DoubleScanner.cpp \
    src/compiler/ErrorPrinter.cpp \
    src/compiler/ExprLexer.cpp \
    src/compiler/Operator.cpp \
    src/compiler/RpnBuilder.cpp \
    src/compiler/RpnCalculator.cpp \
    src/compiler/RpnExpr.cpp \
    src/compiler/RpnExprOperator.cpp \
    src/compiler/Sentence.cpp \
    src/compiler/TokenType.cpp \
    src/main.cpp


HEADERS += \
    src/compiler/CharHelpers.h \
    src/compiler/DoubleScanner.h \
    src/compiler/ErrorPrinter.h \
    src/compiler/ExprLexer.h \
    src/compiler/Operator.h \
    src/compiler/RpnBuilder.h \
    src/compiler/RpnCalculator.h \
    src/compiler/RpnExpr.h \
    src/compiler/RpnExprOperator.h \
    src/compiler/Sentence.h \
    src/compiler/TokenType.h \
    src/containers/impl/VectorIterator.h \
    src/containers/CharBuf.h \
    src/containers/Vector.h \
    src/gold/debug_assert.h


INCLUDEPATH += src

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
