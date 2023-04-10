#pragma once
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci.h>
#include <Pi/PiDxeCis.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesTableLib.h>

#include <Uefi/UefiBaseType.h>
#include <Library/FileHandleLib.h>

#include <PiDxe.h>
#include <Protocol/Cpu.h>

typedef UINTN uint64_t;
typedef UINT8 uint8_t;
typedef BOOLEAN bool;
typedef UINT32 uint32_t;
typedef UINT16 uint16_t;
typedef CHAR16 char16_t;
#define nullptr 0
#define true 1
#define false 0