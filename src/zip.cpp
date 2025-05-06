#include "zip.hpp"

#include "beatsaber-hook/shared/utils/utils.h"
#include "main.hpp"
#include "metacore/shared/strings.hpp"
#include "zip/shared/zip.h"

static std::string ReadFileInternal(zip* zip, int index) {
    struct zip_stat stat;
    std::string ret;

    zip_stat_index(zip, index, 0, &stat);
    ret.resize(stat.size);

    auto file = zip_fopen_index(zip, index, 0);
    zip_fread(file, ret.data(), stat.size);
    zip_fclose(file);

    return ret;
}

std::string CustomModels::ReadFileFromZip(std::string const& path, std::string const& file) {
    if (!fileexists(path) || std::filesystem::is_directory(path))
        return "";

    auto zip = zip_open(path.data(), ZIP_RDONLY, nullptr);
    if (!zip)
        return "";

    std::string ret = "";

    auto index = zip_name_locate(zip, file.data(), 0);
    if (index >= 0)
        ret = ReadFileInternal(zip, index);

    zip_discard(zip);
    return ret;
}

bool CustomModels::ExtractAllLegacyAssets(std::string const& path, std::string const& destination) {
    if (!fileexists(path) || std::filesystem::is_directory(path))
        return false;

    auto zip = zip_open(path.data(), ZIP_RDONLY, nullptr);
    if (!zip)
        return false;

    auto files = zip_get_num_entries(zip, 0);

    for (int file = 0; file < files; file++) {
        std::string name = zip_get_name(zip, file, ZIP_FL_ENC_GUESS);
        if (!name.ends_with(".qsaber") && !name.ends_with(".qbloq") && !name.ends_with(".qwall"))
            continue;

        std::string output = std::filesystem::path(destination) / MetaCore::Strings::UniqueFileName(name, destination);
        writefile(output, ReadFileInternal(zip, file));
    }

    zip_discard(zip);
    return true;
}

void CustomModels::WriteZipFile(std::string const& path, std::vector<std::pair<std::string, std::string>> const& files) {
    auto zip = zip_open(path.data(), ZIP_CREATE, nullptr);
    if (!zip)
        return;

    for (auto& [name, data] : files) {
        auto buffer = zip_source_buffer_create(data.data(), data.size(), 0, nullptr);
        zip_file_add(zip, name.c_str(), buffer, ZIP_FL_ENC_GUESS);
    }

    zip_close(zip);
}
