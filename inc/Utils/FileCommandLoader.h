#pragma once

#include <functional>
#include <string>
#include <vector>
#include <stdint.h>

class Color;
class Vec2D;

using LineNum = uint32_t;

enum CommandType
{
	COMMAND_ONE_LINE = 0,
	COMMAND_MULTI_LINE
};

struct ParseFuncParams
{
	std::string line;
	size_t dilimitPos;
	LineNum lineNum;
};

using ParseFunc = std::function<void(const ParseFuncParams& params)>;

struct Command
{
	CommandType commandType = COMMAND_ONE_LINE;
	std::string command = "";
	ParseFunc parseFunc = nullptr;
};

class FileCommandLoader
{
public:
	void AddCommand(const Command& command);
	bool LoadFile(const std::string& filePath);

	static Color ReadColor(const ParseFuncParams& params);
	static Vec2D ReadSize(const ParseFuncParams& params);
	static int ReadInt(const ParseFuncParams& params);
	static std::string ReadString(const ParseFuncParams& params);
	static char ReadChar(const ParseFuncParams& params);

private:
	std::vector<Command> mCommands;
};