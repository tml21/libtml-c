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

#define VERSION "1.2.0"

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#ifdef LINUX
  #include <stdbool.h> 
#endif // LINUX
#include <sidexTests.h>
#include <tmlTests.h>

/**
 * Run a test depending of test number
 */
bool run_test(int no)
{
  bool bDone = true;
  switch (no){
    case 1:  bDone = sidexTest01(); // SIDEX1
             break;
    case 2:  bDone = sidexTest02(); // SIDEX2
             break;
    case 3:  reloadContent();// Reload
             break;
    case 4:  bDone = listenerTest01();// Listener
             break;
    case 5:  bDone = senderTest01();// sender
             break;
    case 6:  bDone = senderTest02();// sender(ConMgr)
             break;
    case 7:  bDone = sendAndListenTest01();// Sync SEND & LISTEN
             break;
    case 8:  bDone = sendAndListenTest02();// Async SEND & LISTEN
             break;
    case 9:  bDone = sendAndListenTest03();// Sync SEND & LISTEN (ConMgr)
             break;
    case 10: bDone = sendAndListenTest04();// Async SEND & LISTEN (ConMgr)
             break;
    case 11: bDone = validateConnection();// Validate (ConMgr)
             break;
    default: bDone = false;
             break;
  }

  return bDone;
}

/**
 * Main
 */
int main(int argc, char **argv)
{
  int  testno;
  bool bDone = false;

  printf ("** TML (The Missing Link) & SIDEX (Simple Data Exchange Format)\n");
  printf ("** regression tests: version=%s\n**\n", VERSION);
  printf ("** Copyright (C) 2015 wobe-systems GmbH\n**\n");

  if (argc == 2) {
    if (0 == strcmp("-help", argv[1])){
      printf ("TML/SIDEX Test:\n");
      printf ("==============:\n");
      printf ("Possible command line options\n");
      printf ("\"1\"     - perform SIDEX test01\n");
      printf ("\"2\"     - perform SIDEX test02\n");
      printf ("\"3\"     - reload SIDEX\n");
      printf ("\"4\"     - perform TML listener\n");
      printf ("\"5\"     - perform TML sender\n");
      printf ("\"6\"     - TML sender (with connection mgr)\n");
      printf ("\"7\"     - perform TML send & listen (synchron)\n");
      printf ("\"8\"     - perform TML send & listen (asynchron)\n");
      printf ("\"9\"     - perform TML send & listen (synchron with connection mgr)\n");
      printf ("\"10\"    - perform TML send & listen (asynchron with connection mgr)\n");
      printf ("\"11\"    - validate (connection mgr)\n");
      printf ("\"sidex\" - perform all SIDEX tests\n");
      printf ("\"tml\"   - perform all TML tests\n");
      printf ("\"all\"   - perform all SIDEX and TML tests\n");
      printf ("\"-help\" - prints this text\n");
      bDone = true;
    }
    else{
      if (0 == strcmp("all", argv[1])){
        printf ("Perform SIDEX / TML tests\n");
        for (testno = 1; testno <= 4; ++testno){
          bDone = run_test(testno);
        }
      }
      else{
        if (0 == strcmp("sidex", argv[1])){
          printf ("Perform SIDEX tests\n");
          for (testno = 1; testno <= 2; ++testno){
            bDone = run_test(testno);
          }
        }
        else{
          if (0 == strcmp("tml", argv[1])){
            printf ("Perform TML tests\n");
            for (testno = 3; testno <= 4; ++testno){
              bDone = run_test(testno);
            }
          }
          else{
            if(sscanf(argv[1], "%d", &testno) == EOF) {
              fprintf(stderr, "WARNING: Incorrect test number\n");
              return -1;
            }
            bDone = run_test(testno);
          }
        }
      }
    }
  }
  else {
    char sInput[256];
    do{
      printf ("TML/SIDEX Test:\n");
      printf ("==============:\n");

      printf ("SIDEX test01                                     - enter \"1<enter>\"\n");
      printf ("SIDEX test02                                     - enter \"2<enter>\"\n");
      printf ("Reload SIDEX                                     - enter \"3<enter>\"\n");
      printf ("TML listener                                     - enter \"4<enter>\"\n");
      printf ("TML sender                                       - enter \"5<enter>\"\n");
      printf ("TML sender (with connection mgr)                 - enter \"6<enter>\"\n");
      printf ("TML send & listen (synchron)                     - enter \"7<enter>\"\n");
      printf ("TML send & listen (asynchron)                    - enter \"8<enter>\"\n");
      printf ("TML send & listen (synchron with connection mgr) - enter \"9<enter>\"\n");
      printf ("TML send & listen (asynchron with connection mgr)- enter \"10<enter>\"\n");
      printf ("validate (connection mgr)                        - enter \"11<enter>\"\n");
      printf ("Exit demo                        - enter \"exit<enter>\"\n");
      int i = scanf("%s", sInput);
      if (1 <= i){
        if (strcmp(sInput, (char*) "1") == 0)
          bDone = run_test(1);
        if (strcmp(sInput, (char*) "2") == 0)
          bDone = run_test(2);
        if (strcmp(sInput, (char*) "3") == 0)
          bDone = run_test(3);
        if (strcmp(sInput, (char*) "4") == 0)
          bDone = run_test(4);
        if (strcmp(sInput, (char*) "5") == 0)
          bDone = run_test(5);
        if (strcmp(sInput, (char*) "6") == 0)
          bDone = run_test(6);
        if (strcmp(sInput, (char*) "7") == 0)
          bDone = run_test(7);
        if (strcmp(sInput, (char*) "8") == 0)
          bDone = run_test(8);
        if (strcmp(sInput, (char*) "9") == 0)
          bDone = run_test(9);
        if (strcmp(sInput, (char*) "10") == 0)
          bDone = run_test(10);
        if (strcmp(sInput, (char*) "11") == 0)
          bDone = run_test(11);
        if (strcmp(sInput, (char*) "exit") == 0)
          bDone = true;
      }
     }while (!bDone); 
  }
  closePossibleExistingCoreHandle();
  if (!bDone)
    printf ("WARNING: Unknown test number\n");
}
