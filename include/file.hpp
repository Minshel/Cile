#ifndef FILE_H
#define FILE_H

#include "QStringList"
#include "qcontainerfwd.h"
#include <filesystem>
#include <fstream>
#include <ios>
#include <string>
class File {
    public:
        std::string filepath;

        inline std::string sRead (const File *file) {
            std::fstream stream = fStream(file->filepath);
            std::string data = "";
            std::string line;

            if (stream.is_open() && !std::filesystem::exists(file->filepath)) { return ""; }

            while (std::getline(stream, line)) {
                data += line;
            }
            stream.close();

            return data;
        }

        inline QStringList qslReadLines (const File *file) {
            std::fstream stream = fStream(file->filepath);
            QStringList lines;
            std::string line;

            if (stream.is_open() && !std::filesystem::exists(file->filepath)) { return QStringList(); }

            while (std::getline(stream, line)) {
                lines.append(QString::fromStdString(line));
            }
            stream.close();

            return lines;
        }

    protected:
        inline static std::fstream fStream (const std::string filepath) {
            std::ios::openmode streamType = std::ios::in;
            const std::filesystem::path path = filepath;

            std::fstream stream (filepath, streamType);
            return stream;
        }
};

#endif
