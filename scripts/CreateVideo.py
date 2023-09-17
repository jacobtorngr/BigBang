import os
import cv2
from tqdm import tqdm

FRAME_RATE = 20

def create_video_from_png_files(folder_path, output_file):
    png_files = [file for file in os.listdir(folder_path) if file.endswith('.png')]
    png_files.sort()  # Sort the files alphabetically

    if not png_files:
        print("No .png files found in the folder.")
        return

    # Initialize variables for width and height
    width, height = None, None

    # Define the codec and create VideoWriter object
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    video = None

    for png_file in tqdm(png_files, desc="Creating video"):
        image_path = os.path.join(folder_path, png_file)
        frame = cv2.imread(image_path, cv2.IMREAD_COLOR)

        if frame is None:
            print(f"Failed to read the image: {png_file}. Skipping.")
            continue

        if width is None or height is None:
            height, width, _ = frame.shape
            video = cv2.VideoWriter(output_file, fourcc, FRAME_RATE, (width, height))

        video.write(frame)

        os.remove(png_file)

    if video is not None:
        video.release()
        print("Video created successfully.")
    else:
        print("No valid images found to create the video.")

if __name__ == "__main__":
    # build_dir = os.path.join(os.getcwd(),"build")
    build_dir = os.getcwd()
    create_video_from_png_files(build_dir,"output_video.avi")