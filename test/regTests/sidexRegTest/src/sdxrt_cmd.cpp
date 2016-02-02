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

#ifdef LINUX
  #include <wchar.h>
  #include <stdbool.h>
  #include <string.h>
#else
  #include <stdio.h>
  #include <tchar.h>
#endif
#include <iostream>
using namespace std;
#include "sdxrt_test_int.h"
#include "sdxrt_test_float.h"
#include "sdxrt_test_string.h"
#include "sdxrt_test_bool.h"
#include "sdxrt_test_none.h"
#include "sdxrt_test_doc.h"
#include "sdxrt_test_datetime.h"
#include "sdxrt_test_dict.h"
#include "sdxrt_test_string.h"
#include "sdxrt_test_binary.h"
#include "sdxrt_test_list.h"
#include "sdxrt_test_table.h"

enum Test {no_test=0, test_int=1, test_float=2, test_bool=3, test_none=4, test_doc=5, test_date=6, test_dict=7, test_string=8, test_bin=9, test_list=10, test_table=11};

/** @brief goes through all tests without stopping if a subtest fails
* @param bool stop: returns after a subtest fails
*
* @returns int : 0 if everything went okay, -1 if a failure/error occurred in the tests.
*/
int option_all(bool stop) {
    int returnVal = 0;

    if(test_sidex_int(0,stop)) {
        cout << "test_sidex_int OK " << endl;
    } else {
        cout << "test_sidex_int FAILED" << endl;
        returnVal = -1;
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_float(0,stop)) {
            cout << "test_sidex_float OK " << endl;
        } else {
            cout << "test_sidex_float FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_bool(0,stop)) {
            cout << "test_sidex_bool OK " << endl;
        } else {
            cout << "test_sidex_bool FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_none(0,stop)) {
            cout << "test_sidex_none OK " << endl;
        } else {
            cout << "test_sidex_none FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_doc(0,stop)) {
            cout << "test_sidex_doc OK " << endl;
        } else {
            cout << "test_sidex_doc FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_date(0,stop)) {
            cout << "test_sidex_date OK " << endl;
        } else {
            cout << "test_sidex_date FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_dict(0,stop)) {
            cout << "test_sidex_dict OK " << endl;
        } else {
            cout << "test_sidex_dict FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_string(0,stop)) {
            cout << "test_sidex_string OK " << endl;
        } else {
            cout << "test_sidex_string FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_binary(0,stop)) {
            cout << "test_sidex_bin OK " << endl;
        } else {
            cout << "test_sidex_bin FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_list(0,stop)) {
            cout << "test_sidex_list OK " << endl;
        } else {
            cout << "test_sidex_list FAILED" << endl;
            returnVal = -1;
        }
    }
    if(!stop && returnVal != -1) {
        if(test_sidex_table(0,stop)) {
            cout << "test_sidex_table OK " << endl;
        } else {
            cout << "test_sidex_table FAILED" << endl;
            returnVal = -1;
        }
    }

    //return value
    return returnVal;
}


