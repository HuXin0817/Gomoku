#pragma once

#include "common.h"

#include <fstream>

#include <QStandardPaths>

class MoveRecords
{
    static inline auto dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString() + "/.gomoku";

    static inline auto moveRecordsDataFilePath = dir + "/move_records.data";

  public:
    MoveRecords()
    {
        auto          filename = moveRecordsDataFilePath;
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::filesystem::create_directory(dir);
            return;
        }
        while (!file.eof()) {
            int x, y;
            file >> x >> y;
            moveRecords.emplace_back(x, y);
        }
    }

    auto back() const { return moveRecords.back(); }

    void pop_back()
    {
        moveRecords.pop_back();
        saveIntoDisk();
    }

    void emplace_back(int x, int y)
    {
        moveRecords.emplace_back(x, y);
        saveIntoDisk();
    }

    auto get() const { return moveRecords; }

    void clear()
    {
        moveRecords.clear();
        saveIntoDisk();
    }

  private:
    std::vector<Point> moveRecords;

    void saveIntoDisk()
    {
        auto          filename = moveRecordsDataFilePath;
        std::ofstream file(filename, std::ios::binary);
        for (auto [x, y] : moveRecords) {
            file << x << " " << y << std::endl;
        }
    }
};
