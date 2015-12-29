# Install libTML  binary package {#tml_bin_install}	

Currently the following binary packages are tested:

- [Windows](#WinLink)
- [debian libtml-c](#debianLinkC)
- [debian libtml-java](#debianLinkJava)
- [OS X](#OsxLink)
- [freeBSD libtml-c](#freeBSDLinkC)
- [freeBSD libtml-java](#freeBSDLinkJava)

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

**Note:** The Windows binary installer don't change the Windows path environment.


## Windows installation content ##
The installation destination folder contains

- **bin** dynamic link library files (C- API & JNI) 
	- **win32**	dynamic link libraries for 32 bit systems
	- **x64**	dynamic link libraries for 64 bit systems
- **include** include files for C- development
- **lib** lib files for C- development
	- **win32**	lib files for 32 bit C- development
	- **x64**	lib files for 64 bit C- development
- **java** java source files
	- **com\tmlsidex\jni**	JNI binding of the C- API functions
	- **com\tmlsidex\sidex** classes to simplify the usage of SIDEX- API
	- **com\tmlsidex\tml** classes to simplify the usage of TML- API
- **pascal** pascal source files
	- **examples**	Lazarus example using the TML Messaging Suite libraries
	- **graphics**	component icons
	- **src**	component source files
	- **packages\Lazarus**	Lazarus component source files
	- **packages\C++ Builder**	C++ Builder component source files
	- **packages\Delphi**	Delphi component source files

## Windows C- development environment requirements ##

To use the TML Messaging Suite for Windows C- development

- add the **include** path to your development project include path 
- add the **lib\win32** respectively **lib\x64** path to your development project lib path 
- take care of having the **bin\win32** respectively **bin\x64** path in your execution path
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t*** data type calling the libTML C- API.
- Declare the compiler prepocessor values **SIDEX\_UNICODE\_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t*** data type calling the libTML C- API.


## Debian package installer ##

The TML Messaging Suite debian packages depend on the libaxl1 and libvortex-1.1 debian packages from "Advanced Software Production Line"

    # public aspl debian repository (WHEEZY)
    deb http://www.aspl.es/debian/public/wheezy/
	
    # public aspl debian repository (SQUEEZE)
    deb http://www.aspl.es/debian/public/squeeze/

    # public aspl debian repository (LENNY)
    deb http://www.aspl.es/debian/public/lenny/

See: [http://www.aspl.es/vortex/download.html](http://www.aspl.es/vortex/download.html)

<a name="debianLinkC"></a>
## libTML debian C- package ##
Download the TML Messaging Suite debian C- package onto your target system.

Install the package using shell command


	sudo dpkg -i /PATH/TO/PACKAGE/PACKAGENAME.deb 

## Debian C- package installation content ##
The debian  C- package installation contains


- shared libraries for the C- API written to **/usr/lib**
- include files for C- development written to **/usr/include/tml**

## Debian C- development environment requirements ##

To use the TML Messaging Suite for Windows C- development

- add **/usr/include/tml** to your development project include path 
- add the **/usr/lib** to your development project lib path 
- Declare the compiler prepocessor **LINUX** .
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t*** data type calling the libTML C- API.
- Declare the compiler prepocessor values **SIDEX\_UNICODE\_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t*** data type calling the libTML C- API.


<a name="debianLinkJava"></a>
## libTML debian Java- package ##
Download the TML Messaging Suite debian Java- package onto your target system.

Install the package using shell command


	sudo dpkg -i /PATH/TO/PACKAGE/PACKAGENAME.deb 

## Debian Java- package installation content ##
The debian Java- package installation contains


- shared libraries for the Java Native Interface written to **/usr/lib**

<a name="OsxLink"></a>
## OS-X Disk Image ##

## Installation steps: ###

 - Download the TML Messaging Suite disk image file (.dmg) onto your OS-X target system.
 - Mount the disk image file.
 - Copy shared libraries:
   - on a 32 bit environment
   		-  copy ***)** the directory content of **/Volumes/libtml/usr/lib32** to the folder **/usr/lib**
   - on a 64 bit environment
   		-  copy ***)** the directory content of **/Volumes/libtml/usr/lib64** to the folder **/usr/lib**

***)**

	cp /Volumes/libtml/usr/lib32/* /usr/lib
respectively

	cp /Volumes/libtml/usr/lib64/* /usr/lib
on the command shell out of "superuser mode". 

## OS-X installation content ##
The mounted TML Messaging Suite disk image file contains

- **use** shared library files (C- API & JNI) 
	- **lib32**	shared library files for 32 bit environment
	- **lib64**	shared library files for 64 bit environment
- **include** include files for C- development
- **java** java source files
	- **com\tmlsidex\jni**	JNI binding of the C- API functions
	- **com\tmlsidex\sidex** classes to simplify the usage of SIDEX- API
	- **com\tmlsidex\tml** classes to simplify the usage of TML- API
- **pascal** pascal source files
	- **examples**	Lazarus example using the TML Messaging Suite libraries
	- **graphics**	component icons
	- **src**	component source files
	- **packages\Lazarus**	Lazarus component source files

## OS-X C- development environment requirements ##

To use the TML Messaging Suite for Windows C- development

- Copy **include** content to your development project / include path 
- add the **/usr/lib** to your development project lib path 
- Declare the compiler prepocessor **LINUX** and **OS_X** .
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t*** data type calling the libTML C- API.
- Declare the compiler prepocessor values **SIDEX\_UNICODE\_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t*** data type calling the libTML C- API.



## freeBSD package installer ##

The TML Messaging Suite freeBSD packages depend on the libaxl1 and libvortex-1.1 freeBSD packages from "Advanced Software Production Line"

    # public aspl freeBSD 9 repository
    http://www.aspl.es/freebsd/public/9/

    # public aspl freeBSD 10 repository
    http://www.aspl.es/freebsd/public/10/


<a name="freeBSDLinkC"></a>
## libTML freeBSD C- package ##
Download the TML Messaging Suite freeBSD C- package onto your target system.

Install the package.

## freeBSD C- package installation content ##
The freeBSD  C- package installation contains


- shared libraries for the C- API written to **/usr/lib**
- include files for C- development written to **/usr/include/tml**

## freeBSD C- development environment requirements ##

To use the TML Messaging Suite for Windows C- development

- add **/usr/include/tml** to your development project include path 
- add the **/usr/lib** to your development project lib path 
- Declare the compiler prepocessor **LINUX** .
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t*** data type calling the libTML C- API.
- Declare the compiler prepocessor values **SIDEX\_UNICODE\_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t*** data type calling the libTML C- API.


<a name="freeBSDLinkJava"></a>
## freeBSD Java- package ##
Download the TML Messaging Suite freeBSD Java- package onto your target system.

Install the package.

## freeBSD Java- package installation content ##
The freeBSD Java- package installation contains


- shared libraries for the Java Native Interface written to **/usr/lib**

