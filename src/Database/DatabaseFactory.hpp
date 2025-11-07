#pragma once

#include "DatabaseInterface.hpp"

std::unique_ptr<DatabaseInterface> CreateProductionDatabase();
std::unique_ptr<DatabaseInterface> CreateTestDatabase();
