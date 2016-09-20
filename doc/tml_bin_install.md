# Install libTML-c  binary package {#tml_bin_install}	

Currently the following libTML-c binary packages are tested:

- [Windows](#WinLink)
- [debian](#debianLink)
- [OS-X](#OsxLink)
- [freeBSD](#freeBSDLink)

<br>

----------

<a name="TLSOption"></a>
## Optional libTML TLS binary package ##

It is possible to use TLS encrypted connections.
In that case you have to install the optional libTML TLS profile supporting binary package.



<br>

----------

## Library dependencies ##

The libTML-c binary package depends on the **libiconv** library (except on Windows).


- **Homepage:** [`http://www.gnu.org/software/libiconv`](http://www.gnu.org/software/libiconv)
- **Download:** [`http://www.gnu.org/software/libiconv/#downloading`](http://www.gnu.org/software/libiconv/#downloading)
- **Documentation:** [`http://www.gnu.org/software/libiconv/#introduction`](http://www.gnu.org/software/libiconv/#introduction)
- **Remarks:**  If not available on the target platform, the library can be compiled from source

    	./configure
    	make
    	make install
    

<br>

----------

## Runtime environment ##

The libTML-c binary package depends on the following runtime environment:

- **Windows:** Visual Studio 2010 Runtime
- **Debian i386:** gcc 4.8.4
- **Debian amd64:** gcc 4.8.4
- **Debian armhf:** gcc 4.6.3
- **FreeBSD:** gcc 4.9.4
- **OS X:** AppleClang 5.0.0

<br>

----------

<a name="WinLink"></a>
## Windows installer (32/64 bit) ##

The TML Messaging Suite Windows binary installer supports both, 32 bit and 64 bit Windows systems. Download and and launch it on your Windows target system. Follow the instructions.

	
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

The Windows installation **includes** the [libTML TLS profile supporting libraries](#TLSOption).

**Note:** The Windows binary installer doesn't change the Windows path environment.

## Windows C/C++ development environment requirements ##

To use the TML Messaging Suite for Windows C/C++ development

- add the **include** path to your development project include path 
- add the **lib\\win32** respectively **lib\\x64** path to your development project lib path 
- take care of having the **bin\\win32** respectively **bin\\x64** path in your execution path
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t\*** data type calling the libTML C API
- Declare the compiler prepocessor values **SIDEX_UNICODE_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t\*** data type calling the libTML C API

<br>

----------

<a name="debianLink"></a>


## libTML-c debian package ##
Download the libTML-c debian package onto your target system.

Install the package using shell command


	sudo dpkg -i /PATH/TO/PACKAGE/PACKAGENAME.deb 

To use [libTML TLS profile support](#TLSOption), download and install the **libTML TLS** debian package too.

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

<br>

----------

<a name="OsxLink"></a>
## OS-X Disk Image ##

 - Download the TML Messaging Suite disk image file (.dmg) onto your OS-X target system
 - Mount the disk image file

## OS-X Disk Image content ##
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

The OS-X Disk Image **includes** the [libTML TLS profile supporting libraries](#TLSOption).

##System Integrity Protection - using OS X 10.11 (El Capitan)##

If the System Integrity Protection is enabled (default on El Capitan), root is not permitted to copy to `"/usr/lib"`.
    
To disable System Integrity Protection, you must boot to Recovery OS and run the csrutil(1) command from the Terminal.<br>[See:Configuring System Integrity Protection](https://developer.apple.com/library/mac/documentation/Security/Conceptual/System_Integrity_Protection_Guide/ConfiguringSystemIntegrityProtection/ConfiguringSystemIntegrityProtection.html).

- Boot to Recovery OS by restarting your machine and holding down the Command and R keys at startup.
- Launch Terminal from the Utilities menu.
- Enter the following command:

		$ csrutil disable

After disabling System Integrity Protection on a machine, a reboot is required.    

##Copy shared library files##

- on a 32 bit environment
	- copy **1)** the directory content of `"/Volumes/libtml/usr/lib32"` to the folder `"/usr/lib"`

- on a 64 bit environment
	- copy **2)** the directory content of `"/Volumes/libtml/usr/lib64"` to the folder `"/usr/lib"`


**1)** on the command shell out of "superuser mode":

	cp /Volumes/libtml/usr/lib32/* /usr/lib


**2)** on the command shell out of "superuser mode":

	cp /Volumes/libtml/usr/lib64/* /usr/lib

## OS-X C/C++ development environment requirements ##

To use the TML Messaging Suite for OS-X C/C++ development

- Copy `"include"` content to your development project / include path 
- add the `"/usr/lib"` to your development project lib path 
- Declare the compiler prepocessor **LINUX** and **OS_X**
- Declare the compiler prepocessor values **SIDEX\_UNICODE** and **TML\_UNICODE** in case you want to use the **wchar_t\*** data type calling the libTML C API
- Declare the compiler prepocessor values **SIDEX\_UNICODE\_16** and **TML\_UNICODE\_16** in case you want to use the  **char16_t\*** data type calling the libTML C API

<br>

----------

<a name="freeBSDLink"></a>
## libTML-c freeBSD package ##
Download the libTML-c freeBSD package onto your target system.

Install the package using shell command


	pkg install /PATH/TO/PACKAGE/PACKAGENAME.txz


To use [libTML TLS profile support](#TLSOption), download and install the **libTML TLS** freeBSD package too.

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


