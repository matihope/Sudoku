#include <JsonBridge/JsonBridge.hpp>
#include <fstream>

namespace mk {
	JsonBridge::JsonBridge(const ResPath& filePath) {
		load(filePath);
	}

	bool JsonBridge::saveCurrentState() {
		std::fstream jsonFile(m_file_path.getPath(), std::ios::out);
		if (!jsonFile.is_open()) return false;
		jsonFile << data.dump(4) << std::endl;
		jsonFile.close();
		return true;
	}

	JsonBridge JsonBridge::new_file(const ResPath& file) {
		JsonBridge bridge;
		bridge.m_file_path = file;
		return bridge;
	}

	void JsonBridge::load(const ResPath& path) {
		m_file_path = path;
		std::ifstream jsonFile(m_file_path.getPath());

		if (!jsonFile.is_open())
			throw std::runtime_error(
				std::format("Cannot find file %", m_file_path.getPath().c_str())
			);

		jsonFile >> data;
		jsonFile.close();
	}
}  // namespace mk
