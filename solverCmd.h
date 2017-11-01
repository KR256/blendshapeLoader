#pragma once

#include <string>
#include <vector>

#include <maya/MPxCommand.h>
#include <maya/MDataBlock.h>

class SolverCmd : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator() { return new SolverCmd; }

private:
	std::string openFileDialog();
	void getFilesList(std::string filePath, std::string extension, std::vector<std::string> & returnFileName);
};
