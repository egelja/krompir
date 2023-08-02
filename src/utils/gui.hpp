#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include <string>

namespace gui {
namespace utils {

inline constexpr std::string
get_wx_version_string()
{
    return (                                 //
        wxSTRINGIZE(wxMAJOR_VERSION)         //
        "." wxSTRINGIZE(wxMINOR_VERSION)     //
        "." wxSTRINGIZE(wxRELEASE_NUMBER)    //
#ifdef wxSUBRELEASE_NUMBER                   //
        "." wxSTRINGIZE(wxSUBRELEASE_NUMBER) //
#endif                                       //
    );
}

} // namespace utils
} // namespace gui
