QT       += core gui sql xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseAnime
TEMPLATE = app


VERSION = 1.2.0.4
#Last digit:
# 0 - Pre-Alpha
# 1 - Alpha
# 2 - Beta
# 3 - Release Candidate
# 4 - Release

RC_ICONS = images/DBA_Icon.ico
QMAKE_TARGET_COMPANY = LibertaSoft
QMAKE_TARGET_PRODUCT = DatabaseAnime
QMAKE_TARGET_DESCRIPTION = Your database of an anime
QMAKE_TARGET_COPYRIGHT = (CopyLeft) LibertaSoft

HEADERS += \
    mainwindow.h \
    mngrconnection.h \
    mngrquerys.h \
    dialogs/addamv.h \
    dialogs/addanime.h \
    dialogs/adddorama.h \
    dialogs/addmanga.h \
    dialogs/settings.h \
    widgets/imagecover.h \
    widgets/lookprogressbar.h \
    widgets/stackwgt_mousehideover.h \
    definespath.h \
    globalenum.h \
    xmldbareader.h \
    xmldbawriter.h \
    dbalocalization.h \
    shikimoriapi.h \
    dbasettings.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mngrconnection.cpp \
    mngrquerys.cpp \
    dialogs/addamv.cpp \
    dialogs/addanime.cpp \
    dialogs/adddorama.cpp \
    dialogs/addmanga.cpp \
    dialogs/settings.cpp \
    widgets/imagecover.cpp \
    widgets/lookprogressbar.cpp \
    widgets/stackwgt_mousehideover.cpp \
    definespath.cpp \
    xmldbareader.cpp \
    xmldbawriter.cpp \
    dbalocalization.cpp \
    shikimoriapi.cpp \
    dbasettings.cpp

FORMS += \
    mainwindow.ui \
    addamv.ui \
    addanime.ui \
    adddorama.ui \
    addmanga.ui \
    settings.ui \

OTHER_FILES += \
    README.md \
    README_eng.md \
    ToDo.md \
    ChangeLog.md

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    DatabaseAnime_ru.ts \
    DatabaseAnime_en.ts
