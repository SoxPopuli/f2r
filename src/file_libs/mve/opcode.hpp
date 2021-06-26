#pragma once
#include "integers.hpp"
#include <vector>

namespace file::mve
{
    enum OpcodeType: u8
    {
        EndOfStream           =  0x00,
        EndOfChunk            =  0x01,
        CreateTimer           =  0x02,
        InitAudioBuffers      =  0x03,
        ToggleAudioPlayback   =  0x04,
        InitVideoBuffers      =  0x05,
        Unknown1              =  0x06,
        DisplayBuffer         =  0x07,
        AudioFrame            =  0x08,
        SilentAudioFrame      =  0x09,
        InitVideoMode         =  0x0A,
        CreateGradient        =  0x0B,
        SetPalette            =  0x0C,
        SetPaletteCompressed  =  0x0D,
        Unknown2              =  0x0E,
        SetDecodingMap        =  0x0F,
        Unknown3              =  0x10,
        VideoData             =  0x11,
        Unused1               =  0x12,
        Unknown4              =  0x13,
        Unused2               =  0x14,
        Unknown5              =  0x15
    };

    struct Opcode
    {
        u32 length;
        u8 type;
        u8 version;
    };

    struct TimerOpcode: public Opcode
    {
        u32 rate;
        u16 subdivision;
    };
}
