//RBPathFuncs: provides functions to manipulate paths on a supported platform.
#ifndef RB_PATH_FUNCS_H
#define RB_PATH_FUNCS_H
#pragma once

//Standard c++ includes.
#include <filesystem>
namespace RaisedBar
{
	namespace PathFunctions
	{
		std::experimental::filesystem::path AppDataPath();
		std::experimental::filesystem::path HelpPath();
		std::experimental::filesystem::path AppConfigPath();
		std::experimental::filesystem::path SpeechPath();
		std::experimental::filesystem::path ProtocolPath();
		std::experimental::filesystem::path SurfacePath();
		std::experimental::filesystem::path LogPath();
		void EnsurePathExists(std::experimental::filesystem::path& path);
	}
}
#endif