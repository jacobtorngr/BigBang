import os
from PIL import Image

def convert_tga_to_png(folder_path):
    tga_files = [file for file in os.listdir(folder_path) if file.endswith('.tga')]

    if not tga_files:
        print("No .tga files found in the folder.")
        return

    # Exclude "empty.tga" from conversion
    tga_files = [file for file in tga_files if file != 'empty.tga']

    for tga_file in tga_files:
        tga_path = os.path.join(folder_path, tga_file)
        image = Image.open(tga_path)
        png_path = os.path.splitext(tga_path)[0] + '.png'
        image.save(png_path)

        # Remove original tga file
        os.remove(tga_path)

if __name__ == "__main__":
    # build_dir = os.path.join(os.getcwd(), "build")
    build_dir = os.getcwd()
    convert_tga_to_png(build_dir)