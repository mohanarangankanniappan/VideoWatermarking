
Video Watermarking

	- Frame Extraction: 
		Extracts frames from prvided input mp4 file and stores into frames/ folder as jpg format
	- Watermarking:
		Blends provided overlay image(.png/.jpg) over extracted frames and stores into blended/ folder as jpg format
	- MP4 creation
		Creates MP4 video using the blended frames and stores into out/ folder

Dependencies

	- opencv library package
	
Usage
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH=./
	./watermarker <input.mp4> <overlay.jpg>
	Note:
	1. Supported input formats video input-.mp4,  overlay-'.jpg', '.png'
	2. Output files gets stored in frames/ blended/ out/ folders
		
	
