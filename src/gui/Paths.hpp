#pragma once
#include <QString>

namespace Paths {
QString AppDataDir();   // ~/Library/Application Support/RepetitionAI
QString DatabaseFile(); // AppDataDir()/repetition.db
QString AudioDir();     // AppDataDir()/audio
QString ImagesDir();    // AppDataDir()/images
}
