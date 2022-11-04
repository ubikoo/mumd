/*
 * test-file.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "atto/core/core.hpp"
#include "test-file.hpp"
using namespace atto;

/** ---- File utilities -------------------------------------------------------
 */
void infolog(core::File *file)
{
    std::cout << core::str_format(
        "is_open %d, is_binary %d, is_good %d, is_error %d, is_eof %d\n",
        file->is_open(),
        file->is_binary(),
        file->is_good(),
        file->is_error(),
        file->is_eof());
}

/**
 * test_core_file
 * @brief test core::File api.
 */
void test_core_file(void)
{
    /* ---- Test 1: FileStdIn and FileStdOut ----------------------------------
     */
    std::cout << "\n>>> Test 1 FileStdIn and FileStdOut\n";
    std::vector<std::string> buffer_stdin;

    try {
        core::FileStdIn fin;
        std::cout << "\n>>> Test 1 readlines:\n";
        infolog(static_cast<core::File *>(&fin));
        fin.readlines(buffer_stdin);

        core::FileStdOut fout;
        std::cout << "\n>>> Test 1 writeline, sep='\\n':\n";
        infolog(static_cast<core::File *>(&fout));
        fout.writelines(buffer_stdin, "\n");

        std::cout << "\n>>> Test 1 writeline, sep='(@)':\n";
        infolog(static_cast<core::File *>(&fout));
        fout.writelines(buffer_stdin, "(@)");

        std::cout << "\n>>> Test 1 writeline, sep='\\n':\n";
        for (auto &line : buffer_stdin) {
            fout.writeline(line,"\n");
        }
        std::cout << "\n>>> Test 1 writeline, sep='(@)':\n";
        for (auto &line : buffer_stdin) {
            fout.writeline(line,"(@)");
        }

        std::cout << "\n>>> Test 1 printf:\n";
        for (auto &line : buffer_stdin) {
            fout.printf("%s", line.c_str());
        }
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    /* ---- Test 2: FileIn and FileOut ----------------------------------------
     */
    std::cout << "\n>>> Test 2 FileIn and FileOut\n";
    std::vector<std::string> buffer_in;


    std::cout << "\n>>> Test 2 open filename.c_str():\n";
    try {
        std::string filename("data/lorem_ipsum.txt");

        core::FileIn fin(filename.c_str());
        infolog(static_cast<core::File *>(&fin));
        fin.close();
        infolog(static_cast<core::File *>(&fin));

        std::cout << "\n>>> Test 2 open filename:\n";
        fin.open(filename);
        infolog(static_cast<core::File *>(&fin));
        fin.close();
        infolog(static_cast<core::File *>(&fin));
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    std::cout << "\n>>> Test 2 read and write filename:\n";
    try {
        std::string filename("data/lorem_ipsum.txt");

        core::FileIn fin(filename);
        infolog(static_cast<core::File *>(&fin));
        fin.readlines(buffer_in);

        /*
         * Compare stdin and file buffers
         */
        auto it_stdin = buffer_stdin.begin();
        auto it_in = buffer_in.begin();
        while (it_stdin != buffer_stdin.end() && it_in != buffer_in.end()) {
            core_assert(*it_stdin == *it_in, "FAIL");
            ++it_stdin;
            ++it_in;
        }
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    std::cout << "\n>>> Test 2 read filename line by line:\n";
    try {
        std::string filename("data/lorem_ipsum.txt");

        core::FileIn fin(filename);
        infolog(static_cast<core::File *>(&fin));

        std::string line;
        while (fin.readline(line)) {
            std::cout << line << "\n";
        }
        core_assert(!fin.is_error(), "I/O error");
        fin.close();
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    std::cout << "\n>>> Test 2 readline/writeline:\n";
    try {
        core::FileIn fin("data/lorem_ipsum.txt");
        infolog(static_cast<core::File *>(&fin));

        core::FileOut fout_new("data/lorem_ipsum_new.txt");
        infolog(static_cast<core::File *>(&fout_new));

        core::FileOut fout_csv("data/lorem_ipsum_csv.txt");
        infolog(static_cast<core::File *>(&fout_csv));

        std::string line;
        while (fin.readline(line)) {
            fout_new.writeline(line, "\n");
            fout_csv.writeline(line, ",");
        }

        core_assert(!fin.is_error(), "I/O error");
        fin.close();

        core_assert(!fout_new.is_error(), "I/O error");
        fout_new.close();

        core_assert(!fout_csv.is_error(), "I/O error");
        fout_csv.close();
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    std::cout << "\n>>> Test 2 readline/writeline,"
              << " size=4096, delim='.', ignore='#':\n";
    try {
        core::FileIn fin("data/comment_lorem_ipsum.txt");
        infolog(static_cast<core::File *>(&fin));

        core::FileOut fout_new("data/comment_lorem_ipsum_new_1.txt");
        infolog(static_cast<core::File *>(&fout_new));

        core::FileOut fout_csv("data/comment_lorem_ipsum_csv_1.txt");
        infolog(static_cast<core::File *>(&fout_csv));

        std::string line;
        while (fin.readline(line, 4096, '.', '#')) {
            fout_new.writeline(line, "\n");
            fout_csv.writeline(line, ",");
        }

        core_assert(!fin.is_error(), "I/O error");
        fin.close();

        core_assert(!fout_new.is_error(), "I/O error");
        fout_new.close();

        core_assert(!fout_csv.is_error(), "I/O error");
        fout_csv.close();
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    std::cout << "\n>>> Test 2 readlines/writelines,"
              << " size=4096, delim='.', ignore='#':\n";
    try {
        core::FileIn fin("data/comment_lorem_ipsum.txt");
        infolog(static_cast<core::File *>(&fin));

        core::FileOut fout_new("data/comment_lorem_ipsum_new_2.txt");
        infolog(static_cast<core::File *>(&fout_new));

        core::FileOut fout_csv("data/comment_lorem_ipsum_csv_2.txt");
        infolog(static_cast<core::File *>(&fout_csv));

        std::vector<std::string> buffer;
        fin.readlines(buffer, 4096, '.', '#');
        if (!fin.is_error()) {
            fout_new.writelines(buffer, "\n");
            fout_csv.writelines(buffer, ",");
        }

        core_assert(!fin.is_error(), "I/O error");
        fin.close();

        core_assert(!fout_new.is_error(), "I/O error");
        fout_new.close();

        core_assert(!fout_csv.is_error(), "I/O error");
        fout_csv.close();
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    /* ---- Test 3: binary read/write -----------------------------------------
     */
    std::cout << "\n>>> Test 3 FileIn and FileOut binary\n";

    std::cout << "\n>>> Test 3 read txt/write bin:\n";
    try {
        /*
         * Read text file into character buffer.
         */
        core::FileIn fin("data/lorem_ipsum.txt", core::FileIn::Binary);
        infolog(static_cast<core::File *>(&fin));

        int64_t length = fin.length();
        std::vector<char> buffer(length);

        fin.read(buffer.data(), buffer.size());
        core_assert(!fin.is_error(), "I/O error");

        fin.close();
        infolog(static_cast<core::File *>(&fin));

        std::cout << core::str_format("file length = %lld bytes\n", length);
        std::cout << core::str_format("buffer size = %lu\n", buffer.size());
        for (char n : buffer) {
            std::cout << n;
        }
        std::cout << "\n";

        /*
         * Write binary file.
         */
        core::FileOut fout("data/lorem_ipsum_1.bin", core::FileOut::Binary);
        infolog(static_cast<core::File *>(&fout));

        fout.write(buffer.data(), buffer.size());
        core_assert(!fout.is_error(), "I/O error");

        fout.close();
        infolog(static_cast<core::File *>(&fout));
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    std::cout << "\n>>> Test 3 read bin/write bin:\n";
    try {
        /*
         * Read text file into character buffer.
         */
        core::FileIn fin("data/lorem_ipsum_1.bin", core::FileIn::Binary);
        infolog(static_cast<core::File *>(&fin));

        int64_t length = fin.length();
        std::vector<char> buffer(length);

        fin.read(buffer.data(), buffer.size());
        core_assert(!fin.is_error(), "I/O error");

        fin.close();
        infolog(static_cast<core::File *>(&fin));

        std::cout << core::str_format("file length = %lld bytes\n", length);
        std::cout << core::str_format("buffer size = %lu\n", buffer.size());
        for (char n : buffer) {
            std::cout << n;
        }
        std::cout << "\n";

        /*
         * Write binary file.
         */
        core::FileOut fout("data/lorem_ipsum_2.bin", core::FileOut::Binary);
        infolog(static_cast<core::File *>(&fout));

        fout.write(buffer.data(), buffer.size());
        core_assert(!fout.is_error(), "I/O error");

        fout.close();
        infolog(static_cast<core::File *>(&fout));
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    /* Return OK */
    std::printf("OK\n");
}
