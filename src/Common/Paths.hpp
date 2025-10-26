#pragma once

#include <string>

namespace AppData {
std::string AppDataDirectory();
std::string DatabaseFile();
std::string AudioDirectory();
std::string ImagesDirectory();
}

namespace AppResources {
std::string QMLMainURL();
std::string SQLSchema();
}
