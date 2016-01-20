# Introduction to libTML-c {#tml_intro}

* [How to use the library](#howtouse)
	* [Create a TML Interface](#createTMLInterface)
	* [Call a TML Interface](#callTMLcommand)
	* [Passing parameters and results](#PassTMLParam)
* [Error handling](#errHandling)

> All examples in this introduction contain only code lines to explain a special aspect of the API. Important things like error handling are left out in favor of better readability. 

<a name="howtouse"></a>
## How to use the library ##

The libTML-c API is a set of functions operating on handles of objects. Two fundamental modules are available. TML (The Missing Link) provides the messaging and network functions and SIDEX (Simple Data Exchange) provides the API to access message data. After [installing the library](\ref tml_install) you can import TML and SIDEX into your project.

~~~~{.c}
import "tmlCore.h"
import "sidex.h"
~~~~

The library does not need any kind of initialization. You can start creating your first TML enabled application.

<a name="createTMLInterface"></a>
#### Create a TML Interface ###

To use TML messaging, an application creates a TMLCore object. The function `tml_Core_Open()` is creating the TMLCore and returns a handle. All subsequent calls to TMLCore methods use the handle. A single TMLCore processes inbound and outbound traffic. To accept inbound traffic a listener is started. 

This example shows how to create a TMLCore and a profile with a single function. 

Basically a profile is an interface providing one or more commands with unique IDs. The profile is identified by a unique string. Any string can be used but it has to be unique and it is recommended to use a URN (Unified Resource Name). The first part of the URN should be a domain name registered by the creator of the interface to make sure it does not conflict with other interfaces. If no registered domain is available, select any string that is as unique as possible. 
Multiple profiles can be registered at a single TMLCore with `tml_Profile_Register()`. There is no need for another listener if the application provides more than one interface.


This command handler function is printing a message on the console to signal an incoming call. The `msg` parameter is the received command. The `data` parameter is a pointer to a custom object registered with the command. 

~~~~{.c}
// a simple command handler function
void FUNC_C_DECL cmd_handler_4711(TML_COMMAND_HANDLE msg, TML_POINTER data){
 printf ("CMD 4711 called\n");
} 
~~~~

Multiple functions are registered by repeating the call to `tml_Profile_Register_Cmd()`. 

~~~~{.c}
// create the TMLCore handle
TML_CORE_HANDLE coreHandle = TML_HANDLE_TYPE_NULL;
tml_Core_Open(&coreHandle, 0);

// set listener IP and Port for inbound traffic
// setting the IP address to 0.0.0.0 binds the listener to all interfaces
tml_Core_Set_ListenerIP (coreHandle, "0.0.0.0");
tml_Core_Set_ListenerPort (coreHandle, "44100");

// register profile
tml_Profile_Register (coreHandle, "urn:libtml:org:profile:example1");

// register command at core and profile
tml_Profile_Register_Cmd (coreHandle,
   "urn:libtml:org:profile:example1",
   (TML_COMMAND_ID_TYPE) 4711, cmd_handler_4711, TML_HANDLE_TYPE_NULL);

// start the listener
tml_Core_Set_ListenerEnabled (coreHandle, TML_TRUE);

// ****** APPLICATION LOOP *******
// The application is waiting now for
// incoming traffic is processed in 
// a different thread
// *******************************

// end of application
tml_Core_Close(&coreHandle);
~~~~

After the call to `tml_Core_Set_ListenerEnabled()` incoming traffic is processed by the handler function in different threads.

> If the handler function is not thread safe, the application may become unstable. For example accessing global data structures from multiple threads may lead to unpredictable results.

If the communication is no longer needed within the application, a call to `tml_Core_Close()` disposes all related objects and the TMLCore.

<a name="callTMLcommand"></a>
#### Call a remote procedure ####

Sending messages via TML is also accomplished using a TMLCore object. Applications already listening for incoming traffic don't need to instantiate another TMLCore but can send with the the present one. 

Two methods of message calls are available:
- **Synchronous calls**: The application is stopped until a reply or an error is received.
- **Asynchronous calls**: A callback is registered with a command to handle a reply. The application does not stop and wait after the call.

This example is creating a message object with `tml_Cmd_Create()` and assigns a command ID with `tml_Cmd_Header_SetCommand()`. From the previous example 4711 is used. To call the remote function a call to `tml_Send_SyncMessage()` sends the message and waits for a reply. If an error occurred on the wire an error code is added to the message header and is checked with `tml_Cmd_Header_GetError()`. If the error code is `TML_SUCCESS`, everything is OK. 

~~~~{.c}

TML_CORE_HANDLE coreHandle;
TML_COMMAND_HANDLE cmdMsg;

TML_INT32 cmdErr;
TML_INT32 eMsgLen;
TML_CTSTR *eMsg;

// create a TML core object
coreHandle = TML_HANDLE_TYPE_NULL;
tml_Core_Open(&coreHandle, 0);

cmdMsg = TML_HANDLE_TYPE_NULL;
tml_Cmd_Create (&cmdMsg);
tml_Cmd_Header_SetCommand (cmdMsg, (TML_COMMAND_ID_TYPE)4711);

// send message and wait for reply
tml_Send_SyncMessage(coreHandle, cmdMsg,
                   "urn:libtml:org:profile:example1",
                   "192.168.11.137", "44100", 6000);
// check for error
tml_Cmd_Header_GetError(cmdMsg, &cmdErr);
tml_Cmd_Header_GetErrorMessage(cmdMsg, &eMsg, &eMsgLen);

if (cmdErr != TML_SUCCESS) {
   fwprintf (stderr, L"Appl. spec. execution error message: \"%ls\"\n", errMsg);
}
sidex_Free_ReadString(eMsg);

// dispose the command object
tml_Cmd_Free(&cmdMsg);
~~~~

<a name="PassTMLParam"></a>
#### Passing parameters and results ####

The TML Messaging Suite handles all data with the SIDEX API. Calling the function `tml_Cmd_Acquire_Sidex_Handle()` returns a reference to a SIDEX_HANDLE representing a SIDEX document and locks the access to it for other threads. TML is multithreaded and the lock is required for synchronization. After accessing the data the lock has to be removed with `tml_Cmd_Release_Sidex_Handle()`.

A SIDEX document is organized in groups and keys, identified with strings. Unicode strings can be used to identify parameters in any language. A set of functions allows to add c-types directly to the document. 

> The functions `sidex_Variant_Write()` and `sidex_Variant_Read()` are used to add SIDEX variant values. Container types like lists, dictionaries and tables are available to combine a set of values and allow hieararchical structures. 

The following example shows how to add some parameters to a call and get the result. The interface function from the [first section](#createTMLInterface) has to be modified to do something meaningful. The sum of two float values are calculated and returned as a result.

~~~~{.c}
// calculate the sum of two float values
void FUNC_C_DECL cmd_handler_4711(TML_COMMAND_HANDLE msg, TML_POINTER data){
  SIDEX_HANDLE sDocument = SIDEX_HANDLE_TYPE_NULL; 
  double a1;
  double a2;

  tml_Cmd_Acquire_Sidex_Handle(msg, &sDocument);

  // read the values from the document
  sidex_Float_Read(sDocument,"Parameters","arg1",&a1);
  sidex_Float_Read(sDocument,"Parameters","arg2",&a2);

  // write the result to the document
  sidex_Float_Write(sDocument,"Result","sum",a1+a2);

  // IMPORTANT: unlock the document handle
  tml_Cmd_Release_Sidex_Handle(msg);
} 
~~~~

Calling the remote function and error handling is shown in the [previous section](#callTMLcommand). The code below shows how to extend the example to call the sum function with parameters.

~~~~{.c}
TML_COMMAND_HANDLE cmdMsg; // this example assumes the 
SIDEX_HANDLE sDocument = SIDEX_HANDLE_TYPE_NULL;
double s;

// ... after creating the cmdMsg with tml_Cmd_Create() and before sending
// add some parameters

  tml_Cmd_Acquire_Sidex_Handle(msg, &sDocument);

  // add the arguments for the sum
  sidex_Float_Write(sDocument,"Parameters","arg1",12);
  sidex_Float_Write(sDocument,"Parameters","arg2",30);

  // IMPORTANT: unlock the document handle
  tml_Cmd_Release_Sidex_Handle(msg);

// now send the command and handle errors (see previous example)

// access the result and print it
  tml_Cmd_Acquire_Sidex_Handle(msg, &sDocument);

  sidex_Float_Read(sDocument,"Result","sum",&s);
  wprintf (L"sum = %4.2f: \"%ls\"\n", s);
  
  // IMPORTANT: unlock the document handle
  tml_Cmd_Release_Sidex_Handle(msg);
~~~~
 

<a name="errHandling"></a>
## Error Handling ##

TML and SIDEX API functions return error codes. If no error was found `SIDEX_SUCCESS` or `TML_SUCCESS` is returned. Any value different from `SIDEX_SUCCESS` or `TML_SUCCESS` needs special attention. 
The example shows a simple error handling.

~~~~{.c}
int32_t err

err = tml_Core_Set_ListenerEnabled (coreHandle, TML_TRUE);
if (err != TML_SUCCESS){
	printf(TML error found":error %i\n", err); 
}
~~~~
