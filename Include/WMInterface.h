#ifndef WMINTERFACE_H_
#define WMINTERFACE_H_

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "WMInterface.h"
#include "WMProcess.h"

using namespace std;

enum FILE_TYPE {
	FILE_VID = 1, // for .mp4
	FILE_IMG, // for .jpg/.png
	FILE_MAX
};
enum BLENDER_TYPE {
	BLENDER_OPENCV = 1,
	BLENDER_MAX
};


enum EXCEPTION {
	EXCP_MP4_INVALID_FILENAME = -1, // file name doesn't end with *.mp4
	EXCP_OVERLAY_INVALID_FILENAME = -2, // file name doesn't end with *.mp4
	EXCP_MP4_FILE_NOT_FOUND = -3, // file name not found
	EXCP_OVERLAY_FILE_NOT_FOUND = -4, // file name not found
	EXCP_IS_NOT_VID = -5, // file is not a mp4 file
	EXCP_UNABLE_TO_CREATE_FRAMES_FOLDER = -6, // folder creation failed
	EXCP_UNABLE_TO_CREATE_OVERLAY_FOLDER = -7, // folder creation failed
	EXCP_UNABLE_TO_CREATE_OUT_FOLDER = -8, // folder creation failed
	EXCP_FILE_NOT_SUPPORTED = -9, // file is not video/image
	EXCP_IS_NOT_IMG = -10, // file is not video/image
	EXCP_BLENDER_NOT_SUPPORTED = -11, // Unsupported blender
	EXCP_NO_FRAMES_TO_CAPTURE = -12
};

class inputFile {

	string filename;

	static bool endsWithExt(
			const std::string &value,
			FILE_TYPE type)
	{
		vector<string> videoExts = { STR_MP4 };
		vector<string> imageExts = { STR_JPG, STR_PNG };
		if (type >= FILE_MAX)
			throw EXCP_FILE_NOT_SUPPORTED;
		const vector<string> &exts = (FILE_IMG == type) ? imageExts : videoExts;

		for (auto file_ext : exts) {
			if (file_ext.size() > value.size()) {
				continue;
			}
			bool ret = std::equal(file_ext.crbegin(), file_ext.crend(),
					value.crbegin(),
					[](const unsigned char a, const unsigned char b) {
						return std::tolower(a) == std::tolower(b);
					});
			if (ret)
				return ret;
		}
		return false;
	}
public:
	inputFile() {
	}
	void setAndValidateFileExt(const string &fname, FILE_TYPE type) {
		if (!endsWithExt(fname, type))
			type == FILE_VID ?
					throw EXCP_MP4_INVALID_FILENAME :
					throw EXCP_OVERLAY_INVALID_FILENAME;
		if (access(fname.c_str(), 0))
			type == FILE_VID ?
					throw EXCP_MP4_FILE_NOT_FOUND :
					throw EXCP_OVERLAY_FILE_NOT_FOUND;
		filename = fname;
	}
	string getFileName() {
		return filename;
	}
};

class cWMInterface {

	inputFile input_video;
	inputFile wm_image;
	int number_of_frames;
	int wm_pos_x;
	int wm_pos_y;
	string ext_frames;
	string overlay_dir;
	string out_dir;
	IOverlayBlender *blender;

	void createDirOrClean(const string &dir);


public:
	cWMInterface(
			const string &filename,
			const string &overlay,
			const string &f_dir,
			const string &ov_dir,
			const string &o_dir,
			BLENDER_TYPE b_type=BLENDER_OPENCV,
			int x = 100,
			int y = 100
			);
	void captureFrames();
	void insertWM();
	void generateMp4();
	~cWMInterface() {
		if(blender) delete blender;
	}
};


#endif /* OVERLAYBLENDER_H_ */
