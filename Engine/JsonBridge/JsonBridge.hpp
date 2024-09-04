#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include "ResPath/ResPath.hpp"

namespace mk {
	class JsonBridge {
		ResPath m_file_path;

	public:
		JsonBridge() = default;
		JsonBridge(const ResPath& file);

		static JsonBridge new_file(const ResPath& file);

		nlohmann::json data;

		void load(const ResPath& path);
		bool saveCurrentState();

		template<class T>
		auto operator[](T&& key) const {
			return data[key];
		}
	};
}  // namespace mk
