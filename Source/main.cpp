/**
    - Function Outline   : Process the command line arguments,
						  calls the water marking functions to watermark
	
	
						  on the video and saves the same.
    - Author : Mohanarangan.K
						  
*/

#include <iostream>
#include <chrono>


#include "WMInterface.h"
#include "WMOpenCV.h"
#include "logger.h"

using namespace std;
using namespace std::chrono;

#define FRAMES_DIR "./OrigFrames/"
#define OVERLAY_DIR "./WMFrames/"
#define OUT_DIR "./WMVideo/"



using namespace std;
using namespace cv;


/**
	Validate the number of arguments
	Arguments:
		logger - Logger handle
		argc - Number of Arguments
		argv - Arguments


*/

int argument_validator(logger *logger,int argc, char **argv) {
	if (argc != 3) {
			logger->log_info(
					"\n\tUsage: " + (string) argv[0]
							+ " <input.mp4> <overlay.jpg>\n");
			logger->log_info("\tNote: ");
			logger->log_info(
					"\t\t1. Supported input formats video i/p-.mp4,  overlay-'.jpg', '.png'");
			logger->log_info(
					"\t\t2. Output files gets stored in frames/ blended/ out/ folders\n");
			logger->log_info("\t\t Exiting...\n");
			return -1;
		}
	return 0;
}
/**
	Handle the exceptions of arguments
	Arguments:
		Exception - Logger handle
		filename - Number of Arguments
		Logo - Arguments


*/

int exception_handler(EXCEPTION &e,string filename ,string Logo){

			   if (e == EXCP_MP4_INVALID_FILENAME)
					cout <<("Supported only .mp4 or .MP4 as input");
				else if (e == EXCP_OVERLAY_INVALID_FILENAME)
					cout <<("Supported only .jpg or .JPG as overlay");
				else if (e == EXCP_MP4_FILE_NOT_FOUND)
					cout <<("File not found - " + filename);
				else if (e == EXCP_OVERLAY_FILE_NOT_FOUND)
					cout <<("File not found - " + Logo);
				else if (e == EXCP_UNABLE_TO_CREATE_FRAMES_FOLDER)
					cout <<(
							"Folder creation failed - " + (string) FRAMES_DIR);
				else if (e == EXCP_UNABLE_TO_CREATE_OVERLAY_FOLDER)
					cout <<(
							"Folder creation failed - " + (string) OVERLAY_DIR);
				else if (e == EXCP_UNABLE_TO_CREATE_OUT_FOLDER)
					cout <<("Folder creation failed - " + (string) OUT_DIR);
				else if (e == EXCP_FILE_NOT_SUPPORTED)
					cout <<("File format not supported");
				else if (e == EXCP_IS_NOT_VID)
					cout <<("Not a video file - " + filename);
				else if (e == EXCP_IS_NOT_IMG)
					cout <<("Not a image file - "+ Logo);
				else if (e == EXCP_NO_FRAMES_TO_CAPTURE)
					cout <<("No frames availble in video - "+ filename);

				return -1;


}
/**
	Watermarking application, interfaces with the algorithms
	Arguments:
		cWMInterface - Interface for Watermarking
		filename - Number of Arguments
		Logo - Arguments


*/
int wm_application(cWMInterface *lb,string filename ,string Logo ){
	try {
			
			cout << "Logo file name :" << Logo ;
		
			//lb = new cWMInterface(
			//		filename, Logo, FRAMES_DIR, OVERLAY_DIR,
			//		OUT_DIR, BLENDER_OPENCV, WM_LOGO_POS_X, WM_LOGO_POS_Y);

			cout << ("STEP 1: Frame Extraction started") << endl;

			auto start = std::chrono::system_clock::now();
			auto up_time = start;
			
			/* start frame extraction */
			lb->captureFrames();
			auto end = std::chrono::system_clock::now();
			chrono::duration<double> elapsed_seconds = end - start;

			cout << (
					"Frame Extraction completed in sec: "
							+ to_string(elapsed_seconds.count()) + "\n") << endl;

			cout << ("STEP 2: Adding Overlay for all frames...") << endl;

			start = std::chrono::system_clock::now();
			
			/* add overlays on extracted frames */
			lb->insertWM();
			
			end = std::chrono::system_clock::now();
			elapsed_seconds = end - start;

			cout << ("Overlay Addition completed in sec: "
							+ to_string(elapsed_seconds.count()) + "\n") << endl;

			cout << ("STEP 3: Creating mp4...");

			start = std::chrono::system_clock::now();

			/* create mp4 file */
			lb->generateMp4();

			auto close_time = end = std::chrono::system_clock::now();

			elapsed_seconds = end - start;

			cout << (
					"MP4 creation completed in sec: "
							+ to_string(elapsed_seconds.count()) + "\n") << endl;

			elapsed_seconds = close_time - up_time;

			cout << (
					"Total process duration in sec: "
							+ to_string(elapsed_seconds.count()) + "\n") << endl;
		} catch (EXCEPTION &e) {

			exception_handler(e,filename , Logo);

			cleanup: cout << ("Cleanup...") << endl;
			delete lb;
			return 0;
		}
	return 0;
}

/**
	Main file to call all interaces related to watermarking

	Arguments:
		argc - Interface for Watermarking
		argv - Number of Arguments



*/

int main(int argc, char **argv) {



	cWMInterface *init;


	logger *logger = logger::getInstance();

	argument_validator(logger, argc,argv);

	// All Initialization

	string filename = argv[1];
	string logoFile = argv[2];
	string inputFramesDir = FRAMES_DIR;
	string WMLogoDir = OVERLAY_DIR;
	string outputFramesDir = OUT_DIR;
	int  logoPosX = WM_LOGO_POS_X;
	int  logoPosY = WM_LOGO_POS_Y;



    // Creates or clears the directory
	init = new cWMInterface(
			filename,
			logoFile,
			inputFramesDir,
			WMLogoDir,
			outputFramesDir,
			BLENDER_OPENCV,
			logoPosX,
			logoPosY
			);

	// Call the water marking application to water mark on the video file
    wm_application(init, filename, logoFile);



}
