NECESSARY INSTALLATIONS

sudo apt-get install libjpeg-dev
sudo apt-get install libfreeimage-dev


gcc -o serialGauss 01_serialGaussianBlur.c -lm
./serialGauss 1 600

gcc -o parallelGauss 01_parallelGaussianBlur.c -fopenmp -lm
./parallelGauss 1 600

gcc -o serialBox 01_serialBoxBlur.c -ljpeg -fopenmp
./serialBox

gcc -o parallelBox 01_parallelBoxBlur.c -ljpeg -fopenmp
./parallelBox

gcc -o serialInvert 01_serialmageInversion.c -lfreeimage -fopenmp
./serialInvert input3.jpg outputSS.jpg

gcc -o parallelInvert 01_parallelImageInversion.c -lfreeimage -fopenmp
./parallelInvert input3.jpg outputSP.jpg
