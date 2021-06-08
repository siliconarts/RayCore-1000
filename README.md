## Introduction
RayCore Series 1000 is an ideal real-time ray-tracing GPU IP for mobile and embedded applications. 
This state-of-the-art GPU IP provides reality-like 3D graphics by fully supporting natural light effects such as reflection, refraction, transmission, and various types of shadow.
As an Intel Gold partner, we provide you with a ray tracing GPU solution that has RayCore 1000 integrated into Intel Arria 5 FPGA. 

## Requirement
- OS: Ubuntu 18.04.5
- Compiler: GCC 8.4.0
- Hardware device: Alcointera - Arria V GX Starter Kit or  Altera - Stratix V GX FPGA Development Kit

## Build
1. Clone the repository.
2. Install the X11 library.
3. Make a directory named linux_x64 in the app/lib directory
4. Run the build.sh script.
```
git clone https://github.com/siliconarts/raycore1000.git
mkdir ./app/lib/linux_x64
sh build.sh
```

## Usage
1. Follow the instruction in the "RayCore® 1000 Dev Kit Quick Start Guide.pdf" file in the "doc" directory to set up the FPGA hardware.
2. Download [scene.zip](https://drive.google.com/file/d/1JEegzHSKsFu4QYNlTnQFkOTu4gQj_xnE/view?usp=sharing), and uncompress the zip file to the "bin" directory.
3. Run demos
	- You can change materials' properties in some demos. For example in the "GraphicEffect_Cup" demo, you can press "1" key to select the cup material, and then control its transparency, reflectance or refraction using T, G, R, and F keys. Please, refer to the "Key_event_and_materials.txt" in the "doc" directory for more details.
	
	```
	# At the root of the repository
	unzip scene_data.zip -d ./bin
	cd bin
	./rc_cup
	```

	- Using the following commands for the "sceneplayer" demos, all input scene data of the "sceneplayer" are in the sceneplayer directory:
	```
	# command syntax: ./rc_sceneplayer scene_path [width] [height]	
	cd bin
	./rc_sceneplayer ./sceneplayer/basic/bedchair/ 800 480
	```
4. If you want to create your own samples, you can refer to other documents in the "doc" directory.

## License
See [License](License)
