# Forked from SunOner
## 🛠 Build the Project from Source

1. **Install Visual Studio 2022 Community**  
   Download and install from the [official website](https://visualstudio.microsoft.com/vs/community/).

2. **Install Windows SDK**  
   Ensure you have Windows SDK version **10.0.26100.0** installed.

3. **Install CUDA and cuDNN**  
   - **CUDA 12.8**  
     Download from [NVIDIA CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit).
   - **cuDNN 9.7.1**  
     Available on the [NVIDIA cuDNN archive](https://developer.nvidia.com/cudnn-downloads) website.

4. **Set Up Project Structure**  
   Create a folder named `modules` in the directory `sunone_aimbot_cpp/sunone_aimbot_cpp/modules`.

5. **Build OpenCV with CUDA Support**
	- Download and install [CMake](https://cmake.org/) and [CUDA 12.8](https://developer.nvidia.com/cuda-12-8-0-download-archive).
	- Download [OpenCV](https://github.com/opencv/opencv).
	- Download [OpenCV Contrib](https://github.com/opencv/opencv_contrib/tags).
	- Create new directories: `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/opencv/` and `sunone_aimbot_cpp/modules/opencv/build`.
	- Extract `opencv-4.10.0` to `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/opencv/opencv-4.10.0` and `opencv_contrib-4.10.0` to `sunone_aimbot_cpp/modules/opencv/opencv_contrib-4.10.0`.
	- Extract cuDNN to `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/cudnn`.
	- Open CMake and set the source code location to `sunone_aimbot_cpp/modules/opencv/opencv-4.10.0`.
	- Set the build directory to `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/opencv/build`.
	- Click `Configure`.
	- (Some options will appear after the next configuration application. For example, to configure the CUDNN_LIBRARY paths, you first need to activate the WITH_CUDA option and click configure.)
	- Check or configure:
		- `WITH_CUDA`
		- `WITH_CUBLAS`
		- `ENABLE_FAST_MATH`
		- `CUDA_FAST_MATH`
		- `WITH_CUDNN`
		- `CUDNN_LIBRARY` = `<full path>sunone_aimbot_cpp/sunone_aimbot_cpp/modules/cudnn/lib/x64/cudnn.lib`
		- `CUDNN_INCLUDE_DIR` = `<full path>sunone_aimbot_cpp/sunone_aimbot_cpp/modules/cudnn/include`
		- `CUDA_ARCH_BIN` = Visit the [CUDA Wiki](https://en.wikipedia.org/wiki/CUDA) to find your Nvidia GPU architecture. For example, for `RTX 3080-TI`, enter `8.6`.
		- `OPENCV_DNN_CUDA`
		- `OPENCV_EXTRA_MODULES_PATH` = `<full path>sunone_aimbot_cpp/sunone_aimbot_cpp/modules/opencv/opencv_contrib-4.10.0/modules`
		- `BUILD_opencv_world`
		
		- Uncheck:
			- `WITH_NVCUVENC`
			- `WITH_NVCUVID`
   - Click `Configure` again and ensure that the flags for `CUDA_FAST_MATH` and `ENABLE_FAST_MATH` are not reset.
   - Click `Generate` to build the C++ solution.
   - Close CMake and open `sunone_aimbot_cpp/modules/opencv/build/OpenCV.sln`, or click `Open Project` in cmake.
   - Switch the build configuration to `x64` and `Release`.
   - Open the `CMakeTargets` folder in the solution.
   - Right-click on `ALL_BUILD` and select `Build`. (Building the project can take up to two hours.)
   - After building, right-click on `INSTALL` and select `Build`.
   - Verify the built files exist in the following folders:
     - `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/opencv/build/install/include/opencv2` - Contains `.hpp` and `.h` files.
     - `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/opencv/build/install/x64/vc16/bin` - Contains `.dll` files.
     - `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/opencv/build/install/x64/vc16/lib` - Contains `.lib` files.

6. **Download Required Libraries**  
	- [simpleIni](https://github.com/brofield/simpleini/blob/master/SimpleIni.h)
	- [serial](https://github.com/wjwwood/serial)
	- [TensorRT-10.8.0.43](https://developer.nvidia.com/tensorrt/download/10x)
	- [GLWF Windows pre-compiled binaries](https://www.glfw.org/download.html)
	
7. **Extract Libraries**  
	Place the downloaded libraries into the respective directories:
	- `SimpleIni.h` -> `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/SimpleIni.h`
	- `serial` -> `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/serial`
	- `TensorRT-10.8.0.43` -> `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/TensorRT-10.8.0.43`
	- `GLWF` -> `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/glfw-3.4.bin.WIN64`
	
8. **Compile serial**
	- Navigate to the serial directory `sunone_aimbot_cpp/sunone_aimbot_cpp/modules/serial/visual_studio/`
	- Open `visual_studio.sln`
	- Go to project settings -> C/C++ -> Code Generation -> and change `Runtime Library` to `Multi-threaded (/MT)`
	- Build in `Release x64`
   
9. **Configure Project Settings**
	- Open the project in Visual Studio.
	- Ensure all library paths are correctly set in **Project Properties** under **Library Directories**.
	- Go to NuGet packages and install `Microsoft.Windows.CppWinRT`.

10. **Verify CUDA Integration**
	- Right-click on the project in Visual Studio.
	- Navigate to **Build Dependencies** > **Build Customizations**.
	- Ensure that **CUDA 12.8** (.targets, .props) is included.

11. **Build the Project**
    - Switch the build configuration to **Release**.
    - Build the project by selecting **Build** > **Build Solution**.
## Export model to run model in RZCTL-AI-Aimbot:
- Export model to onnx format with dynamic shape:
	```bash
	yolo export model=best.pt format=onnx dynamic=true simplify=true
	```
- Select .onnx model in AI tab.
