/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   _10_backward_png;
    const int            _10_backward_pngSize = 34365;

    extern const char*   _10_forward_png;
    const int            _10_forward_pngSize = 34152;

    extern const char*   backward_png;
    const int            backward_pngSize = 47494;

    extern const char*   forward_png;
    const int            forward_pngSize = 9843;

    extern const char*   pause_png;
    const int            pause_pngSize = 16661;

    extern const char*   play_png;
    const int            play_pngSize = 30172;

    extern const char*   restart_png;
    const int            restart_pngSize = 24110;

    extern const char*   stop_png;
    const int            stop_pngSize = 32376;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
