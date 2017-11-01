
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <algorithm>
//#include <stdexcept>

//#include "FacialSolverLib.h"

#include <maya/MIOStream.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnMesh.h>

#include "solverCmd.h"
#include <stdlib.h>
#include <vector>




#ifndef NOMINMAX
#define NOMINMAX
#endif

MStatus SolverCmd::doIt(const MArgList &args)
{
	cout<<"Please enter Blendshape folder name"<<endl;
	MGlobal::displayInfo("Please select Neutral.obj file ...\n");
	std::string NEUT_FILE = openFileDialog();
	std::replace(NEUT_FILE.begin(), NEUT_FILE.end(), '\\', '/');
	cout << "Loading file:" << NEUT_FILE << endl;
	MString MNEUT_FILE = NEUT_FILE.c_str();
	MGlobal::executeCommand(MString("file -import -namespace \"NTemp\" -mergeNamespacesOnClash true") + "\"" + MNEUT_FILE + "\"" );
	MGlobal::executeCommand(MString("select \"NTemp:*\""));
	MGlobal::executeCommand(MString("rename ") + "Neutral");

	
	std::size_t botDirPos = NEUT_FILE.find_last_of("/");
	// get directory
	std::string BS_DIR = NEUT_FILE.substr(0, botDirPos+1);
	cout << "Loading blendshapes from:" << BS_DIR << endl;


	//std::vector<std::string> objFiles = FindAllObjInFolder(BS_DIR);
	std::vector<std::string> filesPaths;
	std::string optfileName = "";
	std::string inputFolderPath = "";
	std::string extension = "*.obj";
	getFilesList(inputFolderPath, extension, filesPaths);
	std::vector<std::string>::const_iterator it = filesPaths.begin();

	//MGlobal::executePythonCommand(MString("import maya.cmds as cmds"));
	std::vector<std::string> allBSNames;
	int ind = 0;
	while (it != filesPaths.end())
	{
		std::size_t botDirPos = it->find_last_of(".");
		std::string BS_NAME = "BS_" + it->substr(0, botDirPos);
		allBSNames.push_back(BS_NAME);


		//cout << "Reading BS file: " << *it << endl;//read file name
		std::string fullBSpath = BS_DIR + *it;
		std::replace(fullBSpath.begin(), fullBSpath.end(), '\\', '/');
		cout << "Reading BS file: " << fullBSpath << endl;//read file name
		//if (fullBSpath == NEUT_FILE)
			//continue;
		MString MfullBSpath = fullBSpath.c_str();
		MGlobal::executeCommand(MString("file -import -namespace \"Temp\" -mergeNamespacesOnClash true") + "\"" + MfullBSpath + "\"" );
		MGlobal::executeCommand(MString("select \"Temp:*\""));
		MGlobal::executeCommand(MString("rename ") + "\"" + MString(BS_NAME.c_str()) + "\"");
		MGlobal::executeCommand(MString("select ") + "\"" + MString(BS_NAME.c_str()) + "\"");
		MGlobal::executeCommand(MString("move -r ") + "-x (500+200*((" + ind + ")%10)) " +
			"-y (-300.0*((" + ind + ")/10))" + " -z 0");
		it++;
		ind++;
	}
	
	/*MGlobal::executeCommand(MString("select \"blendshapes\" "));
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);
	MFnMesh meshFn;
	MDagPath dagPath;
	MItSelectionList iter(selection, MFn::kMesh);
	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath);
		meshFn.setObject(dagPath);
		meshFn.
	}*/

	system("pause");
	MGlobal::executeCommand(MString("group -n \"blendshapes\" \"BS_*\" "));
	MGlobal::executeCommand(MString("select \"BS_*\""));
	MGlobal::executeCommand(MString("select -tgl \"Neutral\""));
	MGlobal::executeCommand(MString("blendShape"));
	
	return MS::kSuccess;
}

std::string SolverCmd::openFileDialog()
{
	char filename[MAX_PATH];
	std::string str;

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = _T("Text Files\0*.obj\0Any File\0*.*\0");
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = _T("Select a File, yo!");
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		//std::cout << "You chose the file \"" << filename << "\"\n";
		str = filename;
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: cout << "You cancelled.\n";
		}
	}

	return str;
}

void SolverCmd::getFilesList(std::string filePath, std::string extension, std::vector<std::string> & returnFileName)
{
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	std::string  fullPath = filePath + extension;
	hFind = FindFirstFile(fullPath.c_str(), &fileInfo);
	if (hFind != INVALID_HANDLE_VALUE) {
		returnFileName.push_back(filePath + fileInfo.cFileName);
		while (FindNextFile(hFind, &fileInfo) != 0) {
			returnFileName.push_back(filePath + fileInfo.cFileName);
		}
	}
}


