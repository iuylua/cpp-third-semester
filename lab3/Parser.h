//обрабатывает аргументы командной строки программы
#pragma once

#include <string>
#include <vector>

struct ProgramArgs {
    std::string config_file;
    std::string output_file;
    std::vector<std::string> input_files;
    bool show_help = false; //нужна ли справка?
};

ProgramArgs parse_command_line(int argc, char* argv[]);//парсинг аргументов
void print_usage();