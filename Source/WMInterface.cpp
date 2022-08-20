/**
    - File Outline : Watermarking  main file.
    - file    : WMInterface.cpp
    - author  : Mohanarangan Kanniappan
    - version : 0.1
*/


#include <iostream>

#include "logger.h"
#include "WMInterface.h"
#include "WMOpenCVInterface.h"


using namespace std;

/**
	Creates folder to save the extracted images, and water marked images.
	Arguments:
		dir - the folder name


*/

void cWMInterface::createDirOrClean(const string &dir) {
	struct stat buffer;
	if (stat(dir.c_str(), &buffer) != 0) {
		if (mkdir(dir.c_str(), 0777)) {
			if (dir == ext_frames)
				throw EXCP_UNABLE_TO_CREATE_FRAMES_FOLDER;
			else if (dir == overlay_dir)
				throw EXCP_UNABLE_TO_CREATE_OVERLAY_FOLDER;
			else if (dir == out_dir)
				throw EXCP_UNABLE_TO_CREATE_OUT_FOLDER;
		}
	} else {
		string cmnd = (string) "rm " + dir + (string) "*";
		system(cmnd.c_str());
	}
}

/**
	Creates folder to save the extracted images, and water marked images.
	Arguments:
		filename - the video file  name
		overlay  - logo
		f_dir    - frames directory
		ov_dir   - logo embedded image directory
		x        -
		y        -

*/
cWMInterface::cWMInterface(
		const string &filename,
		const string &overlay,
		const string &f_dir,
		const string &ov_dir,
		const string &o_dir,
		BLENDER_TYPE blender_type,
		int x,
		int y)
{
	if(blender_type >= BLENDER_MAX) throw EXCP_BLENDER_NOT_SUPPORTED;
	input_video.setAndValidateFileExt(filename, FILE_VID);
	wm_image.setAndValidateFileExt(overlay, FILE_IMG);
	number_of_frames = 0;
	wm_pos_x = x;
	wm_pos_y = y;
	ext_frames = f_dir;
	overlay_dir = ov_dir;
	out_dir = o_dir;

	// Validate the input mp4 file
	blender = cWMOpenCV::getBlender(blender_type);

	if(!blender->isValidVideo(    input_video.getFileName())) {
		throw EXCP_IS_NOT_VID;
	}
	if(!blender->isValidImage(wm_image.getFileName())) {
		throw EXCP_IS_NOT_IMG;
	}
	// Create Directories
	createDirOrClean(ext_frames);
	createDirOrClean(overlay_dir);
	createDirOrClean(out_dir);
}
/**
	Interface file for frame extractor.
	Arguments:

*/

void cWMInterface::captureFrames() {

	if(blender)
		number_of_frames = blender->extractFrames(  input_video.getFileName(), ext_frames);

	if(number_of_frames == 0) throw EXCP_NO_FRAMES_TO_CAPTURE;

	else if(number_of_frames < 0) throw EXCP_IS_NOT_VID;
}

/**
	Interface file for logo insertion.
	Arguments:

*/
void cWMInterface::insertWM() {
	if(blender)
		if(blender->addWaterMark(ext_frames, wm_image.getFileName(), overlay_dir, number_of_frames, wm_pos_x, wm_pos_y) < 0)
			throw EXCP_IS_NOT_IMG;
}

/**
	Generate MP4 after logo has been embedded.
	Arguments: None

*/

void cWMInterface::generateMp4() {
	if(blender)
		blender->createMp4(overlay_dir, out_dir);
}
