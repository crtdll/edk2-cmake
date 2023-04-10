#include "stdafx.h"

EFI_DEVICE_PATH* g_windows_device_path;
EFI_HANDLE g_bootmgr_handle;
EFI_LOADED_IMAGE_PROTOCOL* g_bootmgr_interface;

EFI_STATUS find_file(CHAR16* image_path, EFI_DEVICE_PATH** device_path) {
	*device_path = NULL;

	UINTN num_nandles;
	EFI_HANDLE* handles;
	EFI_STATUS status = gBS->LocateHandleBuffer(ByProtocol,
		&gEfiSimpleFileSystemProtocolGuid,
		NULL,
		&num_nandles,
		&handles);

	if (EFI_ERROR(status))
		return status;

	for (UINTN i = 0; i < num_nandles; i++) {
		EFI_FILE_IO_INTERFACE* io_device;
		status = gBS->OpenProtocol(handles[i],
			&gEfiSimpleFileSystemProtocolGuid,
			(VOID**)&io_device,
			gImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);

		if (status != EFI_SUCCESS)
			continue;

		EFI_FILE_HANDLE volume_handle;
		status = io_device->OpenVolume(io_device, &volume_handle);
		if (EFI_ERROR(status))
			continue;

		EFI_FILE_HANDLE file_handle;
		status = volume_handle->Open(volume_handle,
			&file_handle,
			image_path,
			EFI_FILE_MODE_READ,
			EFI_FILE_READ_ONLY);

		if (!EFI_ERROR(status)) {
			volume_handle->Close(file_handle);
			*device_path = FileDevicePath(handles[i], image_path);
			break;
		}
	}

	FreePool(handles);
	return status;
}

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table) {
  gST->ConOut->ClearScreen(gST->ConOut);
  Print(L"Hello!\n");
    
	UINTN index = 0;
	gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &index);

  // find the boot manager
  if (find_file(L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi", &g_windows_device_path) < 0 || g_windows_device_path == nullptr)
		return EFI_LOAD_ERROR;

  // load the boot manager
  if (gBS->LoadImage(TRUE, image_handle, g_windows_device_path, nullptr, 0, &g_bootmgr_handle) < 0)
		return EFI_LOAD_ERROR;

  // get loaded image information on the boot manager
  if (gBS->HandleProtocol(g_bootmgr_handle, &gEfiLoadedImageProtocolGuid, &g_bootmgr_interface) < 0 || g_bootmgr_interface == nullptr)
		return EFI_LOAD_ERROR;

  return gBS->StartImage(g_bootmgr_handle, NULL, NULL);
}

EFI_STATUS EFIAPI UefiUnload(EFI_HANDLE image_handle) {
	return EFI_ACCESS_DENIED;
}