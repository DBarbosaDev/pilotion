#pragma once

#define REGISTRY_PATH _T("SOFTWARE\\Pilotion")

#define REGISTRY_SHARED_MEMORY_STACK_PLANES _T("PlanesStackName")
#define REGISTRY_SHARED_MEMORY_STACK_PLANES_VALUE _T("Pilotion\\PlanesStack")

#define REGISTRY_TMP_CONTROL_PATH _T("SOFTWARE\\Pilotion\\tmp\\control\\")
#define REGISTRY_TMP_CONTROL_STATUS _T("status")
#define REGISTRY_TMP_CONTROL_NAMEDPIPE _T("namedPipe")
#define REGISTRY_TMP_CONTROL_MAX_AIRPORTS _T("maxAirports")
#define REGISTRY_TMP_CONTROL_MAX_PLANES _T("maxPlanes")

// TODO delete
// #define REGISTRY_TMP_AVIAO_PATH _T("SOFTWARE\\Pilotion\\tmp\\aviao\\")
// #define REGISTRY_TMP_AVIAO_WRITABLE_SHARED_MEMORY _T("writableSharedMemory")
// #define REGISTRY_TMP_AVIAO_READABLE_SHARED_MEMORY _T("readableSharedMemory")