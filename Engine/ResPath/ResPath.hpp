#pragma once

#include <string>
#include <filesystem>

namespace mk {
	class ResPath {
		friend class std::hash<ResPath>;

	public:
		ResPath() = default;
		ResPath(const std::string& path);
		ResPath(const char* path);

		[[nodiscard]]
		const std::filesystem::path& getPath() const;

		[[nodiscard]]
		const char* strPath() const;

		[[nodiscard]]
		bool exists() const;

		bool operator()(const ResPath& other) const { return real_path == other.real_path; }

		bool operator<(const ResPath& other) const { return real_path < other.real_path; }

	private:
		std::filesystem::path real_path;
	};
}  // mk

template<>
struct std::hash<mk::ResPath> {
	std::size_t operator()(const mk::ResPath& k) const {
		return std::hash<std::filesystem::path>()(k.real_path);
	}
};
