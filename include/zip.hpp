#pragma once

#include <string>

namespace CustomModels {
    std::string ReadFileFromZip(std::string const& path, std::string const& file);

    bool ExtractAllLegacyAssets(std::string const& path, std::string const& destination);

    void WriteZipFile(std::string const& path, std::vector<std::pair<std::string, std::string>> const& files);
}
