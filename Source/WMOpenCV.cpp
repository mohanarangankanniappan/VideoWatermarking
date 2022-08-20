
#include "WMOpenCV.h"

#include <iostream>

using namespace std;
using namespace cv;
/**
	Check the file is valid image.

	Arguments:
	    - file - passed image file

*/
bool OpencvOverlayBlender::isValidImage(const string &file) {
	Mat fg = imread(file, IMREAD_UNCHANGED);
	if (fg.empty())	return false;
	return true;
}

/**
	Check the file is valid video.

	Arguments:
	    - file - passed video file

*/


bool OpencvOverlayBlender::isValidVideo(const string &file) {
	VideoCapture cap;
	cap.open(file);
	if (!cap.isOpened()) return false;
	Mat frame;
	if (!cap.read(frame)) return false;
	cap.release();
	return true;
}

/**

	Extract the image frames from the video.

	Arguments:
	    - file - passed video file
	    - frames_dir - directory at which image frames saved

*/

int OpencvOverlayBlender::extractFrames(const string &file, const string &frames_dir) {
	vector<Mat> frames;
	if(!isValidVideo(file)) return -1;
	VideoCapture cap;
	cap.open(file);
	if (!cap.isOpened()) return -1;
	int number_of_frames = cap.get(CAP_PROP_FRAME_COUNT);

	int width  =   cap.get(CAP_PROP_FRAME_WIDTH);//Setting the width of the video
	int height =   cap.get(CAP_PROP_FRAME_HEIGHT);

	cout << "Video frame width :" << width << endl;
	cout << "Video frame height :" << height << endl;

	vector<int> compression_params;
	compression_params.push_back(IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	int f_num = 1;
	for(int f_num = 1; f_num <= number_of_frames; f_num) {
		Mat frame;
		// Read Frame
		if (!cap.read(frame)) {
			getLogger()->log_error("Frame read failed from source !!! so stopping the Frame capture...");
			break;
		}
		frames.push_back(frame);
		string filePath = frames_dir + to_string(static_cast<long long>(f_num))
				+ STR_JPG;
		// Writing as JPG
		imwrite(filePath, frame, compression_params);
		f_num++;
	}
	cap.release();
	return number_of_frames;
}

/**

	Extract the image frames from the video.

	Arguments:
	    - file - passed video file
	    - frames_dir - directory at which image frames saved

*/

int OpencvOverlayBlender::addWaterMark(const string &frames_dir, const string &file,
		const string &overlay_dir, int number_of_frames, int pos_x, int pos_y) {
	
	Mat fg = imread(file, IMREAD_UNCHANGED);


	imshow("fg Image",fg);
	waitKey(0);

	int up_width = WM_SIZE_X;
	int up_height = WM_SIZE_Y;
	Mat resized_up;
	//resize up
	resize(fg, fg, Size(up_width, up_height), INTER_LINEAR);
		// Display Images and press any key to continue
	imshow("Resized Down by defining height and width", fg);
	waitKey();


	if (fg.empty())	return false;
	
	for (int img_indx = 1; img_indx <= number_of_frames; img_indx++) {
		// Read JPGs
		Mat bg = imread(frames_dir + to_string(img_indx) + STR_JPG, -1);
		Mat output;
		// Apply Blending This code is need to blend overlay image on background
		bg.copyTo(output);
		if (bg.empty())	return false;
		Point2i pos = Point(pos_x, pos_y);
		
		for (int y = max(pos.y, 0); y < bg.rows; ++y) {
			int fY = y - pos.y;
			if (fY >= fg.rows)
				break;
			for (int x = max(pos.x, 0); x < bg.cols; ++x) {
				int fX = x - pos.x;
				if (fX >= fg.cols)
					break;
				double opacity = ((double) fg.data[fY * fg.step
						+ fX * fg.channels() + 3]) / 255.;
				for (int c = 0; opacity > 0 && c < output.channels(); ++c) {
					unsigned char foregroundPx = fg.data[fY * fg.step
							+ fX * fg.channels() + c];
					unsigned char backgroundPx = bg.data[y * bg.step
							+ x * bg.channels() + c];
					output.data[y * output.step + output.channels() * x + c] =
							backgroundPx * (1. - opacity)
									+ foregroundPx * opacity;
				}
			}
		}
		vector<int> compression_params;
		compression_params.push_back(IMWRITE_JPEG_QUALITY);
		compression_params.push_back(100);
		// Write output as JPG
		cout << overlay_dir + to_string(img_indx) + STR_JPG << endl ;
		imshow("Output Image",output);
			waitKey(0);
		imwrite(overlay_dir + to_string(img_indx) + STR_JPG, output, compression_params);
		cout << "Watermarked Image Written " << endl;
	}
	return 0;
}
/**

	Extract the image frames from the video.

	Arguments:
	    - file - passed video file
	    - frames_dir - directory at which image frames saved

*/
int OpencvOverlayBlender::createMp4(const string &overlay_dir, const string &out_dir) {
	// Create VideoCapture with folder with image number sequence
	VideoCapture vid_capture(overlay_dir + (string) "%d" + STR_JPG);
	int frame_width = static_cast<int>(vid_capture.get(CAP_PROP_FRAME_WIDTH));
	int frame_height = static_cast<int>(vid_capture.get(CAP_PROP_FRAME_HEIGHT));
	Size frame_size(frame_width, frame_height);
	double fps = static_cast<double>(vid_capture.get(CAP_PROP_FPS));
	cout << "The frames per second value is :" << fps << endl;
	// Create VideoWriter with 'mp4v'
	VideoWriter writer(out_dir + (string) OUT_MP4,
			VideoWriter::fourcc('m', 'p', '4', 'v'), fps, frame_size);
	while (vid_capture.isOpened()) {
		Mat frame;
		// read image
		bool isSuccess = vid_capture.read(frame);
		if (!isSuccess)
			break;
		// write frame to video
		writer.write(frame);
	}
	writer.release();
	vid_capture.release();
	return 0;
}