/** @brief executes a specific test or subtest of a test (if any subtest was specified)
* @param bool stop: if true, returns after a subtest failed- otherwise all subtests are executed 
* @param int test: enum of a specific test, @see enum Test
* @param int subtest: number of a subtest, if it is 0- no subtest was specified and all subtests are executed
*
* @returns int : 0 if everything went okay, -1 if a failure/error occurred in the tests.
*/
int option_specific_test(bool stop, int test, int subtest) {
    int returnVal = 0;
    switch(test) {
    case 0:
        returnVal = -1;
    case 1:
        if(test_sidex_int(subtest, stop)) {
            cout << "test_sidex_int OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_int FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 2:
        if(test_sidex_float(subtest, stop)) {
            cout << "test_sidex_float OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_float FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 3:
        if(test_sidex_bool(subtest, stop)) {
            cout << "test_sidex_bool OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_bool FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 4:
        if(test_sidex_none(subtest, stop)) {
            cout << "test_sidex_none OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_none FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 5:
        if(test_sidex_doc(subtest, stop)) {
            cout << "test_sidex_doc OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_doc FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 6:
        if(test_sidex_date(subtest, stop)) {
            cout << "test_sidex_date OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_date FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 7:
        if(test_sidex_dict(subtest, stop)) {
            cout << "test_sidex_dict OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_dict FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 8:
        if(test_sidex_string(subtest, stop)) {
            cout << "test_sidex_string OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_string FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 9:
        if(test_sidex_binary(subtest, stop)) {
            cout << "test_sidex_binary OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_binary FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 10:
        if(test_sidex_list(subtest, stop)) {
            cout << "test_sidex_list OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_list FAILED" << endl;
            returnVal = -1;
            break;
        }
    case 11:
        if(test_sidex_table(subtest, stop)) {
            cout << "test_sidex_table OK " << endl;
            returnVal = 0;
            break;
        } else {
            cout << "test_sidex_table FAILED" << endl;
            returnVal = -1;
            break;
        }
    default:
        returnVal = -1;
    }
    return returnVal;
}

/** @brief checks which test 'test' is and returns appropriate enum
* @param char* test: command line argument given by user
* @returns int : enum Test, 1 until 11 depending on input of user
*/
int return_specific_test(char* test) {
    int returnValue = -1;
    if(strcmp(test, "test_int") == 0) {
        returnValue = 1;
    } else if (strcmp(test, "test_float") == 0) {
        returnValue = 2;
    } else if (strcmp(test, "test_bool") == 0) {
        returnValue = 3;
    } else if (strcmp(test, "test_none") == 0) {
        returnValue = 4;
    } else if (strcmp(test, "test_doc") == 0) {
        returnValue = 5;
    } else if (strcmp(test, "test_date") == 0) {
        returnValue = 6;
    } else if (strcmp(test, "test_dict") == 0) {
        returnValue = 7;
    } else if (strcmp(test, "test_string") == 0) {
        returnValue = 8;
    } else if (strcmp(test, "test_bin") == 0) {
        returnValue = 9;
    } else if (strcmp(test, "test_list") == 0) {
        returnValue = 10;
    } else if (strcmp(test, "test_table") == 0) {
        returnValue = 11;
    }
    
    return returnValue;
}

/** helper func for seARCHING MEMORY LEAKS - not included right now
*/
bool memLeaks(int test, int index) {
    switch(test) {
    case 1:
        for (int i = 0; i < index; i++) {
            test_sidex_int(0, false);
        }
        break;
    case 2:
        for (int i = 0; i < index; i++) {
            test_sidex_float(0, false);
        }
        break;
    case 3:
        for (int i = 0; i < index; i++) {
            test_sidex_bool(0, false);
        }
        break;
    case 4:
        for (int i = 0; i < index; i++) {
            test_sidex_none(0, false);
        }
        break;
    case 5:
        for (int i = 0; i < index; i++) {
            test_sidex_doc(0, false);
        }
        break;
    case 6:
        for (int i = 0; i < index; i++) {
            test_sidex_date(0, false);
        }
        break;
    case 7:
        for (int i = 0; i < index; i++) {
            test_sidex_dict(0, false);
        }
        break;
    case 8:
        for (int i = 0; i < index; i++) {
            test_sidex_string(0, false);
        }
        break;
    case 9:
        for (int i = 0; i < index; i++) {
            test_sidex_binary(0, false);
        }
        break;
    case 10:
        for (int i = 0; i < index; i++) {
            test_sidex_list(0, false);
        }
        break;
    case 11:
        for (int i = 0; i < index; i++) {
            test_sidex_table(0, false);
        }
        break;
    }
    return true;
}

/** @brief Main function, accepts command line parameters
* @param int argc : contains amount of arguments in argv
* @param char* argv[] : contains input from user
* @returns int : 0 if everythign went okay, -1 if a failure/error occurred or the options of the user were not recognized
*/
#ifdef LINUX
    int main(int argc, char* argv[])
#else //LINUX
    int _tmain(int argc, char* argv[])
#endif
{ 
    //command line arguments
    if (argc == 4) {    //three parameters / options were given
        char* opt1 = argv[1];   //has to be <specific test>
        int test = return_specific_test(argv[1]);   //which one of the <specific tests> is it
        if(test == -1) {
            //not recognized option
            cout << "Option not recognized. Restart program with option help for insructions" << endl;
            return -1;
        }
        int opt2 = atof(argv[2]);   //has to be number of subtest
        char* opt3 = argv[3];       //has to be go or stop
        if(strcmp(opt3, "go") == 0) {
            return option_specific_test(false, test, opt2);
        } else if (strcmp(opt3, "stop") == 0) {
             return option_specific_test(true, test, opt2);
        } else {
            //not recognized option
            cout << "Option not recognized. Restart program with option help for instructions" << endl;
            return -1;
        }
    } else if (argc == 3) { //two parameters / options were given
        char* opt1 = argv[1];   //can be 'all' or a <specific test>
        char* opt2 = argv[2];   //can be either 'go', 'stop' or a number of a subtest
        if(strcmp(opt1,"all") == 0) {
            if(strcmp(opt2, "go") == 0) {
                return option_all(false);
            } else if(strcmp(opt2, "stop") == 0) {
                return option_all(true);
            } else {
            //not recognized option
            cout << "Option not recognized. Restart program with option help for instructions" << endl;
            return -1;
            }
        }
        int test = return_specific_test(opt1);
        if(test != -1) {
            if(strcmp(opt2, "go") == 0) {
                return option_specific_test(false,test,0);
            } else if(strcmp(opt2, "stop") == 0) {
                return option_specific_test(true,test,0);
            }
            //otherwise opt2 has to be a number
            int subtest = atof(opt2);
            return option_specific_test(false,test,subtest);
        } else {
            //not recognized option
            cout << "Option not recognized. Restart program with option help for instructions" << endl;
            return -1;
        }
    } else if (argc == 2) { //just one parameter / option was given
        char* opt1 = argv[1];
        // option help
        if(strcmp(opt1,"help") == 0 || strcmp(opt1, "Help") == 0) {
            //output with help options
            cout << "Option help: execute the program with or without options:" << endl;
            cout << "+no input: program executes all tests and does not stop if one subtest fails" << endl;
            cout << "+option 'all': program executes all tests and does not stop if one subtest fails" << endl;
            cout << "+option 'go': program executes all tests and does not stop if one subtest fails" << endl;
            cout << "+option 'stop': program executes all tests and stops if one subtest fails" << endl;
            cout << "+option 'all go': program executes all tests and does not stop if one subtest fails" << endl;
            cout << "+option 'all stop': program executes all tests and stops if one subtest fails" << endl;
            cout << "+option '<specific test> [number]': program executes the specific test, if the optional number was given, only that one subtest, and does not stop if one subtest fails" << endl;
            cout << "+option '<specific test> [number] go': same as without go. Program executes the specific test, if the optional number was given, only that one subtest, and does not stop if one subtest fails" << endl;
            cout << "+option '<specific test> [number] stop': program executes the specific test, if the optional number was given, only that one subtest, and stops if one subtest fails" << endl;
            cout << "+testnames: test_int : <1-8> | test_float : <1-7> | test_bool : <1-7> | test_none : <1-4> | test_doc : <1-7> | test_dict : <1-11>" << endl;
            cout << "test_string : <1-9> | test_bin : <1-7> | test_list : <1-11> | test_table : <1-17>" << endl;
            return 0;
        } else if (strcmp(opt1, "all") == 0) {
            //execute all tests and don't stop if a subtest fails
            return option_all(false);
        } else if (strcmp(opt1, "stop") == 0) {
            //execute all tests and stop if a subtest fails
            return option_all(true);
        } else if (strcmp(opt1, "go") == 0) {
            //execute all tests and don't stop if a subtest fails
            return option_all(false);
        } else {
            int test = return_specific_test(argv[1]);
            if (test > 0) {
                return option_specific_test(false, test, 0);
            } else {
                //not recognized option
                cout << "Option not recognized. Restart program with option help for instructions" << endl;
                return -1;
            }
        }
    } else if (argc == 1) {
        //execute all tests and don't stop if a subtest fails
        return option_all(false);
    } else {
        cout << "Too much input. Restart program with option help for instructions" << endl;
        return -1;
    }
}
