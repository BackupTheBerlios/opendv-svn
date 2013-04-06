////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceDef.h                                               //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////

#ifndef WINSERVICEDEF_H
#define WINSERVICEDEF_H

#include <wx/dlimpexp.h>

// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_WINSERVICE in each class declaration:
//          class WXDLLIMPEXP_WINSERVICE myCompClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_WINSERVICE in the declaration of each global function:
//          WXDLLIMPEXP_WINSERVICE int myGlobalFunc();
//

#ifdef WXMAKINGDLL_WINSERVICE
    #define WXDLLIMPEXP_WINSERVICE                  WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WINSERVICE                  WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_WINSERVICE
#endif

class WXDLLIMPEXP_WINSERVICE  CWinServiceEvent;
class WXDLLIMPEXP_WINSERVICE  CWinServiceStatus;
class WXDLLIMPEXP_WINSERVICE  CWinServiceThread;
class WXDLLIMPEXP_WINSERVICE  CWinServiceApp;

#endif
