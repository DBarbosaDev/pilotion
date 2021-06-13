#pragma once

#define INPUT_BUFF_SIZE 100

#define SHARED_MEMORY_STACK_PLANES _T("Global\PlanesStack")
#define SHARED_MEMORY_STACK_WRITE_INDEX _T("Global\PlanesStackLastLength")
#define SHARED_MEMORY_STACK_WRITE_INDEX_MUTEX _T("Global\PlanesStackLastLengthMutex")
#define SHARED_MEMORY_STACK_READ_INDEX _T("Global\planesStackIndexToRead")
#define SHARED_MEMORY_STACK_READ_INDEX_MUTEX _T("Global\planesStackIndexToReadMutex")
#define SHARED_MEMORY_STACK_LENGTH_VALUE _T("Global\planesStackLength")
#define SHARED_MEMORY_STACK_SEMAPHORE _T("Global\planesStackSemaphore")
#define SHARED_MEMORY_STACK_SEMAPHORE_NUM_ITEM _T("Global\planesStackNumItem")

//#define EVENT_ALERT_PLANE_CONNECTION _T("Global\alertPlaneConnection")

#define REGISTRY_PATH _T("SOFTWARE\\Pilotion")

#define REGISTRY_TMP_CONTROL_PATH _T("SOFTWARE\\Pilotion\\tmp\\control\\")
#define REGISTRY_TMP_CONTROL_STATUS _T("status")
#define REGISTRY_TMP_CONTROL_NAMEDPIPE _T("namedPipe")
#define REGISTRY_TMP_CONTROL_MAX_AIRPORTS _T("maxAirports")
#define REGISTRY_TMP_CONTROL_MAX_PLANES _T("maxPlanes")

#define WINDOW_CLASS_MAIN_NAME _T("ControlWindowClass")
#define WINDOW_CLASS_MAP_NAME _T("MapWindowClass")
#define WINDOW_CLASS_LOGS_NAME _T("LogsWindowClass")
#define WINDOW_CLASS_PANNEL_NAME _T("PannelWindowClass")

#define GUI_BUTTON_ID_SUBMIT_AIRPORT _T("CREATE_AIRPORT")
