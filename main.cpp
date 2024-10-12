#include <string>
#include <lunasvg.h>
#include <byondapi.h>
#include <byondapi_cpp_wrappers.h>
#include <filesystem>

using namespace lunasvg;

extern "C" BYOND_EXPORT CByondValue GenerateImage(u4c n, ByondValue v[]) {

	if (!v[0].IsStr()) return ByondValue(-1);

	std::string fileName = v[0].ToString();
	std::string fullPath = "assets\\" + fileName + ".svg";

	s4c width = -1;
	s4c height = -1;

	if (v[1].IsNum()) {
		width = (s4c)v[1].GetNum();
	}
	if (v[2].IsNum()) {
		height = (s4c)v[2].GetNum();
	}

	std::string tempFolderName = "svg2byondTemp";
	std::filesystem::path folderPath = std::filesystem::current_path() / tempFolderName;

	if (!std::filesystem::exists(folderPath)) {
		std::filesystem::create_directory(folderPath);
	}

	fileName = fileName + std::to_string(width) + "x" + std::to_string(height) + ".png";
	std::string resultPath = folderPath.string() + "\\" + fileName;

	// Check if the file already exists
	if (std::filesystem::exists(resultPath)) {
		return ByondValue(fileName); 
	}

	auto document = Document::loadFromFile(fullPath);
	if (document == nullptr)
		return ByondValue(-1);

	auto bitmap = document->renderToBitmap(width, height);
	if (bitmap.isNull())
		return ByondValue(-1);

	if (bitmap.writeToPng(resultPath)) {
		return ByondValue(fileName);
	}
	else {
		return ByondValue(-1);
	}
}
