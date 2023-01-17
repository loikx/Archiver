//#include "archiver.h"
//
//std::string GetFileNameFromPath(const std::string& path) {
//    return path.substr(path.find_last_of('/') + 1);
//}
//
//std::string GetFilePathWithoutName(const std::string& path) {
//    return path.substr(0, path.find_last_of('/') + 1);
//}
//
//void ArchivedFile::writeToArchive(std::fstream &file, const std::vector<bool>& block) {
//    auto hamming = GenerateHammingCode(block);
//
//    if (hamming.size() < 8) {
//        hamming.insert(hamming.begin(), true);
//        uint8_t decimal = ToDecimal(block);
//        file.write(reinterpret_cast<char*>(&decimal), sizeof(decimal));
//        return;
//    }
//    uint64_t index = 0;
//    while (index < hamming.size()) {
//        std::vector<bool> byte;
//        while (byte.size() != 7 && index < hamming.size()) {
//            byte.push_back(hamming[index]);
//            ++index;
//        }
//        byte.insert(byte.begin(), true);
//        uint16_t decimal = ToDecimal(byte);
//        file.write(reinterpret_cast<char*>(&decimal), sizeof(decimal));
//    }
//
//}
//
//void ArchivedFile::getBinaryCode(std::fstream& file_to_write) {
//    std::ifstream file(path_, std::ios::binary);
//    std::vector<bool> block(0);
//
//    while (true) {
//        uint16_t tmp = file.get();
//
//        if (file.eof())
//            break;
//
//        for (const auto& item: huffman_code_[tmp]){
//            block.emplace_back(item - '0');
//            if (block.size() == bits_block_) {
//                writeToArchive(file_to_write, block);
//                block.clear();
//            }
//        }
//    }
//
//    if (!block.empty()) {
//        writeToArchive(file_to_write, block);
//        block.clear();
//    }
//}
//
//ArchivedFile::ArchivedFile(const std::string& archive_path, uint64_t n) {
//    path_ = archive_path;
//    bits_block_ = n;
//    huffman_code_ = GenerateHuffmanCode(archive_path);
//}
//
//std::map<uint16_t, std::string> ArchivedFile::getHuffmanCode() const {
//    return huffman_code_;
//}
//
//std::string ArchivedFile::getPath() const {
//    return path_;
//}
//
//
//std::uint64_t ArchivedFile::getBlockLength() const {
//    return bits_block_;
//}
//
//std::uint64_t ArchivedFile::getHammingBlockLength() const {
//    return FindHammingLength(bits_block_);
//}
//
//Archive::Archive(const std::string& archive_path, const std::vector<ArchivedFile>& files_to_archive) {
//    archive_path_ = archive_path;
//
//    std::fstream archive_file(archive_path);
//    archive_file.open(archive_path);
//
//    archive_file << files_to_archive.size() << '\n';
//
//    for (const auto& toArchive: files_to_archive)
//        putDataIntoFile(toArchive);
//}
//
//Archive::Archive(const std::string& archive_path) {
//    archive_path_ = archive_path;
//}
//
//std::string Archive::getPath() const {
//    return archive_path_;
//}
//
//void Archive::putDataIntoFile(ArchivedFile archive) {
//    std::fstream archive_file(archive_path_, std::ios::binary | std::ios::out);
//    std::fstream archive_read(archive_path_, std::ios::binary | std::ios::in);
//
//    uint64_t file_in_archive_cnt_ = 0;
//    archive_read.read(reinterpret_cast<char*>(&file_in_archive_cnt_), sizeof(file_in_archive_cnt_));
//
//    archive_read.close();
//    ++file_in_archive_cnt_;
//
//    archive_file.write(reinterpret_cast<char*>(&file_in_archive_cnt_), sizeof(file_in_archive_cnt_));
//    archive_file.write("\n", 1);
//    archive_file.seekp(0, std::ios::end);
//
//    std::string file_name = GetFileNameFromPath(archive.getPath());
//    uint64_t file_name_size = file_name.size();
//    archive_file.write(reinterpret_cast<char*>(&file_name_size), sizeof(file_name_size));
//    archive_file.write("\n", 1);
//    archive_file.write(file_name.c_str(), (int)file_name_size);;
//    archive_file.write("\n", 1);
//
//    const auto huffman_code = archive.getHuffmanCode();
//    uint64_t huffman_size = huffman_code.size();
//    archive_file.write(reinterpret_cast<char*>(&huffman_size), sizeof(huffman_size));
//    archive_file.write("\n", 1);
//
//    for (const auto& code: huffman_code) {
//        uint8_t byte_value = code.first;
//        archive_file.write(reinterpret_cast<char*>(&byte_value), sizeof(byte_value));
//        archive_file.write(" ", 1);
//        archive_file.write(code.second.c_str(), 10);
//        archive_file.write("\n", 1);
//    }
//    // 1 additional xor bit
//    uint64_t hamming_block_length = archive.getHammingBlockLength() + 1;
//    archive_file.write(reinterpret_cast<char*>(&hamming_block_length), sizeof(hamming_block_length));
//    archive_file.write("\n", 1);
//    archive.getBinaryCode(archive_file);
//    archive_file.write("\0", 1);
//    archive_file.write("\0", 1),
//    archive_file.write("\0", 1),
//
//    archive_file.write("\n", 1);
//
//    archive_file.close();
//}
//
//void Archive::printFilesList() {
//    std::fstream  archive_file(archive_path_);
//
//    std::string str;
//    bool flag = false;
//    unsigned long long file_number = 1;
//
//    while (std::getline(archive_file, str)) {
//        if (str.empty()) {
//            flag = true;
//            continue;
//        }
//
//        if (flag) {
//            std::cout << "File " << file_number << ": " << str << std::endl;
//            flag = false;
//        }
//    }
//
//    archive_file.close();
//}
//
//void Archive::deleteFiles(const std::vector<std::string>& files_to_delete) {
//    std::fstream archive_file(archive_path_);
//
//    if (files_to_delete.empty()) {
//        std::ofstream file(archive_path_, std::ios::trunc);
//        file << 0 << '\n';
//        file.close();
//        files_in_archive_cnt_ = 0;
//        return;
//    }
//
//    std::string tmp;
//    bool flag = false;
//    uint64_t cnt_files_after_extract = 0;
//
//    std::string tmp_file_path = GetFilePathWithoutName(archive_path_) + "TMP.haf";
//
//    std::ofstream tmp_file(tmp_file_path);
//
//    tmp_file << files_in_archive_cnt_ << '\n';
//
//    while (std::getline(archive_file, tmp)) {
//        if (tmp.empty()) {
//            flag = true;
//            continue;
//        }
//
//        if (flag) {
//            flag = false;
//
//            auto file_position =
//                    std::find(files_to_delete.begin(), files_to_delete.end(), tmp);
//
//            if (file_position == files_to_delete.end()) {
//                ++cnt_files_after_extract;
//                tmp_file << '\n' << tmp << '\n';
//                int cnt = 0;
//                while (cnt != 3) {
//                    std::getline()
//                    tmp_file <<
//                }
//
//            }
//        }
//    }
//
//    files_in_archive_cnt_ = cnt_files_after_extract;
//
//    tmp_file.seekp(0);
//    tmp_file << files_in_archive_cnt_ << '\n';
//    tmp_file.close();
//
//    archive_file.close();
//
//    std::remove(archive_path_.c_str());
//
//    std::rename(tmp_file_path.c_str(), archive_path_.c_str());
//}
//
//void Archive::extractFiles(const std::vector<std::string>& files_to_extract, const std::string& path_to_extract) {
//    if (!std::filesystem::exists(path_to_extract))
//        throw std::invalid_argument("Path to extract: " + path_to_extract + " does not exist");
//
//    std::fstream archive_file(archive_path_, std::ios::binary | std::ios::in);
//    uint64_t files_in_archive = 0;
//    // how many files in archive
//    archive_file.read(reinterpret_cast<char*>(&files_in_archive), sizeof(files_in_archive));
//    char buf[1];
//    archive_file.read(buf, 1);
//    while (!archive_file.eof()) {
//        uint64_t file_name;
//        archive_file.read(reinterpret_cast<char*>(&file_name), sizeof(file_name));
//        archive_file.read(buf, 1);
//
//        char name[file_name];
//        archive_file.read(name, file_name);
//        archive_file.read(buf, 1);
//
//        // TODO: переделать достование
//
//    }
//}
//
//
//void WriteToFile(std::ofstream& file_to_write, std::ifstream& file_from) {
//    std::string tmp;
//
//    while (std::getline(file_from, tmp)) {
//        file_to_write << tmp;
//    }
//}
//
//Archive MergeArchives(const Archive& archive1, const Archive& archive2, const std::string& archive_name) {
//    std::ofstream merge_archive(archive_name);
//
//    std::ifstream file_archive1(archive1.getPath());
//    std::ifstream file_archive2(archive2.getPath());
//
//    std::string pointer1;
//    std::string pointer2;
//
//    std::getline(file_archive1, pointer1);
//    std::getline(file_archive2, pointer2);
//
//    merge_archive << (std::stoll(pointer1) + std::stoll(pointer2)) << '\n' << '\n';
//
//    WriteToFile(merge_archive, file_archive1);
//
//    merge_archive << '\n';
//    WriteToFile(merge_archive, file_archive2);
//
//    merge_archive.close();
//
//    return std::move(Archive(archive_name));
//}
