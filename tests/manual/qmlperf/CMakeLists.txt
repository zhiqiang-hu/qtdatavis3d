qt_add_executable(qmlperf
    GUI
    SOURCES
        datagenerator.cpp datagenerator.h
        main.cpp
    PUBLIC_LIBRARIES
        Qt::Gui
)

set(qmlperf_resource_files
    "qml/qmlperf/main.qml"
)

qt_add_resource(qmlperf "qmlperf"
    PREFIX
        "/"
    FILES
        ${qmlperf_resource_files}
)
