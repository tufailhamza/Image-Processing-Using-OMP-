Certainly! It looks like your README file could use some formatting improvements and additional information. Here's a revised version:

---

# Image Processing Using OpenMP

This repository contains C programs for image processing using OpenMP (Open Multi-Processing). The provided programs include serial and parallel implementations of Gaussian blur, box blur, and image inversion.

## Prerequisites

Before running the programs, make sure you have the necessary libraries installed:

```bash
sudo apt-get install libjpeg-dev
sudo apt-get install libfreeimage-dev
```

## Compile and Run

### Serial Gaussian Blur

```bash
gcc -o serialGauss 01_serialGaussianBlur.c -lm
./serialGauss 1 600
```

### Parallel Gaussian Blur

```bash
gcc -o parallelGauss 01_parallelGaussianBlur.c -fopenmp -lm
./parallelGauss 1 600
```

### Serial Box Blur

```bash
gcc -o serialBox 01_serialBoxBlur.c -ljpeg -fopenmp
./serialBox
```

### Parallel Box Blur

```bash
gcc -o parallelBox 01_parallelBoxBlur.c -ljpeg -fopenmp
./parallelBox
```

### Serial Image Inversion

```bash
gcc -o serialInvert 01_serialImageInversion.c -lfreeimage -fopenmp
./serialInvert input3.jpg outputSS.jpg
```

### Parallel Image Inversion

```bash
gcc -o parallelInvert 01_parallelImageInversion.c -lfreeimage -fopenmp
./parallelInvert input3.jpg outputSP.jpg
```

## Screenshots

Here are some screenshots showcasing the output of the programs:

- [Screenshot 1](https://github.com/tufailhamza/Image-Processing-Using-OMP-/assets/83925732/e9978efb-2967-4ec9-87da-e38f2ece2451)
- [Screenshot 2](https://github.com/tufailhamza/Image-Processing-Using-OMP-/assets/83925732/755da73a-1d00-4108-aea1-cfb35e8992dd)
- [Screenshot 3](https://github.com/tufailhamza/Image-Processing-Using-OMP-/assets/83925732/358a9818-d231-4b25-8dc0-b3c64e72c936)
- [Screenshot 4](https://github.com/tufailhamza/Image-Processing-Using-OMP-/assets/83925732/e84493b2-15ea-4ac0-94aa-653e3518b9c9)
- [Screenshot 5](https://github.com/tufailhamza/Image-Processing-Using-OMP-/assets/83925732/4beec9e4-4178-4ce7-83ba-aea12842d925)

Feel free to explore the repository and use the provided programs for your image processing tasks. If you encounter any issues or have questions, please refer to the documentation or open an issue.

