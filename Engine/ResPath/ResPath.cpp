#include "ResPath.hpp"
#include <format>

namespace mk {
	ResPath::ResPath(const std::string& path):
		  real_path(canonical(std::filesystem::path("assets") / path)) {}

	const std::filesystem::path& ResPath::getPath() const { return real_path; }

	bool ResPath::exists() const { return std::filesystem::exists(real_path); }

	ResPath::ResPath(const char* path): ResPath(std::string(path)) {}

	const char* ResPath::strPath() const { return real_path.c_str(); }
}  // mk
