#ifndef LAB4_ARCHIVER_H
#define LAB4_ARCHIVER_H

#include <iostream>
#include <filesystem>
#include <cmath>
#include <cstring>
#include "../huffman_code/huffman.h"
#include "../hamming_code/hamming.h"


std::string GetFileNameFromPath(const std::string& path);

std::string GetFilePathWithoutName(const std::string& path);

void WriteToFile(std::ofstream& file_to_write, std::ifstream& file_from);

class ArchivedFile {
private:
    std::map<uint16_t, std::string> huffman_code_;
    std::string path_;
    uint64_t bits_block_;

    void writeToArchive(std::fstream& file, const std::vector<bool>& block);

public:
    explicit ArchivedFile(const std::string& archive_path, uint64_t n);

    void getBinaryCode(std::fstream& file_to_write);

    [[nodiscard]] std::map<uint16_t, std::string> getHuffmanCode() const;

    [[nodiscard]] std::string getPath() const;

    [[nodiscard]] std::uint64_t getBlockLength() const;

    [[nodiscard]] std::uint64_t getHammingBlockLength() const;
};

class Archive {
private:
    uint64_t files_in_archive_cnt_ = 0;
    std::string archive_path_;

public:
    Archive(const std::string& archive_path, const std::vector<ArchivedFile>& files_to_archive);

    explicit Archive(const std::string& archive_path);

    [[nodiscard]] std::string getPath() const;

    void putDataIntoFile(ArchivedFile archive);

    void printFilesList();

    void deleteFiles(const std::vector<std::string>& files_to_delete);

    void extractFiles(const std::vector<std::string>& files_to_extract,
                      const std::string& path_to_extract = "C:\\toExtract\\");
};

Archive MergeArchives(const Archive& archive1, const Archive& archive2, const std::string& archive_name);

#endif //LAB4_ARCHIVER_H
