pe-file-editor
==============

Simple .NET Wrapper for the pe-file code. It is currently missing a nice interface for resources and section creation which exist in the underlying library.

It is designed to be used directly from PowerShell. Example usage:

    PS> [Reflection.Assembly]::LoadFile("D:\PEFileEditor.dll")
    PS> $exe = new-object PE.File -Args "C:\Windows\explorer.exe",true
    PS> $exe.UpdatePEChkSum() # returns False since file was opened read-only
    
    False
    
    PS> $exe
    
    IsModified      : False
    FileVersion     : 6.1.7601.17567
    ExtraData       :
    Data            : {4D, 5A, 90, 00, 03, 00, 00, 00, 04, 00, ...}
    Size            : 002BD200
    SectionHeaders  : {.text, CodeSection, VA: 00001000-000B8AB9, Data: 00000600-000B8200, .rdata, InitializedDataSection, 
                      VA: 000B9000-000E7B94, Data: 000B8200-000E6E00, .data, WritableInitializedDataSection, VA: 000E8000-00
                      0EC064, Data: 000E6E00-000EAA00, .pdata, InitializedDataSection, VA: 000ED000-000F9E04, Data: 000EAA00
                      -000F7A00...}
    DataDirectories : {00000000-00000000, 000DDD28-000DDEB8, 000FA000-002BCE88, 000ED000-000F9E04...}
    OptionalHeader  : NT64, WindowsGUI, DynamicBase, NXCompat, TerminalServerAware
    FileHeader      : AMD64, 6 Sections, ExecutableImage, LargeAddressAware
    NtHeaders       : PE.Editor.NtHeaders64
    ImageBase       : 0000000100000000
    Is64Bit         : True
    Is32Bit         : False
    IsReadOnly      : True
    IsLoaded        : True
    
    PS> $exe.FileHeader

    Characteristics      : ExecutableImage, LargeAddressAware
    SizeOfOptionalHeader : 240
    NumberOfSymbols      : 0
    PointerToSymbolTable : 00000000
    TimeDateStamp        : 2/25/2011 4:24:04 AM
    NumberOfSections     : 6
    Machine              : AMD64

    PS> $exe.OptionalHeader
    
    DataDirectories         : {00000000-00000000, 000DDD28-000DDEB8, 000FA000-002BCE88, 000ED000-000F9E04...}
    NumberOfRvaAndSizes     : 16
    LoaderFlags             : 00000000
    SizeOfHeapCommit        : 0000000000001000
    SizeOfHeapReserve       : 0000000000100000
    SizeOfStackCommit       : 000000000000E000
    SizeOfStackReserve      : 0000000000080000
    DllCharacteristics      : DynamicBase, NXCompat, TerminalServerAware
    Subsystem               : WindowsGUI
    CheckSum                : 002C8AF6
    SizeOfHeaders           : 00000600
    SizeOfImage             : 002C0000
    < truncated for brevity >

    PS> $exe.SectionHeaders
    
    Characteristics      : CodeSection
    NumberOfLinenumbers  : 0
    NumberOfRelocations  : 0
    PointerToLinenumbers : 00000000
    PointerToRelocations : 00000000
    PointerToRawData     : 00000600
    SizeOfRawData        : 000B7C00
    VirtualAddress       : 00001000
    VirtualSize          : 000B7AB9
    Name                 : .text
    Index                : 0
    
    Characteristics      : InitializedDataSection
    NumberOfLinenumbers  : 0
    NumberOfRelocations  : 0
    PointerToLinenumbers : 00000000
    PointerToRelocations : 00000000
    PointerToRawData     : 000B8200
    SizeOfRawData        : 0002EC00
    VirtualAddress       : 000B9000
    VirtualSize          : 0002EB94
    Name                 : .rdata
    Index                : 1
    
    < truncated for brevity >
