# gicDemo 
This is a small embedded application skeleton for demonstrating how to configure GIC v3 for interrupt handling on ARMv8A systems.
This demo application is incrementally built on top of [aohzju/embAppStartup](https://github.com/aohzju/embAppStartup). It's built and tested on ARM virtual platform ARMv8A AEM model: FVP_Base_RevC-2xAEMv8A. 


 # Build and run
 the armclang toolchain is used to build this example. And the build script is CMake script. It's tested on ARM FVP FVP_Base_RevC-2xAEMv8A. Assuming your development environment is correctly set up, this is the steps to build:
 1. Creae a build directory: mkdir build
 2. CD to build: cd build.
 3. cmake ../ 
 4. cmake --build . -v
 
 Hopefully, you'll get appGicDemo.axf up to this point.
 
 To run the FVP, use these model parameters:
 
 -C bp.secure_memory=false -C bp.terminal_0.mode=raw -C bp.refcounter.non_arch_start_at_default=1 -C cluster0.gicv3.FIQEn-RAO=1 
 
 # ARM FVP
 The virtual platform used in this application is FVP_Base_RevC-2xAEMv8A, which can be downloaded [here](https://developer.arm.com/tools-and-software/simulation-models/fixed-virtual-platforms). 
