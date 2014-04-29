InterplanetarySC
================

Using Martin Schweiger's "Orbiter" simulator to create a interplanetary spacecraft.

================
How to get it running:

1.  To compile the dll-file properly you should place the Orbiter folder in the same 
directory as your repository folder.

The structure should look like the following:
----------------------
<<Repository>>
..*InterplanetarySC
..*SeminarDateien
..*...
..*...

..*Orbiter
..*...
..*...
..*Orbiter.exe
..*...
----------------------

The absolute installation path isn't relevant as long as you keep to the directory structure described above.
I've used "Visual Studio 2012 Ultimate" but it should also work just fine with any other VS2012 version.
The generated dll file will be placed in the right folder "Orbiter\Modules\".
    
2.  The config file named "InterplanetarySC.cfg" from the repository root should be placed in "Orbiter\Config\Vessels\"

3.  The 3D model file from "SeminarDateien\Meshes\ThermalFissionCraft.msh" should be placed in "Orbiter\meshes\"
