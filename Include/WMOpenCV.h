#ifndef OVERLAYBLENDER_SRC_OPENCVOVERLAYBLENDER_H_
#define OVERLAYBLENDER_SRC_OPENCVOVERLAYBLENDER_H_

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "WMProcess.h"
#include "WMInterface.h"
#include "WMOpenCV.h"

using namespace std;
using namespace cv;


#define WM_SIZE_X  300
#define WM_SIZE_Y  100
#define WM_LOGO_POS_X (1280-WM_SIZE_X)
#define WM_LOGO_POS_Y 20


class OpencvOverlayBlender: public IOverlayBlender {
//	VideoCapture cap;
//	Mat fg;
public:
	OpencvOverlayBlender(logger *l): IOverlayBlender(l) {

	}
	bool isValidVideo(const string &file);
	bool isValidImage(const string &file);
	int extractFrames(const string &fivideo_filele, const string &frames_dir);
	int addWaterMark(const string &frames_dir, const string &file,
			const string &overlay_dir, int number_of_frames, int pos_x, int pos_y);
	int createMp4(const string &overlay_dir, const string &out_dir);
};





#endif /* OVERLAYBLENDER_SRC_OPENCVOVERLAYBLENDER_H_ */
