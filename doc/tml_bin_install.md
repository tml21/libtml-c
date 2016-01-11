# Install libTML-c  binary package {#tml_bin_install}	

Currently the following libTML-c binary packages are tested:

- [Windows](#WinLink)
- [debian](#debianLink)
- [OS-X](#OsxLink)
- [freeBSD](#freeBSDLink)

<a name="WinLink"></a>
## Windows installer (32/64 bit) ##

Download the TML Messaging Suite Windows binary installer and launch it on your Windows target system. It supports both, 32 bit and 64 bit Windows systems.

Using the wizard based installer you are able to

- navigate to the next setup wizard page using the “Next” button.
- navigate to the previous setup wizard page using the “Back” button.
- terminate the setup wizard session by using the “Cancel” button.

### Installation step "Collecting information"###
Introductory information about TML Messaging Suite.
### Installation step "Preparing information"###
Allows changing  of the installation destination folder.
### Installation step "Installation"###
During this step the TML Messaging Suite will be installed to the chosen destination folder.
### Installation step "Finalizing installation"###
The TML Messaging Suite is ready to be used.

	
## Windows installation content ##
The installation destination folder contains

- **bin** dynamic link library files (C API & JNI) 
	- **win32**	dynamic link libraries for 32 bit systems
	- **x64**	dynamic link libraries for 64 bit systems
- **include** include files for C/C++ development
- **lib** lib files for C/C++ development
	- **win32**	lib files for 32 bit C/C++ development
	- **x64**	lib files for 64 bit C/C++ development
- **java** java source files
	- **com\\tmlsidex\\jni**	JNI binding of the C API functions
	- **com\\tmlsidex\\sidex** classes to simplify the usage of SIDEX API
	- **com\\tmlsidex\\tml** classes to simplify the usage of TML API
- **pascal** pascal source files
	- **examples**	Lazarus example using the TML Messaging Suite libraries
	- **graphics**	component icons
	- **src**	component source files
	- **packages\\Lazarus**	Lazarus component source files
	- **packages\\C++ Builder**	C++ Builder component source files
	- **packages\\Delphi**	Delphi component source files

Furthermore the libtml-c dynamic link library files are installed into the system folder

- **Windows**
	- **System32** (Windows 32 bit & Windows 64 bit target platform)
	- **SysWOW64** (Windows 64 bit target platform)

**Note:** The Windows binary installer doesn't change the Windows path environment.

## Windows C/C++ development environment requirements ##

To use the TML Messaging Suite for Windows C/C++ development

- add the **include** path to your development project include path 
- add the **lib\\win32** respectively **lib\\x64** path to your development project lib path 
- take care of having the **bin\\win32** respectively **bin\\x64** path in your execution path
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t\*** data type calling the libTML C API
- Declare the compiler prepocessor values **SIDEX_UNICODE_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t\*** data type calling the libTML C API
<br><br>

----------

<a name="debianLink"></a>
## Debian package dependencies  ##

The libTML-c debian package depends on the libaxl1 and libvortex-1.1 debian packages from "Advanced Software Production Line"

    # public aspl debian repository (WHEEZY)
    deb http://www.aspl.es/debian/public/wheezy/
	
    # public aspl debian repository (SQUEEZE)
    deb http://www.aspl.es/debian/public/squeeze/

    # public aspl debian repository (LENNY)
    deb http://www.aspl.es/debian/public/lenny/

See: [http://www.aspl.es/vortex/download.html](http://www.aspl.es/vortex/download.html)

## libTML-c debian package ##
Download the libTML-c debian package onto your target system.

Install the package using shell command


	sudo dpkg -i /PATH/TO/PACKAGE/PACKAGENAME.deb 

## libTML-c debian package installation content ##
The libTML-c debian package installation contains


- shared libraries for the C API installed to folder `"/usr/lib"`
- include files for C development installed to folder  `"/usr/include/tml"`

## Debian C/C++ development environment requirements ##

To use the TML Messaging Suite for debian C/C++ development

- add `"/usr/include/tml"` to your development project include path 
- add the `"/usr/lib"` to your development project lib path 
- Declare the compiler prepocessor **LINUX**
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t\*** data type calling the libTML C API
- Declare the compiler prepocessor values **SIDEX\_UNICODE\_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t\*** data type calling the libTML C API
<br><br>

----------

<a name="OsxLink"></a>
## OS-X Disk Image ##

## Installation steps: ###

 - Download the TML Messaging Suite disk image file (.dmg) onto your OS-X target system
 - Mount the disk image file
 - Copy shared libraries:
   - on a 32 bit environment
   		-  copy **1)** the directory content of `"/Volumes/libtml/usr/lib32"` to the folder `"/usr/lib"`
   - on a 64 bit environment
   		-  copy **2)** the directory content of `"/Volumes/libtml/usr/lib64"` to the folder `"/usr/lib"`


**1)** on the command shell out of "superuser mode":

	cp /Volumes/libtml/usr/lib32/* /usr/lib


**2)** on the command shell out of "superuser mode":

	cp /Volumes/libtml/usr/lib64/* /usr/lib

## OS-X installation content ##
The mounted TML Messaging Suite disk image file contains

- `"usr"` shared library files (C API & JNI)
	- `"lib32"`	shared library files for 32 bit environment
	- `"lib64"`	shared library files for 64 bit environment
- `"include"` include files for C/C++ development
- `"java"` java source files
	- `"com/tmlsidex/jni"`	JNI binding of the C API functions
	- `"com/tmlsidex/sidex"` classes to simplify the usage of SIDEX API
	- `"com/tmlsidex/tml"` classes to simplify the usage of TML API
- `"pascal"` pascal source files
	- `"examples"`	Lazarus example using the TML Messaging Suite libraries
	- `"graphics"`	component icons
	- `"src"`	component source files
	- `"packages/Lazarus"`	Lazarus component source files

## OS-X C/C++ development environment requirements ##

To use the TML Messaging Suite for OS-X C/C++ development

- Copy `"include"` content to your development project / include path 
- add the `"/usr/lib"` to your development project lib path 
- Declare the compiler prepocessor **LINUX** and **OS_X**
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t\*** data type calling the libTML C API
- Declare the compiler prepocessor values **SIDEX\_UNICODE\_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t\*** data type calling the libTML C API
<br><br>

----------

<a name="freeBSDLink"></a>
## freeBSD package dependencies  ##

The TML Messaging Suite libTML-c freeBSD package depends on the libaxl1 and libvortex-1.1 freeBSD packages from "Advanced Software Production Line"

    # public aspl freeBSD 9 repository
    http://www.aspl.es/freebsd/public/9/

    # public aspl freeBSD 10 repository
    http://www.aspl.es/freebsd/public/10/


## libTML-c freeBSD package ##
Download the libTML-c freeBSD package onto your target system.

Install the package using shell command


	pkg install /PATH/TO/PACKAGE/PACKAGENAME.txz


## libTML-c freeBSD package installation content ##
The libTML-c freeBSD package installation contains


- shared libraries for the C API installed to folder  `"/usr/local/lib"`
- include files for C/C++ development installed to folder  `"/usr/local/include/tml"`

## freeBSD C/C++ development environment requirements ##

To use the TML Messaging Suite for freeBSD C/C++ development

- add `"/usr/local/include/tml"` to your development project include path 
- add the `"/usr/local/lib"` to your development project lib path 
- Declare the compiler prepocessor **LINUX** 
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t\*** data type calling the libTML C API
- Declare the compiler prepocessor values **SIDEX\_UNICODE\_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t\*** data type calling the libTML C API


