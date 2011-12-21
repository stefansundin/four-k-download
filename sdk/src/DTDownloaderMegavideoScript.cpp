/// Copyright 2010-2012 4kdownload.com (developers@4kdownload.com)
/**
    This file is part of 4k Download.

    4k Download is free software; you can redistribute it and/or modify
    it under the terms of the one of two licenses as you choose:

    1. GNU GENERAL PUBLIC LICENSE Version 3
    (See file COPYING.GPLv3 for details).

    2. 4k Download Commercial License
    (Send request to developers@4kdownload.com for details).
   
*/




#include <openmedia/DTHeaders.h>


#include <openmedia/DTScript.h>
#include <string>
#include "DTServiceScript.h"
#include <iostream>

namespace openmedia { namespace downloader { namespace megavideo {

static const char * DecryptProgramMegavideo = "(function decrypt(_args) \n\
{\n\
str = _args.un;\n\
key1 = _args.key1;\n\
key2 = _args.key2;\n\
var _loc1 = []; for (var _loc3 = 0; _loc3 < str.length; ++_loc3)\n\
{\n\
switch (str.charAt(_loc3))\n\
{\n\
case \"0\": { _loc1.push(\"0000\"); break; }\n\
case \"1\": { _loc1.push(\"0001\"); break; }\n\
case \"2\": { _loc1.push(\"0010\"); break; }\n\
case \"3\": { _loc1.push(\"0011\"); break; }\n\
case \"4\": { _loc1.push(\"0100\"); break; }\n\
case \"5\": { _loc1.push(\"0101\"); break; }\n\
case \"6\": { _loc1.push(\"0110\"); break; }\n\
case \"7\": { _loc1.push(\"0111\"); break; }\n\
case \"8\": { _loc1.push(\"1000\"); break; }\n\
case \"9\": { _loc1.push(\"1001\"); break; }\n\
case \"a\": { _loc1.push(\"1010\"); break; }\n\
case \"b\": { _loc1.push(\"1011\"); break; }\n\
case \"c\": { _loc1.push(\"1100\"); break; }\n\
case \"d\": { _loc1.push(\"1101\"); break; }\n\
case \"e\": { _loc1.push(\"1110\"); break; }\n\
case \"f\": { _loc1.push(\"1111\"); break; }\n\
} // End of switch\n\
} // end of for\n\
_loc1 = _loc1.join(\"\").split(\"\");\n\
var _loc6 = [];\n\
for (var _loc3 = 0; _loc3 < 384; ++_loc3)\n\
{\n\
key1 = (key1 * 11 + 77213) % 81371;\n\
key2 = (key2 * 17 + 92717) % 192811;\n\
_loc6[_loc3] = (key1 + key2) % 128;\n\
} // end of for\n\
for (var _loc3 = 256; _loc3 >= 0; --_loc3)\n\
{\n\
var _loc5 = _loc6[_loc3];\n\
var _loc4 = _loc3 % 128;\n\
var _loc8 = _loc1[_loc5];\n\
_loc1[_loc5] = _loc1[_loc4];\n\
_loc1[_loc4] = _loc8;\n\
} // end of for\n\
for (var _loc3 = 0; _loc3 < 128; ++_loc3)\n\
{\n\
_loc1[_loc3] = _loc1[_loc3] ^ _loc6[_loc3 + 256] & 1;\n\
} // end of for\n\
var _loc12 = _loc1.join(\"\");\n\
var _loc7 = [];\n\
for (var _loc3 = 0; _loc3 < _loc12.length; _loc3 = _loc3 + 4)\n\
{\n\
var _loc9 = _loc12.substr(_loc3, 4);\n\
_loc7.push(_loc9);\n\
} // end of for\n\
var _loc2 = [];\n\
for (var _loc3 = 0; _loc3 < _loc7.length; ++_loc3)\n\
{\n\
switch (_loc7[_loc3])\n\
{\n\
case \"0000\": { _loc2.push(\"0\"); break; }\n\
case \"0001\": { _loc2.push(\"1\"); break; }\n\
case \"0010\": { _loc2.push(\"2\"); break; }\n\
case \"0011\": { _loc2.push(\"3\"); break; }\n\
case \"0100\": { _loc2.push(\"4\"); break; }\n\
case \"0101\": { _loc2.push(\"5\"); break; }\n\
case \"0110\": { _loc2.push(\"6\"); break; }\n\
case \"0111\": { _loc2.push(\"7\"); break; }\n\
case \"1000\": { _loc2.push(\"8\"); break; }\n\
case \"1001\": { _loc2.push(\"9\"); break; }\n\
case \"1010\": { _loc2.push(\"a\"); break; }\n\
case \"1011\": { _loc2.push(\"b\"); break; }\n\
case \"1100\": { _loc2.push(\"c\"); break; }\n\
case \"1101\": { _loc2.push(\"d\"); break; }\n\
case \"1110\": { _loc2.push(\"e\"); break; }\n\
case \"1111\": { _loc2.push(\"f\"); break; }\n\
} // End of switch\n\
} // end of for\n\
return (_loc2.join(\"\"));\n\
})";

std::string DecryptTokenUrl(const std::string & Un, const std::string & Key1, const std::string & Key2)
{
    std::string res = "";
    try 
    {
        const std::string DecryptProgramMegavideoLoaded = get_service_script(mediaSiteMegavideo, 1);
        const std::string Script = (DecryptProgramMegavideoLoaded.size()) ? DecryptProgramMegavideoLoaded : DecryptProgramMegavideo;

        ::openmedia::script::script_ptr script = ::openmedia::script::create(::openmedia::script::scriptQt, Script);

        typedef ::openmedia::script::string_string_pair ssp;

        ::openmedia::script::params params;

        params.push_back( ssp("un", Un) );
        params.push_back( ssp("key1", Key1) );
        params.push_back( ssp("key2", Key2) ); 
        res = script->execute("decrypt", params); 
    }
    catch(...)
    {

    }

    return res;
}

} } }
