cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 17)

# Все файлы в текущей папке
set(SOURCES
        main.cpp
        Parser.cpp
        ConfigParser.cpp
        ConverterFactory.cpp
        WAVReader.cpp
        WAVWriter.cpp
        MuteConverter.cpp
        MixConverter.cpp
        RobotVoiceConverter.cpp
)

set(HEADERS
        Parser.h
        ConfigParser.h
        ConverterFactory.h
        WAVReader.h
        WAVWriter.h
        Converter.h
        AudioStream.h
        MuteConverter.h
        MixConverter.h
        RobotVoiceConverter.h
)

add_executable(lab3 ${SOURCES} ${HEADERS})
