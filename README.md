pe-file-editor
==============

Simple .NET Wrapper for the pe-file code. It is currently missing a nice interface for resources and direction Section access.

It is designed to be used directly from PowerShell. Example usage:

    PS> [Reflection.Assembly]::LoadFile("D:\PEFileEditor.dll")
    PS> $exe = new-object PE.File -Args "C:\Windows\explorer.exe",true
    PS> $exe.UpdatePEChkSum() # returns False since file was opened read-only
    
    False
    
    PS> $exe
    
    ModifiedFlag    : False
    FileVersion     : 6.1.7601.17567
    HasExtraData    : False
    Data            : PE.RawData
    Size            : 2871808
    SectionHeaders  : {.text, CodeSection, VA: 00001000-000B8AB9, Data: 00000600-000B8200, .rdata, InitializedDataSection,
                      VA: 000B9000-000E7B94, Data: 000B8200-000E6E00, .data, WritableInitializedDataection, VA: 000E8000-00
                      0EC064, Data: 000E6E00-000EAA00, .pdata, InitializedDataSection, VA: 000ED000-000F9E04, Data: 000EAA0
                      0-000F7A00...}
    DataDirectories : {00000000-00000000, 000DDD28-000DDEB8, 000FA000-002BCE88, 000ED000-000F9E04...}
    NtHeaders64     : PE.NtHeaders64
    NtHeaders32     : PE.NtHeaders32
    FileHeader      : AMD64, 6 Sections, ExecutableImage, LargeAddressAware
    ImageBase       : 4294967296
    Is64Bit         : True
    Is32Bit         : False
    IsReadOnly      : True
    IsLoaded        : True
    
    PS> $exe.FileHeader

    Characteristics      : ExecutableImage, LargeAddressAware
    SizeOfOptionalHeader : 240
    NumberOfSymbols      : 0
    PointerToSymbolTable : 0
    TimeDateStamp        : 2/25/2011 4:24:04 AM
    NumberOfSections     : 6
    Machine              : AMD64
    
    PS> $exe.NtHeaders64.OptionalHeader
    
    DataDirectories             : {00000000-00000000, 000DDD28-000DDEB8, 000FA000-002BCE88, 000ED000-000F9E04...}
    NumberOfRvaAndSizes         : 16
    LoaderFlags                 : 0
    SizeOfHeapCommit            : 4096
    SizeOfHeapReserve           : 1048576
    SizeOfStackCommit           : 57344
    SizeOfStackReserve          : 524288
    DllCharacteristics          : DynamicBase, NXCompat, TerminalServerAware
    Subsystem                   : WindowsGUI
    CheckSum                    : 2919158
    SizeOfHeaders               : 1536
    < truncated for brevity >

    PS> $exe.SectionHeaders
    
    Characteristics      : CodeSection
    NumberOfLinenumbers  : 0
    NumberOfRelocations  : 0
    PointerToLinenumbers : 0
    PointerToRelocations : 0
    PointerToRawData     : 1536
    SizeOfRawData        : 752640
    VirtualAddress       : 4096
    VirtualSize          : 752313
    Name                 : .text
    Index                : 0
    
    Characteristics      : InitializedDataSection
    NumberOfLinenumbers  : 0
    NumberOfRelocations  : 0
    PointerToLinenumbers : 0
    PointerToRelocations : 0
    PointerToRawData     : 754176
    < truncated for brevity >

