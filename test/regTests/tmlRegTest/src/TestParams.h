/* 
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2016 wobe-systems GmbH
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
 *  Homepage:
 *    http://www.libtml.org
 *
 *  For professional support contact us:
 *
 *    wobe-systems GmbH
 *    support@libtml.org
 *
 * Contributors:
 *    wobe-systems GmbH
 */
#ifndef TESTPARAMS_H
#define TESTPARAMS_H

#include "tmlrt_Utils.h"

/*
  Used abbreviations:
  -------------------
  S_  = String
  TN  = Test params document name
  TG  = Group
  TP  = Parameter
  TD  = Default value
  CRT = Certificate
  FP  = Fingerprint
*/

// document name
extern SIDEX_TCHAR* S_TN_TESTPARAMS;

// group names
extern SIDEX_TCHAR* S_TG_NETWORK;
extern SIDEX_TCHAR* S_TG_TEST;
extern SIDEX_TCHAR* S_TG_TLS;

// parameter names
extern SIDEX_TCHAR* S_TP_CARDS;
extern SIDEX_TCHAR* S_TP_FIRSTPORTNUMBER;

extern SIDEX_TCHAR* S_TP_LOOPCOUNT;

extern SIDEX_TCHAR* S_TP_ISREPEATER;
extern SIDEX_TCHAR* S_TP_MAXIDLETIME;
extern SIDEX_TCHAR* S_TP_TLSREPEATERHOST;
extern SIDEX_TCHAR* S_TP_TLSREPEATERPORT;

extern SIDEX_TCHAR* S_TP_LOCATION;
extern SIDEX_TCHAR* S_TP_CRT;
extern SIDEX_TCHAR* S_TP_CRT2;
extern SIDEX_TCHAR* S_TP_KEY;
extern SIDEX_TCHAR* S_TP_KEY2;
extern SIDEX_TCHAR* S_TP_FP_SHA1;
extern SIDEX_TCHAR* S_TP_FP_MD5;

// default values
extern SIDEX_TCHAR* S_TD_127_0_0_1;

extern SIDEX_TCHAR* S_TD_TLS_LOCATION;
extern SIDEX_TCHAR* S_TD_TLS_CRT;
extern SIDEX_TCHAR* S_TD_TLS_CRT2;
extern SIDEX_TCHAR* S_TD_TLS_KEY;
extern SIDEX_TCHAR* S_TD_TLS_KEY2;
extern SIDEX_TCHAR* S_TD_TLS_FP_SHA1;
extern SIDEX_TCHAR* S_TD_TLS_FP_MD5;

class CTestParams
{
protected:
  SIDEX_HANDLE m_sdxParams;
  SIDEX_TCHAR* m_ParamsFileName;

  bool ensureDefaultParams();

public:
  CTestParams(SIDEX_TCHAR* testParamsFileName = NULL);
  ~CTestParams();

  bool         hasParams();
  SIDEX_TCHAR* getParamsFileName();               // returned string is a borrowed reference - do not free!

  int          getNetworkCardCount();
  SIDEX_TCHAR* getNetworkCard(SIDEX_INT32 index); // returned string has to be freed after use

  int          getFirstPortNumber();

  int          getTestLoopCount();

  bool         isActingAsRepeater();
  int          getMaxIdleTime();
  SIDEX_TCHAR* getTlsRepeaterHost();              // returned string has to be freed after use
  int          getTlsRepeaterPort();

  SIDEX_TCHAR* getTlsLocation();                  // returned string has to be freed after use
  SIDEX_TCHAR* getTlsCrt();                       // returned string has to be freed after use
  SIDEX_TCHAR* getTlsCrt2();                      // returned string has to be freed after use
  SIDEX_TCHAR* getTlsKey();                       // returned string has to be freed after use
  SIDEX_TCHAR* getTlsKey2();                      // returned string has to be freed after use
  SIDEX_TCHAR* getTlsFingerprintSHA1();           // returned string has to be freed after use
  SIDEX_TCHAR* getTlsFingerprintMD5();            // returned string has to be freed after use
};

extern CTestParams* TestParams;

#endif //TESTPARAMS_H
