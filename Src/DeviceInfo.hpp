#ifndef __ARSLEXIS_DEVICE_INFO_HPP__
#define __ARSLEXIS_DEVICE_INFO_HPP__

#include <BaseTypes.hpp>

#if defined(_PALM_OS)
bool isTreo600();
#endif

ArsLexis::String deviceInfoToken();

#endif
