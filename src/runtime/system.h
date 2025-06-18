/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"

namespace peak {

class System {
public:
	static void LocateEcho(std::function<void(const std::string&)> func);
	static void LocateLoadText(std::function<std::string(const std::string&)> func);

public:
	static void Echo(const std::string& message);
	static const std::string& OpenSrc(const std::string& filename, std::string& outAbsPath);
	static void AddSearchSrcDir(const std::string& dir);

	static std::string LoadText(const std::string& filename);

private:
	static std::function<void(const std::string&)> _funcEcho;
	static std::function<std::string(const std::string&)> _funcOpenSrc;
	static std::unordered_set<std::string> _searchPathList;
	static std::unordered_map<std::string, std::string> _abspathToSrcMap;
};
} // namespace peak