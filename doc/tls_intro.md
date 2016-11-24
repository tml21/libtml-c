# Securing a TML Connection (or How to use the TLS profile) {#tls_intro}

TML is based on the peer to peer standard protocol BEEP (Blocks Extensible Exchange Protocol), defined in RFC3080 and RFC3081.

The main advantage the BEEP protocol has is that it solves many common problems that the network protocol designer will have to face while designing an application protocol. Securing a connection to avoid other parties to access data exchanged by BEEP peers is one of them.

The idea behind the TLS profile is to enable user level applications to activate the TLS profile for a given connection. Using the libTml library API has no difference about using a connection that is secured from one that is not.


> All examples in this section contain only code lines to explain a special aspect of the API. Important things like error handling are left out in favor of better readability.


### Common scenario for a secured connection TML client application is: ###

Create a connection to a remote peer 

~~~~{.c}
  #define DESTINATION_NETWORK_BINDING (char*) "10.11.12.131:44100"

  TML_CORE_HANDLE       coreHandle       = TML_HANDLE_TYPE_NULL;
  TML_CONNECTION_HANDLE connectionHandle = TML_HANDLE_TYPE_NULL;

  // Open an instance of a TMLCore 
  iErr = tml_Core_Open(&coreHandle, 0);

  // Create a new connection
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Connect(coreHandle, DESTINATION_NETWORK_BINDING, &connectionHandle);
~~~~

Secure the connection already created

~~~~{.c}
  TML_BOOL bEncrypted = TML_FALSE;
  
  if (TML_SUCCESS == iErr)
    iErr = tml_Tls_Connection_StartNegotiation(connectionHandle, TML_FALSE, &bEncrypted);
~~~~

Then use the connection, as usual, sending command / data over it

~~~~{.c}
  #define IO_PROFILE (char*) "http://libtml.org/profiles/plain_profile"

  TML_COMMAND_HANDLE cmdMsg = TML_HANDLE_TYPE_NULL;

  // Create a command handle:
  iErr = tml_Cmd_Create (&cmdMsg);
  if (TML_SUCCESS == iErr)
    iErr = tml_Cmd_Header_SetCommand (cmdMsg, (TML_COMMAND_ID_TYPE)4711);

  // Send command 4711
  if (TML_SUCCESS == iErr)
    iErr = tml_Connection_SendSync(connectionHandle, IO_PROFILE, cmdMsg, 6000);
~~~~

From the application programmer's point of view there is no difference from using a connection secured to one that is not.

### On the TLS profile TML listener side: ###

A TML_ON_ACCEPT_TLS_REQUEST_CB_FUNC() handler should be defined to control if an incoming TLS requests is accepted 
~~~~{.c}
TML_BOOL check_and_accept_tls_request (SIDEX_VARIANT vServerName){

  // perform some special operations against the serverName value,
  // return TML_FALSE to deny the TLS request, or TML_TRUE to allow it
    
  return TML_TRUE;
}
~~~~

A TML_ON_CERTIFICATE_FILE_LOCATION_CB_FUNC() handler should be defined to control which certificate file is to be used
~~~~{.c}
SIDEX_VARIANT certificate_file_location (SIDEX_VARIANT vServerName)
{
  SIDEX_VARIANT var;

  // perform some special operation to choose which 
  // certificate file to be used, then return it

  sidex_Variant_New_String((char*)"TmlTestTls.crt", &var);
  return var; 
}
~~~~

A TML_ON_CERTIFICATE_PRIVATE_KEY_LOCATION_CB_FUNC() handler should be defined to control which private key file is to be used
~~~~{.c}
SIDEX_VARIANT private_key_file_location (SIDEX_VARIANT vServerName)
{
  SIDEX_VARIANT var;

  // perform some special operation to choose which 
  // private key file to be used, then return it

  sidex_Variant_New_String((char*) "TmlTestTls.key", &var);
  return var; 
}
~~~~

Now use previous handlers to configure how TLS profile is supported for the current TML Library instance

~~~~{.c}
    TML_CORE_HANDLE coreHandle = TML_HANDLE_TYPE_NULL;

    // Open an instance of a TMLCore 
    iErr = tml_Core_Open(&coreHandle, 0);

    TML_BOOL bAccepted = TML_FALSE;

    // Allow to configure if the provided tml core will accept TLS incoming connections
    if (TML_SUCCESS == iErr)
      iErr = tml_Tls_Core_AcceptNegotiation(coreHandle, check_and_accept_tls_request, certificate_file_location, private_key_file_location, &bAccepted);
~~~~

Listener initialisation

~~~~{.c}
    #define IO_PROFILE                      (char*) "http://libtml.org/profiles/plain_profile"
    #define LISTENER_NETWORK_INTERFACE_ADDR (char*) "0.0.0.0:44100" 

    TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;

    // Register profile (unique profile name) for command dispatch action
    iErr = tml_Profile_Register (coreHandle, IO_PROFILE);

    // Create a listener
    if (TML_SUCCESS == iErr)
      iErr = tml_Core_Listener_Create(coreHandle, LISTENER_NETWORK_INTERFACE_ADDR, &listenerHandle);

    // Enable the listener
    if (TML_SUCCESS == iErr)
      iErr = tml_Listener_Set_Enabled(listenerHandle, TML_TRUE);

~~~~

Now your listener is prepared to receive incoming connections and enable TLS on them.


----------

## How to create a test certificate and a private key to be used by the TLS profile ##

Now we have successfully configured the TLS profile.For listener side we need to create a certificate/private key pair. Currently libTML TLS support is built using OpenSSL [(http://www.openssl.org)](http://www.openssl.org). This SSL toolkit comes with some tools to create such files. Here is an example to create a test certificate and a private key that can be used for testing purposes.

Create a 1024 bits private key using:

    openssl genrsa 1024 > TmlTestTls.key

Now create the public certificate reusing previously created key as follows:


    openssl req -new -x509 -nodes -sha1 -days 3650 -key TmlTestTls.key > TmlTestTls.crt

Once finished, you can check certificate data using:


    openssl x509 -noout -fingerprint -text < TmlTestTls.crt


