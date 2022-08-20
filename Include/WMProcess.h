#ifndef WM_PROCESS_H_
#define WM_PROCESS_H_

#include "logger.h"

#define STR_MP4 ".mp4"
#define STR_JPG ".jpg"
#define STR_PNG ".png"
#define OUT_MP4 "output.mp4"

class IOverlayBlender {
	logger *log;
public:
	IOverlayBlender(logger *l) {
		log = l;
	}
	virtual bool isValidVideo(const string &file) = 0;
	virtual bool isValidImage(const string &file) = 0;
	virtual int extractFrames(const string &file, const string &frames_dir) = 0;
	virtual int addWaterMark(const string &frames_dir, const string &file,
			const string &overlay_dir, int number_of_frames, int pos_x, int pos_y) = 0;
	virtual int createMp4(const string &overlay_dir, const string &out_dir) = 0;
	virtual ~IOverlayBlender() {}
	logger *getLogger() { return log; }
};

#endif /* OVERLAYBLENDER_IOVERLAYBLENDER_H_ */
