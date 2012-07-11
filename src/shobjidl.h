/*!
 * \file shobjidl.h
 * \author Copyright &copy; Microsoft Corporation
 * \author Extracted and documented by Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 * \version Microsoft Windows SDK v7.0A
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
 *
 * \brief This file is a minimal required part of \c %shobjidl.h from <b>Microsoft Windows SDK v7.0A</b>.
 *
 *  This file is an extract of \c %shobjidl.h from <b>Microsoft Windows SDK v7.0A</b>.
 *  It is needed to support <b>Windows 7 Taskbar Extensions</b> (namely, <em>Progress Bars</em>)
 *  and contains minimal required definition of \c ITaskbarList3 interface.
 *
 *  Tested with \b MSVC and \b mingw32. Doesn't need <b>Windows 7 SDK</b> itself.
 *
 * \note Don't forget to link to \b ole32 library when using this file (usually \c -lole32 linker flag is enough).
 */

#ifndef _SHOBJIDL_H
#define _SHOBJIDL_H

#define INITGUID
#include <windows.h>

#ifndef DOXYGEN_EXCLUDE
DEFINE_GUID(CLSID_TaskbarList, 0x56fdf344,0xfd6d,0x11d0,0x95,0x8a,0x00,0x60,0x97,0xc9,0xa0,0x90);
DEFINE_GUID(IID_ITaskbarList3, 0xea1afb91,0x9e28,0x4b86,0x90,0xe9,0x9e,0x9f,0x8a,0x5e,0xef,0xaf);
#endif

enum TBPFLAG {
	/*! Stops displaying progress and returns the button to its normal state. Call this method with this flag to dismiss the progress bar when the operation is complete or cancelled. */
	TBPF_NOPROGRESS		= 0,

	/*! The progress indicator does not grow in size, but cycles repeatedly along the length of the taskbar button. This indicates activity without specifying what proportion of the progress is complete. Progress is taking place, but there is no prediction as to how long the operation will take. */
	TBPF_INDETERMINATE	= 0x1,

	/*! The progress indicator grows in size from left to right in proportion to the estimated amount of the operation completed. This is a determinate progress indicator; a prediction is being made as to the duration of the operation. */
	TBPF_NORMAL			= 0x2,

	/*! The progress indicator turns red to show that an error has occurred in one of the windows that is broadcasting progress. This is a determinate state. If the progress indicator is in the indeterminate state, it switches to a red determinate display of a generic percentage not indicative of actual progress. */
	TBPF_ERROR			= 0x4,

	/*! The progress indicator turns yellow to show that progress is currently stopped in one of the windows but can be resumed by the user. No error condition exists and nothing is preventing the progress from continuing. This is a determinate state. If the progress indicator is in the indeterminate state, it switches to a yellow determinate display of a generic percentage not indicative of actual progress. */
	TBPF_PAUSED			= 0x8
};

#ifndef DOXYGEN_EXCLUDE
DECLARE_INTERFACE_(ITaskbarList3, IUnknown)
{
	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppv) PURE;
	STDMETHOD_(ULONG, AddRef) (THIS) PURE;
	STDMETHOD_(ULONG, Release) (THIS) PURE;
	/*** ITaskbarList specific methods ***/
	STDMETHOD(HrInit) (THIS) PURE;
	STDMETHOD(AddTab) (THIS_ HWND hwnd) PURE;
	STDMETHOD(DeleteTab) (THIS_ HWND hwnd) PURE;
	STDMETHOD(ActivateTab) (THIS_ HWND hwnd) PURE;
	STDMETHOD(SetActiveAlt) (THIS_ HWND hwnd) PURE;
	/*** ITaskbarList2 specific methods ***/
	STDMETHOD(MarkFullscreenWindow) (THIS_ HWND hwnd, BOOL fFullscreen) PURE;
	/*** ITaskbarList3 specific methods (only used ones) ***/
	STDMETHOD(SetProgressValue) (THIS_ HWND hwnd, ULONGLONG ullCompleted, ULONGLONG ullTotal) PURE;
	STDMETHOD(SetProgressState) (THIS_ HWND hwnd, TBPFLAG tbpFlags) PURE;
};
#else
// This block is strictly for doxygen.
/*!
 * \brief Exposes methods that control the taskbar.
 *
 *  Extends ITaskbarList2 by exposing methods that support the unified launching and switching taskbar button functionality added in Windows 7. This functionality includes thumbnail representations and switch targets based on individual tabs in a tabbed application, thumbnail toolbars, notification and status overlays, and progress indicators. This interface also provides the methods of the ITaskbarList and ITaskbarList2 interfaces, from which it inherits.
 */
public interface class ITaskbarList3: public ITaskbarList2 {
	/*!
	 * \brief Sets the type and state of the progress indicator displayed on a taskbar button.
	 * \param hwnd The handle of the window in which the progress of an operation is being shown. This window's associated taskbar button will display the progress bar.
	 * \param tbpFlags Flags that control the current state of the progress button.
	 * \return Returns S_OK if successful, or an error value otherwise.
	 *
	 * \note A call to SetProgressValue() will switch a progress indicator currently in an indeterminate mode (#TBPF_INDETERMINATE) to a normal (determinate) display and clear the #TBPF_INDETERMINATE flag.
	 */
	HRESULT SetProgressState(HWND hwnd, TBPFLAG tbpFlags);
	/*!
	 * \brief Displays or updates a progress bar hosted in a taskbar button to show the specific percentage completed of the full operation.
	 * \param hwnd The handle of the window whose associated taskbar button is being used as a progress indicator.
	 * \param ullCompleted An application-defined value that indicates the proportion of the operation that has been completed at the time the method is called.
	 * \param ullTotal An application-defined value that specifies the value \a ullCompleted will have when the operation is complete.
	 */
	HRESULT SetProgressValue(HWND hwnd, ULONGLONG ullCompleted, ULONGLONG ullTotal);
};
#endif

#endif /* _SHOBJIDL_H */
