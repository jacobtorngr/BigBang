import os
import cv2
from PIL import Image
from tqdm import tqdm
import shutil

FRAME_RATE = 20

def move_tga_files(folder_path):
    tga_files = [file for file in os.listdir(folder_path) if file.endswith('.tga') and file != 'empty.tga']
    tga_files.sort()  # Sort the files alphabetically

    tga_folder_path = os.path.join(folder_path, "tga_files")
    os.makedirs(tga_folder_path, exist_ok=True)

    moved = 0
    for tga_file in tga_files:
        tga_path = os.path.join(folder_path, tga_file)
        shutil.move(tga_path, os.path.join(tga_folder_path, tga_file))

    if moved > 0:
        print("TGA files (excluding 'empty.tga') moved to 'tga_files' folder.")

def convert_tga_to_png(folder_path):
    tga_files = [file for file in os.listdir(folder_path) if file.endswith('.tga')]
    tga_files.sort()  # Sort the files alphabetically

    if not tga_files:
        print("No .tga files found in the folder.")
        return

    # Exclude "empty.tga" from conversion
    tga_files = [file for file in tga_files if file != 'empty.tga']

    for tga_file in tqdm(tga_files, desc="Converting to PNG"):
        tga_path = os.path.join(folder_path, tga_file)
        image = Image.open(tga_path)
        png_path = os.path.splitext(tga_path)[0] + '.png'
        image.save(png_path)

    print("Image conversion completed.")

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

    if video is not None:
        video.release()
        print("Video created successfully.")
    else:
        print("No valid images found to create the video.")

    # Delete the .png files after video creation
    for png_file in tqdm(png_files, desc="Deleting PNG files"):
        png_path = os.path.join(folder_path, png_file)
        os.remove(png_path)

    print("PNG files deleted.")

if __name__ == "__main__":
    folder_path = "/home/student/test/BigBang/build"
    output_file = "output_video.avi"

    move_tga_files(folder_path)
    convert_tga_to_png(os.path.join(folder_path, "tga_files"))
    create_video_from_png_files(os.path.join(folder_path, "tga_files"), output_file)
