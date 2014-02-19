# Copyright (C) 2010-2012 DarkCore <http://www.darkpeninsula.eu/>
# Copyright (C) 2010-2012 Project SkyFire <http://www.projectskyfire.org/>
# Copyright (C) 2008-2012 Trinity <http://www.trinitycore.org/>
# Copyright (C) 2005-2012 MaNGOS <http://www.getmangos.com/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#
# Force out-of-source build
#

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" BUILDING_IN_SOURCE)

if( BUILDING_IN_SOURCE )
  message(FATAL_ERROR "
    This project requires an out of source build. Remove the file 'CMakeCache.txt'
    found in this directory before continuing, create a separate build directory
    and run 'cmake path_to_project [options]' from there.
  ")
endif()
