/* 
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2015 wobe-systems GmbH
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 *
 *  For commercial support on build UJO enabled solutions contact us:
 *  
 * Contributors:
 *    wobe-systems GmbH
 */

/** @file tmlDoxygenMain.h
  * @brief This is a helper file for the doxygen document generation
  */

/** @mainpage TML C API
  * 
  * The TML <B>(The Missing Link)</B> provides an API for peer to peer network communication over TCP/IP.<br>
  * It is written in <B>C/C++</B> using a BEEP implementation from <a href="http://www.aspl.es" target="_blank">Advanced Software Production Line S.L.</a>.<br>
  *
  * @note TML uses SIDEX as data interface.
  * Read the "SIDEX C API" documentation for further reference.<br>
  *
  * <br><br>
  *  To use the TMLCore API library, add the<br>
  *         - API definitions tmlCore.h, sidex.h,
  *         - general definitions globalDefines.h and tmlGlobalDefines.h,
  *         - data type definitions stdIntTypes.h, sidexStdTypes.h and tmlStdTypes.h,
  *         - error definitions tmlErrors.h and sidexErrors.h
  * <br><br>
  *  to your projects include path.<br>
  *
  * - for Windows 32 add<br>
  * the tmlCore11.lib and sidex11.lib to projects library path and make sure to have the tmlCore11.dll, sidex11.dll, libvortex-1.1.dll and the libaxl.dll in your path.<br><br>
  * - for Windows 64 add<br>
  * the tmlCore64-11.lib and sidex64-11.lib to projects library path and make sure to have the tmlCore64-11.dll, sidex64-11.dll, libvortex64-1.1.dll and the libaxl64.dll in your path.<br><br>
  * - Linux (i386):<br>
  * make sure to have the libtmlCore11.so, libsidex11.so, libvortex-1.1.so.0.0.0 (with it's symbolic library link libvortex-1.1.so.0) and the libaxl.so.0.0.0  (with it's symbolic library link libaxl.so.0) in your library path.<br>
  * Declare a compiler preprocessor definition with the value "LINUX".<br><br>
  * - Linux (x86_64):<br>
  * make sure to have the libtmlCore64-11.so, libsidex64-11.so, libvortex-1.1.so.0.0.0 (with it's symbolic library link libvortex-1.1.so.0) and the libaxl.so.0.0.0  (with it's symbolic library link libaxl.so.0) in your library path.<br>
  * Declare a compiler preprocessor definition with the value "LINUX".<br><br>
  * - OS-X:<br>
  * make sure to have the libtmlCore11.dylib, libsidex11.dylib, libvortex-1.1.0.dylib and the libaxl.0.dylib in your library path.<br>
  * Declare a compiler preprocessor definition with the value "LINUX" and a compiler preprocessor definition with the value "OS_X".<br><br>
  */
//////////////////////////////////////////
// SUPRESS examples via doxygen:
#ifndef DOXYGEN_NO_EXAMPLES
/**
  * </a><h2><a class="anchor" name="example_general">
  * General:
  * </a></h2>
  *  To use the TMLCore API it is necessary to:
  *    - link the tmlCore11.lib (tmlCore11d.lib in case of the debug version) into your <B>C / C<SUP>++</SUP></B> project.
  *         <br>
  *    - include header files into your source:
  *         - the API declaration for sidex header I/O and callback registrations, logic and communication will be found in the header file tmlCore.h
  *         - the sidex API declaration for data I/O based on XML will be found in the header file sidex.h (see SIDEX documentation).
  *         - error codes in tmlErrors.h and sidexErrors.h
  *         - general definitions in globalDefines.h and tmlGlobalDefines.h
  *         - data type definitions  in stdIntTypes.h, sidexStdTypes.h and tmlStdTypes.h
  *         .
  *         <br>
  *    - to execute your application take care to have the following dll files in your path:
  *         - libvortex-1.1.dll
  *         - libaxl.dll
  *         - tmlCore11.dll (tmlCore11d.dll in case of the debug version)
  *         - sidex11.dll (sidex11d.dll in case of the debug version)
  *         .
  *    .
  * <br><br>
  * The initial API call on server side or client side is tml_Core_Open().<br>
  * According to this the finishing API call must be tml_Core_Close().<br>
  * <br>
  * <a href="#examles">Back to the Top</a>
  * <br>
  * 
  *
  * <br><br><br><br><br>
  *
  * </a><h2><a class="anchor" name="example_server">
  * Example code for the server side implementation:
  * </a></h2>
  * The server has to execute commands / messages from the client. To perform this:
  *    - Get an instance of a TMLCore at the beginning.
  *    - Initialize the Core listener.
  *    - Configure the Core dispatcher.
  *    - Release the instance of the TMLCore at the end.
  *    .
  * The following example code shows a simple main body of a server:
  * @verbinclude example_serverMain.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows how to initialize the Core listener called by the main body:
  * @verbinclude example_initListener.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows how to configure the dispatcher called by the main body:
  * @verbinclude example_initDispatcher.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows the callback method that will be called on every command deletion registered in the initDispatcher() method:
  * @verbinclude example_serverCallbackOnDelete.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows the callback method that will be called on every unregistered command detected in the dispatch action:
  * @verbinclude example_serverCustomCallback.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows the callback method that simulates some work registered in the initDispatcher() method:
  * @verbinclude example_someWorkCallback.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows the callback method that simulates complex work with progress reply's registered in the initDispatcher() method:
  * @verbinclude example_complexWorkCallback.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  *
  * <br><br><br><br><br>
  *
  * </a><h2><a class="anchor" name="example_client">
  * Example code for the client side implementation:
  * </a></h2>
  * The following example code shows a simple main body of a client:
  * @verbinclude example_clientMain.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows the sendMessage() method called by the body:
  * @verbinclude example_clientSendMessage.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows the progress callback method that will be called on async. progress messages from the server registered in the sendMessage() method:
  * @verbinclude example_clientProgressCB.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  * <br>
  * The following example code shows the status callback method that will be called on async. status messages from the server registered in the sendMessage() method:
  * @verbinclude example_clientStatusCB.c
  * <br>
  * <a href="#examles">Back to the Top</a><br>
  *
  * <p>
  */
#endif  // DOXYGEN_NO_EXAMPLESX
/////////////////////////////////////////////////
// end of SUPRESS examples via doxygen.
/////////////////////////////////////////////////
  
